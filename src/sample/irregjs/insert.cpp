/**
 * @author  Thomas Tautenhahn
 * @version 2.3rc1
 */

#include <stdlib.h>
#include <limits.h>

#include "insert.hpp"

//**************************************************************************

const int SHIFT_STEPS=10; 

//**************************************************************************

void JS_Inserter::init(Lisa_JsSchedule * in_sched, int in_zfn)
  {
    sched = in_sched;
    P=sched->JSP;
    sched2=new Lisa_JsSchedule(P);
    zfn=in_zfn;
    order=new Lisa_Order(P->n, P->m);
  }

//**************************************************************************
    
void JS_Inserter::rattle( int job )
{
  TIMETYP delay;
  int last_machine, i;
  
  sched->TailsFromHeads();
  sched->SetValue(zfn);
  (*sched2)=(*sched);
  last_machine=(*(P->MOpred))[job][SINK];
  delay=sched2->GetHead(job,SINK) - (*P->di)[job];
  /*  cout << "job " << job << " has delay " << delay << " and weights "
       << (*P->wi)[job] << " late / " 
       << (*P->wi_early)[job] << " early" << endl; */
  delay=delay/SHIFT_STEPS;

  if (delay > 0) 
    { 
      sched2->ComputeHeadsTails(FALSE,TRUE);
      while (TRUE)
	{
	  //  cout << "trying to make "<< job<<","<<last_machine<<" earlier by " 
	  //	       << delay << endl;;
          sched2->pushstop(job,last_machine,
			   1+sched->GetTail(job,last_machine));
	  for (i=1; i<=P->n; i++)
	    if (sched2->GetTail(i,SOURCE)>MAXTIME)
              {
	        // cout << "collision with time 0" << endl;
                return;
              }
	  sched2->SetValue(zfn);
	  if (sched2->GetValue()>=sched->GetValue())
	    return;
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
	    return;
	  (*sched)=(*sched2);
          sched->TailsFromHeads(); 
	} 
    }
}     

//**************************************************************************
     
void JS_Inserter::insert()
{
  int i, j, inspos, anzop, shiftjob, firstop, k;
  TIMETYP bestvalue;
  Lisa_JsSchedule * best = new Lisa_JsSchedule(P);
  
  simple_order();
  anzop=P->n*P->m;

  for (k=0; k<anzop; k++)
    {     
      i=order->row(k)+1; j=order->col(k)+1;
      bestvalue=LONG_MAX;
      inspos=SOURCE;
      do                                                   // for all inspos
	{ 
	  (*sched2)=(*sched);

	  sched2->ComputeHeadsTails(FALSE,TRUE);
	  if (sched2->insert(i,j,inspos)>=OK)
	    {
              // adjust timeslot for last operations:
	      if (sched2->GetMOsucc(i,j)==SINK)
		sched2->pushstop(i,j,MAXTIME-(*P->di)[i]);
              sched2->HeadsFromTails();
           
              // repair collisions with time 0:
	      sched2->ComputeHeadsTails(TRUE,FALSE);
	      for (shiftjob=1; shiftjob<=P->n; shiftjob++)
		{
		  firstop=sched2->GetMOsucc(shiftjob,SOURCE);
		  if (sched2->GetHead(shiftjob,firstop)<0)
		    sched2->pushstart(shiftjob, firstop, 0);
		}
	      sched2->TailsFromHeads();
	      //  for (shiftjob=1; shiftjob<=P->n; shiftjob++)
	      //	sched2->rattle(shiftjob);
	      sched2->TailsFromHeads();
	      

	      // consider solution:
	      sched2->SetValue(zfn);
	      if (sched2->GetValue()<=bestvalue)
		{
		  bestvalue=sched2->GetValue();
		  (*best)=(*sched2);
		}
            }
	  inspos=sched->GetJOsucc(inspos,j);
	}
      while (inspos!=SINK);
      (*sched)=(*best);
    }
}

//**************************************************************************

void JS_Inserter::simple_order()
  {
    int i, j, k;
    
    for (i=1; i<=P->n; i++)
      for (j=(*P->MOpred)[i][SINK], k=0; j!=SOURCE; j=(*P->MOpred)[i][j],k++)
         order->read_one_key(i-1,j-1, (double) k);
    order->sort();

    for (i=1; i<=P->n; i++)
       for (j=1; j<=P->m; j++)
          order->read_one_key(i-1,j-1,MAXTIME-(*P->time)[i][1]
                                             -(*P->time)[i][2]
           -(*P->time)[i][3]-(*P->time)[i][4]-(*P->time)[i][5]
			      );
    order->sort();
    
   for (i=1; i<=P->n; i++)
       for (j=1; j<=P->m; j++)
          order->read_one_key(i-1,j-1,(double) i);
    order->group();

  } 

//**************************************************************************

JS_Inserter::~JS_Inserter()
  { 
    delete order;
    delete sched2;
  }

//**************************************************************************

void rattle_js( Lisa_JsSchedule * sched)
{
  TIMETYP delay;
  int last_machine, i,job,n;
  Lisa_JsProblem *P=sched->JSP;  
  Lisa_JsSchedule * sched2= new Lisa_JsSchedule(P);
  n=sched->P->n;
  

  for (job=1; job<=n; job++)
    {
      sched->TailsFromHeads();
      sched->SetValue(IRREG2);
      (*sched2)=(*sched);
      last_machine=(*(P->MOpred))[job][SINK];
      delay=sched2->GetHead(job,SINK) - (*P->di)[job];
      
      if (delay > 0) 
	{ 
	  sched2->ComputeHeadsTails(FALSE,TRUE);
	  while (TRUE)
	    {
              sched2->pushstop(job,last_machine,
			       1+sched->GetTail(job,last_machine));
	      for (i=1; i<=P->n; i++)
		if (sched2->GetTail(i,SOURCE)>MAXTIME)
		  goto end_of_loop;
              sched2->HeadsFromTails();  
	      sched2->SetValue(IRREG2);
	      if (sched2->GetValue()>=sched->GetValue())
		goto end_of_loop;
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
	      sched2->SetValue(IRREG2);
	      if (sched2->GetValue()>=sched->GetValue())
		goto end_of_loop;;
	      (*sched)=(*sched2);
	      sched->TailsFromHeads(); 
	    } 
	}
    end_of_loop:
    if (delay);
    }
  delete sched2;
}     

//**************************************************************************

