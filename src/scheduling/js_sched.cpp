/**
 * @author  Thomas Tautenhahn
 * @version 2.3final
 */

#include "../main/global.hpp"
#include "../basics/order.hpp"

#include "js_sched.hpp"

using namespace std;

//**************************************************************************

Lisa_JsProblem::Lisa_JsProblem(const Lisa_Values * Pin) : Lisa_ShpProblem( Pin ) 
   {
     int i, j;
     if ( !(MOsucc = new Lisa_Matrix<int>( n+1, m+1 )) )
       cerr << "\nError: no more memory";
     if ( !(MOpred = new Lisa_Matrix<int>( n+1, m+1 )) )
       cerr << "\nError: no more memory";
     MOsucc->fill(SINK);
     MOpred->fill(SOURCE);

     // copy machine orders
     if (!Pin->MO) 
       {
	 // if used as 1 machine problem create MOs
	 if (m==1)                 
	   for (i=1; i<=n; i++)
	     (*MOsucc)[i][SOURCE]=(*MOpred)[i][SINK]=1;
	 else
	   {
	     G_ExceptionList.lthrow("no machine orders, assuming flow shop",
				                      Lisa_ExceptionList::WARNING);
	     // create MOs to make it a flowshop
	     for (i=1; i<=n; i++)
	       {
		 for (j=0; j<n; j++) (*MOsucc)[i][j]=j+1;
		 (*MOsucc)[i][n]=SINK;
                 for (j=1; j<=n; j++) (*MOpred)[i][j]=j-1;
		 (*MOpred)[i][SINK]=n;
	       }
	   }
       }	     
     else
       { 
         int j_old;
         for ( i=0; i<n; i++ )
            { 
	      j_old = MO_SOURCE;
	      j=Pin->MO->succ( i, j_old );
              while ( j!=NO_SUCCESSOR )
	           {
	             (*MOsucc)[i+1][j_old+1] = j+1;
	             (*MOpred)[i+1][j+1] = j_old+1;
	             j_old = j;
	             j=Pin->MO->succ( i, j_old );
	           }
	      (*MOpred)[i+1][SINK]=j_old+1;
            }
       }

     // make sure that SIJ is correct
     for ( i=1; i<=n; i++ )
       for ( j=1; j<=m; j++ )
            (*sij)[i][j]= ((*MOsucc)[i][j]!=SINK) || ((*MOpred)[i][SINK]==j);
     if (Pin->SIJ)
       {
	 for ( i=1; i<=n; i++ )
	   for ( j=1; j<=m; j++ )
	     if ((*Pin->SIJ)[i-1][j-1] != (*sij)[i][j]) 
         G_ExceptionList.lthrow("given job orders do not fit operation set", 
		                            Lisa_ExceptionList::INCONSISTENT_INPUT);
       }
   }

//**************************************************************************

Lisa_JsProblem::~Lisa_JsProblem()
   {
     delete MOsucc;
     delete MOpred;
   }

//**************************************************************************

// returns an empty feasible schedule to *Pin with MIJ=emptyset 
Lisa_JsSchedule::Lisa_JsSchedule(const Lisa_JsProblem* Pin)
     { 
       int  i,j;
       ComputeTails = false;
       ComputeHeads = true;
       P = JSP = Pin;
       MOsucc = JSP->MOsucc;
       MOpred = JSP->MOpred;
       if ( !(JOsucc = new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(JOpred = new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(head   = new Lisa_Matrix<TIMETYP>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       tail = NULL; 
       for (i=0; i<=(P->n); i++)
          for (j=0; j<=(P->m); j++)
	     { 
	       (*JOsucc)[i][j]=SINK;
	       (*JOpred)[i][j]=SOURCE;
	       (*head)[i][j]=(*P->ri)[i];
	     }
       value=(TIMETYP) 0;
       ScheduleOK=true;
 
     }

//**************************************************************************

Lisa_JsSchedule::~Lisa_JsSchedule()
     { 
       delete head;
       delete JOsucc;
       delete JOpred;
       if (tail)
          delete tail;
     }

//**************************************************************************

void Lisa_JsSchedule::InitHeadsTails()
{
  int i, M;
  TIMETYP hh,tt;
  ComputeHeadsTails(true, true );
  
  // init heads
  for ( i=1; i<=P->n; i++ )
    {
      M = 0; hh = 0;
      do
	{
	  M = GetMOsucc( i, M );
	  (*head)[i][M] = hh;
	  hh += (*P->time)[i][M];
	}
      while ( M != 0 );
    }
  // init tails
  for ( i=P->n; i>=1; i-- )
    {
      M = 0; tt = 0;
      do
	{
	  M = GetMOpred( i, M );
	  (*tail)[i][M] = tt;
	  tt += (*P->time)[i][M];
	}
      while ( M != 0 );
    }
}

//**************************************************************************
 
void Lisa_JsSchedule::operator=(Lisa_JsSchedule &other)
     { 
       if (other.P != P)
         {
           G_ExceptionList.lthrow("can only copy schedules of same problem!");
           return;
         }
       MOsucc    =  other.MOsucc;
       MOpred    =  other.MOpred;
       (*JOsucc) = *other.JOsucc;
       (*JOpred) = *other.JOpred;
       (*head)   = *other.head;
       if (other.tail)
         {
           if (!tail)
	     tail =new Lisa_Matrix<TIMETYP>(P->n+1,P->m+1);
           (*tail)=*other.tail;
         }        
       else if (tail) delete tail;
       ScheduleOK=other.ScheduleOK;
       ComputeTails=other.ComputeTails;
       ComputeHeads=other.ComputeHeads;
       lastinsi=other.lastinsi; lastinsj=other.lastinsj;  
       value=other.value;
     }

//**************************************************************************

int Lisa_JsSchedule::insert( int i, int j, int woi )
{ 
  int h, xyz;  
  TIMETYP oldmosucc=0, oldjosucc=0;
  
  if (!ScheduleOK) 
    { 
      G_ExceptionList.lthrow("schedule not feasible --> insert not done");
      return(ERROR);
    }
#ifdef LISA_DEBUG
  if ( exists(i,j) )
    { 
      cerr<<"\nError: operation ("<<i<<","<<j<<") is already inserted!";
      return(ERROR);
    }
  if ( !exists(woi,j) )
    { 
      cerr << "\nError: insertion position does not exist!";
      return(ERROR);
    }
#endif
  // remember where it starts:  
  sti=i; stj=j;
  
  // modify lists:
  (*JOsucc)[i][j]=(*JOsucc)[woi][j]; (*JOsucc)[woi][j]=i;
  (*JOpred)[i][j]=woi;   (*JOpred)[(*JOsucc)[i][j]][j]=i;
  
  // recompute heads / tails
  xyz=OK;
  if (ComputeHeads)
    { 
      started=false;
      if ((h=(*JOsucc)[i][j])) oldjosucc=(*head)[h][j];
      if ((h=(*MOsucc)[i][j])) oldmosucc=(*head)[i][h];
      (*head)[i][j]=-MAXTIME;          // to make shure that push is called 
      if (pullhead(i,j)==CYCLE) return CYCLE;
      if ((h=(*JOsucc)[i][j]) && (oldjosucc==(*head)[h][j]))
	if ((h=(*MOsucc)[i][j]) && (oldmosucc==(*head)[i][h]))
	  xyz=PERFEKT;
    }
  if (ComputeTails)
    { 
      started=false;
      (*tail)[i][j]=-MAXTIME;       
      if (pulltail(i,j)==CYCLE) return CYCLE;
    }
  return(xyz); 
}

//**************************************************************************

void Lisa_JsSchedule::exclude(int i, int j)
    { int pi,si, pj, sj;

    if (!ScheduleOK) { cerr << "\nSchedule not feasible -> nothing done!";
		     return;
		   }
#ifdef LISA_DEBUG
      if (!exists(i,j))
	{ 
          cerr<<"\nError: operation ("<<i<<","<<j<<") is not inserted!";
	  return;
	}
#endif

      // modify lists
      pi=(*JOpred)[i][j]; si=(*JOsucc)[i][j];
      pj=(*MOpred)[i][j]; sj=(*MOsucc)[i][j];
      (*JOsucc)[pi][j]=si;  
      (*JOpred)[si][j]=pi;
      
      // reset operation (i,j)

      (*head)[i][j]=(*P->ri)[i]; 
      if (tail) (*tail)[i][j]=0;
      (*JOsucc)[i][j]=(*JOpred)[i][j]=SOURCE;  
 
      // recompute heads and tails
      if (ComputeHeads)
        {
          if (si) { started=false;
                    sti=si; stj=j;
                    pullhead(si,j);
                  }  
          else (*head)[SINK][j]=(*head)[pi][j]+(*P->time)[pi][j]; 
          if (sj) { started=false;
                    sti=i; stj=sj;
                    pullhead(i,sj);
                  }  
          else (*head)[i][SINK]=(*head)[i][pj]+(*P->time)[i][pj];
        }  
      if (ComputeTails)
        {
          if (pi) { started=false;
                    sti=pi; stj=j;
                    pulltail(pi,j);
                  }           
          if (pj) { started=false;
                    sti=i; stj=pj;
                    pulltail(i,pj);
                  }  
        }  
    }

//**************************************************************************

int Lisa_JsSchedule::read_LR(Lisa_Matrix<int> * lr)
 {
   int i, j, r;
   for (i=1; i<=P->n; i++)
      for (j=1; j<=P->m; j++)
         if (exists(i,j)) exclude(i,j);
   for (r=P->n*P->m; r; r--)
      for (i=1; i<=P->n; i++)
         for (j=1; j<=P->m; j++)
            if ((*lr)[i-1][j-1]==r)
              if (insert(i,j,SOURCE)==CYCLE)
                return ERROR;
   ScheduleOK=true;
   return OK;
 }

//**************************************************************************

int Lisa_JsSchedule::read_Cij(Lisa_Matrix<TIMETYP> * cij)
 {
   int i,j,k;
   Lisa_Order * order = new Lisa_Order(P->n, P->m);
   
   for (i=1; i<=P->n; i++)
      for (j=1; j<=P->m; j++)
         order->read_one_key(i-1, j-1, MAXTIME-(*P->time)[i][j]);
   order->sort();
   order->read(cij);
   order->sort();
     
   ComputeHeadsTails(true,false);
   // construct sequence:
   for (k=0; k<(P->n * P->m); k++)
      {
        i=order->row(k)+1; j=order->col(k)+1;
        if (exists(i,j)) exclude(i,j);
        if ((*cij)[i-1][j-1]) 
           if (insert(i,j,(*JOpred)[SINK][j])==CYCLE)
              return ERROR;
      }  
 
   // shift operations to the right time slots:
   sti=stj=0;                              // to disable cycle testing
   for (k=0; k<(P->n * P->m); k++)
      {
        i=order->row(k)+1; j=order->col(k)+1;
        if ((*cij)[i-1][j-1])  
          pushhead(i,j, (*cij)[i-1][j-1]- (*(P->time))[i][j]);
      } 
   ComputeHeadsTails(true,true);
   TailsFromHeads();
   delete order;

   // now test wether input was correct:
   for (i=P->n; i;  i--)
       for (j=P->m; j; j--)
           if (MAXTIME-(*tail)[i][j] != (*cij)[i-1][j-1] ) return ERROR;
   ScheduleOK=true;
   return OK;
  }

//**************************************************************************

