/**
 * @author  Andreas Winkler
 * @version 2.3rc1
 */
 
#include <stdlib.h>

#include "osp_api.hpp"

//**************************************************************************

OSHOP_API_Ngbh::OSHOP_API_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
  {
   machine1 = 1;
   witch_swap = JO;
   job1 = 0;
   seed = 247639875L;
   PP = PPi;

   if ( !( P[0] = new Lisa_OsSchedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
   *(P[0])=*Plan;

   if ( !( P[1] = new Lisa_OsSchedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }

   if ( !( P[2] = new Lisa_OsSchedule( PP ) ))
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }

   if ( !( P[3] = new Lisa_OsSchedule( PP ) ))
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   
   tabulist = NULL;
  }

//**************************************************************************

OSHOP_API_Ngbh::~OSHOP_API_Ngbh()
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

//**************************************************************************

int OSHOP_API_Ngbh::copy_schedule( int a , int b )
  {
   *P[b]=*P[a];
   return OK;
  }

//**************************************************************************

int OSHOP_API_Ngbh::accept_solution()
{
  return copy_schedule( WORK_SOLUTION, ORIG_SOLUTION );
}

//**************************************************************************
  
int OSHOP_API_Ngbh::accept_best_ngh()
{
  return copy_schedule( BEST_NGH_SOLUTION, ORIG_SOLUTION );
}

//**************************************************************************

int OSHOP_API_Ngbh::put_orig_to_best()
{
  return copy_schedule( ORIG_SOLUTION, BEST_SOLUTION );
}

//**************************************************************************

int OSHOP_API_Ngbh::put_work_to_best_ngh()
{
  return copy_schedule( WORK_SOLUTION, BEST_NGH_SOLUTION );
}

//**************************************************************************

int OSHOP_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int test;
    test = OK;

   if ( typ == RAND )
     {
       // determs randomly if swap in JO oder MO
       witch_swap = lisa_random( 1, 2, &seed );
       if ( witch_swap == JO )
	 {
           // determs randomly a machine:
           machine1 = lisa_random( 1, PP->m, &seed );
           // determs randomly a job and look, if it have a predecessor
           do
  	     {
	       do 
		 job2 = lisa_random( 1, PP->n, &seed );
	       while ( (*PP->sij)[job2][machine1] == 0 );
	       job1 = P[0]->GetJOpred( job2, machine1 );
	     }
           while (( job1 == 0 ) || ( (*PP->sij)[job1][machine1]==0 ));
	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = JO;
	   tabu_param[0][1] = machine1;
	   tabu_param[0][2] = job1;
	   tabu_param[0][3] = job2;
           // here could standing precedence constraints
           return OK;
	 }
       else
	 {
	   //determs randomly a job:
           job1 = lisa_random( 1, PP->n, &seed );
           // determs randomly a machine and look, if it have a predecessor
           do
  	     {
	       do
		 machine2 = lisa_random( 1, PP->m, &seed );
	       while ( (*PP->sij)[job1][machine2] == 0 );
	       machine1 = P[0]->GetMOpred( job1, machine2 );
	     }
           while (( machine1 == 0 ) || ( (*PP->sij)[job1][machine1]==0 ));
	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = MO;
	   tabu_param[0][1] = job1;
	   tabu_param[0][2] = machine1;
	   tabu_param[0][3] = machine2;
           // here could stay precedence constraints
           return OK;
	 }
      
     }
   if ( typ == ENUM )
     {

       int return_OK = !OK;
       while ( return_OK == !OK )
	 {
	   test = OK;
	   if (witch_swap == JO)
	     // swap in JO
	     {
	       job1 = P[0]->GetJOsucc( job1, machine1 );
	       job2 = P[0]->GetJOsucc( job1, machine1 );
	       if (( job1 == 0 ) || ( job2 == 0 ))
		 {
		   machine1++;
		   if ( machine1 == PP->m+1 )
		     {
		       witch_swap = MO;
		       job1 = 1;
		       machine1 = 0;
		       //return !OK;
		       test = !OK;
		     }
		   if ( test==OK )
		     {
		       job1 = P[0]->GetJOsucc( 0, machine1 );
		       job2 = P[0]->GetJOsucc( job1, machine1 );
		     }
		 }
	       if ( test==OK )
		 {
		   if (( (*PP->sij)[job1][machine1]==0 ) 
		       || ( (*PP->sij)[job2][machine1]==0 ))
		     //return !OK;
		     continue;
		   // is this move setting to be tabu ? :
		   tabu_param[0][0] = JO;
		   tabu_param[0][1] = machine1;
		   tabu_param[0][2] = job1;
		   tabu_param[0][3] = job2;
		   if ( use_tabulist() != OK )
		     //return !OK;
		     continue;
		   return_OK = OK;
		 }
	     }
	   if (witch_swap == MO)
	     // swap in MO 
	     {
	       machine1 = P[0]->GetMOsucc( job1, machine1 );
	       machine2 = P[0]->GetMOsucc( job1, machine1 );
	       if (( machine1 == 0 ) || ( machine2 == 0 ))
		 {
		   job1++;
		   if ( job1 == PP->n+1 )
		     {
		       witch_swap = JO;
		       job1 = 0;
		       machine1 = 1;
		       //return NO_NGHBOURS;
		       test = !OK;
		     }
		   if ( test==OK )
		     {
		       machine1 = P[0]->GetMOsucc( job1, 0 );
		       machine2 = P[0]->GetMOsucc( job1, machine1 );
		     }
		 }
	       if (( (*PP->sij)[job1][machine1]==0 ) 
		   || ( (*PP->sij)[job1][machine2]==0 ))
		 //return !OK;
		 continue;
	       // is this move setting to be tabu ? :
	       tabu_param[0][0] = MO;
	       tabu_param[0][1] = job1;
	       tabu_param[0][2] = machine1;
	       tabu_param[0][3] = machine2;
	       if ( use_tabulist() != OK )
		 //return !OK;
		 continue;
	       return_OK = OK;
	     }

	 }
       return OK;

     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return !OK;
  }

//**************************************************************************

int OSHOP_API_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
   int help;
   *P[1]=*P[0];
   if (witch_swap==JO)   // swap in JO
     {
       predM1 = P[1]->GetMOpred(job1,machine1);
       P[1]->exclude( job1, machine1 );
       if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
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

   if (witch_swap==MO)   // swap in MO
     {
       predJ1 = P[1]->GetJOpred(job1,machine1);
       P[1]->exclude( job1, machine1 );
       if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	 {
	   //printf("\nnot OK by insert machine1");
	   return !OK;
	 }
       // the following swap means, that in next time the re-move is set 
       // to be tabu
       help = tabu_param[0][2];
       tabu_param[0][2] = tabu_param[0][3];
       tabu_param[0][3] = help;
       return OK;
     }
   return !OK;
 }

//**************************************************************************

int OSHOP_API_Ngbh::anti_neighbour()
 {
   return OK;
 }

//**************************************************************************

void OSHOP_API_Ngbh::set_objective_type( int o )
 {
   objective_type = o;
 }

//**************************************************************************

void OSHOP_API_Ngbh::set_objective( int z, int a)
 {
   if ((a<0) || (a>=4))
     {
      G_ExceptionList.lthrow("wrong plan in set_objective_type("+ztos(a)+")");
      exit( 7 );
     }
   P[a]->SetValue(z);
 }

//**************************************************************************

TIMETYP OSHOP_API_Ngbh::get_objective_value( int a)
 {
  if ((a<0) || (a>=4))
    {
     G_ExceptionList.lthrow("wrong plan in get_objective_type("+ztos(a)+")");
     exit( 7 );
    }
  return P[a]->GetValue();
 }

//**************************************************************************

int OSHOP_API_Ngbh::init_tabulist( unsigned int length )
  {
   if ( !(tabulist = new Lisa_Tabu( length )) )
     {
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
     }
   return OK;
  }

//**************************************************************************

int OSHOP_API_Ngbh::use_tabulist()
  {
   return tabulist->use(tabu_param[0][0],tabu_param[0][1],
			tabu_param[0][2],tabu_param[0][3]);
  }

//**************************************************************************

int OSHOP_API_Ngbh::set_tabulist()
  {
    tabulist->set(tabu_param[1][0],tabu_param[1][1],
		  tabu_param[1][2],tabu_param[1][3]);
    return OK;
  }

//**************************************************************************

void OSHOP_API_Ngbh::store_tabu_param()
  {
    int i;
    for ( i=0; i<=3; i++ )
      tabu_param[1][i] = tabu_param[0][i];
  }

//**************************************************************************

void OSHOP_API_Ngbh::clean_tabu_param()
  {
    int i;
    for ( i=0; i<4; i++ )
      tabu_param[0][i] = 0;
  }

//**************************************************************************

void OSHOP_API_Ngbh::return_schedule( Lisa_OsSchedule *Plan )
  {
    *Plan = *(P[BEST_SOLUTION]);
  }

//**************************************************************************

