/**
 * @author  Andreas Winkler
 * @version 2.3pre3
 */
 
#include <stdlib.h>
 
#include "jsp_3api.hpp"

JSHOP_3_API_Ngbh::JSHOP_3_API_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
  }

int JSHOP_3_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i;

   if ( typ == RAND )
     {
       swaps[0][4] = swaps[1][4] = swaps[2][4] = !OK;
       swaps[0][1] = swaps[1][1] = swaps[2][1] = 0;

       for ( i=0; i<3; i++ )
	 {
	   // determs randomly a machine
	   do
	     machine1 = lisa_random( 1, PP->m, &seed );
	   while ( (machine1==swaps[0][1])||(machine1==swaps[1][1])||
		   (machine1==swaps[2][1]) );
	   // determs randomly a job and look if it have a predecessor
	   do
	     {
	       job2 = lisa_random( 1, PP->n, &seed );
	       job1 = P[0]->GetJOpred( job2, machine1 );
	     }
	   while ( job1 == 0 );
	   if (( (*PP->sij)[job1][machine1]==0 ) 
	       || ( (*PP->sij)[job2][machine1]==0 ))
	     return !OK;
	   swaps[i][1] = machine1;
	   swaps[i][2] = job1; swaps[i][3] = job2;
	 }
       
       // here could stay precedence constraints
       return OK;
     }
   if ( typ == ENUM )
     {
       G_ExceptionList.lthrow("enumerativ method not implemented in jsp_3api.cpp"); 
       exit(7);
     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return !OK;
 }

int JSHOP_3_API_Ngbh::do_move()
 {
   int predM1;
   int i;
   *P[1]=*P[0];

   for ( i=0; i<3; i++ )
     if ( swaps[i][4] == OK )
       {
	 machine1 = swaps[i][1];
	 job1 = swaps[i][2]; job2 = swaps[i][3];
	 predM1 = P[1]->GetMOpred(job1,machine1);
	 P[1]->exclude( job1, machine1 );
	 if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
	   {
	     //printf("\nnot OK by insert Job1");
	     return !OK;
	   }
       }
   return OK;
 
 }
 
int JSHOP_3_API_Ngbh::use_tabulist()
  {
    int i, ret;
    ret = !OK;
    for ( i=0; i<3; i++ )
      if (tabulist->use(0,swaps[i][1],
			swaps[i][2],swaps[i][3]) == OK)
	{
	  ret = OK;
	  swaps[i][4] = OK;
	}
    return ret;
  }

int JSHOP_3_API_Ngbh::set_tabulist()
  {
    int i;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK )
	tabulist->set(0,swaps[i][1],swaps[i][3],swaps[i][2]);
    return OK;
  }






