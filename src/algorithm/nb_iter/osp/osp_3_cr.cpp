/**
 * @author  Andreas Winkler
 * @version 2.3pre3
 */
 
#include <stdlib.h>

#include "osp_3_cr.hpp"

OSHOP_3_CR_Ngbh::OSHOP_3_CR_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
    // make a order for randomly choice the operations
    if ( !(ROrd = new Lisa_Order(PP->n, PP->m)) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    OrdCount = 0;
    NewOrder = 0;
    int i;
    for ( i=0; i<PP->n*PP->m; i++ )
      ROrd->read_one_key( i, float(lisa_random( 1, 1000, &seed )) );
    ROrd->sort();
    if ( !(temp_schedule = new Lisa_OsSchedule(PP)) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
  }

OSHOP_3_CR_Ngbh::~OSHOP_3_CR_Ngbh()
  {
    delete ROrd;
    delete temp_schedule;
  }

int OSHOP_3_CR_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i;

    TIMETYP Cmax;

    clean_tabu_param();
    if ( typ == RAND )
      {
	
	// make a new order for choice the operations
	if ( NewOrder > 2 )
	  {
	    //cout<<"new order\n";
	    for ( i=0; i<PP->n*PP->m; i++ )
	      ROrd->read_one_key( i, float(lisa_random( 1, 1000, &seed )) );
	    ROrd->sort();
	    NewOrder = 0;
	    OrdCount = 0;
	  }
	
	// look at all elements of the order and take only critical operations
       
	P[0]->SetValue(CMAX);
	Cmax = P[0]->GetValue();
	while ( (P[0]->GetHead( ROrd->row(OrdCount)+1, ROrd->col(OrdCount)+1 )
		 + (*PP->time)[ ROrd->row(OrdCount)+1][ROrd->col(OrdCount)+1] 
		 + P[0]->GetTail(ROrd->row(OrdCount)+1,ROrd->col(OrdCount)+1) 
		 != Cmax ) 
	    || ((*PP->sij)[ROrd->row(OrdCount)+1][ROrd->col(OrdCount)+1]==0)) 
	  {
	    OrdCount++;
	    if ( OrdCount == PP->n * PP->m )
	      {
		OrdCount = 0;
		NewOrder++;
	      }
	  }
       	
	machine1 = ROrd->col( OrdCount ) + 1;
	job1     = ROrd->row( OrdCount ) + 1;
	//cout<<" ("<<job1<<","<<machine1<<") ";
	OrdCount++;
	if ( OrdCount == PP->n * PP->m )
	  {
	    OrdCount = 0;
	    NewOrder++;
	  }
	
	// determs randomly if swap in JO or MO, equal for all proposals
	witch_swap = lisa_random( 1, 2, &seed );
	//witch_swap = JO;
	swaps[0][0] = swaps[1][0] = swaps[2][0] = witch_swap;
	swaps[0][4] = swaps[1][4] = swaps[2][4] = OK;
	if ( witch_swap == JO )
	  {
	    swaps[0][1] = machine1;
	    swaps[0][2] = job1;
	    job2        = P[0]->GetJOsucc( job1, machine1 );
	    if (( job2==0 )||( (*PP->sij)[job2][machine1]==0 ))
	      return !OK;
	    swaps[0][3] = job2;
	    machine2    = P[0]->GetMOsucc( job1, machine1 );
	    if ( (*PP->sij)[job1][machine2]==0 )
	      swaps[1][4] = !OK;
	    swaps[1][1] = machine2;
	    swaps[1][2] = job1;
	    swaps[1][3] = P[0]->GetJOsucc( job1, machine2 );
	    if ( (*PP->sij)[swaps[1][3]][machine2]==0 )
	      swaps[1][4] =  !OK;
	    machine3    = P[0]->GetMOpred( job2, machine1 );
	    if ( (*PP->sij)[job2][machine3]==0 )
	      swaps[2][4] = !OK;
	    swaps[2][1] = machine3;
	    swaps[2][2] = P[0]->GetJOpred( job2, machine3 );
	    if ( (*PP->sij)[swaps[2][2]][machine3]==0 )
	      swaps[2][4] = !OK;
	    swaps[2][3] = job2;
	    head = P[0]->GetHead( job1, machine2 );
	    tail = P[0]->GetHead( job2, machine3 )
	           + (*PP->time)[job2][machine3];

	    // here could stay precedence constraints
	    return OK;
	  }
	else
	  {
	    swaps[0][1] = job1;
	    swaps[0][2] = machine1;
	    machine2    = P[0]->GetMOsucc( job1, machine1 );
	    if (( machine2 == 0 )||( (*PP->sij)[job1][machine2]==0 ))
	      return !OK;
	    swaps[0][3] = machine2;
	    job2        = P[0]->GetJOsucc( job1, machine1 );
	    if ( (*PP->sij)[job2][machine1]==0 )
	      swaps[1][4] = !OK;
	    swaps[1][1] = job2;
	    swaps[1][2] = machine1;
	    swaps[1][3] = P[0]->GetMOsucc( job2, machine1 );
	    if ( (*PP->sij)[job2][swaps[1][3]]==0 )
	      swaps[1][4] = !OK;
	    job3        = P[0]->GetJOpred( job1, machine2 );
	    if ( (*PP->sij)[job3][machine2]==0 )
	      swaps[2][4] = !OK;
	    swaps[2][1] = job3;
	    swaps[2][2] = P[0]->GetMOpred( job3, machine2 );
	    if ( (*PP->sij)[job3][swaps[2][2]]==0 )
	      swaps[2][4] = !OK;
	    swaps[2][3] = machine2;
	    head = P[0]->GetHead( job2, machine1 );
	    tail = P[0]->GetTail( job3, machine2 )
	           + (*PP->time)[job3][machine2];

	    // here could stay precedence constraints
	    return OK;
	  } 
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in osp_3_cr.cpp");
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

int OSHOP_3_CR_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
   *P[1] = *P[0];
   *temp_schedule = *P[0];
   //swaps[1][4]=!OK; 
   //swaps[2][4]=!OK;
   if (witch_swap==JO)
     {
       if ( (swaps[0][4]==OK) && (swaps[0][2]!=0) && (swaps[0][3]!=0) )
	 {
	   machine1 = swaps[0][1];
	   job1 = swaps[0][2]; job2 = swaps[0][3];
	   predM1 = P[1]->GetMOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	     {
	       swaps[0][4] = !OK;
	       return !OK;
	     }
	   *temp_schedule = *P[1];
	 }
       else return !OK;
       if ( (P[1]->GetHead(swaps[0][2],swaps[0][1])
	    + (*PP->time)[ swaps[0][2] ][ swaps[0][1] ] >= head)
	    && (swaps[1][4]==OK) && (swaps[1][2]!=0) && (swaps[1][3]!=0) )
	 {
	   machine1 = swaps[1][1];
	   job1 = swaps[1][2]; job2 = swaps[1][3];
	   predM1 = P[1]->GetMOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	     {
	       swaps[1][4] = !OK;
	       *P[1] = *temp_schedule;
	       //return !OK;
	     }
	 } 
       else swaps[1][4] = !OK;
       if ( (P[1]->GetHead(swaps[0][3],swaps[0][1])<=tail)
	    && (swaps[2][4]==OK) && (swaps[2][2]!=0) && (swaps[2][3]!=0) )
	 {
	   machine1 = swaps[2][1];
	   job1 = swaps[2][2]; job2 = swaps[2][3];
	   predM1 = P[1]->GetMOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	     {
	       swaps[2][4] = !OK;
	       *P[1] = *temp_schedule; 
	       //return !OK;
	     }
	 } 
       else swaps[2][4] = !OK;
       return OK;
     }
   if (witch_swap==MO) 
     {
       if ( (swaps[0][4]==OK) && (swaps[0][2]!=0) && (swaps[0][3]!=0) )
	 {
	   job1 = swaps[0][1];
	   machine1 = swaps[0][2]; machine2 = swaps[0][3];
	   predJ1 = P[1]->GetJOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	     {
	       swaps[0][4] = !OK;
	       return !OK;
	     }
	   *temp_schedule = *P[1];
	 } 
       else return !OK;
       if ( (P[1]->GetHead(swaps[0][1],swaps[0][2])
	     + (*PP->time)[ swaps[0][1] ][ swaps[0][2] ] >= head)
	    && (swaps[1][4]==OK) && (swaps[1][2]!=0) && (swaps[1][3]!=0) )
	 {
	   job1 = swaps[1][1];
	   machine1 = swaps[1][2]; machine2 = swaps[1][3];
	   predJ1 = P[1]->GetJOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	     {
	       swaps[1][4] = !OK;
	       *P[1] = *temp_schedule;
	       //return !OK;
	     }
	 }   
       else swaps[1][4] = !OK;
       if ( (P[1]->GetHead(swaps[0][1],swaps[0][3])<=tail)
	    && (swaps[2][4]==OK) && (swaps[2][2]!=0) && (swaps[2][3]!=0) )
	 {
	   job1 = swaps[2][1];
	   machine1 = swaps[2][2]; machine2 = swaps[2][3];
	   predJ1 = P[1]->GetJOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	     {
	       swaps[2][4] = !OK;
	       *P[1] = *temp_schedule;
	       //return !OK;
	     }
	 } 
       else swaps[2][4] = !OK;
       return OK;
     } 
   return !OK;
 }
 
int OSHOP_3_CR_Ngbh::use_tabulist()
  {
    int i, ret;
    ret = !OK;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK ) 
	{
	  if (tabulist->use(swaps[i][0],swaps[i][1],
			    swaps[i][2],swaps[i][3]) == OK)
	    {
	      ret = OK;
	      swaps[i][4] = OK;
	    }
	  else
	    swaps[i][4] = !OK;
	}
    return ret;
  }

int OSHOP_3_CR_Ngbh::set_tabulist()
  {
    int i;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK )
	tabulist->set(swaps[i][0],swaps[i][1],swaps[i][3],swaps[i][2]);
    return OK;
  }

void OSHOP_3_CR_Ngbh::clean_tabu_param()
  {
    int i;
    for ( i=0; i<=3; i++ )
      swaps[i][0] = swaps[i][1] = swaps[i][2] = swaps[i][3] = 0;
  }







