
#ifndef _graph_h 
#define _graph_h 

#include <iostream>

#include "../lisa/filentry.hpp"
#include "matrix.hpp"
#include "pair.hpp"

/// flags indicating various possible connections between knots
enum{NOT_POSSIBLE=-2
    ,CRA /// backwards ARC ;)
    ,NO
    ,ARC
    ,EDGE};

/// LiSA's graph object 
/** The Lisa_Graph Model stores three kind of connections: edges, arcs and 
    backwards directed arcs. There are three double-linked lists for every 
    single vertice, to determine its arc-predecessors, arc-successors and 
    edge-neighbours. The matrix includes these structures. The process of 
    adding or deleting a connection changes these lists, according to the 
    choosen kind of connection.

    - ARC + (backwards) ARC = EDGE
    - ARC + EDGE =EDGE
    - EDGE - ARC = (backwards) ARC

    remark: (number of knots + 1) determines the end of a list
    
    @author Christian Schulz
    @version 2.3final
*/  
class Lisa_Graph  : public Lisa_FileEntry {
private:

  ///number of vertices
  int   n;
  
  /// end (number of vertices + 1) 
  int end;
  
  ///storage for the predecessor and successor lists of every vertex
  /** this is an adjacency matrix, however entries in the matrix are not only
      saying wether there is an ARC,EDGE,CRA or not, but are also used to
      indicate where in the matrix the next neighbour (successor, predecessor)
      is located ... just like a single linked list
      
      column 0:
      x=begin of successor list
      y=begin is end of successor list 

      row 0 :
      x=begin of predecessor list
      y=begin is end of  predecessor list 

      matrix[i][j] i,j>0
      x pointer in forward direction
      y pointer in backward direction 
    */
  Lisa_Matrix<Lisa_Pair> *matrix;
  
  ///pointer to a Vector
  /** that storages the current position within the successor and predeccessor 
      list of every vertex */
  Lisa_Vector<Lisa_Pair> *succ_pred_pointer;
  
  ///determine the signum of a connection
  int signum(int start, int end) const;
  
  /// needs documentation
  int ix,iy;

public:
  
  /// constructor
  /** Create a graph object for a number of vertices. */
  Lisa_Graph(int number_of_vertices);

  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_Graph(const Lisa_Graph& othergraph);

  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_Graph(const Lisa_Graph* othergraph);

  /// destructor
  ~Lisa_Graph();

  /// assign another graph
  /** Copy one graph object to the other */ 
  const Lisa_Graph& operator=(const Lisa_Graph& other);

  /// return the number of vertices
  inline int get_vertices()const{return n;};

  ///initialize the pointer for the successors list of a given vertice
  bool init_succ_pointer(int vertex);

  /// initialize the pointer for the predecessor list of a given vertice
  bool init_pred_pointer(int vertex);

  /// generate an adjazent matrix according to the predecessor and successor lists
  Lisa_Matrix<int>* get_ADJ_Matrix(Lisa_Matrix<int>* adj) const;

  /// create predecessor and successor lists with the help of an adjazent matrix
  void read_ADJ(Lisa_Matrix<int>* adj);

  /// (re)initialize a graph with a given number of vertices
  /** this will clear the graph */
  void init(int number_of_vertex);

  /// insert an edge into the graph
  /** Add edge from vertice start to vertice end. */
  bool insert_edge(int start, int end);

  /// insert an arc into the graph
  /** Add arc from vertice start to vertice  end. */
  bool insert_arc(int start, int end);

  /// remove an edge from the graph 
  /** Delete edge from vertice start to vertice end. */
  bool exclude_edge(int start, int end);

  /// remove an arc from the graph
  /** Delete arc from vertice start to vertice end. */
  bool exclude_arc(int start, int end);

  /// get the kind of connection between two vertices 
  /** possible return values are:
      - NO 0
      - ARC 1
      - CRA -1 which is an arc from end to start ;)
      - EDGE 2 */
  int get_connection(int start, int end);

  ///access to the first element of the successor list
  int get_first_successor(int vertex);

  ///access to the last element of the successor list
  int get_last_successor(int vertex); 

  ///access to the first element of the predecessor list
  int get_first_predecessor(int vertex);

  ///access to the last element of the predecessor list
  int get_last_predecessor(int vertex);

  /// get a successsor of a vertex
  /** Returns the next successor of a vertex and moves the according list 
      pointer to the next following successor. Returning n+1 stands for the end 
      of this vertice's successor list and for a new initialization of its 
      successor list pointer. */  
  int get_next_successor(int vertex);

  /// get a predecessor of a vertex
  /** Returns the next predeccessor of a vertex and moves the according 
      vertice pointer to the next following predeccessor. Returning n+1 stands
      for the end of this vertice's predeccessor list and for a new 
      initialization of its predeccessor list pointer. */  
  int get_next_predeccessor(int vertex);

  /// get vertices that form an edge together with the argument vertice 
  /** returns only connected edges of a vertice, returning n+1 stands for the 
      end of this vertice's edge list */
  int get_next_edge(int vertex);

  /// test if there are no edges in the graph
  bool no_edges();

  /// returns the number of successors for a vertice
  /** This is the sum of edges and arcs. */
  int number_of_succ(int vertex);

  /// returns the number of successors for a vertice
  /** This is the sum of edges backwards arcs (CRA's). */
  int number_of_pred(int vertex);

  /// sort vertices topologically
  /** The vertices will be put in topoligical order into the 
      vertex_sequence vector, i.e. vertex i in the original graph 
      is assigned to the vertex indicated by knot_sequence(i).
      returns true: all vertices are topsorted, graph contains no cycles
      returns false: graph contains cycle, elements of vertex_sequence are 
      undefined */ 
  bool topsort(Lisa_Vector<int>* vertex_sequence);

  /// delete all ARC's CRA's and edges connected with that vertice
  bool remove_all_con(int vertex);

  /// write this objects data structure to cout
  /** Mostly used for debugging. */
  void show();

  /// check whether the objects data structures are consistent
  /** Mostly used for debugging. */
  bool valid();

  /// write the object to a stream 
  void write(std::ostream& = std::cout) const;

  /// read the object from a stream
  void read(std::istream& = std::cin);
};

#endif

