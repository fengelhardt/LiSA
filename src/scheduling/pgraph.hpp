/*
 * ************** pgraph.hpp *******************************
 * description: used graph classes
 *              Sequence Graph, Complete Sequence Graph
 *              and Comparability Graph.
 *
 * @author Per Willenius
 *
 * date:18.11.1997
 * @version 2.3pre3
*/

#ifndef _pgraph_h
#define _pgraph_h


//  ************** Lisa Includes  ****************** 
#include "schedule.hpp"


//  ************** Class Definitions  ******************

/** Sequence Graph
  */
class Lisa_SGraph { 
  // Einige Funktionen greifen auf das globale Feld C_BSg.SIJ zurueck
  // Dies wurde gemacht, um auch Probleme mit nichtvollstaendiger 
  // Operationenmenge verwalten zu koennen.
  // Da C_BSg.SIJ bereits existiert, braucht kein neues Feld angelegt werden 
  
public:
  bool ts; // flag, ob bereits TOPSORT durchgefuehrt wurde
  int n,m;
  int valid;
  int * v_edge;   // gibt fuer jeden Knoten die nachfolgende vert. Kante an  
  int * h_edge;   // gibt fuer jeden Knoten die nachfolgende hor. Kante in horadl an
  int * v_pred;   // vertikaler Vorgaenger  
  int * h_pred;   // horizontaler Vorgaenger  
  int * TS;      // Liste von Knoten entsprechend TOPSORT

  Lisa_SGraph();
  Lisa_SGraph(int pm,int pn); // leider nicht wie im Rest des Programmes
  void print(void);
  Lisa_SGraph(Lisa_Schedule *PL);
  Lisa_SGraph(class CSgraph & myCSG);
  ~Lisa_SGraph();
  void new_graph();
  void ausplan(Lisa_Schedule * PL);
  int hsucc(int i);
  int vsucc(int i);
  int hpred(int i);
  int vpred(int i);
  int index(int ni, int mi); // gibt Index des Knotens (ni,mi) zurueck
  int row(int i);           // gibt Zeile des Knotens mit Index i zurueck
  int column(int i);           // gibt Spalte des Knotens mit Index i zurueck
  bool topsort(); // topologische Sortierung, gibt 1 zurueck, wenn Zyklen
};

class Cgraph { // allgemeiner Graph (vor allen fuer
  // transitive Huelle und Comp_graph)
  int tUndir; // =1, wenn ungerichtet
public:
  int n,m;
  int number_of_edges;  // Anzahl Kanten
  int * val;
  class ad_list ** adlst;     // Adjazenzliste fuer jeden Knoten
  int * adm;     // Adjazenzmatrix
  Cgraph( Cgraph & CG);
  Cgraph( Lisa_SGraph & PG);
  Cgraph(int pm, int pn);
  Cgraph(Cgraph & CG, int flag);
  Cgraph();
  void new_graph();
  void ins_edge(int v, int w); // Fuegt Kante ein
  void del_edge(int v, int w); // Entfernt Kante
  int next(int v); // naechster zu v adjazenter Knoten
  void reset(int v); // setzt den Zaehler von v zurueck
  int edge(int v, int w); // testet ob (v,w) existiert
  void mksh(); // Symmetrische Huelle (mache ungerichtet)
  int index(int ni, int mi); // Index des Knotens (ni,mi) 
  int row(int i);           // Zeile des Knotens mit Index i
  int column(int i);           // Spalte des Knotens mit Index i
  ~Cgraph();
  void make_undirected(); // Symmetrische Huelle (mache ungerichtet)
  void print();
int operator <(Cgraph & zweite);
};

class vertice_set{ // Knotenmenge
public:
  int number_of_vertices;
  int * v_set; // Knotenmenge (1, wenn Knoten)
  vertice_set();
  vertice_set(int groesse);
  ~vertice_set();
  void add_vertice(int v);
  void del_vertice(int v);
  int tst_vertice(int v);
  void del_all();
  void del_vertice();
  int operator <=(vertice_set & zweite);
};

class ad_list{
public:
  int m,n;
  class ad_list_el * leer;
  class ad_list_el * first;
  class ad_list_el * actual;
  class ad_list_el * last;
  class ad_list_el ** all; // Zeigt fuer jeden Knoten auf das
  // entsprechende ad_list_el 
  // 0, wenn v nicht adjazent  
  void add_e( int v); // Fuegt Kante hinzu
  void del_e( int v); // entfernt Kante
  ad_list(int m, int n);
  void reset();  // setzt den aktuellen Nachbarknoten zurueck
  int next(); // naechster Nachbarknoten
  ~ad_list();
};

class ad_list_el{
public:
  ad_list_el * pred;
  ad_list_el * next;
  int adj_vertice;
  ad_list_el();
};

class Vlist{
public: 
  int actual;
  int *feld;
  int *succ;  // Enthaelt Index des naechsten
  int ef; // Zeigt auf den ersten freien

  Vlist();
  Vlist(int groesse);
  void insert(int ki, int rank);
  ~Vlist();
  void reset(); // setzt aktuell zurueck
  int next(); // gibt naechstes Element zurueck
};

class CSgraph{ //Enthaelt die Kanten des CS-Graphen  
  int *h_edge; // Horizontale Kanten
  int *v_edge;  // Vertikale Kanten
  class ad_list ** v_adlst;     // Adjazenzliste (vertikale Kanten) fuer jeden Knoten
  class ad_list ** h_adlst;     // Adjazenzliste (horizontale Kanten) fuer jeden Knoten
  int i_index,j_index; // Zur Knotenzaehlung
public:
  int m,n;
  CSgraph();
  void new_graph();
  CSgraph(Lisa_Schedule *PL);
  CSgraph(Lisa_Schedule *PL, int tst);
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
  ~CSgraph();
  int index(int ni, int mi); // gibt Index des Knotens (ni,mi) zurueck
  int row(int i);           // gibt Zeile des Knotens mit Index i zurueck
  int column(int i);           // gibt Spalte des Knotens mit Index i zurueck
};

class Lisa_ImplicationClass{ // Implikationsklassen
  
  // Ikl besteht aus fuenf Listen, 
  // 1-4: die Indizes der Kanten (Ikla, ...., Ikld)
  // 5: Zeigt auf jeweils erstes Element in Impl.klasse (Iklz)
  
  void new_icl();
  int size; // vor allem Feldgroesse
public:
  int * icla, * iclb, * iclc, * icld;
  int * iclz; // Zeiger
  int * Iklanz;
  int * zaehler; // Zaehler fuer next_edge
  int actual_icl; // Aktuelle Impl.klsse
  int next_free; // Zeiger auf freien Eintrag
  int number_of_icl; // Anzahl der Impl.klssn
  Lisa_ImplicationClass();
  Lisa_ImplicationClass(int gr);  
  class Lisa_Edge next_edge(int ikl);  // Naechste Kante in dieser Impl.klsse
  void reset(int ikl);
  void add_edge(int z1, int s1, int z2, int s2);
  void print();
  void next_ikl();
  ~Lisa_ImplicationClass();
};



class Lisa_Edge{
public:
  int z1,s1,z2,s2;
  Lisa_Edge();
};

#endif












