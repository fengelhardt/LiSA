/**
 * @author  Marc Moerig
 * @version 2.3final
 */

#include "convertgraph.hpp"

using namespace std;

//**************************************************************************

Lisa_ConvertGraph* Lisa_ConvertGraph::make_object(Lisa_ProblemType* pt,
                                                  Lisa_Matrix<bool>* SIJ,
                                                  Lisa_MO* MO){
  // sort problemtype out 
  int new_pt;
  
  int env = pt->get_property(M_ENV);
  int ri = pt->get_property(RI);
  int obj = pt->get_property(OBJECTIVE);

  if (obj==CMAX) obj = 0;
  else if(obj==LMAX || obj==SUM_CI || obj==SUM_WICI || obj==SUM_UI || 
	  obj==SUM_WIUI || obj==SUM_TI || obj==SUM_WITI) obj = 1;
  else obj = 2;

  if (env==O) env = 0;
  else if (env == J || env==F) env=1;
  else env = 2;

  // argl ... are these all cases i can't handle ?
  if (pt->get_property(PMTN) || pt->get_property(PRECEDENCE)!=EMPTY || 
      pt->get_property(BATCH) || pt->get_property(NO_WAIT) || env==2 ||
      obj==2){ 
    G_ExceptionList.lthrow("Problemtype "+pt->output_problem()+
			   " not supported by Lisa_ConvertGraph.",Lisa_ExceptionList::INCONSISTENT_INPUT);
    return 0;
  }else if(env==1 && MO==0){
    G_ExceptionList.lthrow("No machine order given to  "+pt->output_problem()+
			   " Problem.",Lisa_ExceptionList::INCONSISTENT_INPUT);
    return 0;
  }

  if(env == 0){ // O
    if(obj==0){ // CMAX
      if(ri) new_pt = O_RI_CMAX;
      else new_pt = O_CMAX;
    }else{ // LMAX and OTHER
      if(ri) new_pt = O_RI_LMAX;
      else new_pt = O_LMAX;
    }
  }else{ // J+F
    if(obj==0){ // CMAX
      if(ri) new_pt = J_RI_CMAX;
      else new_pt = J_CMAX;
    }else{ // LMAX and OTHER
      if(ri) new_pt = J_RI_LMAX;
      else new_pt = J_LMAX;
    }
  }

  Lisa_ConvertGraph* cgrp = new Lisa_ConvertGraph(new_pt,SIJ,MO);
  return cgrp;
}

//**************************************************************************

Lisa_ConvertGraph::Lisa_ConvertGraph(const int new_pt,
                                     Lisa_Matrix<bool>* SIJ,
                                     Lisa_MO* MO):curr_pt(new_pt),
                                                  m(SIJ->get_m()),
                                                  n(SIJ->get_n()){
                                                    
  // build lookup tables, 
  // call initialize for the according problemtype
  vert=0;
  // on lookup table
  lookup = new Lisa_Matrix<int>(n,m);
  lookup->fill(0);
  
  for (int i=0;i<n;i++)
    for (int j=0;j<m;j++)
      if ((*SIJ)[i][j]) (*lookup)[i][j] = ++vert;
  
  // and the other one
  I_lookup = new Lisa_Vector<int>(vert+1);
  J_lookup = new Lisa_Vector<int>(vert+1);
  
  (*I_lookup)[0] = -1;
  (*J_lookup)[0] = -1;
  
  for (int i=0;i<n;i++)
    for (int j=0;j<m;j++)
      if ((*lookup)[i][j]){
	(*I_lookup)[(*lookup)[i][j]] = i;
	(*J_lookup)[(*lookup)[i][j]] = j;
      }
  
  
  switch(curr_pt){
  case O_CMAX:
    initialize_O_CMAX(SIJ);
    break;
  case J_CMAX:
    initialize_J_CMAX(SIJ,MO);
    break;
  case O_LMAX:
    initialize_O_LMAX(SIJ);
    break;
  case J_LMAX:
    initialize_J_LMAX(SIJ,MO);
    break;
  case O_RI_CMAX:
    initialize_O_RI_CMAX(SIJ);
    break;
  case J_RI_CMAX:
    initialize_J_RI_CMAX(SIJ,MO);
    break;
  case O_RI_LMAX:
    initialize_O_RI_LMAX(SIJ);
    break;
  case J_RI_LMAX:
    initialize_J_RI_LMAX(SIJ,MO);
    break;
  }
}

//**************************************************************************

Lisa_ConvertGraph::~Lisa_ConvertGraph(){
  delete lookup;
  delete I_lookup;
  delete J_lookup;
  delete disjkt;
}

//**************************************************************************

Lisa_MatrixGraph*  Lisa_ConvertGraph::get_disjkt(){
  return disjkt;
}

//**************************************************************************

void Lisa_ConvertGraph::plan2graph(Lisa_Matrix<int>* plan,
                                   Lisa_MatrixGraph* plangraph){
                                     
  plangraph->clear();

  switch(curr_pt){
  case O_CMAX: 
    plan2graph_O_CMAX(plan,plangraph);
    break;
  case J_CMAX:
    plan2graph_O_CMAX(plan,plangraph);
    break;
  case O_RI_CMAX:
    plan2graph_O_RI_CMAX(plan,plangraph);
    break;
  case J_RI_CMAX:
    plan2graph_J_RI_CMAX(plan,plangraph);
    break;   
  case O_RI_LMAX:
    plan2graph_O_RI_LMAX(plan,plangraph);
    break;
  case J_RI_LMAX:
    plan2graph_J_RI_LMAX(plan,plangraph);
    break;   
  case O_LMAX:
    plan2graph_O_LMAX(plan,plangraph);
    break;
  case J_LMAX:
    plan2graph_J_LMAX(plan,plangraph);
    break;   
  default:
    break;
  }
}

//**************************************************************************

void Lisa_ConvertGraph::graph2plan(Lisa_MatrixGraph* plangraph,
                                   Lisa_Matrix<int>* plan){
                                     
  plan->fill(0);
  
  Lisa_Vector<int> sort(morevert);
  Lisa_GraphAlg::topsort_inverse(plangraph,&sort); 
  
  //cout << sort;
  Lisa_Vector<int> imax(n);
  Lisa_Vector<int> jmax(m);
  imax.fill(0);
  jmax.fill(0);

  int currvert;
  for(int v=0;v<morevert;v++){
    currvert = sort[v];

    if(currvert<=vert){
      int i = (*I_lookup)[currvert];
      int j = (*J_lookup)[currvert];
      
      if (imax[i] > jmax[j]){
	(*plan)[i][j] = ++imax[i];
	jmax[j] = imax[i];
      }else{
	(*plan)[i][j] = ++jmax[j];
	imax[i] = jmax[j];
      }
    }
  }

}
 
//**************************************************************************

void Lisa_ConvertGraph::write(ostream& strm) const{
  cout << "<ConvertGraph>"<< endl;
  cout << *lookup << *I_lookup << *J_lookup << *disjkt;
  cout << "</ConvertGraph>" << endl;
}

//**************************************************************************

void Lisa_ConvertGraph::initialize_J_CMAX(Lisa_Matrix<bool>* SIJ,Lisa_MO* MO){
  morevert = vert;
  // create disjkt
  disjkt = new Lisa_MatrixGraph(vert);
  
  Lisa_Matrix<int> my_mo(n,m);
  MO->write_rank(&my_mo);
  
  for (int i=0;i<n;i++){
    for (int j=0;j<m;j++){
     if ((*SIJ)[i][j]){ //connect it with all other vertice which follow 

       for(int k=i+1;k<n;k++) 
	 if((*SIJ)[k][j])
	   disjkt->insert_edge((*lookup)[i][j],(*lookup)[k][j]);

       int curr = my_mo[i][j];
       for (int k=0;k<m;k++)
	 if (my_mo[i][k]>curr)
	   disjkt->insert_arc((*lookup)[i][j],(*lookup)[i][k]);
     
     }
   }
 }

}

//**************************************************************************

void Lisa_ConvertGraph::initialize_O_CMAX(Lisa_Matrix<bool>* SIJ){
  morevert = vert;
  // create disjkt
  disjkt = new Lisa_MatrixGraph(vert);
  for (int i=0;i<n;i++){
    for (int j=0;j<m;j++){
      if ((*SIJ)[i][j]){ //connect it with all other vertice which follow 
	
	for(int k=i+1;k<n;k++) // same row
	  if((*SIJ)[k][j])
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[k][j]);
	  
	for(int k=j+1;k<m;k++) // same col
	  if((*SIJ)[i][k]) 
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[i][k]);
	 
      }
    }  
  }

}

//**************************************************************************

void Lisa_ConvertGraph::plan2graph_O_CMAX(Lisa_Matrix<int>* plan,
                                          Lisa_MatrixGraph* plangraph){
                                            
  for (int v=1;v<=vert;v++){
    
    int iv = (*I_lookup)[v];
    int jv = (*J_lookup)[v];
    int currop = (*plan)[iv][jv];
   
    for (int i=0;i<n;i++)
      if((*plan)[i][jv] > currop)
	plangraph->insert_arc(v,(*lookup)[i][jv]);

    for (int j=0;j<m;j++)
      if((*plan)[iv][j] > currop)
	plangraph->insert_arc(v,(*lookup)[iv][j]);

  }
}

//**************************************************************************

void Lisa_ConvertGraph::initialize_J_LMAX(Lisa_Matrix<bool>* SIJ,Lisa_MO* MO){
  morevert = vert+n;
 
  // create disjkt
  disjkt = new Lisa_MatrixGraph(morevert);
  
  Lisa_Matrix<int> my_mo(n,m);
  MO->write_rank(&my_mo);
  
  for (int i=0;i<n;i++){
    for (int j=0;j<m;j++){
     if ((*SIJ)[i][j]){ //connect it with all other vertice which follow 

       for(int k=i+1;k<n;k++) 
	 if((*SIJ)[k][j])
	   disjkt->insert_edge((*lookup)[i][j],(*lookup)[k][j]);

       int curr = my_mo[i][j];
       for (int k=0;k<m;k++)
	 if (my_mo[i][k]>curr)
	   disjkt->insert_arc((*lookup)[i][j],(*lookup)[i][k]);

       disjkt->insert_arc((*lookup)[i][j],vert+i+1);
     
     }
    }
    
 }
}

//**************************************************************************

void Lisa_ConvertGraph::initialize_O_LMAX(Lisa_Matrix<bool>* SIJ){
  morevert = vert+n+1;
  // create disjkt
  disjkt = new Lisa_MatrixGraph(morevert);
  for (int i=0;i<n;i++){
    for (int j=0;j<m;j++){
      if ((*SIJ)[i][j]){ //connect it with all other vertice which follow 
	
	for(int k=i+1;k<n;k++) // same row
	  if((*SIJ)[k][j])
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[k][j]);
	  
	for(int k=j+1;k<m;k++) // same col
	  if((*SIJ)[i][k]) 
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[i][k]);
	
	disjkt->insert_edge((*lookup)[i][j],vert+i+1);
     
      }
    } 
   
    disjkt->insert_arc(morevert,vert+i+1);
  }
}

//**************************************************************************

void Lisa_ConvertGraph::plan2graph_O_LMAX(Lisa_Matrix<int>* plan,
                                          Lisa_MatrixGraph* plangraph){
  for (int v=1;v<=vert;v++){
    
    int iv = (*I_lookup)[v];
    int jv = (*J_lookup)[v];
    int currop = (*plan)[iv][jv];
   
    for (int i=0;i<n;i++)
      if((*plan)[i][jv] > currop)
	plangraph->insert_arc(v,(*lookup)[i][jv]);

    for (int j=0;j<m;j++)
      if((*plan)[iv][j] > currop)
	plangraph->insert_arc(v,(*lookup)[iv][j]);
    
    plangraph->insert_arc(v,vert+iv+1);
    
 }
 
 for (int i=vert+1;i<morevert;i++) plangraph->insert_arc(morevert,i);
    
}
//**************************************************************************

void Lisa_ConvertGraph::plan2graph_J_LMAX(Lisa_Matrix<int>* plan,
                                          Lisa_MatrixGraph* plangraph){
  for (int v=1;v<=vert;v++){
    
    int iv = (*I_lookup)[v];
    int jv = (*J_lookup)[v];
    int currop = (*plan)[iv][jv];
   
    for (int i=0;i<n;i++)
      if((*plan)[i][jv] > currop)
	plangraph->insert_arc(v,(*lookup)[i][jv]);

    for (int j=0;j<m;j++)
      if((*plan)[iv][j] > currop)
	plangraph->insert_arc(v,(*lookup)[iv][j]);
    
    plangraph->insert_arc(v,vert+iv+1);
    
 }
    
}

//**************************************************************************

void Lisa_ConvertGraph::initialize_J_RI_CMAX(Lisa_Matrix<bool>* SIJ,
                                             Lisa_MO* MO){
  morevert = vert+n;
 
  // create disjkt
  disjkt = new Lisa_MatrixGraph(morevert);
  
  Lisa_Matrix<int> my_mo(n,m);
  MO->write_rank(&my_mo);
  
  for (int i=0;i<n;i++){
    for (int j=0;j<m;j++){
     if ((*SIJ)[i][j]){ //connect it with all other vertice which follow 

       for(int k=i+1;k<n;k++) 
	 if((*SIJ)[k][j])
	   disjkt->insert_edge((*lookup)[i][j],(*lookup)[k][j]);

       int curr = my_mo[i][j];
       for (int k=0;k<m;k++)
	 if (my_mo[i][k]>curr)
	   disjkt->insert_arc((*lookup)[i][j],(*lookup)[i][k]);

       disjkt->insert_arc(vert+i+1,(*lookup)[i][j]);
     
     }
    }
 }
}

//**************************************************************************

void Lisa_ConvertGraph::initialize_O_RI_CMAX(Lisa_Matrix<bool>* SIJ){
 morevert = vert+n+1;
  // create disjkt
  disjkt = new Lisa_MatrixGraph(morevert);
  for (int i=0;i<n;i++){
    for (int j=0;j<m;j++){
      if ((*SIJ)[i][j]){ //connect it with all other vertice which follow 
	
	for(int k=i+1;k<n;k++) // same row
	  if((*SIJ)[k][j])
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[k][j]);
	  
	for(int k=j+1;k<m;k++) // same col
	  if((*SIJ)[i][k]) 
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[i][k]);
	
	disjkt->insert_edge(vert+i+1,(*lookup)[i][j]);
     
      }
    } 
   
    disjkt->insert_arc(vert+i+1,morevert);
  }
}

//**************************************************************************

void Lisa_ConvertGraph::plan2graph_O_RI_CMAX(Lisa_Matrix<int>* plan,
                                             Lisa_MatrixGraph* plangraph){
  for (int v=1;v<=vert;v++){
    
    int iv = (*I_lookup)[v];
    int jv = (*J_lookup)[v];
    int currop = (*plan)[iv][jv];
   
    for (int i=0;i<n;i++)
      if((*plan)[i][jv] > currop)
	plangraph->insert_arc(v,(*lookup)[i][jv]);

    for (int j=0;j<m;j++)
      if((*plan)[iv][j] > currop)
	plangraph->insert_arc(v,(*lookup)[iv][j]);
    
    plangraph->insert_arc(vert+iv+1,v);
    
 }
 
 for (int i=vert+1;i<morevert;i++) plangraph->insert_arc(i,morevert);
}

//**************************************************************************

void Lisa_ConvertGraph::plan2graph_J_RI_CMAX(Lisa_Matrix<int>* plan,
                                             Lisa_MatrixGraph* plangraph){
  for (int v=1;v<=vert;v++){
    
    int iv = (*I_lookup)[v];
    int jv = (*J_lookup)[v];
    int currop = (*plan)[iv][jv];
   
    for (int i=0;i<n;i++)
      if((*plan)[i][jv] > currop)
	plangraph->insert_arc(v,(*lookup)[i][jv]);

    for (int j=0;j<m;j++)
      if((*plan)[iv][j] > currop)
	plangraph->insert_arc(v,(*lookup)[iv][j]);
    
    plangraph->insert_arc(vert+iv+1,v);
    
 }
}

//**************************************************************************

void Lisa_ConvertGraph::initialize_J_RI_LMAX(Lisa_Matrix<bool>* SIJ,
                                             Lisa_MO* MO){
  morevert = vert+2*n;
 
  // create disjkt
  disjkt = new Lisa_MatrixGraph(morevert);
  
  Lisa_Matrix<int> my_mo(n,m);
  MO->write_rank(&my_mo);
  
  for (int i=0;i<n;i++){
    for (int j=0;j<m;j++){
      if ((*SIJ)[i][j]){ //connect it with all other vertice which follow 
	
	for(int k=i+1;k<n;k++) 
	  if((*SIJ)[k][j])
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[k][j]);
	
	int curr = my_mo[i][j];
	for (int k=0;k<m;k++)
	  if (my_mo[i][k]>curr)
	    disjkt->insert_arc((*lookup)[i][j],(*lookup)[i][k]);
	
	disjkt->insert_arc(vert+i+1,(*lookup)[i][j]);
	disjkt->insert_arc((*lookup)[i][j],vert+n+i+1);
	
	
      }
    }
    
    disjkt->insert_arc(vert+i+1,vert+n+i+1);
  }
}

//**************************************************************************

void Lisa_ConvertGraph::initialize_O_RI_LMAX(Lisa_Matrix<bool>* SIJ){
 morevert = vert+2*(n+1);
  // create disjkt
  disjkt = new Lisa_MatrixGraph(morevert);
  for (int i=0;i<n;i++){
    for (int j=0;j<m;j++){
      if ((*SIJ)[i][j]){ //connect it with all other vertice which follow 
	
	for(int k=i+1;k<n;k++) // same row
	  if((*SIJ)[k][j])
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[k][j]);
	  
	for(int k=j+1;k<m;k++) // same col
	  if((*SIJ)[i][k]) 
	    disjkt->insert_edge((*lookup)[i][j],(*lookup)[i][k]);
	
	disjkt->insert_edge(vert+i+1,(*lookup)[i][j]);
	disjkt->insert_edge((*lookup)[i][j],vert+n+i+2);
      }
    } 

    disjkt->insert_arc(vert+i+1,vert+n+i+2);
    disjkt->insert_arc(vert+i+1,vert+n+1);
    disjkt->insert_arc(morevert,vert+n+i+2);
  }
}

//**************************************************************************

void Lisa_ConvertGraph::plan2graph_O_RI_LMAX(Lisa_Matrix<int>* plan,
                                             Lisa_MatrixGraph* plangraph){
  for (int v=1;v<=vert;v++){
    
    int iv = (*I_lookup)[v];
    int jv = (*J_lookup)[v];
    int currop = (*plan)[iv][jv];
   
    for (int i=0;i<n;i++)
      if((*plan)[i][jv] > currop)
	plangraph->insert_arc(v,(*lookup)[i][jv]);

    for (int j=0;j<m;j++)
      if((*plan)[iv][j] > currop)
	plangraph->insert_arc(v,(*lookup)[iv][j]);
    
    plangraph->insert_arc(vert+iv+1,v);
    plangraph->insert_arc(v,vert+n+iv+2);
  
 }
 
 for (int i=0;i<n;i++){ 
   plangraph->insert_arc(vert+i+1,vert+i+n+2);
   plangraph->insert_arc(vert+i+1,vert+n+1);
   plangraph->insert_arc(morevert,vert+i+n+2);
 }
}

//**************************************************************************

void Lisa_ConvertGraph::plan2graph_J_RI_LMAX(Lisa_Matrix<int>* plan,
                                             Lisa_MatrixGraph* plangraph){
  for (int v=1;v<=vert;v++){
    
    int iv = (*I_lookup)[v];
    int jv = (*J_lookup)[v];
    int currop = (*plan)[iv][jv];
   
    for (int i=0;i<n;i++)
      if((*plan)[i][jv] > currop)
	plangraph->insert_arc(v,(*lookup)[i][jv]);

    for (int j=0;j<m;j++)
      if((*plan)[iv][j] > currop)
	plangraph->insert_arc(v,(*lookup)[iv][j]);
    
    plangraph->insert_arc(vert+iv+1,v);
    plangraph->insert_arc(v,vert+n+iv+1);
  
 }
 
 for (int i=0;i<n;i++){ 
   plangraph->insert_arc(vert+i+1,vert+i+n+1);
 }
}

//**************************************************************************

