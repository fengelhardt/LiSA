/*
 * ******************** ijs_nb.cpp ******************************
 * 
 * description:      job shop API neighborhood with "rattling"
 * 
 * owner:            Tautenhahn
 *
 * date:             23.2.1999
 *
 */

#include "ijs_nb.hpp"

int Lisa_IjsApi::do_move()
{
  int rv;
  rv=JSHOP_API_Ngbh::do_move();
  
  if (rv!=OK) return rv;
  TIMETYP delay;
  int last_machine, i, job;
  
  for (job=1; job <=PP->n; job++)
    {
      sched->TailsFromHeads();
      sched->SetValue(zfn);
      (*sched2)=(*sched);
      last_machine=(*(PP->MOpred))[job][SINK];
      delay=sched2->GetHead(job,SINK) - (*PP->di)[job];
      cout << "job " << job << " has delay " << delay << " and weights "
	   << (*PP->wi)[job] << " late / " 
	   << (*PP->wi_early)[job] << " early" << endl;
      
      if (delay > 0) 
	{ 
	  sched2->ComputeHeadsTails(FALSE,TRUE);
	  while (TRUE)
	    {
	      //  cout << "trying to make "<< job<<","<<last_machine<<" earlier by " 
	      //	       << delay << endl;;
	      sched2->pushstop(job,last_machine,
			       1+sched->GetTail(job,last_machine));
	      for (i=1; i<=PP->n; i++)
		if (sched2->GetTail(i,SOURCE)>MAXTIME)
		  {
		    // cout << "collision with time 0" << endl;
		    return OK;
		  }
	      sched2->SetValue(zfn);
	      if (sched2->GetValue()>=sched->GetValue())
		return OK;
	      (*sched)=(*sched2);
	      sched->HeadsFromTails();
	    } 
	}
      if (delay<0)
	{ 
	  sched2->ComputeHeadsTails(TRUE,FALSE);
	  while (TRUE)
	    {
	      sched2->pushstart(job,last_machine,
				sched->GetHead(job,last_machine)+1);
	      sched2->SetValue(zfn);
	      if (sched2->GetValue()>=sched->GetValue())
		return OK;
	      (*sched)=(*sched2);
	      sched->TailsFromHeads(); 
	    } 
	}
    }
  return OK;
}





