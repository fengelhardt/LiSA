/**
 * @author  Per Willenius
 * @version 2.3rc1
 */
 
#include <stdio.h>

#include "../lisa/lvalues.hpp"
#include "pgraph.hpp"

using namespace std;

//**************************************************************************

extern class Lisa_Values G_Values;

//**************************************************************************

void ende(char * str){
  cerr << "Kein Speicher frei. Abbruch bei " << str << "\n";
}

//**************************************************************************

Lisa_SGraph::Lisa_SGraph(){
  m=0;
  n=0;
  new_graph();
}

//**************************************************************************

Lisa_SGraph::Lisa_SGraph(int pm, int pn){
  m=pm;
  n=pn;
  new_graph();
}

//**************************************************************************

Lisa_SGraph::Lisa_SGraph(Lisa_CSGraph & myCSG){
  m=myCSG.m;
  n=myCSG.n;
  new_graph();

  int i;
  int u=-1,v=-1,w=-1;
  int nach_alt, nach_neu;
  
  for(v=0;v<n*m;v++)
    {
      // fuelle v_edge[i],h_edge,v_pred,h_pred entsprechend myCSG
      myCSG.reset(v);
      nach_alt=nach_neu=-1;
      // zuerst die Horizontalen
      nach_neu=myCSG.next_h(v);
      while(nach_neu>=0) {
	if(nach_alt>=0)
	  {
	    if(myCSG.edge(nach_neu,nach_alt))
	      {
		nach_alt=nach_neu;
	      }
	  }
	else nach_alt=nach_neu;
	nach_neu=myCSG.next_h(v);
	}
      h_edge[v]=nach_alt;
      if(nach_alt>=0) h_pred[nach_alt]=v;
      
      nach_alt=nach_neu=-1;
      // nun die Vertikalen Kanten
      nach_neu=myCSG.next_v(v);
      while(nach_neu>=0) { 
	if(nach_alt>=0)
	  {
	    if(myCSG.edge(nach_neu,nach_alt))
	      {
		nach_alt=nach_neu;
	      }
	  }
	else nach_alt=nach_neu;
	nach_neu=myCSG.next_v(v); 
	}
      v_edge[v]=nach_alt;
      if(nach_alt>=0) v_pred[nach_alt]=v;
    }

  // Nun muss topologisch sortiert werden
  
  int *ein_val; // Eingangsvalenz
  
  ein_val= new int [n*m];
  class AD_List quelle(m,n);
  
  // Initialisierung der Valenzmatrix und der Quellen
  for(v=0;v<n*m;v++) { 
    ein_val[v]=0;
    if (v_pred[v]>=0) ein_val[v]++;
    if (h_pred[v]>=0) ein_val[v]++;
    if (ein_val[v]==0) quelle.add_e(v);
  }
  
  i=0;    
  quelle.reset();
  w=quelle.next();
  while(w>=0) {
    // gehe nun alle Quellen durch und fuege neue Knoten hinzu
    TS[i]=w;
    i++;
    quelle.del_e(w);
    u=v_edge[w];
    v=h_edge[w];
    if(u>=0) ein_val[u]--;
    if (ein_val[u]==0) // aha neue Quelle
      quelle.add_e(u);
    if(v>=0) ein_val[v]--;
    if (ein_val[v]==0) // aha neue Quelle
      quelle.add_e(v);
    quelle.reset();
    w=quelle.next();
  }
  
  if(i!=m*n) 
    {
      printf("Zyklen\n");
      valid=0;
    }
  else  
    delete [] ein_val;
}

//**************************************************************************

Lisa_SGraph::Lisa_SGraph(Lisa_Schedule * PL) {
  m=PL->get_m();
  n=PL->get_n();
  new_graph();
  ausplan(PL);
}

//**************************************************************************

void Lisa_SGraph::new_graph() {
  int i;
  ts=0;
  valid=1; // von vornherein Zulaessig
  if((v_edge=new int[n*m])==NULL) ende("Lisa_SGraph");
  if((h_edge=new int[n*m])==NULL) ende("Lisa_SGraph");
  if((TS    =new int[n*m])==NULL) ende("Lisa_SGraph");
  if((v_pred=new int[n*m])==NULL) ende("Lisa_SGraph");
  if((h_pred=new int[n*m])==NULL) ende("Lisa_SGraph");
  
  for(i=0;i<n*m;i++){
    h_edge[i]=-1;
    v_edge[i]=-1;
    v_pred[i]=-1;
    h_pred[i]=-1;
  }
}

//**************************************************************************

void Lisa_SGraph::ausplan(Lisa_Schedule * PL){
  // Topologische Sortierung
  int min,element,j;
  
  if(n!=PL->get_n()||m!=PL->get_m()) 
    {printf("Fehler bei ausplan (%d,%d)!=(%d,%d)\n",n,m,PL->get_n(),PL->get_m());return;}
  int i,mi,ni;
  V_List rnglst(n*m);
   for(i=0;i<n*m;i++) 
    {
      if ( (*G_Values.SIJ)[row(i)][column(i)]) 
	rnglst.insert(i,int( (*PL->LR)[row(i)][column(i)]));
    }
  rnglst.reset();
  for(i=0;i<n*m;i++) 
    if ( (*G_Values.SIJ)[row(i)][column(i)]) 
      TS[i]=rnglst.next();
  
  // die Struktur des Planes PL macht die Erstellung des Graphen einfach
  int vnach;
  int hnach;
  for(i=0;i<n*m;i++){
    v_pred[i]=-1;
    h_pred[i]=-1;
  }

  for(i=0;i<n*m;i++) {
    if ( (*G_Values.SIJ)[row(i)][column(i)]) {
      mi=column(i);
      ni=row(i);
      // nun wird die Zeile des Nachfolgers bestimmt:
      min=m*n+1;
      element= (*PL->LR)[ni][m];
      vnach=-1;
      for(j=0;j<m;j++) 
	{
	  if( element > (*PL->LR)[ni][m] && element< min && element>0) 
	    {
	      min=element;
	      vnach=j;
	    }
	}
      
      // nun wird die Spalte des Nachfolgers bestimmt:
      hnach=-1; min=m*n+1;
      for(j=0;j<m;j++) 
	{
	  if( element > (*PL->LR)[n][mi] && element< min && element>0) 
	    {
	      min=element;
	      hnach=j;
	    }
	}
      if (vnach!=0) {
	v_edge[i]=index(vnach,mi);
	v_pred[index(vnach,mi)]=i;}
      else v_edge[i]=-1;
      if (hnach!=0) {
	h_edge[i]=index(ni,hnach);
	h_pred[index(ni,hnach)]=i;}
      else h_edge[i]=-1;
    }
  }  
}

//**************************************************************************

void  Lisa_SGraph::print(){
  printf("\n Plangraph: Format:%dx%d\n",n,m);
  int i;
  for(i=0;i<m*n;i++) {
    printf("v(%d,%d) ",i,v_edge[i]);
    printf("h(%d,%d) \n",i,h_edge[i]);
  }
  
  printf("oder:\n");
  for(i=0;i<m*n;i++) {
    printf("v(%d,%d) ",v_pred[i],i);
    printf("h(%d,%d) \n",h_pred[i],i);
  }
}

//**************************************************************************

int Lisa_SGraph::vsucc(int i){
  return v_edge[i];
}

//**************************************************************************

int Lisa_SGraph::hsucc(int i){
  return h_edge[i];
}

//**************************************************************************

int Lisa_SGraph::vpred(int i){
  return v_pred[i];
}

//**************************************************************************

int Lisa_SGraph::hpred(int i){
  return h_pred[i];
}

//**************************************************************************

int Lisa_SGraph::index(int ni, int mi){
  return(m*ni+mi);
}

//**************************************************************************

int Lisa_SGraph::row(int i){
  if (i==-1)    return (-1);
  else
    return(int(i/m)); 
}

//**************************************************************************

int Lisa_SGraph::column(int i){
  int zahl;
  if (i==-1)    return (-1);
  else
    zahl=int(i/m);
  return i-zahl*m; 
}

//**************************************************************************

Lisa_SGraph::~Lisa_SGraph(){
  if (v_edge)  delete [] v_edge;
  if (h_edge)  delete [] h_edge;
  if (TS)  delete [] TS    ;
  if (v_pred)  delete [] v_pred;
  if (h_pred)  delete [] h_pred;
}

//**************************************************************************

AD_List::AD_List(int on, int om){
  int i;
  n=on;
  m=om;
  first=0; 
  actual=0;
  last=0;
  if((all= new AD_List_Node * [n*m])==NULL) ende("AD_List");
  for(i=0;i<n*m;i++){
    all[i]=0;
  }
  
}

//**************************************************************************

void AD_List::reset()
{
  actual=first;
}

//**************************************************************************

int  AD_List::next(){
  int v;
  if (actual){
    v=actual->adj_vertice; 
    actual=actual->next;
    return v;
  }
  return -1;
}

//**************************************************************************

void AD_List::add_e( int v){
  if (v>=0){
    AD_List_Node * new_el;
    if((new_el =new AD_List_Node)==NULL) ende("AD_List add_e");
    if (first){
      last->next=  new_el;
      new_el->next=0;
      new_el->pred= last;
    }
    else {
      first= new_el;
      first->next=0;
     }
    all[v]= new_el;
    last=  new_el;
    new_el->adj_vertice=v;
   }
}

//**************************************************************************

void AD_List::del_e( int v){
  if (all[v]){ // Aha, Kante existiert
    AD_List_Node * vor,* nach;
     vor=all[v]->pred;
    nach=all[v]->next;
    if (all[v]==first)
      {
	first=first->next;
      }
    if (all[v]==last)
      {
	last=last->pred;
      }
    
    if (vor) vor->next=nach;
    if (nach) nach->pred=vor;
    if (all[v]==actual) {
      actual=nach;
    }
    delete all[v];
    if(first==last) actual=first; // Liste ist leer, oder hat nur ein El.
    all[v]=0;
  }
}

//**************************************************************************

AD_List::~AD_List(){
  int i;
  for(i=0;i<n*m;i++) if(all[i]!=NULL) delete all[i];
  delete [] all;
}

//**************************************************************************

AD_List_Node::AD_List_Node(){
  pred=0; next=0;
  adj_vertice=0;
}

//**************************************************************************

V_List::V_List(int size){
  int i;
  if((feld= new int[2*size])==NULL) ende("V_List");
  if((succ= new int[2*size+1])==NULL) ende("V_List");
  
  for(i=0;i<2*size;i++) {feld[i]=-1;succ[i]=i+1;}
  ef=size;
  succ[size-1]=-1; // Zeiger auf letztes Element
}

//**************************************************************************

V_List::~V_List(){
  if (feld) delete [] feld;
  delete [] succ;
}

//**************************************************************************

void V_List::insert(int ki,int rank)
{
  int rnk;
  rnk=rank-1;
  if (feld[rnk]==-1){ // Aha, ist leer
    feld[rnk]=ki;
  }
  else{ // biege Zeiger um und schreibs in das nexte freie Feld
    feld[ef]=ki;
    succ[ef]=succ[rnk];
    succ[rnk]=ef;
    ef++;
  }
}

//**************************************************************************

void V_List::reset()
{
  actual=0;
}

//**************************************************************************

int V_List::next(){
  int alt=actual;
  actual=succ[actual];
  return(feld[alt]);
}

//**************************************************************************

Lisa_CSGraph::Lisa_CSGraph(){
  m=0;n=0;
  new_graph();
}

//**************************************************************************

Lisa_CSGraph::Lisa_CSGraph(Lisa_Schedule *PL){
  int i,j,k;
  m=PL->get_m();n=PL->get_n();
  new_graph();
  for(i=0;i<n;i++)
    for(j=0;j<m;j++)
      for(k=0;k<m;k++)
	if( (*PL->LR)[i][j]< (*PL->LR)[i][k]) add_hedge(i,j,k);
  
  for(i=0;i<m;i++)
    for(j=0;j<n;j++)
      for(k=0;k<n;k++)
	if( (*PL->LR)[j][i]< (*PL->LR)[k][i]) add_vedge(i,j,k);

  i_index=0; j_index=0;
}

//**************************************************************************

void Lisa_CSGraph::new_graph(){
  int i;
  h_edge =new int[n*m*m];
  v_edge =new int[m*n*n]; 
  
  for(i=0;i<n*m*m;i++)h_edge[i]=0;
  for(i=0;i<m*n*n;i++)v_edge[i]=0;
  
  if((v_adlst=new AD_List * [n*m])==NULL) ende("Lisa_CSGraph");
  for(i=0;i<n*m;i++)
    if((v_adlst[i]=new AD_List(n,m))==NULL) ende("Lisa_CSGraph");
  
  if((h_adlst=new AD_List * [n*m])==NULL) ende("Lisa_CSGraph");
  for(i=0;i<n*m;i++)
    if((h_adlst[i]=new AD_List(n,m))==NULL) ende("Lisa_CSGraph");
}

//**************************************************************************

Lisa_CSGraph::Lisa_CSGraph(Lisa_Schedule *PL, int tst){
  int i,j,k;
  if(tst==-1){
    m=PL->get_m();n=PL->get_n();
    new_graph();
    
    for(i=0;i<n;i++)
      for(j=0;j<m;j++)
	for(k=0;k<m;k++)
	  if( (*PL->LR)[i][j]< (*PL->LR)[i][k]) add_hedge(i,k,j);
    
    for(i=0;i<m;i++)
      for(j=0;j<n;j++)
	for(k=0;k<n;k++)
	  if( (*PL->LR)[j][i]< (*PL->LR)[k][i]) add_vedge(i,k,j);
    
    i_index=0; j_index=0;
  }
}

//**************************************************************************

void Lisa_CSGraph::reset(int v){
  if (v_adlst[v]) v_adlst[v]->reset();
  if (h_adlst[v]) h_adlst[v]->reset();
}

//**************************************************************************

void Lisa_CSGraph::reset_vertice(){
  i_index=j_index=-1;
}

//**************************************************************************

int Lisa_CSGraph::next_vertice(){
  if(i_index<0){i_index=j_index=0;}
  else{
    j_index++;
    if(j_index>=m) 
      {
	j_index=0; 
	i_index++;
	if (i_index>=n) i_index=-1;
      }
  }
  return(index(i_index,j_index));
}

//**************************************************************************

void Lisa_CSGraph::add_edge(int v, int w){
  int z1,s1,z2,s2;
  z1=row(v);
  s1=column(v);
  z2=row(w);
  s2=column(w);
  if(z1==z2) // aha: horizontale Kante
    {
      if(tst_hedge(z1,s1,s2)==0) // noch nicht gesetzt
	{ 
	  h_edge[m*(m*z1+s1)+s2]=1;
	  h_adlst[v]->add_e(w);
	}
    }
  else if(s1==s2) // vertikale Kante
    {
      if(tst_vedge(s1,z1,z2)==0) // noch nicht gesetzt
	{
	  v_edge[n*(n*s1+z1)+z2]=1;
	  v_adlst[v]->add_e(w);
	}
    }
}

//**************************************************************************

void Lisa_CSGraph::del_edge(int v, int w){
  int z1,s1,z2,s2;
  z1=row(v);
  s1=column(v);
  z2=row(w);
  s2=column(w);
  if(z1==z2) // aha: horizontale Kante
    {
      if(tst_hedge(z1,s1,s2)==1) // gesetzt
	{
	  h_edge[m*(m*z1+s1)+s2]=0;
	  h_adlst[v]->del_e(w);
	}
    }
  else if(s1==s2) // vertikale Kante
    {
      if(tst_vedge(s1,z1,z2)==1) // gesetzt
	{
	  v_edge[n*(n*s1+z1)+z2]=0;
	  v_adlst[v]->del_e(w);
	}
    }
}

//**************************************************************************

int  Lisa_CSGraph::next(int v){

  if(v>=0){
    int merke=-1;
    if (h_adlst[v]) merke= h_adlst[v]->next();
    if(merke<0)
      if (v_adlst[v]) return v_adlst[v]->next();
    return merke;
  }
  return -1;
}

//**************************************************************************

int  Lisa_CSGraph::next_v(int v){
  if(v>=0)
    {
      if (v_adlst[v]) return v_adlst[v]->next();
    }
  return -1;
}

//**************************************************************************

int  Lisa_CSGraph::next_h(int v){
  
  if(v>=0)
    {
      if (h_adlst[v]) return h_adlst[v]->next();
    }
   return -1;
}

//**************************************************************************

void Lisa_CSGraph::add_vedge(int column, int i, int j){
  if(tst_vedge(column,i,j)==0) // noch nicht gesetzt
    {
      v_edge[n*(n*column+i)+j]=1;
      v_adlst[index(i,column)]->add_e(index(j,column));
    }
}

//**************************************************************************

int Lisa_CSGraph::tst_vedge(int column, int i, int j){
  return v_edge[n*(n*column+i)+j];
}

//**************************************************************************

void Lisa_CSGraph::add_hedge(int row, int i, int j){
  
  if(tst_hedge(row,i,j)==0) // noch nicht gesetzt
    { 
      h_edge[m*(m*row+i)+j]=1;
      h_adlst[index(row,i)]->add_e(index(row,j));
    }
}

//**************************************************************************

int Lisa_CSGraph::tst_hedge(int row, int i, int j){
  return h_edge[m*(m*row+i)+j];
}

//**************************************************************************

int Lisa_CSGraph::edge(int v, int w){
  int z1,s1,z2,s2;
  z1=row(v);
  s1=column(v);
  z2=row(w);
  s2=column(w);
  if(z1==z2) // aha: horizontale Kante
    {
      if(tst_hedge(z1,s1,s2)==1) // gesetzt
	return 1;
    }
  else if(s1==s2) // vertikale Kante
    {
      if(tst_vedge(s1,z1,z2)==1) // gesetzt
	return 1;
    }
  return 0;
}

//**************************************************************************

void Lisa_CSGraph::print(){
  int v=0,w=0;
  
  printf("\n ************ CS-Graph *******************\n");
  for(v=0;v<m*n;v++){
    printf("\n %d: ",v);
    for(w=0;w<m*n;w++){
      if(edge(v,w)) printf("%d, ",w);
    }
  }
  printf("\n******************************************\n");
  
}

//**************************************************************************

int Lisa_CSGraph::index(int ni, int mi){		
  return(m*ni+mi);				
}

//**************************************************************************

int Lisa_CSGraph::row(int i){
  return(int(i/m));
}

//**************************************************************************

int Lisa_CSGraph::column(int i){
  int zahl;
  
  zahl=int(i/m);
  return i-zahl*m;
}

//**************************************************************************

Lisa_CSGraph::~Lisa_CSGraph(){
  int i;
  
  for(i=0;i<n*m;i++)
    {  delete v_adlst[i];
    delete h_adlst[i];
    }
  delete [] h_edge;
  delete [] v_edge;
  delete [] v_adlst;
  delete [] h_adlst;
  
}

//**************************************************************************

