
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

  ///number of Knots
  int   n;
  
  /// end
  int end;
  
  ///storage for the predecessor and successor lists of every knot
  Lisa_Matrix<Lisa_Pair> *matrix;
  
  ///pointer to a Vector
  /** that storages the actuall positin within the successor and predeccessor 
      list of every knot */
  Lisa_Vector<Lisa_Pair> *succ_pred_pointer;
  
  ///determine the signum of a connection
  int   signum(int start, int end) const;
  
  /// needs documentation
  int ix,iy;

public:
  
  /// constructor
  /** Create a graph object for a numberof  vertices. */
  Lisa_Graph(int number_of_knots);

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
  int get_knots() const;

  ///initialize the pointer for the successors list of a given vertice
  bool init_succ_pointer(int knot);

  /// initialize the pointer for the predecessor list of a given vertice
  bool init_pred_pointer(int knot);

  /// generate an adjazent matrix according to the predecessor and successor lists
  Lisa_Matrix<int>* get_ADJ_Matrix(Lisa_Matrix<int>* adj) const;

  /// create predecessor and successor lists with the help of an adjazent matrix
  void read_ADJ(Lisa_Matrix<int>* adj);

  /// initialize a graph with a given number of vertices
  void init(int number_of_knots);

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
  int  get_connection(int start, int end);

  ///access to the first element of the successor list
  int   get_first_successor(int knot);

  ///access to the last element of the successor list
  int   get_last_successor(int knot); 

  ///access to the first element of the predecessor list
  int   get_first_predecessor(int knot);

  ///access to the last element of the predecessor list
  int   get_last_predecessor(int knot);

  /// get a successsor of a vertice
  /** Returns the next successor of a vertice and moves the according list 
      pointer to the next following successor. Returning n+1 stands for the end 
      of this vertice's successor list and for a new initialization of its 
      successor list pointer. */  
  int get_next_successor(int knot);

  /// get a predecessor of a vertice
  /** Returns the next predeccessor of a vertice and moves the according 
      vertice pointer to the next following predeccessor. Returning n+1 stands
      for the end of this vertice's predeccessor list and for a new 
      initialization of its predeccessor list pointer. */  
  int get_next_predeccessor(int knot);

  /// get vertices that form an edge together with the argument vertice 
  /** returns only connected edges of a vertice, returning n+1 stands for the 
      end of this vertice's edge list */
  int get_next_edge(int knot);

  /// test if there are no edges in the graph
  bool no_edges();

  /// returns the number of successors for a vertice
  /** This is the sum of edges and arcs. */
  int number_of_succ(int knot);

  /// returns the number of successors for a vertice
  /** This is the sum of edges backwards arcs (CRA's). */
  int number_of_pred(int knot);

  /// sort vertices topologically
  /** The vertices will be put  in topoligical order in the 
      knot_sequence vector, i.e. vertice i in the original graph 
      is assigned to the vertice indicated by knot_sequence(i).
      returns true: all vertices are topsorted, graph contains no cycles
      returns false: otherwise */ 
  bool topsort(Lisa_Vector<int>* knot_sequence);

  /// delete all ARC's CRA's and edges connected with that vertice
  bool remove_all_con(int knot);

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

