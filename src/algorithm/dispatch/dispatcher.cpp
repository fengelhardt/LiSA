/**
 * @author  Thomas Tautenhahn
 * @version 2.3final
 */

#include <time.h>

#include "dispatcher.hpp"

using namespace std;

//**************************************************************************
// helper methods: 

/**
 * return the earliest possible completion time of operation (i,j) 
 * with respect to the existing partial schedule
 */
TIMETYP Lisa_Dispatcher::getECT(int i, int j)
{
  return MAX((*jstart)[i],(*mstart)[j])+(*problem->time)[i][j];
}

/**
 * return the earliest possible starting time of operation (i,j) 
 * with respect to the existing partial schedule
 */
TIMETYP Lisa_Dispatcher::getEST(int i, int j)
{
  return MAX((*jstart)[i],(*mstart)[j]);
}

/**
 * return earliest possible completion time of any unscheduled operation 
 * with respect to the existing partial schedule
 */
TIMETYP Lisa_Dispatcher::getEECT() 
{
  TIMETYP rv=MAXTIME;
  for (int i=1; i<=n; i++)
	for (int j=1; j<=m; j++)
	  if ((!schedule->exists(i,j))&& (getECT(i,j)<rv)&&
              (*problem->sij)[i][j])
	    {
	      rv=getECT(i,j);
	    }
  return rv;
}


//**************************************************************************

TIMETYP Lisa_Dispatcher::priority(int i, int j)
{
  // sometimes we add one to make sure that priority>-MAXTIME holds
  switch(rule)
    {
    case RAND: return lisa_random(0,1000,&myseed);
    case FCFS: return -schedule->GetHead(i,j);
    case EDD : return -(*problem->di)[i]+1;
    case LQUE: if (my_problem==J)
                 return 1-(*problem->di)[i]+schedule->GetTail(i,j)
	                +(*problem->time)[i][j];
               else 
		 {
		   TIMETYP slack=1-(*problem->di)[i];
		   for (int k=1; k<=m; k++)
		     if (!schedule->exists(i,k))
		       slack+=(*problem->time)[i][k];
		   TIMETYP machineRemainingLoad=0;
		   for (int l=1; l<=n; l++)
		     if (!schedule->exists(l,j))
		       machineRemainingLoad+=(*problem->time)[l][j];
		   return slack-1/machineRemainingLoad;
		 }
    case SPT : return -(*problem->time)[i][j];
    case WSPT: return (*problem->wi)[i]/(*problem->time)[i][j];
    case ECT : return -getECT(i,j);
    case WI  : return (*problem->wi)[i]; 
    case LPT : return (*problem->time)[i][j];
    }
  return 0;
}  

//**************************************************************************

Lisa_Dispatcher::Lisa_Dispatcher()
{
  my_problem=ERROR;
  problem=NULL; schedule=NULL;
  mstart=jstart=NULL;
  rule=SPT;
  myseed=time(0);
} 

//**************************************************************************

Lisa_Dispatcher::~Lisa_Dispatcher()
{
  delete problem;
  delete schedule;
  delete jstart;
  delete mstart; 
}

//**************************************************************************

bool Lisa_Dispatcher::SetProblem(Lisa_ProblemType* PT, 
                                 Lisa_Values* LValues, Lisa_Schedule* s)
{
  LSchedule=s;
  if (!LSchedule->LR) LSchedule->make_LR();
  
  delete problem;
  delete schedule;
  n=LValues->get_n();  m=LValues->get_m();
  switch (PT->get_property(M_ENV))
    { 
    case O:
      problem= new Lisa_OsProblem(LValues);
      my_problem=O;
      break;
    case ONE:
    case J:
    case F:
      problem= new Lisa_JsProblem(LValues);
      my_problem=J;
      if (PT->get_property(M_ENV)==ONE)
	G_ExceptionList.lcatch(Lisa_ExceptionList::INCONSISTENT_INPUT);      
      break;
    default: G_ExceptionList.lthrow("wrong problemtype for Lisa_Dispatcher");
      return false;
    }
  delete mstart; delete jstart;
  jstart = new Lisa_Vector<TIMETYP>(problem->n+1);
  mstart = new Lisa_Vector<TIMETYP>(problem->m+1);
  return true;
}

//**************************************************************************
    
void Lisa_Dispatcher::SetRule(string r)
{
  int i;
  for (i=0; i<NUMBER_RULES; i++)
    if (RULE_NAMES[i]==r) 
      {
	rule=i;
	return;
      }
  G_ExceptionList.lthrow("unknown name of priority rule, assuming SPT");
}

//**************************************************************************

void Lisa_Dispatcher::js_dispatch_nondelay()
{
  int i,j;
  float maxprio;
  // vector of next operations of all jobs:
  Lisa_Vector<int> * nextm = new Lisa_Vector<int>(n+1);
  
  // initialize set of waiting operations:
  (*jstart)=(*problem->ri);
  mstart->fill(0);
  for (i=1; i<=n; i++)
    (*nextm)[i]=(*((Lisa_JsProblem*)problem)->MOsucc)[i][SOURCE];
     
  // initialize schedule:
  delete schedule;
  schedule=new Lisa_JsSchedule((Lisa_JsProblem*)problem);
  if (rule==LQUE) schedule->ComputeHeadsTails(true,true);
  
  // append operations to the schedule:
  TIMETYP time;
  do 
    {
      // find next time to schedule something:
      time=MAXTIME;
      for (j=1; j<=n; j++)
	if (((*nextm)[j]!=SINK)&&((*jstart)[j]<time))
	  time=(*jstart)[j];
      
      // choose one of the available operation:
      maxprio=-MAXTIME; i=SINK;      
      for (j=1; j<=n; j++)
	if (((*nextm)[j]!=SINK)&&((*jstart)[j]==time)&&
            (priority(j,(*nextm)[j])>maxprio))
	  {
	    maxprio=priority(j,(*nextm)[j]);
	    i=j;
	  }
      
      if (i!=SINK)
	{
	  // append operation:
	  ((Lisa_JsSchedule*)schedule)
	    ->insert(i,(*nextm)[i], schedule->GetJOpred(SINK,(*nextm)[i]));
	  time+=(*problem->time)[i][(*nextm)[i]];
          for (j=1; j<=n; j++)
	    if (((*nextm)[j]==(*nextm)[i])&& ((*jstart)[j]<time)) 
	      (*jstart)[j]=time;
          (*mstart)[(*nextm)[i]]=time; 
	  // include new operation into set:
          (*nextm)[i]=(*((Lisa_JsProblem*)problem)->MOsucc)[i][(*nextm)[i]];
          if ( (*nextm)[i] != SINK)
	    {
	      if ((*mstart)[(*nextm)[i]]>(*jstart)[i])
                 (*jstart)[i]=(*mstart)[(*nextm)[i]];
            }
	}
    } while(i!=SINK);
}

//**************************************************************************

void Lisa_Dispatcher::js_dispatch_active()
{
  Lisa_Vector<int> * nextMachine = new Lisa_Vector<int>(n+1);
  for (int i=1; i<=n; i++)
      (*nextMachine)[i]=(*((Lisa_JsProblem*)problem)->MOsucc)[i][SOURCE];

  (*jstart)=(*problem->ri);
  mstart->fill(0);

  // initialize schedule:
  delete schedule;
  schedule=new Lisa_JsSchedule((Lisa_JsProblem*)problem);
  if (rule==LQUE) schedule->ComputeHeadsTails(true,true);
 
  // append operations to the schedule:
  int choosenJob=SINK;
  do 
    {
      TIMETYP latestStart=MAXTIME;
      for (int i=1; i<=n; i++)
	if (((*nextMachine)[i]!=SINK)&& 
            (getECT(i,(*nextMachine)[i])<latestStart))
	  {
	    latestStart=getECT(i,(*nextMachine)[i]);
	  }
      
      // choose one of the available operation:
      float maxprio=-MAXTIME; 
      choosenJob=SINK;      
      for (int i=1; i<=n; i++)
	{
	  int j=(*nextMachine)[i];
	  if ( (j!=SINK) && (priority(i,j)>maxprio) && 
	       (getEST(i,j)<latestStart) )
	    {
	      maxprio=priority(i,j);
	      choosenJob=i;
	    }
	}
      
      if (choosenJob!=SINK)
	{
	  // append operation:
	  int i=choosenJob;
          int j=(*nextMachine)[choosenJob];
	  ((Lisa_JsSchedule*)schedule)->
	    insert(i,j, schedule->GetJOpred(SINK,j));
          
	  (*jstart)[i]=(*mstart)[j]=
	    MAX((*jstart)[i],(*mstart)[j])+(*problem->time)[i][j];
	  
          // include new operation into set:
          (*nextMachine)[i]=(*((Lisa_JsProblem*)problem)->MOsucc)[i][j];
	}
    } while(choosenJob!=SINK);
}

//**************************************************************************

void Lisa_Dispatcher::js_dispatch()
{
  int i,j;
  float maxprio;
  // vector of next operations of all jobs:
  Lisa_Vector<int> * nextm = new Lisa_Vector<int>(n+1);
  if (rule==ECT)
    {
      (*jstart)=(*problem->ri);
      mstart->fill(0);
    }  
  // initialize set of waiting operations:
  
  for (i=1; i<=n; i++)
      (*nextm)[i]=(*((Lisa_JsProblem*)problem)->MOsucc)[i][SOURCE];
  
  // initialize schedule:
  delete schedule;
  schedule=new Lisa_JsSchedule((Lisa_JsProblem*)problem);
  if (rule==LQUE) schedule->ComputeHeadsTails(true,true);
 
  // append operations to the schedule:
  do 
    {
      // choose one of the available operation:
      maxprio=-MAXTIME; i=SINK;      
      for (j=1; j<=n; j++)
	if (((*nextm)[j]!=SINK)&& (priority(j,(*nextm)[j])>maxprio))
	  {
	    maxprio=priority(j,(*nextm)[j]);
	    i=j;
	  }
      
      if (i!=SINK)
	{
	  // append operation:
          j=(*nextm)[i];
	  ((Lisa_JsSchedule*)schedule)
	    ->insert(i,j, schedule->GetJOpred(SINK,j));
          if (rule==ECT)
	    (*jstart)[i]=(*mstart)[j]=MAX((*jstart)[i],(*mstart)[j])+
	      (*problem->time)[i][j];
	  
          // include new operation into set:
          (*nextm)[i]=(*((Lisa_JsProblem*)problem)->MOsucc)[i][j];
	}
    } while(i!=SINK);
}

//**************************************************************************

void Lisa_Dispatcher::os_dispatch_nondelay()
{
  int i=0,j=0, k,l;
  float maxprio;
  Lisa_Matrix<TIMETYP> * opstart=new Lisa_Matrix<TIMETYP>(n+1,m+1);
  
  // initialize starting times:
  (*jstart)=(*problem->ri);
  mstart->fill(0);
  for (i=1; i<=n; i++)
    for (j=1; j<=m; j++)
      (*opstart)[i][j]= (*problem->sij)[i][j] ? (*problem->ri)[i]: MAXTIME;
  
  // initialize schedule:
  delete schedule;
  schedule=new Lisa_OsSchedule((Lisa_OsProblem*)problem);
  if (rule==LQUE) schedule->ComputeHeadsTails(true,true);
  
  // append operations to the schedule:
  TIMETYP time;
  do 
    {
      // find next time to schedule something:
      time=MAXTIME;
      for (i=1; i<=n; i++)
	for (j=1; j<=m; j++)
	  if ((*opstart)[i][j]<time)
	    time=(*opstart)[i][j];
      // choose one of the available operations:
      if (time<MAXTIME)
	{
	  maxprio=-MAXTIME;       
	  for (k=1; k<=n; k++)
	    for (l=1; l<=m; l++)
	      if (((*opstart)[k][l]==time)&& (priority(k,l)>maxprio))
		{
		  maxprio=priority(k,l);
		  i=k; j=l;
		}
	  
          // append operation:
	  ((Lisa_OsSchedule*)schedule)
	    ->insert(i,j, schedule->GetJOpred(SINK,j),
		     schedule->GetMOpred(i,SINK));
	  time+=(*problem->time)[i][j];
          (*jstart)[i]=time;
          (*mstart)[j]=time;
	  for (k=1; k<=n; k++) (*opstart)[k][j]=MAX((*opstart)[k][j],time);
          for (k=1; k<=m; k++) (*opstart)[i][k]=MAX((*opstart)[i][k],time);
	  (*opstart)[i][j]=MAXTIME;
	}
    } while(time<MAXTIME);
  delete opstart;
}

//**************************************************************************

void Lisa_Dispatcher::os_dispatch_active()
{
  int i=0,j=0, k,l;
  float maxprio;
  
  // initialize schedule:
  delete schedule;
  schedule=new Lisa_OsSchedule((Lisa_OsProblem*)problem);
  if (rule==LQUE) schedule->ComputeHeadsTails(true,true);

  (*jstart)=(*problem->ri);
  mstart->fill(0);

  // append operations to the schedule:
  do 
    {
      TIMETYP latestStart=getEECT();
      // choose one of the available operation:
      maxprio=-MAXTIME; 
      for (k=1; k<=n; k++)
	for (l=1; l<=m; l++)
	  if ((!schedule->exists(k,l))&& (priority(k,l)>maxprio)&&
	      getEST(k,l)<latestStart &&
              (*problem->sij)[k][l])
	    {
	      maxprio=priority(k,l);
	      i=k; j=l;
	    }
      if (maxprio!=-MAXTIME)
	{
	  // append operation:
	  ((Lisa_OsSchedule*)schedule)
	    ->insert(i,j, schedule->GetJOpred(SINK,j),
                          schedule->GetMOpred(i,SINK));
	  
	  (*jstart)[i]=(*mstart)[j]=MAX((*jstart)[i],(*mstart)[j])+
	      (*problem->time)[i][j];
	}
    } while(maxprio!=-MAXTIME);
}

//**************************************************************************

void Lisa_Dispatcher::os_dispatch()
{
  int i=0,j=0, k,l;
  float maxprio;
  
  // initialize schedule:
  delete schedule;
  schedule=new Lisa_OsSchedule((Lisa_OsProblem*)problem);
  if (rule==LQUE) schedule->ComputeHeadsTails(true,true);

  if (rule==ECT)
    {
      (*jstart)=(*problem->ri);
      mstart->fill(0);
    } 

  // append operations to the schedule:
  do 
    {
      // choose one of the available operation:
      maxprio=-MAXTIME; 
      for (k=1; k<=n; k++)
	for (l=1; l<=m; l++)
	  if ((!schedule->exists(k,l))&& (priority(k,l)>maxprio)&&
              (*problem->sij)[k][l])
	    {
	      maxprio=priority(k,l);
	      i=k; j=l;
	    }
      if (maxprio!=-MAXTIME)
	{
	  // append operation:
	  ((Lisa_OsSchedule*)schedule)
	    ->insert(i,j, schedule->GetJOpred(SINK,j),
                          schedule->GetMOpred(i,SINK));
	  if (rule==ECT)
	    (*jstart)[i]=(*mstart)[j]=MAX((*jstart)[i],(*mstart)[j])+
	      (*problem->time)[i][j];
	}
    } while(maxprio!=-MAXTIME);
}

//**************************************************************************
  
void Lisa_Dispatcher::dispatch()
{
  if (my_problem==J) js_dispatch();
  if (my_problem==O) os_dispatch();
  if (my_problem==ERROR) 
    G_ExceptionList.lthrow("cannot dispatch without defined problem");
  // write the result to the output object
  schedule->write_LR(LSchedule->LR);
}

//**************************************************************************
 
void Lisa_Dispatcher::dispatch_active()
{
  if (my_problem==J) js_dispatch_active();
  if (my_problem==O) os_dispatch_active();
  if (my_problem==ERROR) 
    G_ExceptionList.lthrow("cannot dispatch without defined problem");
  // write the result to the output object
  schedule->write_LR(LSchedule->LR);
}

//**************************************************************************

void Lisa_Dispatcher::dispatch_nondelay()
{
  if (my_problem==J) js_dispatch_nondelay();
  if (my_problem==O) os_dispatch_nondelay();
  if (my_problem==ERROR) 
    G_ExceptionList.lthrow("cannot dispatch without defined problem");
  // write the result to the output object
  schedule->write_LR(LSchedule->LR);
}

//**************************************************************************

