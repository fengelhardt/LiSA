/*
 * ******************** jsp_api.cpp ******************************
 * 
 * description:      job shop API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             21.10.1998
 *
 */
#include <stdlib.h>

#include "jsp_api.hpp"


JSHOP_API_Ngbh::JSHOP_API_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
  {
    machine1 = 1;
    job1 = 0;
    seed = 247639875L;
    PP = PPi;
    new_solution = TRUE;
    
    if ( !( P[0] = new Lisa_JsSchedule( PP ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    P[0]->ComputeHeadsTails( 1, 1 );
    *(P[0])=*Plan;

    if ( !( P[1] = new Lisa_JsSchedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    P[1]->ComputeHeadsTails( 1, 1 );


    if ( !( P[2] = new Lisa_JsSchedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    P[2]->ComputeHeadsTails( 1, 1 );

    if ( !( P[3] = new Lisa_JsSchedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    P[3]->ComputeHeadsTails( 1, 1 );

    tabulist = NULL;
  }

JSHOP_API_Ngbh::~JSHOP_API_Ngbh()
  {
    delete P[0];
    delete P[1];
    if ( P[2] != NULL )
      delete P[2];
    if ( P[3] != NULL )
      delete P[3];
    if ( tabulist != NULL )
      delete tabulist;
  }

int JSHOP_API_Ngbh::copy_schedule( int a , int b )
  {
   *P[b]=*P[a];
   return OK;
  }

int JSHOP_API_Ngbh::accept_solution()
{
  new_solution = TRUE;
  return copy_schedule( WORK_SOLUTION, ORIG_SOLUTION );
}
  
int JSHOP_API_Ngbh::accept_best_ngh()
{
  new_solution = TRUE;
  return copy_schedule( BEST_NGH_SOLUTION, ORIG_SOLUTION );
}

int JSHOP_API_Ngbh::put_orig_to_best()
{
  return copy_schedule( ORIG_SOLUTION, BEST_SOLUTION );
}

int JSHOP_API_Ngbh::put_work_to_best_ngh()
{
  return copy_schedule( WORK_SOLUTION, BEST_NGH_SOLUTION );
}


int JSHOP_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int test;
    test = OK;
    
    if ( typ == RAND )
      {
	// determs randomly a machine
	machine1 = lisa_random( 1, PP->m, &seed );
	// determs randomly a job and look if it have a predecessor
	do
	  {
	    do
	      job2 = lisa_random( 1, PP->n, &seed );
	    while ( (*PP->sij)[job2][machine1]==0 );
	    job1 = P[0]->GetJOpred( job2, machine1 );
	  }
	while (( job1 == 0 ) || ( (*PP->sij)[job1][machine1]==0 ));
	// is this move setting to be tabu ? :
	tabu_param[0][0] = 0;
	tabu_param[0][1] = machine1;
	tabu_param[0][2] = job1;
	tabu_param[0][3] = job2;
	// here could stay precedence constraints
	return OK;
      }
    if ( typ == ENUM )
      {
	job1 = P[0]->GetJOsucc( job1, machine1 );
	job2 = P[0]->GetJOsucc( job1, machine1 );
	if ( (job2 == 0) || (job1 == 0) )
	  {
	    machine1++;
	    if ( machine1 == PP->m+1 )
	      {
		job1 = 0;
		machine1 = 1;
		return !OK;
	      }
	    job1 = P[0]->GetJOsucc( 0, machine1 );
	    job2 = P[0]->GetJOsucc( job1, machine1 );
	  }
	if ( test==OK )
	  {
	    if (( (*PP->sij)[job1][machine1]==0 ) 
		|| ( (*PP->sij)[job2][machine1]==0 ))
	      return !OK;
	    // is this move setting to be tabu ? :
	    tabu_param[0][0] = 0;
	    tabu_param[0][1] = machine1;
	    tabu_param[0][2] = job1;
	    tabu_param[0][3] = job2;
	    return OK;
	  }
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

int JSHOP_API_Ngbh::do_move()
 {
   short predM1;
   int help;
   *P[1]=*P[0];
   predM1 = P[1]->GetMOpred(job1,machine1);
   P[1]->exclude( job1, machine1 );
   if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
     {
       //printf("\nnot OK by insert Job1");
       return !OK;
     }
   // the following swap means, that in next time the re-move is set 
   // to be tabu
   help = tabu_param[0][2];
   tabu_param[0][2] = tabu_param[0][3];
   tabu_param[0][3] = help;
   return OK;
 }

int JSHOP_API_Ngbh::anti_neighbor()
 {
   Lisa_Vector<int>     *cr_job;
   Lisa_Vector<int>     *cr_mach;
   Lisa_Vector<int>     *cr_rand;
   Lisa_Vector<int>     *JOrd;
   Lisa_Order           *cr_order;
   Lisa_JsSchedule      *copied;
   int i, j, cr_count, succ, count;
   int job, machine, pos, test;
   TIMETYP Cmax;

   if ( !( copied = new Lisa_JsSchedule( PP ) ))
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   
   if ( !( cr_job = new Lisa_Vector<int>( PP->m * PP->n ) ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   if ( !( cr_mach = new Lisa_Vector<int>( PP->m * PP->n ) ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   if ( !( JOrd = new Lisa_Vector<int>( PP->n+2 ) ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }

   P[0]->SetValue(CMAX);
   Cmax = P[0]->GetValue();

   // determs the critical operations
   cr_count = 0;

   for (i=1; i<=PP->n; i++)
     for (j=1; j<=PP->m; j++)
       //if ( (*PP->sij)[i][j] == 1 )
       if ( ( (*PP->sij)[i][j] == 1 ) 
	    && ( P[0]->GetHead(i,j) + (*PP->time)[i][j] 
		 + P[0]->GetTail(i,j) == Cmax ) )
	 {
	   (*cr_job)[cr_count] = i;
	   (*cr_mach)[cr_count] = j;
	   cr_count++;
	 }
   
   // determs the order to insert the operations
   if ( !( cr_rand = new Lisa_Vector<int>( cr_count ) ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   if ( !( cr_order = new Lisa_Order( cr_count ) ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   for (i=0; i<cr_count; i++)
     (*cr_rand)[i] = lisa_random( 1, 1000, &seed );

   cr_order->read( cr_rand );
   cr_order->sort();

   // exclude all critical operations
   for (i=0; i<cr_count; i++)
     P[0]->exclude( (*cr_job)[i], (*cr_mach)[i] );

   // now include the excluded operations
   for (i=0; i<cr_count; i++)
     {
       job     = (*cr_job) [ (*cr_order)[i] ];
       machine = (*cr_mach)[ (*cr_order)[i] ];

       // determs the order on the machine
       (*JOrd)[0] = 0;
       succ = 0; count = 1;
       do
	 {
	   succ = P[0]->GetJOsucc( succ, machine );
	   (*JOrd)[count] = succ;
	   count++;
	 }
       while (succ != 0);
       count--;
       
       // determs randomly an insertion position 
       // and test if the insertion is OK
       do
	 {
	   *copied = *P[0];
	   pos = lisa_random( 0, count-1, &seed );
	   test = OK;
	   if ( copied->insert( job, machine, (*JOrd)[pos] ) == CYCLE )
	     test = !OK;
	 }
       while ( test != OK );
       *P[0] = *copied;
     }
   
   // attantion! we have a new solution
   new_solution = TRUE; 

   delete cr_job;
   delete cr_mach;
   delete cr_rand;
   delete cr_order;
   delete JOrd;
   delete copied;

   return OK;
 }

void JSHOP_API_Ngbh::set_objective_type( int o )
 {
   objective_type = o;
 }

void JSHOP_API_Ngbh::set_objective( int z, int a)
 {
   if ((a<0) || (a>=4))
     {
       G_ExceptionList.lthrow("wrong plan in set_objective_type("+ztos(a)+")");
      exit( 7 );
     }
   P[a]->SetValue(z);
 }

TIMETYP JSHOP_API_Ngbh::get_objective_value( int a)
 {
  if ((a<0) || (a>=4))
    {
      G_ExceptionList.lthrow("wrong plan in get_objective_type("+ztos(a)+")");
     exit( 7 );
    }
  return P[a]->GetValue();
 }

int JSHOP_API_Ngbh::init_tabulist( unsigned int length )
  {
   if ( !(tabulist = new Lisa_Tabu( length )) )
     {
       G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
     }
   return OK;
  }

int JSHOP_API_Ngbh::use_tabulist()
  {
   return tabulist->use(tabu_param[0][0],tabu_param[0][1],
			tabu_param[0][2],tabu_param[0][3]);
  }

int JSHOP_API_Ngbh::set_tabulist()
  {
    tabulist->set(tabu_param[1][0],tabu_param[1][1],
		  tabu_param[1][2],tabu_param[1][3]);
    return OK;
  }

void JSHOP_API_Ngbh::store_tabu_param()
  {
    int i;
    for ( i=0; i<=3; i++ )
      tabu_param[1][i] = tabu_param[0][i];
  }

void JSHOP_API_Ngbh::clean_tabu_param()
  {
    int i;
    for ( i=0; i<=3; i++ )
      tabu_param[0][i] = 0;
  }

void JSHOP_API_Ngbh::return_schedule( Lisa_JsSchedule *Plan )
  {
    *Plan = *(P[BEST_SOLUTION]);
  }







