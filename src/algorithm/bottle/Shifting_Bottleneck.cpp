/**
 * @author  Christian Schulz
 * @version 2.3final
 */

#include <fstream>
#include "../single/Single_Machine_BB.hpp"
#include "Shifting_Bottleneck.hpp"

using namespace std;

//**************************************************************************

Shifting_Bottleneck::Shifting_Bottleneck(Lisa_JsSchedule* pJsSch, bool m)
{ 
  single_machine_mode=m;
  JsPro=pJsSch->JSP;
  JsSch=pJsSch;
  JsSch->ComputeHeadsTails(TRUE,TRUE);
  JsSch->InitHeadsTails();
  Done_Machines=new Lisa_Vector<int> (JsPro->m);
  Done_Machines->fill(0);
  Plan=new Lisa_Graph((JsPro->m)*(JsPro->n));
  init_graph();
}

//**************************************************************************

Shifting_Bottleneck::~Shifting_Bottleneck()
{
  delete JsPro;
  delete Plan;
  delete Done_Machines;
}

//**************************************************************************

void Shifting_Bottleneck::init_graph()
{
  int jobs=(JsPro->n)+1;
  int mach=(JsPro->m);
  int s;  //successor
  int e;  //predecessor

  for(int j=1; j<jobs; j++){
    s=(*(JsPro->MOsucc))[j][0];
    e=(*(JsPro->MOsucc))[j][s];
    while(e!=0){
      Plan->insert_arc(((j-1)*mach)+s,((j-1)*mach)+e);
      s=e;
      e=(*(JsPro->MOsucc))[j][s];
    }
  }
} 

//**************************************************************************

void Shifting_Bottleneck::run(Lisa_Schedule* pSch)
{
  shifting_bottleneck();
  pSch->make_LR();
  JsSch->write_LR(pSch->LR);
}

//**************************************************************************

void Shifting_Bottleneck::shifting_bottleneck()
{ 
  Lisa_Vector<int>* schedule=new Lisa_Vector<int> (JsPro->n);

  //stores the actuall transitive hull of the current Plan
  Lisa_Graph* t=new Lisa_Graph((JsPro->m)*(JsPro->n));  

  double temp=0;
  double Cmax=0;
  double Lmax=0;
  int first=1;
  int machine=0;
  int count=0;
  int size=0;

  //searches JsPro->m (number of machines) -  times 
  //for the next bottleneck machine 

  for(int i=1; i<=(JsPro->m); i++){
    
    first=1;

    //computing of Cmax
    for(int row=1; row<=(JsPro->n); row++){         
      for(int col=1; col<=(JsPro->m); col++){      
	  temp=(JsSch->GetHead(row,col));
	  temp+=(JsSch->GetTail(row,col));
	  temp+=((*JsPro->time)[row][col]);
	  
	  if((first)||(Cmax<temp)){
	    Cmax=temp;
	    first=0;
	  }
      }
    } //end of computing  Cmax
    
    first=1;
    
    // for every undone Machine j solve the special
    // single_machine problem
    
    for(int j=1; j<=(JsPro->m); j++){
      if( (*Done_Machines)[j-1] == 0){
	count=0;
	
	// count the jobs on machine j
	for(int job=1; job<=(JsPro->n); job++){
	  if((*JsPro->sij)[job][j]){
	    count++;
	  }
	}
	
	// create arrays with the right size, to store parameters
	// for the single machine problems
	
	Lisa_Vector<int>* jobNumbers=new Lisa_Vector<int> (count);
	Lisa_Matrix<double>* pt=new Lisa_Matrix<double> (count,1);
	Lisa_Vector<double>* rd=new Lisa_Vector<double> (count);
	Lisa_Vector<double>* dd=new Lisa_Vector<double> (count);
	Lisa_Graph* pg=new Lisa_Graph(count);
	            
	// assign new numbers to the jobs, that will be done 
	// on machine j
	// (*jobNumbers[i]) contains job i s original number

	for(int job=1, newJobNumber=0; job<=(JsPro->n); job++){
	  if((*JsPro->sij)[job][j]){
	    (*jobNumbers)[newJobNumber]=job;
	    newJobNumber++;
	  }
	}
	      
	// fill the arrays pt, rd, dd, which are used as parameters
	// for single machine problem
	
	for(int job=0; job<count; job++){
	  (*rd)[job]=JsSch->GetHead((*jobNumbers)[job],j);
	  (*dd)[job]=Cmax-(JsSch->GetTail((*jobNumbers)[job],j));
	  (*pt)[job][0]=(*JsPro->time)[(*jobNumbers)[job]][j];
	}
	      
	// creates the neccessary Lisa_Values object, to initialize 
	// the single machine problem

	Lisa_Values* pLV=new Lisa_Values();
	pLV->init(count,1);
	pLV->PT=pt;
	pLV->RD=rd;
	pLV->DD=dd;

	// look for possible priorities within the transitive hull t 
	// of the current Plan, store them in Lisa_Graph pg
	// the integers  k1, k2, c1 and c2 are used to switch 
	// between original and new jobnumbers 

	int k1=0;
	int k2=0;
   
	for(int c1=0; c1<count; c1++){
	  k1=(((*jobNumbers)[c1]-1)*(JsPro->m))+j;
	  for(int c2=0; c2<count; c2++){
	    k2=(((*jobNumbers)[c2]-1)*(JsPro->m))+j;
	    if(ARC==(t->get_connection(k1,k2))){
	      pg->insert_arc(c1+1,c2+1);
	    }
	  }
	}
       
	// set up and solve the single machine problem
	SingleMachineBB* smBB=new SingleMachineBB( pLV, pg, single_machine_mode ); 
	Lisa_Schedule* pSchedule=new Lisa_Schedule(count,1);
	cout<<"step="<<i<<" machine= "<<j<<endl;
	smBB->run(pSchedule);
	temp=smBB->get_bestLmax();

	// select the machine j with lowest Lmax

	if(first||Lmax<temp){
	  Lmax=temp;
	  first=0;
	  size=count;
	  for(int s=0; s<size; s++){
	    (*schedule)[s]=(*jobNumbers)[(*smBB->get_bestSchedule())[s]];
	    machine=j;
	  }
	}

	delete smBB;
	delete jobNumbers;
	delete pLV; // includes delete pt; delete rd; delete dd;
	delete pg;
      }      
    } //end of single machine problem 
    

    // extend the Plan by the new ARCs, 
    // according to the computed schedule
    
    int s=(*schedule)[0];
    int e=0;
    int knot_s=((s-1)*(JsPro->m))+machine;
    int knot_e=0;
    
    
    for(int p=1; p<size; p++){
      e=(*schedule)[p];
      knot_e=((e-1)*(JsPro->m))+machine;
      Plan->insert_arc(knot_s,knot_e);
      s=e;
      knot_s=knot_e;
    }


    // update the transitive hull t, 
    // to get the new priorities for the next single machine problem 

    Plan->transitive_hull(t);

    // store the schedule in a JobshopSchedule,
    // computes the new HEADS and TAILS
    
    for(int p=size-1; p>=0; p--){
      JsSch->insert((*schedule)[p] ,machine, SOURCE);
    }

    (*Done_Machines)[machine-1]=1;
    Cmax=Cmax+Lmax;
  } //for(i)
  
  delete schedule;
  delete t;
}

//**************************************************************************

