
#ifndef _pgraph_h
#define _pgraph_h

#include "schedule.hpp"

/// Sequence Graph
/**
  @author Per Willenius
  @version 2.3rc1
  */
class Lisa_SGraph { 
private:
  /// flag, if topsort() was already done
  bool ts;
  
  /// size of graph
  int n,m;
  
  /// graph valid ?
  int valid;
  
  /// contains for each vertex the vollowing vertical edge
  int *v_edge;
  /// contains for each vertex the following horizontal edge
  int *h_edge;
  /// vertical predecessor of a vertex
  int *v_pred;
  /// horizontal predecessor of a vertex  
  int *h_pred;
public:
  /// list of vertex generetad by topsort()  
  int *TS;  
  /// default constructor
  Lisa_SGraph();
  /// create a sequence graph with m x n vertices
  Lisa_SGraph(int pm,int pn);
  /// create sequence from a schedule
  Lisa_SGraph(Lisa_Schedule *PL);
  /// copy constructor
  Lisa_SGraph(class Lisa_CSGraph & myCSG);
  ///destructor
  ~Lisa_SGraph();
  
  /// debug info output
  void print();
  
  /// reinitialize graph, clears all edges
  /** this method looks like it has a big memory leak */
  void new_graph();
  
  /// generate sequence graph from schedule
  void ausplan(Lisa_Schedule * PL);
  
  /// get horizontal successor of vertex i
  int hsucc(int i);
  /// get vertical successor of vertex i
  int vsucc(int i);
  /// get horizontal predecessor of vertex i
  int hpred(int i);
  /// get vertical predecessor of vertex i
  int vpred(int i);
  
  /// returns internal index of vertex at position ni,mi
  int index(int ni, int mi);
  /// returns row of vertex with index i
  int row(int i);
  /// returns column of vertex with index i
  int column(int i);
  /// sort vertice topologically, returns true if graph contains a cycle
  bool topsort();
};

//**************************************************************************

/// adjacency list node
/**
  @author Per Willenius
  @version 2.3rc1
  */
class AD_List_Node{
public:
  /// pointer to previous node
  AD_List_Node * pred;
  /// pointer to next node
  AD_List_Node * next;
  /// vertice number
  int adj_vertice;
  /// constructor
  AD_List_Node();
};

//**************************************************************************


/// adjacency list graph object
/**
  @author Per Willenius
  @version 2.3rc1
  */
class AD_List{
private:

//@{
  ///private data, needs documentation
  int m,n;
  AD_List_Node * leer;
  AD_List_Node * first;
  AD_List_Node * actual;
  AD_List_Node * last;
  AD_List_Node ** all;
//@}

public:  
  /// inserts edge
  void add_e( int v);
  /// removes edge
  void del_e( int v); // entfernt Kante
  
  /// constructor
  AD_List(int m, int n);
  
  /// resets current neighbourvertex
  void reset();
  
  /// goto next neighbour vertex
  int next();
  
  /// destructor
  ~AD_List();
};

//**************************************************************************

/// vertex list
/**
  @author Per Willenius
  @version 2.3rc1
  */
class V_List{
private: 
  /// private data, needs documentation
  int actual;
  /// private data, needs documentation
  int *feld;
  /// contains index of next
  int *succ;
  /// points to the first free
  int ef;
public:
  /// constructor
  /** create vlist with given size */
  V_List(int size);
  /// destructor
  ~V_List();
  
  /// insert
  void insert(int ki, int rank);

  /// reset current
  void reset();
  
  /// returns next element
  int next();
};

//**************************************************************************

/// contains the edges of the CS graph
/**
  @author Per Willenius
  @version 2.3rc1
  */
class Lisa_CSGraph{
private:
  /// horizontal edges
  int *h_edge;
  /// vertical edges
  int *v_edge;
  /// adjacencylist for each vertex
  /** vertical edges */
  AD_List ** v_adlst;
  /// adjacencylist for each vertex
  /** horizontal edges */
  AD_List ** h_adlst;
  /// for counting vertice
  int i_index,j_index;
public:
  /// size
  int m,n;
  /// default constructor
  Lisa_CSGraph();
  /// create graph from given plan
  Lisa_CSGraph(Lisa_Schedule *PL);
  /// create graph from given plan  
  Lisa_CSGraph(Lisa_Schedule *PL, int tst);
  /// destructor
  ~Lisa_CSGraph(); 
  
  /// reinitialize graph data
  /** this again looks like a big memory leak */
  void new_graph();
  
  /// add an edge
  void add_edge(int v, int w);
  
  /// add an edge
  void add_vedge(int column, int i, int j);
  /// add an edge
  void add_hedge(int row, int i, int j);
  
  /// returns true is there is an edge between i and j in column
  int tst_vedge(int column, int i, int j);
  /// returns true is there is an edge between i and j in row  
  int tst_hedge(int row, int i, int j);
  
  /// delete edge
  void del_edge(int v, int w);
  
  /// jump to next vertice
  int next_vertice();
  
  /// reset vertice 
  void reset_vertice();
  
  /// needs documentation
  int next(int v);
  
  /// next vertical edge
  int next_v(int v);
  /// next horizontal edge
  int next_h(int v);
  
  /// edge between v and w ?
  int edge(int v, int w);
  
  /// reset
  void reset(int v);
  
  /// debug output
  void print();

  /// returns index of vertex at ni,mi
  int index(int ni, int mi);
  
  /// returns row of vertex with index i
  int row(int i);
  /// returns colum of vertex with index i
  int column(int i);
};

//**************************************************************************

#endif

