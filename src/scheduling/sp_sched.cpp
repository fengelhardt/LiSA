/**
 * @author  Thomas Tautenhahn
 * @version 2.3final
 */

#include "../main/global.hpp"
#include "../misc/except.hpp"

#include "sp_sched.hpp"

using namespace std;

//**************************************************************************

Lisa_ShpProblem::Lisa_ShpProblem(Lisa_Values * Pin)
{
  int i,j;
  
  // look wether the input makes sense:
  n=Pin->get_n();
  m=Pin->get_m();

// (iroess) 
// following lines temporarly uncommented...
// it has to be checked, if this exception makes any sense or not.
// - until now, i haven't found any example caring about a machine 
// or job number of 1! 
// --------
//  if ((n==1) || (m==1)) 
//    G_ExceptionList.lthrow("shop problem should have size at least 2 x 2",
//                           WARNING);
  if (!(n&&m)) 
    G_ExceptionList.lthrow("shop problem must have machines and jobs",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
   
  time=new Lisa_Matrix<TIMETYP>(n+1,m+1);
  sij=new Lisa_Matrix<bool>(n+1,m+1);
  ri=new Lisa_Vector<TIMETYP>(n+1);
  wi=new Lisa_Vector<float>(n+1); 
  wi_early=new Lisa_Vector<float>(n+1); 
  di=new Lisa_Vector<TIMETYP>(n+1);
  // copy or set processing times (default UET):
  if ((!(Pin->PT))|| (Pin->PT->get_m()!=m) || (Pin->PT->get_n()!=n)) 
    {
      G_ExceptionList.lthrow(
		       "no correct processing times given, assuming p_ij=1");
      time->fill(1);
      for (i=0; i<n; i++) (*time)[i+1][0] = 0;
      for (j=0; j<m; j++) (*time)[0][j+1] = 0;
    } 
  else 
    for (i=0; i<n; i++)
      {
	for (j=0; j<m; j++)
	  {
	    (*time)[i+1][j+1]=(*Pin->PT)[i][j];
	    (*time)[0][j+1] = 0;
	  }
	(*time)[i+1][0] = 0;
      }
  // copy or set operation set (by default no operations of time 0):
  sij->fill(true);
  (*sij)[0][0]=false;
  if (Pin->SIJ)
    for (i=1; i<=n; i++)
      for (j=1; j<=m; j++)
	(*sij)[i][j]=(*Pin->SIJ)[i-1][j-1];
  else
    for (i=1; i<=n; i++)
      for (j=1; j<=m; j++) 
	(*sij)[i][j]=((*time)[i][j]>0);
  // copy or set weights, release times and due dates:
  if (Pin->WI) 
    for (i=0; i<n; i++)
      (*wi)[i+1]=(*Pin->WI)[i];
  else wi->fill(1);
  wi_early->fill(1);
  w_early=w_late=1;
  if (Pin->RD) 
    {
      for (i=0; i<n; i++)
	(*ri)[i+1]=(*Pin->RD)[i];
      (*ri)[0]=0;
    }
  else ri->fill(0);
  if (Pin->DD) 
    for (i=0; i<n; i++)
      (*di)[i+1]=(*Pin->DD)[i];
  else di->fill(MAXTIME);
}

//**************************************************************************

Lisa_ShpProblem::~Lisa_ShpProblem()
   {
     delete time;
     delete ri;
     delete wi;
     delete wi_early;
     delete di;
     delete sij;
   }

//**************************************************************************

void  Lisa_ShpSchedule::ComputeHeadsTails(bool h, bool t)
     { 
       if (h||t)
	  { ComputeHeads=h;
	    ComputeTails=t;
	  }
       else G_ExceptionList.lthrow(
                   "must compute heads or tails in Lisa_ShpSchedule");
       if (t && (!tail))
          {
            tail  =new Lisa_Matrix<TIMETYP>(P->n+1,P->m+1);
	    tail->fill( 0 );
          }
     }

//**************************************************************************
     
int Lisa_ShpSchedule::pushhead(int i, int j, TIMETYP newhead)
     { 
       int h;

       if ( started && (i==sti) && (j==stj) )     // is this a cycle?
         { 
           ScheduleOK=false;
	   return CYCLE;
         }  
       
       if (newhead<=(*head)[i][j])                // is there a change?
         { 
           if (started) return OK;
	   else newhead=(*head)[i][j];
	 }
  
       started=true;                              // recursion
       (*head)[i][j]=newhead;
       newhead+=(*P->time)[i][j];
       if ((h=(*MOsucc)[i][j]))
	 {  
           if (pushhead(i,h,newhead)==CYCLE) return (CYCLE);
         }
       else (*head)[i][SINK]=newhead;
       if ((h=(*JOsucc)[i][j]))
	 { 
           if (pushhead(h,j,newhead)==CYCLE) return (CYCLE);
	 }
       else (*head)[SINK][j]=newhead; 
       return (OK);
     }

//**************************************************************************

// same procedure for tails (easier to read when separate)
int Lisa_ShpSchedule::pushtail(int i, int j, TIMETYP newtail)
     { 
       int h;
       // cout << "pushing tail (" << i << ","<<j<<")->"<<newtail<<endl;
       if ( started && (i==sti) && (j==stj) )     // is this a cycle?
         { 
           ScheduleOK=false;
	   return CYCLE;
         }  
       
       if (newtail<=(*tail)[i][j])                   // is there a change?
         { 
           if (started) return OK;
	   else newtail=(*tail)[i][j];
	 }

       started=true;                              // recursion
       (*tail)[i][j]=newtail;
       newtail+=(*P->time)[i][j];
       if ((h=(*MOpred)[i][j]))
	 {  
           if (pushtail(i,h,newtail)==CYCLE) return (CYCLE);
         }
       else (*tail)[i][SOURCE]=newtail;
       if ((h=(*JOpred)[i][j]))
	 { 
           if (pushtail(h,j,newtail)==CYCLE) return (CYCLE);
	 }
       else (*tail)[SOURCE][j]=newtail; 
       return (OK);
     }

//**************************************************************************

// pull-functions: slower but more flexibel
int Lisa_ShpSchedule::pullhead(int i, int j)
    { 
      int   h;
      TIMETYP newhead;

      // no cycle test necessary, because cycles can only be constructed 
      // by inserting and this will call the push-functions
    
      // what is my head?
  
      if ((h=(*MOpred)[i][j]))      
         newhead=(*head)[i][h]+(*P->time)[i][h];
      else 
         newhead=(*P->ri)[i];
      if ((h=(*JOpred)[i][j]))
         newhead=MAX(newhead, (*head)[h][j]+(*P->time)[h][j]);
      
      // call push-function whenever possible     
    	    
      if (newhead>=(*head)[i][j])
	{
          if (pushhead (i,j,newhead)==CYCLE) return CYCLE;
          return OK;
        }
      
      // otherwhise accept value and tell successors 
        
      (*head)[i][j]=newhead;
      started=true;
      if ((h=(*JOsucc)[i][j]))
	 pullhead(h, j);
      else 
         (*head)[SINK][j]=(*head)[i][j]+(*P->time)[i][j];  
      if ((h=(*MOsucc)[i][j]))
	 pullhead(i, h);
      else 
         (*head)[i][SINK]=(*head)[i][j]+(*P->time)[i][j];           
      return OK;
    }

//**************************************************************************

// again, same function for tails
int Lisa_ShpSchedule::pulltail(int i, int j)
    { 
      int   h;
      TIMETYP newtail;

      // what is my tail?
  
      if ((h=(*MOsucc)[i][j]))      
         newtail=(*tail)[i][h]+(*P->time)[i][h];
      else 
         newtail=0;
      if ((h=(*JOsucc)[i][j]))
         newtail=MAX(newtail, (*tail)[h][j]+(*P->time)[h][j]);
      
      // call push-function whenever possible     
    	    
      if (newtail>=(*tail)[i][j])
	{
          if (pushtail (i,j,newtail)==CYCLE) return CYCLE;
          return OK;
        }
      // otherwhise accept value and tell predecessors
        
      (*tail)[i][j]=newtail;
      started=true;
      if ((h=(*JOpred)[i][j]))
	 pulltail(h, j);
      else 
         (*tail)[SOURCE][j]=(*tail)[i][j]+(*P->time)[i][j];  
      if ((h=(*MOpred)[i][j]))
	 pulltail(i, h);
      else 
         (*tail)[i][SOURCE]=(*tail)[i][j]+(*P->time)[i][j];           
      return OK;
    }

//**************************************************************************

bool Lisa_ShpSchedule::exists(int i, int j)
    { 
      // returns 0 for not, 1 for inserted operation
      if ((!i)||(!j)) return 1;
      return ((*JOpred)[i][j]) || (i==(*JOsucc)[SOURCE][j]);
      // test uses JO because it has to work for job shop too
    }

//**************************************************************************

void Lisa_ShpSchedule::SetValue(int function_type)
    { 
      int i;
     
      value=(TIMETYP) 0;
      switch(function_type)
	{ 
	  case CMAX: 
	       for (i=1;i<=P->n; i++ )
	           value=MAX((*head)[i][SINK],value);
               break;
	  case LMAX:
	       value=-MAXTIME;
               for (i=1;i<=P->n; i++ )
	           value=MAX((*head)[i][SINK]-(*P->di)[i],value);
               break;
          case SUM_CI:
               for (i=1;i<=P->n; i++ )
	           value+=(*head)[i][SINK];
               break;
          case SUM_WICI: 
	       for (i=1;i<=P->n; i++ )
	           value+=TIMETYP((*P->wi)[i]) * (*head)[i][SINK];
	       break;
	  case SUM_UI:
               for (i=1;i<=P->n; i++ )
	           value+=((*P->di)[i] < (*head)[i][SINK] ? 1 : 0);
	       break;
	  case SUM_WIUI:
               for (i=1;i<=P->n; i++ )
	           value+=((*P->di)[i] < (*head)[i][SINK] ? (*P->wi)[i] : 0);
	       break;
	  case SUM_TI:
               for (i=1;i<=P->n; i++ )
	           value+=MAX((TIMETYP) 0, ((*head)[i][SINK]-(*P->di)[i]));
	       break;
	  case SUM_WITI:
               for (i=1;i<=P->n; i++ )
	           value+=MAX((TIMETYP) 0, 
                              ((*head)[i][SINK]-(*P->di)[i])*(*P->wi)[i]);
	       break;
	case IRREG1:
               for (i=1;i<=P->n; i++ )
	          {
		    value+=MAX((TIMETYP) 0, ((*head)[i][SINK]-(*P->di)[i]));
		    value+=MAX((TIMETYP) 0, ((*P->di)[i])-(*head)[i][SINK]);
		  }
               break;
	case IRREG2:
	       {
                 TIMETYP maxlate, minlate, l_i;
                 maxlate=minlate=0; 
                 for (i=1; i<=P->n; i++ )
		   {
		     if (exists(i,(*MOpred)[i][SINK]))
		       { 
			 l_i=(*head)[i][SINK]-(*P->di)[i];
			 if (l_i>0) 
			   {
			     value+=l_i*(*P->wi)[i];
			     if (l_i>maxlate) maxlate=l_i;
			   }
			 else 
			   { 
			     value-=l_i*(*P->wi_early)[i];
			     if (l_i<minlate) minlate=l_i;
			   }   
		       }
		   }
                 value+=maxlate*P->w_late - minlate* P->w_early; 
               }
               break;
	  default:
	       G_ExceptionList.lthrow("Undefined objective function.");
	}
    }

//**************************************************************************

void Lisa_ShpSchedule::push_LR(Lisa_Matrix<int> * LR, int i, int j, int k) 
const
 {
   if ((*LR)[i-1][j-1]>=k) return;
   (*LR)[i-1][j-1]=k;
   if ((*JOsucc)[i][j]) push_LR(LR, (*JOsucc)[i][j], j, k+1);
   if ((*MOsucc)[i][j]) push_LR(LR, i, (*MOsucc)[i][j], k+1);
 }

//**************************************************************************
   
void Lisa_ShpSchedule::write_LR(Lisa_Matrix<int> * LR) const
 {
   int j;
   
   LR->fill(0);
   for (j=1; j<=P->m; j++) 
     if ((*JOsucc)[SOURCE][j]) push_LR(LR, (*JOsucc)[SOURCE][j], j, 1);
 }

//**************************************************************************

void Lisa_ShpSchedule::write(ostream& strm) const 
 {
   Lisa_Matrix<int> * LR;
   
   LR = new Lisa_Matrix<int>(P->n,P->m);
   write_LR( LR);
   strm << "\nvalue= "<< value;
   strm << "\nLR= "<< *LR; 
   strm << "\nstart= "<< *head;
   delete LR;   
 }

//**************************************************************************

void Lisa_ShpSchedule::HeadsFromTails()
{ 
  int i,j;
  for (i=P->n; i;  i--)
    for (j=P->m; j; j--)
      (*head)[i][j]=MAXTIME-(*tail)[i][j]-(*P->time)[i][j];
     
  for (i=P->n; i;  i--)
    (*head)[i][SINK]=MAXTIME-(*tail)[i][(*MOpred)[i][SINK]];
}

//**************************************************************************

void Lisa_ShpSchedule::TailsFromHeads()
 { 
   int i,j;
   for (i=P->n; i;  i--)
       for (j=P->m; j; j--)
          (*tail)[i][j]=MAXTIME-(*head)[i][j]-(*(P->time))[i][j];
   for (i=P->n; i;  i--)
      (*tail)[i][SOURCE]=MAXTIME-(*head)[i][(*MOsucc)[i][SOURCE]];
 }

//**************************************************************************

void Lisa_ShpSchedule::pushstart(int i, int j, TIMETYP v)
 { 
   if (!ScheduleOK) 
     {
       G_ExceptionList.lthrow("cannot push infeasible schedule");
       return;
     }
   sti=i; stj=j; started=false;
   pushhead(i,j,v);
 }

//**************************************************************************
  
void Lisa_ShpSchedule::pushstop(int i, int j, TIMETYP v)
 { 
   if (!ScheduleOK) 
     {
       G_ExceptionList.lthrow("cannot push infeasible schedule");
       return;
     }
   if (!tail)
     {
       tail  =new Lisa_Matrix<TIMETYP>(P->n+1,P->m+1);
       tail->fill( 0 );
     }
   sti=i; stj=j; started=false;
   pushtail(i,j,v);
 }

//**************************************************************************

