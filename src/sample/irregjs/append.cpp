/**
 * @author  Thomas Tautenhahn
 * @version 2.3pre3
 */
 
#include <stdlib.h>

#include "append.hpp"

//**************************************************************************

void ECT_reverse(Lisa_JsSchedule * sched)
{
  int i, bestjob, j;
  TIMETYP lst, start;
  Lisa_JsProblem *P=sched->JSP;
  Lisa_Vector<int> * next_op=new Lisa_Vector<int>(P->n);
  Lisa_Vector<TIMETYP> * m_free=new Lisa_Vector<TIMETYP>(P->m);
 
  for (i=1; i<P->n; i++)
     (*next_op)[i]=sched->GetMOpred(i,SINK);
  m_free->fill(MAXTIME);

  sched->ComputeHeadsTails(FALSE,TRUE);
 
  do
    { 
      // find operation with latest possible start time: 
      bestjob=0;
      lst = -MAXTIME;
      for (i=1; i<P->n; i++)
         {
           j=(*next_op)[i];
           if (j!=SOURCE)
             {
               start=(*m_free)[j]-(*P->time)[i][j];
	       // complete last operations not later than due date:
               if (sched->GetMOsucc(i,j)==SINK)
                 if ((*m_free)[j]>(*P->di)[i])    
		   start=(*P->di)[i]-(*P->time)[i][j];
               if (start>lst) 
                 {
                   bestjob=i;
                   lst=start;
                 }
             }     
         }
 
     // insert it:
     if (bestjob)
       {
	 j=(*next_op)[bestjob];
	 sched->insert(bestjob,j,SOURCE); 
	 if (sched->GetMOsucc(bestjob,j)==SINK)
	   sched->pushstop(bestjob,j,MAXTIME-(*P->di)[bestjob]);
         // update vectors:
         (*next_op)[bestjob]=sched->GetMOpred(bestjob,j);
         (*m_free)[j]=MAXTIME-sched->GetTail(bestjob,j)-(*P->time)[i][j];
       }
    }
  while (bestjob);
  delete next_op;
  delete m_free;

  // repair collisions with time 0:
  for (i=1; i<P->n; i++)
     sched->pushstart(i, sched->GetMOsucc(i,SOURCE),0);
}

//**************************************************************************

