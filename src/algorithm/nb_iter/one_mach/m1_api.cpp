/**
 * @author  Andreas Winkler
 * @version 2.3rc1
 */
 
#include <stdlib.h>

#include "m1_api.hpp"

using namespace std;

//**************************************************************************

API_Neighbourhood::API_Neighbourhood( Lisa_1Schedule *Plan, Lisa_1Problem *PPi )
  {
   num = 1;
   // seed = 12345672L;
   seed = 247639875L;
   PP = PPi;

   if ( !( P[0] = new Lisa_1Schedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
   *(P[0])=*Plan;

   if ( !( P[1] = new Lisa_1Schedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }

   if ( !( P[2] = new Lisa_1Schedule( PP ) ))
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   
   if ( !( P[3] = new Lisa_1Schedule( PP ) ))
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   
   tabulist = NULL;
  }

//**************************************************************************

API_Neighbourhood::~API_Neighbourhood()
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

int API_Neighbourhood::copy_schedule( int a , int b )
  {
   *P[b]=*P[a];
   return OK;
  }

//**************************************************************************

int API_Neighbourhood::accept_solution()
{
  return copy_schedule( WORK_SOLUTION, ORIG_SOLUTION );
}

//**************************************************************************
  
int API_Neighbourhood::accept_best_ngh()
{
  return copy_schedule( BEST_NGH_SOLUTION, ORIG_SOLUTION );
}

//**************************************************************************

int API_Neighbourhood::put_orig_to_best()
{
  return copy_schedule( ORIG_SOLUTION, BEST_SOLUTION );
}

//**************************************************************************

int API_Neighbourhood::put_work_to_best_ngh()
{
  return copy_schedule( WORK_SOLUTION, BEST_NGH_SOLUTION );
}

//**************************************************************************

int API_Neighbourhood::prepare_move( int typ )
  {
   // determ sa possible move
   // typ=ENUM : enumerativ
   // typ=RAND : randomly
   int number, nend, test;
   number = 0; nend = 20;
   if ( typ == ENUM )
     {
       do
	 {
	   test = OK;
	   number++;
	   pos = num++;
	   if ( num == PP->n )
	     {
	       num = 1;
	       return NO_NGHBOURS;
	     }
	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = P[0]->get_sequ(pos);
	   tabu_param[0][1] = P[0]->get_sequ(pos+1);
	   tabu_param[0][2] = 0;
	   tabu_param[0][3] = 0;
	   if ( (*PP->prec)[ P[0]->get_sequ(pos) ]
	        [ P[0]->get_sequ(pos+1)   ] == 1)
	     test = !OK;
	 }
       while( (test==!OK) && (number < nend) ); 
       if ( test==OK )
	 return OK;
       else
	 {
	   cout << "NoNghb  ";
	   return NO_NGHBOURS;
	 }
     }
   if ( typ == RAND )
     {
       do
	 {
	   test = OK;
	   number++;
	   pos = lisa_random( 1, PP->n-1, &seed );
	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = P[0]->get_sequ(pos);
	   tabu_param[0][1] = P[0]->get_sequ(pos+1);
	   tabu_param[0][2] = 0;
	   tabu_param[0][3] = 0;
	   if ( (*PP->prec)[ P[0]->get_sequ(pos) ]
	        [ P[0]->get_sequ(pos+1)   ] == 1)
	     test = !OK;
	 }
       while( (test==!OK) && (number < nend) );
       if ( test==OK )
	 return OK;
       else
	 {
	   cout << "NoNghb  ";
	   return NO_NGHBOURS;
	 }
     }
   printf( "\nwrong parameter in prepare_move( %i )", typ );
   return !OK;
 }

int API_Neighbourhood::do_move()
 {
   int help;
   *P[1]=*P[0];
   // the following swap means, that in next time the re-move is set 
   // to be tabu
   help = tabu_param[0][0];
   tabu_param[0][0] = tabu_param[0][1];
   tabu_param[0][1] = help;
   return P[1]->shift(pos, pos+1);
 }

//**************************************************************************

int API_Neighbourhood::anti_neighbour()
 {
   return OK;
 }

//**************************************************************************

void API_Neighbourhood::set_objective_type( int o )
 {
   objective_type = o;
 }

//**************************************************************************

void API_Neighbourhood::set_objective( int z, int a)
 {
   if ((a<0) || (a>=4))
     {
       G_ExceptionList.lthrow("wrong schedule in set_objective_type("+ztos(a)+")");
       exit( 7 );
     }
   P[a]->SetValue(z);
 }

//**************************************************************************

TIMETYP API_Neighbourhood::get_objective_value( int a)
 {
  if ((a<0) || (a>=4))
    {
      G_ExceptionList.lthrow("wrong schedule in get_objective_type("+ztos(a)+")");
      exit( 7 );
    }
  return P[a]->GetValue();
 }

//**************************************************************************

int API_Neighbourhood::init_tabulist( unsigned int length )
  {
   if ( !(tabulist = new Lisa_Tabu( length )) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   return OK;
  }

//**************************************************************************

int API_Neighbourhood::use_tabulist()
  {
   return tabulist->use(tabu_param[0][0],tabu_param[0][1],
			tabu_param[0][2],tabu_param[0][3]);
  }

//**************************************************************************

int API_Neighbourhood::set_tabulist()
  {
    tabulist->set(tabu_param[1][0],tabu_param[1][1],
		  tabu_param[1][2],tabu_param[1][3]);
    return OK;
  }

//**************************************************************************

void API_Neighbourhood::store_tabu_param()
  {
    int i;
    for ( i=0; i<=3; i++ )
      tabu_param[1][i] = tabu_param[0][i];
  }

//**************************************************************************

void API_Neighbourhood::clean_tabu_param()
  {
    int i;
    for ( i=0; i<=3; i++ )
      tabu_param[0][i] = 0;
  }

//**************************************************************************

void API_Neighbourhood::return_schedule( Lisa_1Schedule *Plan )
  {
    *Plan = *(P[BEST_SOLUTION]);
  }

//**************************************************************************

