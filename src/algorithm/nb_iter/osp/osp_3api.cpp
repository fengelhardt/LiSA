/**
 * @author  Andreas Winkler
 * @version 2.3rc1
 */
 
#include <stdlib.h>

#include "osp_3api.hpp"

//**************************************************************************

OSHOP_3_API_Ngbh::OSHOP_3_API_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
  }

//**************************************************************************

int OSHOP_3_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i;

   if ( typ == RAND )
     {
       // determs randomly, if swap in JO or MO, equal for the 3 machines 
       // or jobs
       witch_swap = lisa_random( 1, 2, &seed );
       swaps[0][0] = swaps[1][0] = swaps[2][0] = witch_swap;
       swaps[0][4] = swaps[1][4] = swaps[2][4] = !OK;
       swaps[0][1] = swaps[1][1] = swaps[2][1] = 0;
       //witch_swap = JO;
       if ( witch_swap == JO )
	 {
	   //cout << "\nJO  ";
	   for ( i=0; i<3; i++ )
	     {
	       // determs randomly a machine
	       do
		 machine1 = lisa_random( 1, PP->m, &seed );
	       while ( (machine1==swaps[0][1])||(machine1==swaps[1][1])||
		       (machine1==swaps[2][1]) );
	       // determs randomly a job and look, if it have a predecessor
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
       else
	 {
	   //cout << "\nMO  ";
	   for ( i=0; i<3; i++ )
	     {
	       //determs randomly a job
	       do
		 job1 = lisa_random( 1, PP->n, &seed );
	       while ( (job1==swaps[0][1])||(job1==swaps[1][1])||
		       (job1==swaps[2][1]) );
	       // determs randomly a machine and look, if it have a predec.
	       do
		 {
		   machine2 = lisa_random( 1, PP->m, &seed );
		   machine1 = P[0]->GetMOpred( job1, machine2 );
		 }
	       while ( machine1 == 0 );
	      if (( (*PP->sij)[job1][machine1]==0 ) 
		   || ( (*PP->sij)[job1][machine2]==0 ))
		 return !OK; 
	       swaps[i][1] = job1;
	       swaps[i][2] = machine1; swaps[i][3] = machine2;
	     }

           // here could stay precedence constraints
           return OK;
	 }
     }
   if ( typ == ENUM )
     {
       G_ExceptionList.lthrow("enumerativ method not implemented in osp_3api.cpp");
       exit(7);
     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return !OK;
 }

//**************************************************************************

int OSHOP_3_API_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
   int i;
   *P[1]=*P[0];
   if (witch_swap==JO)  
     {
       //cout << "\nJO  ";
       for ( i=0; i<3; i++ )
	 if ( swaps[i][4] == OK )
	   {
	     machine1 = swaps[i][1];
	     job1 = swaps[i][2]; job2 = swaps[i][3];
	     predM1 = P[1]->GetMOpred(job1,machine1);
	     P[1]->exclude( job1, machine1 );
	     if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	       {
		 //printf("\nnot OK by insert Job1");
		 return !OK;
	       }
	   }
       return OK;
     }

   if (witch_swap==MO)  
     {
       //cout << "\nMO  ";
       for ( i=0; i<3; i++ )
	 if ( swaps[i][4] == OK )
	   {
	     job1 = swaps[i][1];
	     machine1 = swaps[i][2]; machine2 = swaps[i][3];
	     predJ1 = P[1]->GetJOpred(job1,machine1);
	     P[1]->exclude( job1, machine1 );
	     if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	       {
		 //printf("\nnot OK by insert machine1");
		 return !OK;
	       }
	   }
       return OK;
     }
   return !OK;
 }

//**************************************************************************
 
int OSHOP_3_API_Ngbh::use_tabulist()
  {
    int i, ret;
    ret = !OK;
    for ( i=0; i<3; i++ )
      if (tabulist->use(swaps[i][0],swaps[i][1],
			swaps[i][2],swaps[i][3]) == OK)
	{
	  ret = OK;
	  swaps[i][4] = OK;
	}
    return ret;
  }

//**************************************************************************

int OSHOP_3_API_Ngbh::set_tabulist()
  {
    int i;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK )
	tabulist->set(swaps[i][0],swaps[i][1],swaps[i][3],swaps[i][2]);
    return OK;
  }

//**************************************************************************

