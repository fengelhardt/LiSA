/**
 * @author  Andreas Winkler
 * @version 2.3rc1
 */
 
#include <stdlib.h>
#include "../main/global.hpp"
#include "m1_sched.hpp"

using namespace std;

//**************************************************************************

Lisa_1Problem::Lisa_1Problem( Lisa_Values * Pin )
  {
    n = Pin->get_n();
    if ( n < 1 )
      {
	G_ExceptionList.lthrow("wrong dimension = "+ztos(n)+" for the problem");
	exit( 7 );
      }
    int i,j;

    if ( !( time = new Lisa_Vector<TIMETYP>( n+1 ) ))
      G_ExceptionList.lthrow("no more memory",2);
    if ( !( wi = new Lisa_Vector<float>( n+1 ) ))
      G_ExceptionList.lthrow("no more memory",2);
    if ( !( ri = new Lisa_Vector<TIMETYP>( n+1 ) ))
      G_ExceptionList.lthrow("no more memory",2);
    if ( !( di = new Lisa_Vector<TIMETYP>( n+1 ) ))
      G_ExceptionList.lthrow("no more memory",2);
    if ( !( prec = new Lisa_Matrix<int>( n+1, n+1 ) ))
      G_ExceptionList.lthrow("no more memory",2);

    // copy the problem datas
    if ( Pin->PT )
      {
	for ( i=0; i<n; i++ )
	  (*time)[i+1] = (*Pin->PT)[i][0];
	(*time)[0] = 0;
      }
    else time->fill(0);
    for (i=0; i<n+1; i++)
      for (j=0; j<n+1; j++)
	(*prec)[i][j] = 0;
         // copy or set weights, release times and due dates:
     if (Pin->WI) 
       for (i=0; i<n; i++)
          (*wi)[i+1]=(*Pin->WI)[i];
     else wi->fill(1);
     if (Pin->RD) 
       {
	 for (i=0; i<n; i++)
	   (*ri)[i+1]=(*Pin->RD)[i];
	 (*ri)[0] = 0;
       }
     else ri->fill(0);
     if (Pin->DD) 
       for (i=0; i<n; i++)
	 (*di)[i+1]=(*Pin->DD)[i];
     else di->fill(MAXTIME);
  }

//**************************************************************************

Lisa_1Problem::~Lisa_1Problem()
  {
    delete time;
    delete wi;
    delete ri;
    delete di;
    delete prec;
  }

//**************************************************************************

Lisa_1Schedule::Lisa_1Schedule( Lisa_1Problem *PPi )
  { 
    int i;
    PP = PPi;
    if ( !( sequ = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory.n",2);
	exit( 7 );
      }
    if ( !( Ci   = new Lisa_Vector<TIMETYP>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory.",2);
	exit( 7 );
      }
    if ( !( JOsucc = new Lisa_Vector<int>( PP->n+1 ) ) )
      G_ExceptionList.lthrow("no more memory",2);
    if ( !( JOpred = new Lisa_Vector<int>( PP->n+1 ) ) )
      G_ExceptionList.lthrow("no more memory",2); 
    if ( !( head = new Lisa_Vector<TIMETYP>( PP->n+1 ) ) )
      G_ExceptionList.lthrow("no more memory",2);
    if ( !( tail = new Lisa_Vector<TIMETYP>( PP->n+1 ) ) )
      G_ExceptionList.lthrow("no more memory",2);
    for (i=0; i<=(PP->n); i++)
      { 
	(*JOsucc)[i] = SINK;
	(*JOpred)[i] = SOURCE;
	(*head)[i] = (*PP->ri)[i];
      }

    sequ->fill(0);
    Ci->fill(0);
    tail->fill(0);
    value = 0;

    /*
    // determ the precedence graph:
    seed += 145327;
    int prob; // probability between 0 and 100
    prob = 8;
    for ( i=0; i<=PP->n-2; i++ )
      for ( j=i+1; j<=PP->n-1; j++ )
        if ( lisa_random( 0, 100, &seed ) <= prob )
	  (*PP->prec)[ (*sequ)[i] ][ (*sequ)[j] ] = 1;
    */

    get_Ci(1);
  }

//**************************************************************************

Lisa_1Schedule::~Lisa_1Schedule()
  {
    delete sequ;
    delete Ci;
    delete head;
    delete JOsucc;
    delete JOpred;
    if (tail)
      delete tail;
  }

//**************************************************************************

void  Lisa_1Schedule::ComputeHeadsTails(bool h, bool t)
  { 
    if (h||t)
      { 
	ComputeHeads=h;
	ComputeTails=t;
      }
    else G_ExceptionList.lthrow("you must compute heads or tails!");
    if (t && (!tail))
      {
	if (!(tail  =new Lisa_Vector<TIMETYP>(PP->n+1)))
	  G_ExceptionList.lthrow("no more memory",2);
	tail->fill( 0 );
      }
  }

  //**************************************************************************

void Lisa_1Schedule::get_Ci(int pos)
  {
    if ((pos<1) || (pos>PP->n))
      {
	G_ExceptionList.lthrow("wrong position in function get_Ci("+ztos(pos)+")");
	exit( 7 );
      }
    TIMETYP t;
    int i;
   
    t=(*Ci)[(*sequ)[pos-1]];
    for (  ; pos < PP->n+1;  )
      {
	if ( t < (*PP->ri)[i=(*sequ)[pos++]] )
	  t = (*PP->ri)[i];
	(*Ci)[i]= (t += (*PP->time)[i]);
      }
  }

//**************************************************************************

int Lisa_1Schedule::SetValue( int ZF )
  {
    int i;
    value=0;
    switch ( ZF )
      {
      case LMAX:
	value -= MAXTIME;
	for ( i=1; i<PP->n+1; i++ )
	  if ( (*Ci)[i]-(*PP->di)[i] > value )
	    value = (*Ci)[i]-(*PP->di)[i];
	break;
      case SUM_CI:
	for ( i=1; i<PP->n+1; i++ )
	  value += (*Ci)[i];
	break;
      case SUM_UI:
	for ( i=1; i<PP->n+1; i++ )
	  if ( (*Ci)[i]>(*PP->di)[i])
	    value+= 1;
	break;
      case SUM_WIUI:
	for ( i=1; i<PP->n+1; i++ )
	  if ( (*Ci)[i]>(*PP->di)[i])
	    value+= (*PP->wi)[i];
	break;
      case SUM_TI:
	for ( i=1; i<PP->n+1; i++ )
	  if ( (*Ci)[i]>(*PP->di)[i])
	    value+= (*Ci)[i]-(*PP->di)[i];
	break;
      case SUM_WITI:
	for ( i=1; i<PP->n+1; i++ )
	  if ( (*Ci)[i]>(*PP->di)[i])
	    value+= ((*Ci)[i]-(*PP->di)[i])*(*PP->wi)[i];
	break;
      case SUM_WICI:
	for ( i=1; i<PP->n+1; i++ )
	  value += (*Ci)[i] * (*PP->wi)[i];
	break;
      case CMAX:
	value = (*Ci)[ (*sequ)[PP->n] ];
	break;
      default: 
	G_ExceptionList.lthrow("wrong number for objective function");
	exit( 7 );
     }
    return OK;
  }

//**************************************************************************

TIMETYP Lisa_1Schedule::GetValue()
  {
    return value;
  }

int Lisa_1Schedule::get_sequ(int pos)	
  {
    if ((pos>=PP->n+1)||(pos<1))
      { 
	G_ExceptionList.lthrow("wrong position in get_sequ("+ztos(pos)+").");
	exit( 7 );
      }    
    return (*sequ)[pos];
  }

//**************************************************************************

bool Lisa_1Schedule::exists( int i )
    { 
      // returns 0 for not, 1 for inserted operation
      if ( !i ) return 1;
      return ((*JOpred)[i]) || (i==(*JOsucc)[SOURCE]);
      // test uses JO because it has to work for job shop too
    }

//**************************************************************************

int Lisa_1Schedule::insert( int i, int woi )
  { 
    int h, xyz;  
    TIMETYP oldjosucc=0;
    
#ifdef LISA_DEBUG
    if (exists(i))
      { 
	G_ExceptionList.lthrow("job "+ztos(i)+" is already inserted!");
	return(ERROR);
      }
    if ( !(exists(woi)) )
      { 
	G_ExceptionList.lthrow("insertion position does not exist!");
	return(ERROR);
      }
#endif
    // remember where it starts:  
    sti=i;
    
    // modify lists:
    (*JOsucc)[i] = (*JOsucc)[woi]; (*JOsucc)[woi] = i;
    (*JOpred)[i] = woi;   (*JOpred)[(*JOsucc)[i]] = i;

    // recompute heads / tails
    xyz=OK;
    if (ComputeHeads)
      { 
	started=FALSE;
	if ((h=(*JOsucc)[i])) oldjosucc=(*head)[h];
	if (pullhead(i)==CYCLE) return CYCLE;
	if ((h=(*JOsucc)[i]) && (oldjosucc==(*head)[h]))
	  xyz=PERFEKT;
      }
    if (ComputeTails)
      { 
	started=FALSE;
	if (pulltail(i)==CYCLE) return CYCLE;
      }
    return(xyz); 
  }

//**************************************************************************

void Lisa_1Schedule::exclude( int i )
  { 
    int pi, si;
    
#ifdef LISA_DEBUG
    if (!exists(i))
      { 
	G_ExceptionList.lthrow("job "+ztos(i)+" is not inserted!");
	return;
      }
#endif

    // modify lists
    pi = (*JOpred)[i]; si = (*JOsucc)[i];
    (*JOsucc)[pi] = si;  
    (*JOpred)[si] = pi;
    
    // reset job i

    (*head)[i] = (*PP->ri)[i]; 
    if (tail) (*tail)[i] = 0;
    (*JOsucc)[i] = (*JOpred)[i] = SOURCE;  
 
    // recompute heads and tails
    if (ComputeHeads)
      {
	if (si) 
	  { 
	    started=FALSE;
	    sti=si;
	    pullhead(si);
	  }  
	else (*head)[SINK] = (*head)[pi] + (*PP->time)[pi]; 
      }  
    if (ComputeTails)
      {
	if (pi) 
	  {
	    started=FALSE;
	    sti=pi;
	    pulltail(pi);
	  }           
      }  
  }

//**************************************************************************

int Lisa_1Schedule::pushhead(int i, TIMETYP newhead)
  { 
    int h;
    
    if (newhead<=(*head)[i])                // is there a change?
      { 
	if (started) return OK;
	else newhead=(*head)[i];
      }
  
    started=TRUE;                              // recursion
    (*head)[i]=newhead;
    newhead+=(*PP->time)[i];
    if ((h=(*JOsucc)[i]))
      { 
	if (pushhead(h,newhead)==CYCLE) return (CYCLE);
      }
    else (*head)[SINK]=newhead; 
    return (OK);
  }

//**************************************************************************

// same procedure for tails (easier to read when separate)
int Lisa_1Schedule::pushtail(int i, TIMETYP newtail)
  { 
    int h;
    
    if (newtail<=(*tail)[i])                   // is there a change?
      { 
	if (started) return OK;
	else newtail=(*tail)[i];
      }
  
    started=TRUE;                              // recursion
    (*tail)[i]=newtail;
    newtail+=(*PP->time)[i];
    if ((h=(*JOpred)[i]))
      { 
	if (pushtail(h,newtail)==CYCLE) return (CYCLE);
      }
    else (*tail)[SOURCE]=newtail; 
    return (OK);
  }

//**************************************************************************

int Lisa_1Schedule::pullhead( int i )
  { 
    int   h;
    TIMETYP newhead;

    // what is my head?
  
    newhead = (*PP->ri)[i];
    if ((h=(*JOpred)[i]))
      newhead=MAX( newhead, (*head)[h] + (*PP->time)[h] );
    
    // call push-function whenever possible     
    	    
    if (newhead>=(*head)[i])
      {
	if (pushhead (i,newhead)==CYCLE) return CYCLE;
	return OK;
      }
      
    // otherwhise accept value and tell successors 
        
    (*head)[i] = newhead;
    started = TRUE;
    if ((h=(*JOsucc)[i]))
      pullhead(h);
    else 
      (*head)[SINK]=(*head)[i]+(*PP->time)[i];  
    return OK;
  }

//**************************************************************************

// again, same function for tails
int Lisa_1Schedule::pulltail( int i )
  { 
    int   h;
    TIMETYP newtail;

    // what is my tail?
  
    newtail = 0;
    if ((h=(*JOsucc)[i]))
      newtail=MAX(newtail, (*tail)[h]+(*PP->time)[h]);
      
    // call push-function whenever possible     
    	    
    if (newtail>=(*tail)[i])
      {
	if (pushtail (i,newtail)==CYCLE) return CYCLE;
	return OK;
      }
    
    // otherwhise accept value and tell predecessors
        
    (*tail)[i]=newtail;
    started=TRUE;
    if ((h=(*JOpred)[i]))
      pulltail(h);
    else 
      (*tail)[SOURCE]=(*tail)[i]+(*PP->time)[i];  
    return OK;
  }

//**************************************************************************

int Lisa_1Schedule::shift(int a, int b)
  {
    int c,i;
    if ((b>=PP->n+1)||(a<1)||(a>=PP->n+1)||(b<1)||(a==b))
      { 
	G_ExceptionList.lthrow("wrong position in shift("+ztos(a)+ztos(b)+").");
	exit( 7 );
      }
    // test, wether exists(a) and exists(b) !

    if (a>b) 
      { 
	c=(*sequ)[a];
	for (i=a; i>b; i--)
	  (*sequ)[i]=(*sequ)[i-1];
	(*sequ)[b]=c;
      }
    else
      { 
	c=(*sequ)[a];
	for (i=a; i<b; i++)
	  (*sequ)[i]=(*sequ)[i+1];
	(*sequ)[b]=c;
      }
    if (a<b) get_Ci(a); else get_Ci(b);
    return OK;
  }

//**************************************************************************

void Lisa_1Schedule::operator=(Lisa_1Schedule &other)
  {
    int i;
    if (PP->n!=other.PP->n)
      exit(7);
    PP=other.PP;
    
    value = other.value;
    for (i=1; i<PP->n+1; i++)
       {
	 (*Ci)[i]=(*other.Ci)[i];
	 (*sequ)[i]=(*other.sequ)[i];
       }
  }

//**************************************************************************

void Lisa_1Schedule::write_sequ(){
  int i=1, succ=0;
  sequ->fill(0);
  while ( (*JOsucc)[succ] != 0 )
  {
    (*sequ)[i] = (*JOsucc)[succ];
    succ = (*JOsucc)[succ];
    i += 1;
  }
}

//**************************************************************************

void Lisa_1Schedule::print()
  {
    cout << "\nvalue= "<< value;
    cout << "\nsequ="<< *sequ; 
  }

//**************************************************************************

