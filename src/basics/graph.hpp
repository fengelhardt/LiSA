
#ifndef _graph_h 
#define _graph_h 

//**************************************************************************

#include <iostream>

#include "../lisa/filentry.hpp"
#include "matrix.hpp"
#include "pair.hpp"

//**************************************************************************

/// a base class representing a graph
/** Lisa_Graph is a class with mostly virtual methods, providing a unified
    interface to objects representing simple graphs. All documentation here
    defines how an implementation of the Lisa_Graph interface _should_ work.
    You can convert between different implementations using the
    get_adjacency_matrix() and set_adjacency_matrix() functions.
    
    Vertices are numbered 1...get_vertices().
    
    Lisa_Graph is a simple graph, so there can only be connections between 
    two different vertices and you can only have one connection.
    
    There are three types of connections from a vertex A to another vertex B,
    either an ARC which makes B a successor of A, a CRA ( ARC backwards ;) ) 
    which makes B a predecessor of A or an EDGE which makes B a neighbour of A.
    A neighbour is both a successor and predecessor.
    
    Please consider the following table which shows how existing connections
    are affected by the manipulation functions.
    
    Existing connection from A to B: \-----------+
    ----------------\  NONE | ARC  | CRA  | EDGE |
    function called: +------+------+------+------+
    insert_edge(A,B) | EDGE | EDGE | EDGE | EDGE |
    -----------------+------+------+------+------+
    insert_edge(B,A) | EDGE | EDGE | EDGE | EDGE |
    -----------------+------+------+------+------+
    insert_arc(A,B)  | ARC  | ARC  | EDGE | EDGE |
    -----------------+------+------+------+------+
    insert_arc(B,A)  | CRA  | EDGE | CRA  | EDGE |
    -----------------+------+------+------+------+
    remove_edge(A,B) | NONE | NONE | NONE | NONE |
    -----------------+------+------+------+------+
    remove_edge(B,A) | NONE | NONE | NONE | NONE |
    -----------------+------+------+------+------+
    remove_arc(A,B)  | NONE | NONE | CRA  | CRA  |
    -----------------+------+------+------+------+
    remove_arc(B,A)  | NONE | ARC  | NONE | ARC  |
    
    You can use the init_successor(), next_successor(), etc functions to 
    get all successors etc from a given vertex. Those functions are
    independent, so you can use them mixed at the same time. After calling 
    an insert, remove or clear function you will have to reinitialize though,
    otherwise you may get undefined results. 
    
    The get_successors() etc. functions are also independed from the
    init_successor(), next_successor() functions.
    
    @author Marc Moerig
    @version 2.3final
    
  */
class Lisa_Graph : public Lisa_FileEntry{
public:
  /// flags indicating various possible connections between a pair of vertice
  /** as returned by get_connection() */
  enum {CRA=-1 /// backwards ARC ;)
       ,NONE /// no connection
       ,ARC // an arc
       ,EDGE /// an edge
       };

  /// return the number of vertices in this graph
  virtual int get_vertices()const=0;

  /// (re)initialize a graph with a given number of vertices
  /** this will also clear the graph */
  virtual void init(const int number_of_vertices)=0;  
  
  /// removes all EDGES and ARCS from the graph
  virtual void clear()=0;
  
  /// generate an adjacency matrix representing the graph
  /** The argument matrix adj has to be of size get_vertices() x get_vertices().
      After this operation it will contain a 1 at position A-1,B-1 if there is
      an ARC from A to B. An EDGE from A to B is represented by a 1 at position
      A-1,B-1 and B-1,A-1. If there is no connection from A to B there will be
      a 0 at A-1,B-1 and B-1,A-1. 
      
      You can use this function to convert between different implementatzions
      of a Lisa_Graph. */
  virtual void get_adjacency_matrix(Lisa_Matrix<int> *const adj)const=0;

  /// completely overwrite graph with values from an adjacency matrix
  /** This function expects a matrix of size get_vertices() x get_vertices()
      the same format as delivered by get_adjacency_matrix(). All non zero
      entrys will be interpreted as 1 though, so that it is possible to feed
      weighted adjacency matrices to this function.
      
      You can use this function to convert between different implementatzions
      of a Lisa_Graph. */
  virtual void set_adjacency_matrix(const Lisa_Matrix<int> *const adj)=0;

  /// insert an EDGE into the graph
  /** Regardless of what the connection between start and end was, after this
      operation it will be an EDGE.*/
  virtual void insert_edge(const int start,const int end)=0;

  /// insert an ARC into the graph
  /** Add an ARC from vertex start to vertex end. If there is a CRA between 
      start and end it will be an EDGE afterwards, if there ist no connection,
      an ARC will be created. All other connections will remain unchanged.*/
  virtual void insert_arc(const int start,const int end)=0;
    
  /// remove an EDGE from the graph 
  /** Regardless of what the connection between start and end was, after this
      operation it will NONE.*/
  virtual void remove_edge(const int start,const int end)=0;

  /// remove an ARC from the graph
  /** If there is an
      ARC from start to end it willbe deleted and the new connection will be
      NONE, if there is an EDGE it will be converted into a CRA. In any other
      case the connection will remain unchanged. */
  virtual void remove_arc(const int start,const int end)=0;

  /// delete all ARC's CRA's and EDGE's connected with that vertice
  virtual void clear(const int vertex)=0;
  
  /// get the kind of connection between two vertices 
  /** possible return values are NONE,ARC,CRA or EDGE. */
  virtual int get_connection(const int start,const int end)const=0;

  /// list style access to all successors of a vertex
  /** This reinitializes the access for the given vertex */
  virtual void init_successor(const int vertex)=0;

  /// list style access to all successors of a vertex
  /** Returns the next successor of a vertex or get_vertices()+1. The latter 
      indicates that all successors have been returned in previous operations
      and that the access is also reinitialized. */  
  virtual int next_successor(const int vertex)=0;

  /// list style access to all predecessors of a vertex
  /** This reinitializes the access for the given vertex */
  virtual void init_predecessor(const int vertex)=0;
  
  /// list style access to all predecessors of a vertex
  /** Returns the next predecessor of a vertex or get_vertices()+1. The latter 
      indicates that all predecessors have been returned in previous operations
      and that the access is also reinitialized. */  
  virtual int next_predecessor(const int vertex)=0;
  
  /// list style access to all neighbours of a vertex
  /** This reinitializes the access for the given vertex */
  virtual void init_neighbour(const int vertex)=0;
  
  /// list style access to all neighbours of a vertex
  /** Returns the next neighbour of a vertex or get_vertices()+1. The latter 
      indicates that all neighbours have been returned in previous operations
      and that the access is also reinitialized. */
  virtual int next_neighbour(const int vertex)=0;
      
  /// returns the number of successors for a vertex
  /** Sum of all EDGE's and ARC's connected to that vertex. This functions
      works independently from init_successor(), next_successor().*/
  virtual int get_successors(const int vertex)const=0;

  /// returns the number of predecessors for a vertex
  /** Sum of all EDGE's and CRA's connected to that vertex. This functions
      works independently from init_predecessor(), next_predecessor().*/
  virtual int get_predecessors(const int vertex)const=0;
  
  /// returns the number of neighbours for a vertex
  /** Sum of all EDGE's connected to that vertex. This functions
      works independently from init_neighbour(), next_neighbour().*/
  virtual int get_neighbours(const int vertex)const=0;

  /// write a Lisa_Graph to a stream
  /** The data is written as an adjacency matrix, as returned by
      get_adjacency_matrix(). */
  void write(std::ostream& = std::cout) const;

  /// read a Lisa_Graph from a stream
  void read(std::istream& = std::cin);
   
};

//**************************************************************************

/// graph object 
/** The Lisa_MatrixListGraph Model stores three kind of connections: edges,
    arcs and backwards directed arcs. There are three double-linked lists for
    every single vertice, to determine its arc-predecessors, arc-successors and 
    edge-neighbours. The matrix includes these structures. The process of 
    adding or deleting a connection changes these lists, according to the 
    choosen kind of connection.

    @author Christian Schulz
    @version 2.3final
*/  
class Lisa_MatrixListGraph  : public Lisa_Graph {
private:

  /// number of vertices
  int size;
  
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
  
  /// vector with list pointer
  /** that stores the current position within the successor and predeccessor 
      list of every vertex */
  Lisa_Vector<Lisa_Pair> *succ_pred_pointer;
  /// vector with list pointer
  /** that stores the current position within the edge list of every vertex */
  Lisa_Vector<int> *edge_pointer;
  
  ///determine the signum of a connection
  int signum(const int start,const int end) const;
  
  ///insert EDGE, only if connection was NONE before
  void ins_edge(const int start,const int end);
  
  ///insert ARC, only if connection was NONE before
  void ins_arc(const int start,const int end);
  
  ///remove EDGE, only of connection was EDGE before
  void rem_edge(const int start,const int end);
  
  ///remove ARC, only if connection was ARC before
  void rem_arc(const int start,const int end);
  
public:
  
  /// constructor
  /** Create a graph object for a number of vertices. */
  Lisa_MatrixListGraph(const int number_of_vertices);

  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_MatrixListGraph(const Lisa_MatrixListGraph *const othergraph);
  
  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_MatrixListGraph(const Lisa_Graph *const othergraph);
  
  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_MatrixListGraph(const Lisa_MatrixListGraph & othergraph);
  
  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_MatrixListGraph(const Lisa_Graph& othergraph);

  /// destructor
  ~Lisa_MatrixListGraph();

  inline int get_vertices()const{return size;}
  
  void init(const int number_of_vertex);

  void clear();  
  
  void get_adjacency_matrix(Lisa_Matrix<int> *const adj) const;

  void set_adjacency_matrix(const Lisa_Matrix<int> *const adj);

  void insert_edge(const int start,const int end);

  void insert_arc(const int start,const int end);
    
  void remove_edge(const int start,const int end);

  void remove_arc(const int start,const int end);
  
  void clear(const int vertex);
  
  int get_connection(const int start,const int end)const;

  void init_successor(const int vertex);

  int next_successor(const int vertex);
  
  void init_predecessor(const int vertex);
  
  int next_predecessor(const int vertex);
  
  void init_neighbour(const int vertex);
  
  int next_neighbour(const int vertex);
  
  int get_successors(const int vertex)const;

  int get_predecessors(const int vertex)const;
  
  int get_neighbours(const int vertex)const;

  /// write this objects data structure to cout
  /** Mostly used for debugging. */
  void show();

  /// check whether the objects data structures are consistent
  /** Mostly used for debugging. */
  bool valid();

};

//**************************************************************************

/// graph object 
/** The Lisa_MatrixGraph Model stores three kind of connections: edges, arcs 
    and  backwards directed arcs. This is managed through a simple adjacency
    matrix with entrys of 1 and 0

    @author Marc Moerig
    @version 2.3final
*/  
class Lisa_MatrixGraph : public Lisa_Graph {
private:

  /// number of vertices
  int size;

  /// adjacency matrix
  Lisa_Matrix<int>* matrix;
  
public:
  
  /// constructor
  /** Create a graph object for a number of vertices. */
  Lisa_MatrixGraph(const int number_of_vertices);

  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_MatrixGraph(const Lisa_MatrixGraph *const othergraph);
  
  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_MatrixGraph(const Lisa_Graph *const othergraph);
  
  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_MatrixGraph(const Lisa_MatrixGraph & othergraph);
  
  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_MatrixGraph(const Lisa_Graph& othergraph);

  /// destructor
  ~Lisa_MatrixGraph();

  inline int get_vertices()const{return size;}
  
  void init(const int number_of_vertex);

  void clear();  
  
  void get_adjacency_matrix(Lisa_Matrix<int> *const adj) const;

  void set_adjacency_matrix(const Lisa_Matrix<int> *const adj);

  void insert_edge(const int start,const int end);

  void insert_arc(const int start,const int end);
    
  void remove_edge(const int start,const int end);

  void remove_arc(const int start,const int end);
  
  void clear(const int vertex);
  
  int get_connection(const int start,const int end)const;

  void init_successor(const int vertex);

  int next_successor(const int vertex);
  
  void init_predecessor(const int vertex);
  
  int next_predecessor(const int vertex);
  
  void init_neighbour(const int vertex);
  
  int next_neighbour(const int vertex);
  
  int get_successors(const int vertex)const;

  int get_predecessors(const int vertex)const;
  
  int get_neighbours(const int vertex)const;

};

//**************************************************************************

class Lisa_WeightedGraph : public Lisa_FileEntry{
private:
  /// number of vertices in our graph
  int size;
  
  /// adjacency matrix
  Lisa_Matrix<int>* adj;
  
  /// weight matrix
  Lisa_Matrix<TIMETYP> weights;
 
public:
  
  /// flags indicating various possible connections between a pair of vertice
  /** as returned by get_connection() */
  enum {CRA=-1 /// backwards ARC ;)
       ,NONE /// no connection
       ,ARC // an arc
       };
  
  /// constructor
  /** Create a graph object for a number of vertices. */
  Lisa_WeightedGraph(const int number_of_vertices);

  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_WeightedGraph(const Lisa_WeightedGraph *const othergraph);

  /// constructor
  /** Create a graph as the copy of another graph. */
  Lisa_WeightedGraph(const Lisa_WeightedGraph& othergraph);

  /// destructor
  ~Lisa_WeightedGraph();

  inline int get_vertices()const{return size;}
  
  void init(const int number_of_vertex);

  void clear();  
  
  void get_adjacency_matrix(Lisa_Matrix<int> *const adj)const;

  void set_adjacency_matrix(const Lisa_Matrix<int> *const adj);
  
  void get_weight_matrix(Lisa_Matrix<TIMETYP> *const weights)const;

  void set_weight_matrix(const Lisa_Matrix<TIMETYP> *const adj); 

  void insert_arc(const int start,const int end,const TIMETYP weight=0);

  void remove_arc(const int start,const int end);
  
  void clear(const int vertex);
  
  int get_connection(const int start,const int end)const;

  void set_weight(const int start,const int end,const TIMETYP weight=0);
  
  TIMETYP get_weight(const int start,const int end)const;
  
  void init_successor(const int vertex);

  int next_successor(const int vertex);
  
  void init_predecessor(const int vertex);
  
  int next_predecessor(const int vertex);
  
  /// write a Lisa_WeightedGraph to a stream
  /** The data is written as an adjacency matrix and weight matrix, as returned
      by get_adjacency_matrix() and get_weight_matrix(). */
  void write(std::ostream& = std::cout) const;

  /// read a Lisa_Graph from a stream
  void read(std::istream& = std::cin);

};

//**************************************************************************

/// convert between one and two dimensional indices
/** Each operation in a scheduling problem can be found in LR,CIJ,SIJ etc with 
    two indices, the machine and job number, those are usually numbered 
    0...jobs-1,0..machines-1. LiSA's Graph objects however only accept one
    index for a vertex in the range from 1...vertices. This object creates for
    a given set of operations two lookup tables to convert between those. */
class Lisa_HammingGraphIndex : public Lisa_GenericObject{
private:

  //@{
  /// arrays holding our indices
  int* ij2index;
  int* index2i;
  int* index2j;   
  //@}

  /// number of indices created
  int indices;
  
public:
  /// number of machines
  const int m;
  /// number of jobs
  const int n;

  /// constructor
  /** create an index from the set of operations */
  Lisa_HammingGraphIndex(const Lisa_Matrix<bool> *const SIJ);
  
  /// destructor
  ~Lisa_HammingGraphIndex();
  
  /// return number of indices
  inline int get_indices()const{ return indices; }

  /// get the job number for the one dimensional index
  inline int i(const int index)const{
#ifdef LISA_DEBUG
    if(index <= 0 || index >= indices+1){
      G_ExceptionList.lthrow("Index "+ztos(index)+
                             " out of range in Lisa_HammingGraphIndex::i().",
                             Lisa_ExceptionList::OUT_OF_RANGE);
      return 0;
    }
#endif
    return index2i[index];
  }
  
  /// get the machine number for the one dimensional index
  inline int j(const int index)const{
#ifdef LISA_DEBUG
    if(index <= 0 || index >= indices+1){
      G_ExceptionList.lthrow("Index "+ztos(index)+
                             " out of range in Lisa_HammingGraphIndex::j().",
                             Lisa_ExceptionList::OUT_OF_RANGE);
      return 0;
    }
#endif
    return index2j[index];  
  }
  
  /// get the one dimensional index for an operation
  /** this will either return the one dimensional index for the operation or
      get_indices()+1 if the operation does not exists (similar to the graph
      object returning get_vertices()+1) */
  inline int index(const int i,const int j)const{
#ifdef LISA_DEBUG
    if(i < 0 || i >= n || j < 0 || j >= m){
      G_ExceptionList.lthrow("Operation ("+ztos(i)+","+ztos(j)+
                             ") out of range in Lisa_HammingGraphIndex::index().",
                             Lisa_ExceptionList::OUT_OF_RANGE);
      return indices+1;
    }
#endif
    return ij2index[i*m+j];
  }
  
  /// write object information to a stream
  /** used for debugging */
  void write(std::ostream& = std::cout) const;
  
};

//**************************************************************************

/// some algorithms on graphs
/**
    @author Marc Moerig
    @version 2.3final
  */
class Lisa_GraphAlg{
public:

  /// sort vertices topologically
  /** The vertices will be put in topoligical order into the vertex_sequence
      vector, i.e. vertex i in the original graph is assigned to the vertex
      indicated by knot_sequence(i). returns true: all vertices are topsorted,
      graph contains no cycles returns false: graph contains cycle, elements of
      vertex_sequence are undefined */ 
  static bool topsort(const Lisa_Graph *const g,
                      Lisa_Vector<int> *const vertex_sequence);
                      
  /// Sort vertices topologically.
  /** topsort() performs this operation such that if i is the vertex his
      position will be stored at (*vertex_sequence)[i-1]. This Method however
      does it the other way around, so that i is the position while
      (*vertex_sequence)[i] contains the vertex. */
  static bool topsort_inverse(const Lisa_Graph *const g,
                              Lisa_Vector<int> *const vertex_sequence);
                   
  /// Replaces each ARC with an EDGE.
  static void build_semigraph(Lisa_Graph *const graph);
   
  /// Create a comparability graph.
  /** Build the transitive hull, then create the semigraph of it. The
      targetgraph is supposed to have the same number of vertice as the
      inputgraph, but no EDGE's or ARC's at all.*/ 
  static void build_compgraph(Lisa_Graph *const source,Lisa_Graph *const target);

  /// Create a transitive hull
  /** The targetgraph is supposed to have the same number of vertice as the 
      inputgraph, but no EDGE's or ARC's at all.*/ 
  static void transitive_hull(Lisa_Graph *const source,Lisa_Graph *const target);
 
  /// Compare graphs. 
  /** This method returns true if any EDGE in the first graph is also contained
      in the second graph, and the second graph has at least one more EDGE.
      Both graph's must not contain any ARC's and must have the same number of
      vertices which are also fixed e.g. no permutations of vertices are
      considered. */
  static bool smaller(const Lisa_Graph *const first,const Lisa_Graph *const second);
  
  /// Compare graphs.
  /** This method returns true if any ARC,EDGE in the first graph is also
      contained in the second graph, and vice versa. Both graph's need to have
      the same number of vertices which are fixed e.g. no permutations of 
      vertices are considered. So this method does NOT test if the graphs are
      isomorph. */
  static bool equal(const Lisa_Graph *const first,const Lisa_Graph *const second);
  
};

//**************************************************************************

#endif

