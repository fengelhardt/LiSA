/**
 * @author  Per Willenius
 * @version 2.3rc1
 */

#include "xsched.hpp"

//**************************************************************************

Lisa_XSchedule::Lisa_XSchedule(){
  CP=NULL;
  PG=NULL;
//  CG=NULL;
//  TH=NULL;
//  TH_V=NULL;
  CS=NULL;
//  IKL=NULL;
}

//**************************************************************************

Lisa_XSchedule::Lisa_XSchedule(class Lisa_Schedule *Pl){
  P=Pl; // connect Lisa_XSchedule with Lisa_Schedule
  n=P->get_n(); m=P->get_m();
  CS=NULL;
  CP=new Lisa_Matrix<bool>(n,m); 
  PG=new Lisa_SGraph(m,n); // important: order of the parameter not like Lisa_Matrix
}

//**************************************************************************

void Lisa_XSchedule::init(){
  // if the maschine number or job number has changed
  n=P->get_n(); m=P->get_m();
  if (CP)  delete CP;
  CP=new Lisa_Matrix<bool>(n,m); 
  if (PG)  delete PG;
  PG=new Lisa_SGraph(m,n);
}

//**************************************************************************

void Lisa_XSchedule::make_CP() {
  init();
  if (CP) return; CP=new Lisa_Matrix<bool>(n,m); CP->fill(0);
}

//**************************************************************************

int Lisa_XSchedule::P_to_PG(){
  // Lisa_Schedule to CS-Graph
  // CS-Graph to Lisa_SGraph
  if (P->get_m()==m&&P->get_n()==n)
    {
      if (CS) delete CS;
      CS=new Lisa_CSGraph(P);
      if (PG) delete PG;
      PG=new Lisa_SGraph(*CS);
    }
  else {
    init();
    if (CS) delete CS;
    CS=new Lisa_CSGraph(P);
    if (PG) delete PG;
    PG=new Lisa_SGraph(*CS);
  }
  return 1;
}

//**************************************************************************

int Lisa_XSchedule::PG_to_LR(Lisa_Matrix<bool> *SIJ){
  int v,vi,vj,i;
  int w1,w2,a,b;
  P->LR->fill(0); // delete all entries
  for(i=0;i<m*n;i++){
    v=PG->TS[i];
    vi=PG->row(v);
    vj=PG->column(v);
    if ( (*SIJ)[vi][vj]) {
      w1=PG->hpred(v); // horizontal predecessor
      w2=PG->vpred(v); // vertical predecessor
      a=b=0;
      if(w1!=NOT_DEFINED) a= (*P->LR)[PG->row(w1)][PG->column(w1)];
      if(w2!=NOT_DEFINED) b= (*P->LR)[PG->row(w2)][PG->column(w2)];
      (*P->LR)[vi][vj]=MAX(a,b)+1;
    }
  }
  return 1;
}

//**************************************************************************

Lisa_XSchedule::~Lisa_XSchedule(){
  if (PG) delete PG;
  if (CS) delete CS;
}

//**************************************************************************

