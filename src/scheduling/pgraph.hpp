
#ifndef _pgraph_h
#define _pgraph_h

#include "schedule.hpp"

/// Sequence Graph
/**
  @author Per Willenius
  @version 2.3pre3
  */
class Lisa_SGraph { 
public:
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

class AD_List_Node{
public:
  AD_List_Node * pred;
  AD_List_Node * next;
  int adj_vertice;
  AD_List_Node();
};

//**************************************************************************

class AD_List{
public:
  int m,n;
  AD_List_Node * leer;
  AD_List_Node * first;
  AD_List_Node * actual;
  AD_List_Node * last;
  AD_List_Node ** all; // Zeigt fuer jeden Knoten auf das
  // entsprechende AD_List_Node 
  // 0, wenn v nicht adjazent  
  void add_e( int v); // Fuegt Kante hinzu
  void del_e( int v); // entfernt Kante
  AD_List(int m, int n);
  void reset();  // setzt den aktuellen Nachbarknoten zurueck
  int next(); // naechster Nachbarknoten
  ~AD_List();
};

//**************************************************************************

class V_List{
public: 
  int actual;
  int *feld;
  int *succ;  // Enthaelt Index des naechsten
  int ef; // Zeigt auf den ersten freien

  V_List();
  V_List(int groesse);
  void insert(int ki, int rank);
  ~V_List();
  void reset(); // setzt aktuell zurueck
  int next(); // gibt naechstes Element zurueck
};

//**************************************************************************

class Lisa_CSGraph{ //Enthaelt die Kanten des CS-Graphen  
  int *h_edge; // Horizontale Kanten
  int *v_edge;  // Vertikale Kanten
  class AD_List ** v_adlst;     // Adjazenzliste (vertikale Kanten) fuer jeden Knoten
  class AD_List ** h_adlst;     // Adjazenzliste (horizontale Kanten) fuer jeden Knoten
  int i_index,j_index; // Zur Knotenzaehlung
public:
  int m,n;
  Lisa_CSGraph();
  void new_graph();
  Lisa_CSGraph(Lisa_Schedule *PL);
  Lisa_CSGraph(Lisa_Schedule *PL, int tst);
  void add_edge(int v, int w); // Fuegt Kante hinzu
  void add_vedge(int column, int i, int j);
  void add_hedge(int row, int i, int j);
  int tst_vedge(int column, int i, int j);
  int tst_hedge(int column, int i, int j);
  void del_edge(int v, int w);
  int next_vertice();
  void reset_vertice();
  int next(int v);
  int next_v(int v); // naechste vertikale Kante
  int next_h(int v); // naechste horizontale Kante
  int edge(int v, int w);
  void reset(int v);
  void print();
  ~Lisa_CSGraph();
  int index(int ni, int mi); // gibt Index des Knotens (ni,mi) zurueck
  int row(int i);           // gibt Zeile des Knotens mit Index i zurueck
  int column(int i);           // gibt Spalte des Knotens mit Index i zurueck
};

//**************************************************************************

#endif

