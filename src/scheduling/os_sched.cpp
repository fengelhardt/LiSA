/**
 * @author  Thomas Tautenhahn
 * @version 2.3final
 */

#include "../main/global.hpp"
#include "../basics/order.hpp"

#include "os_sched.hpp"

using namespace std;

//**************************************************************************

Lisa_OsProblem::Lisa_OsProblem(const Lisa_Values * Pin) : Lisa_ShpProblem( Pin ) 
  {
  }

//**************************************************************************

// returns an empty feasible schedule to *Pin with MIJ=emptyset 
Lisa_OsSchedule::Lisa_OsSchedule(const Lisa_OsProblem* Pin)
     { int  i,j;
       
       ComputeTails=false;
       ComputeHeads=true;
       P = OSP = Pin;
       if ( !(MOsucc=new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(MOpred=new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(JOsucc=new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(JOpred=new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(head  =new Lisa_Matrix<TIMETYP>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       tail=NULL; 
       for (i=0; i<=(P->n); i++)
          for (j=0; j<=(P->m); j++)
	     { 
               (*MOsucc)[i][j]=(*JOsucc)[i][j]=SINK;
	       (*MOpred)[i][j]=(*JOpred)[i][j]=SOURCE;
               (*head)[i][j]=(*P->ri)[i];
	     }
       value=(TIMETYP) 0;
       ScheduleOK=true;
     }

//**************************************************************************

Lisa_OsSchedule::~Lisa_OsSchedule()
     { 
       delete head;
       delete MOsucc;
       delete MOpred;
       delete JOsucc;
       delete JOpred;
       if (tail)
          delete tail;
     }

//**************************************************************************
 
void Lisa_OsSchedule::operator=(const Lisa_OsSchedule &other)
     { 
       if (other.P != P)
         {
           cout << "Error: can only copy schedules of same problem!\n";
           return;
         }
       (*MOsucc)=*other.MOsucc;
       (*JOsucc)=*other.JOsucc;
       (*MOpred)=*other.MOpred;
       (*JOpred)=*other.JOpred;
       (*head)=*other.head;
       if (other.tail){
           if (!tail)
             tail =new Lisa_Matrix<TIMETYP>(P->n+1,P->m+1);
           (*tail)=*other.tail;
       }else if(tail){
         delete tail;
         tail = 0;
       }
       ScheduleOK=other.ScheduleOK;
       ComputeTails=other.ComputeTails;
       ComputeHeads=other.ComputeHeads;
       lastinsi=other.lastinsi; lastinsj=other.lastinsj;  
       value=other.value;
     }

//**************************************************************************

int Lisa_OsSchedule::insert(int i, int j, int woi, int woj)
    { 
      int h, xyz;  
      TIMETYP oldmosucc=0, oldjosucc=0;

#ifdef LISA_DEBUG
      if (!ScheduleOK) { 
        G_ExceptionList.lthrow("Schedule not feasible.",
                               Lisa_ExceptionList::UNDEFINED_OBJECT);
		    return ERROR;
		  }
      if(exists(i,j)){ 
        G_ExceptionList.lthrow("Operation ("+ztos(i)+","+ztos(j)+
                               ") is already inserted!",
                               Lisa_ExceptionList::UNDEFINED_OBJECT);
        return ERROR;
      }
      if (!exists(woi,j)){
        G_ExceptionList.lthrow("Insertion position ("+ztos(woi)+","+ztos(j)+
                               ") does not exist!",
                               Lisa_ExceptionList::UNDEFINED_OBJECT);
        return ERROR;
      }
      if(!exists(i, woj)){ 
        G_ExceptionList.lthrow("Insertion position ("+ztos(i)+","+ztos(woj)+
                               ") does not exist!",
                               Lisa_ExceptionList::UNDEFINED_OBJECT);
        return ERROR;
      }
#endif

      // remember where it starts:  
      sti=i; stj=j;

      // modify lists:
      (*JOsucc)[i][j]=(*JOsucc)[woi][j]; (*JOsucc)[woi][j]=i;
      (*JOpred)[i][j]=woi;   (*JOpred)[(*JOsucc)[i][j]][j]=i;
      (*MOsucc)[i][j]=(*MOsucc)[i][woj]; (*MOsucc)[i][woj]=j;
      (*MOpred)[i][j]=woj;   (*MOpred)[i][(*MOsucc)[i][j]]=j;

      // recompute heads / tails
      xyz=OK;
      if (ComputeHeads)
	 { 
           started=false;
	   if ((h=(*JOsucc)[i][j])) oldjosucc=(*head)[h][j];
           if ((h=(*MOsucc)[i][j])) oldmosucc=(*head)[i][h];  
	   (*head)[i][j]=-MAXTIME;
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

void Lisa_OsSchedule::exclude(int i, int j)
    { 
      int pi,si, pj, sj;
    
#ifdef LISA_DEBUG
    if (!ScheduleOK){ 
	    G_ExceptionList.lthrow("Schedule not feasible.",
                             Lisa_ExceptionList::UNDEFINED_OBJECT);
	    return;
    }
    if (!exists(i,j)){ 
      G_ExceptionList.lthrow("Operation ("+ztos(i)+","+ztos(j)+
                             ") is not inserted!",
                             Lisa_ExceptionList::UNDEFINED_OBJECT);
	    return;
    }
#endif

      // modify lists
      pi=(*JOpred)[i][j]; si=(*JOsucc)[i][j];
      pj=(*MOpred)[i][j]; sj=(*MOsucc)[i][j];
      (*JOsucc)[pi][j]=si;  
      (*JOpred)[si][j]=pi;
      (*MOsucc)[i][pj]=sj;  
      (*MOpred)[i][sj]=pj; 
      
      // reset operation (i,j)

      (*head)[i][j]=(*P->ri)[i]; 
      if (tail) (*tail)[i][j]=0;
      (*MOsucc)[i][j]=(*MOpred)[i][j]=(*JOsucc)[i][j]=(*JOpred)[i][j]=SOURCE;  
 
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

int Lisa_OsSchedule::read_LR(const Lisa_Matrix<int> * lr)
 {
   int i, j, r;
   for (i=1; i<=P->n; i++)
      for (j=1; j<=P->m; j++)
         if (exists(i,j)) exclude(i,j);
   for (r=P->n*P->m; r; r--)
      for (i=1; i<=P->n; i++)
         for (j=1; j<=P->m; j++)
            if ((*lr)[i-1][j-1]==r)
              insert(i,j,SOURCE,SOURCE);
   ScheduleOK=true;
   return OK;
 }

//**************************************************************************

int Lisa_OsSchedule::read_Cij(const Lisa_Matrix<TIMETYP> * cij)
 {
   int i,j,k;
   Lisa_Order * order = new Lisa_Order(P->n, P->m);
   
   for (i=1; i<=P->n; i++)
      for (j=1; j<=P->m; j++)
         order->read_one_key(i-1, j-1, MAXTIME-(*P->time)[i][j]);
   order->sort();
   order->read(cij);
   order->sort();
   
   // construct sequence:
   for (k=0; k<(P->n * P->m); k++)
      {
        i=order->row(k)+1; j=order->col(k)+1;
        if (exists(i,j)) exclude(i,j);
        if ((*cij)[i-1][j-1]) 
	  insert(i,j,(*JOpred)[SINK][j],(*MOpred)[i][SINK]);
      }
    
   // shift operations to the right time slots:
   sti=stj=0;                      // to disable cycle test
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
   ScheduleOK=true;
   for (i=P->n; i;  i--)
       for (j=P->m; j; j--)
           if (MAXTIME-(*tail)[i][j] != (*cij)[i-1][j-1] ) return ERROR;
   return OK;
  }

//**************************************************************************

