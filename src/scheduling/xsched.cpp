/**
 * @author  Per Willenius
 * @version 2.3pre3
 */

#include "xsched.hpp"

Lisa_XSchedule::Lisa_XSchedule(){
  CP=NULL;
  PG=NULL;
  CG=NULL;
  TH=NULL;
  TH_V=NULL;
  CS=NULL;
  IKL=NULL;
}

Lisa_XSchedule::Lisa_XSchedule(class Lisa_Schedule *Pl){
  P=Pl; // connect Lisa_XSchedule with Lisa_Schedule
  n=P->get_n(); m=P->get_m();
  CG=NULL;
  TH=NULL;
  TH_V=NULL;
  CS=NULL;
  IKL=NULL;
  CP=new Lisa_Matrix<bool>(n,m); 
  PG=new Lisa_SGraph(m,n); // important: order of the parameter not like Lisa_Matrix
}

void Lisa_XSchedule::init(){
  // if the maschine number or job number has changed
  n=P->get_n(); m=P->get_m();
  if (CP)  delete CP;
  CP=new Lisa_Matrix<bool>(n,m); 
  if (PG)  delete PG;
  PG=new Lisa_SGraph(m,n);
}

void Lisa_XSchedule::make_CP() {
  init();
  if (CP) return; CP=new Lisa_Matrix<bool>(n,m); CP->fill(0);
}

/*double Lisa_XSchedule::objfkt(int objective_name, 
			      Lisa_Matrix<bool> *SIJ, 
			      Lisa_Matrix<TIMETYP> *PT) {
  if (P->valid==FALSE) {cerr << "Sequence not valid\n"; return 0;}
  int i,j,maxi,maxj,v,sn,sm,mi,ni;
  double wert,max=0,xmax;
  if(  (m!=P->get_m()) || (n!=P->get_n())) {cerr << "m,n != P->n,m\n"; init();}
  bool vollst=1;
    // test, if the latin rechtange is complete
  for(i=0;i<n;i++)
    for(j=0;j<m;j++) 
      if( (*SIJ)[i][j]) { 
	if( (*P->LR)[i][j]<1) { 
	vollst=0; }
      }

  if(vollst) {
     if (P==NULL) cerr << "No Lisa_Schedule defined\n";
    if (objective_name==CMAX){
      if (P->CIJ==NULL) {
	P->make_CIJ();
      }
      P->CIJ->fill(0);
      for(i=0;i<n*m;i++)
	{
	  v=PG->TS[i]; 
	  sn=PG->row(v);sm=PG->column(v);wert=0;
	  if( (*SIJ)[sn][sm]) 
	    {
	      // find maximal value in row and column 
	      xmax=0;
	      for(mi=0;mi<m;mi++) {
		if (xmax< (*P->CIJ)[sn][mi] &&sm!=mi) xmax=(*P->CIJ)[sn][mi];}
	      for(ni=0;ni<n;ni++) {
		if (xmax< (*P->CIJ)[ni][sm] &&sn!=ni) xmax=(*P->CIJ)[ni][sm];}
	      wert=xmax+ (*PT)[sn][sm];
	      (*P->CIJ)[sn][sm]=wert;
	      if (wert>max) {max=wert;maxi=sn;maxj=sm;}
	    }
	}
      if(!CP) make_CP();
      CP->fill(0);
      int local_end=0;   
      while(local_end==0){
        (*CP)[maxi][maxj]=1;
	local_end=1;
	// get predecessor in row and column
	for(ni=0;ni<n;ni++) 
	  if ( (*P->CIJ)[maxi][maxj]==(*P->CIJ)[ni][maxj]+(*PT)[maxi][maxj]) { 
	    local_end=0;
	    maxi=ni;
	    ni=n;
	  }
	if(local_end)
	  for(mi=0;mi<m;mi++) 
 	    if ( (*P->CIJ)[maxi][maxj]==(*P->CIJ)[maxi][mi]+(*PT)[maxi][maxj]) {
	      local_end=0;
	      maxj=mi;
	      mi=m;
	    }
      }
    }
  }  
  else cerr << "not complete\n";
  if (objective_name==CMAX) return (max);  
  //G_ExceptionList.lthrow("This version of LiSA can only compute C_max objective",ANY_ERROR);
  // if the objective function cannot be computed, return 0
  return 0;
}
*/
int Lisa_XSchedule::P_to_PG(){
  // Lisa_Schedule to CS-Graph
  // CS-Graph to Lisa_SGraph
  if (P->get_m()==m&&P->get_n()==n)
    {
      if (CS) delete CS;
      CS=new CSgraph(P);
      if (PG) delete PG;
      PG=new Lisa_SGraph(*CS);
    }
  else {
    init();
    if (CS) delete CS;
    CS=new CSgraph(P);
    if (PG) delete PG;
    PG=new Lisa_SGraph(*CS);
  }
  return 1;
}

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

Lisa_XSchedule::~Lisa_XSchedule(){
  if (PG) delete PG;
  if (CG) delete CG;
  if (TH) delete TH;
  if (TH_V) delete TH_V;
  if (CS) delete CS;
  if (IKL) delete IKL;
}

