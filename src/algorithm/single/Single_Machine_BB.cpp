/**
 * @author  Christian Schulz
 * @version 2.3pre3
 */
 
#include "Single_Machine_BB.hpp"

//**************************************************************************

SingleMachineBB::SingleMachineBB(Lisa_Values* pValues, bool m, Lisa_Graph* pgraph)
{
 
  n=pValues->get_n();
  mode=m;
  FirstSchedule=true;
  priority=pgraph;
  pLV=pValues;
  Done=new Lisa_Vector<bool> (n);
  Done->fill(false);
  Sources=new Lisa_Vector<int> (n);
  Schedule=new Lisa_Vector<int> (n);
  bestSchedule=new Lisa_Vector<int> (n);
  maxDD=(*pLV->DD)[(pLV->DD->index_of_max())];
  maxRD=(*pLV->RD)[(pLV->RD->index_of_max())];
}

//**************************************************************************

SingleMachineBB::SingleMachineBB(Lisa_Values* pValues, Lisa_Graph* pgraph, bool m)
{
  n=pValues->get_n();
  mode=m;
  FirstSchedule=true;
  priority=pgraph;
  pLV=pValues;
  Done=new Lisa_Vector<bool> (n);
  Done->fill(false);
  Sources=new Lisa_Vector<int> (n);
  Schedule=new Lisa_Vector<int> (n);
  bestSchedule=new Lisa_Vector<int> (n);
  maxDD=(*pLV->DD)[(pLV->DD->index_of_max())];
  maxRD=(*pLV->RD)[(pLV->RD->index_of_max())];
}

//**************************************************************************

SingleMachineBB::~SingleMachineBB()
{
  delete Done;
  delete Sources;
  delete bestSchedule;
  delete Schedule;
}

//**************************************************************************

bool SingleMachineBB::run(Lisa_Schedule* pSchedule)
{
  if(priority!=0){
    Lisa_Vector<int>* order=new Lisa_Vector<int> (n);
    order->fill(0);
    if(priority->topsort(order)){
      delete order;
    }
    else{
      delete order;
      return false;
    }
    
    for(int i=0; i<n; i++){
      (*Sources)[i]=priority->number_of_pred(i+1);
    }
  }
  else{
    Sources->fill(0);
  }
  branching();
  pSchedule->make_LR();
  for(int i=0; i<n; i++){
    (*pSchedule->LR)[(*bestSchedule)[i]][0]=i+1;
  }
  return true;
}

//**************************************************************************

bool SingleMachineBB::branching(int k)
{ 
  if(k<n){  
    int counter=0;
    Lisa_Vector<int>* s=new Lisa_Vector<int> (n);
    
    // count jobs, that are undone and 
    // do not have any predecessors within the priority graph 
    for(int i=0; i<n; i++){
      (*s)[i]=(*Sources)[i];
      if((!(*Done)[i])&&((*Sources)[i]==0)){
	counter++;
      }
    }

    // compute lower bounds for all possible jobs

    Lisa_Vector<double>* lbs=new Lisa_Vector<double> (counter);

    for(int i=0, j=0; i<n; i++){
      // choose only released undone jobs, without predecessors
      if((!(*Done)[i])&&((*Sources)[i]==0)){
	(*Schedule)[k]=i;
	(*Done)[i]=true;
	if(priority!=0){
	  // decrement number of predecessors for every
	  // connected job
	  for(int h=0; h<n; h++){
	    if(h!=i){
	      if(ARC==(priority->get_connection(i+1,h+1))){
		(*Sources)[h]=(*Sources)[h]-1;
	      }
	    }
	  }
	} //if(priority)
	// lower bound computation for choosen job
	// at k-th position in Schedule
	(*lbs)[j]=lb(k);
	// set up values for next lower bound computation
	for(int h=0; h<n; h++){
	  (*Sources)[h]=(*s)[h];
	}
	(*Done)[i]=false;
	j++;
      } //if
    }
    
    // compare minimum of lower bounds 
    // with lower bound of bestSchedule (bestLmax)
    bool speed=true;
    double minMax=(*lbs)[lbs->index_of_min()];
    
    if((bestLmax<=minMax)&&(!FirstSchedule)){
      return true;
    }
    
    // choose first job with minimum of lower bounds
    // and branch one step further
            
    for(int i=0, j=0; i<n; i++){
      if((!(*Done)[i])&&((*Sources)[i]==0)&&speed){
	if((*lbs)[j]==minMax){
	  //job=i;
	  speed=mode;
	  if(priority!=0){
	    // update predecessors
	    for(int h=0; h<n; h++){
	      if(h!=i){
		if(ARC==(priority->get_connection(i+1,h+1))){
		  (*Sources)[h]=(*Sources)[h]-1;
		}
	      }
	    }
	  } //if(priority)
	  
	  (*Schedule)[k]=i;
	  (*Done)[i]=true;

	  branching(k+1);
	  
	  for(int h=0; h<n; h++){
	    (*Sources)[h]=(*s)[h];
	  }
	  
	  (*Done)[i]=false;

	} //if(LowerBounds==minMax)  
	j++;
      }                 //if
    }                   //for
    delete s;
    delete lbs;
  }                    //if k<n
  if(k==n){
    newSchedule();
  }
  return true;
}

//**************************************************************************

double SingleMachineBB::lb(int k)
{
  int job=0;
  double t1=0;
  double t2=0;
  int rest=n;
  // all 3 vectors store job attributes
  Lisa_Vector<double>* rest_time=new Lisa_Vector<double>(n);
  Lisa_Vector<double>* lmax=new Lisa_Vector<double>(n);
  Lisa_Vector<bool>*   done=new Lisa_Vector<bool>(n);
  done->fill(false);
  
  // sets rest time of every jobs to proccessing time   
  for(int i=0; i<n; i++){
    (*rest_time)[i]=(*pLV->PT)[i][0];
  }

  // fill the 3 job attribute vectors

  for(int i=0; i<=k; i++){
    job=(*Schedule)[i];
    // sets rest time of every done job to zero 
    (*rest_time)[job]=0;
    // which jobs are already done
    (*done)[job]=true;
    rest--;
    // start time t1 for premption
    if((*pLV->RD)[job]>t1){
      t1=(*pLV->RD)[job]+(*pLV->PT)[job][0];
    }
    else{
      t1=t1+(*pLV->PT)[job][0];
    }
    // Lmax value of every done job  
    (*lmax)[job]=t1-(*pLV->DD)[job];  
  }
   
  // Lmax for eyery undone job
  
  double max;
  int aNewJob=0;
  
  while(rest!=0){
    max=0;
    aNewJob=0;

    // selection of next job, dependant to its predecessors
      
    for(int i=0; i<n; i++){
      // test if job i is available, compare its due date
      if( (!(*done)[i] )&&( (*pLV->RD)[i]<=t1 )&&( (*Sources)[i]==0 )&&
	  ( ( (*pLV->DD)[i]<max)|| (aNewJob==0) ) ) {
	job=i;	
	max=(*pLV->DD)[i];
	aNewJob=1;
      }                    //if
    }                      //for
    
    if(aNewJob==1){
      
      // dertermine time t2 for next interruption      
      t2=t1+(*rest_time)[job];
            
      for(int i=0; i<n; i++){
          if( ((*Sources)[i]==0) || (!(*done)[i]) ){
              if (((*pLV->RD)[i]>t1) && ((*pLV->RD)[i]<t2)){
                  t2=(*pLV->RD)[i];
              }
          }
      }
      
      // for the time t2-t1, the job is done  
      (*rest_time)[job]=(*rest_time)[job]-(t2-t1);

      // test if job is completly done
      if((*rest_time)[job]<=0){
	(*done)[job]=true;
	// Lmax
	(*lmax)[job]=t2-(*pLV->DD)[job];
	rest--;
	// update priorities
	if(priority!=0){
	  for(int j=0; j<n; j++){
	    if(j!=job){
	      if(ARC==(priority->get_connection(job+1,j+1))){
		(*Sources)[j]=(*Sources)[j]--;
	      }
	    }
	  }
	}
      }
    }  //if (aNewJob==1)
    else{
      // no new job
      t2=maxRD;
      for(int i=0; i<n; i++){
	if((*Sources)[i]==0){
	  if(((*pLV->RD)[i]>t1) && ((*pLV->RD)[i]<t2)){
	    t2=(*pLV->RD)[i];
	  }
	}
      }
    }//if(aNewJob)
        
    t1=t2;
  } //while rest
  
  //  get maximum of Lmax 
  double maxLmax=(*lmax)[lmax->index_of_max()];

  delete lmax;
  delete done;
  delete rest_time;

  return maxLmax;
  
}

//**************************************************************************

void SingleMachineBB::newSchedule()
{
  // Lmax value of Schedule;

  int job=(*Schedule)[0];
  double time=(*pLV->RD)[job]+(*pLV->PT)[job][0];
  double max=time-(*pLV->DD)[job];
  
  for(int i=1; i<n; i++){
    job=(*Schedule)[i];
    if((*pLV->RD)[job]>time){
      time=(*pLV->RD)[job]+(*pLV->PT)[job][0];
    }
    else{
      time=time+(*pLV->PT)[job][0];
    }
    if((time-(*pLV->DD)[job])>max){
      max=time-(*pLV->DD)[job];
    }
  }
  
  // compare with Lmax of bestSchedule (bestLmax) 
  
  if(FirstSchedule||(max<bestLmax)){
    // Schedule better than bestSchedule
    FirstSchedule=false;
    bestLmax=max;
    for(int i=0; i<n; i++){
      (*bestSchedule)[i]=(*Schedule)[i];
    }
  }
}

//**************************************************************************

