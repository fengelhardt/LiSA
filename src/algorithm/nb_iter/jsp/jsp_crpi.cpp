/**
 * @author  Andreas Winkler
 * @version 2.3rc1
 */
 
#include <stdlib.h>

#include "jsp_crpi.hpp"

//**************************************************************************

JSHOP_cr_API_Ngbh::JSHOP_cr_API_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
   if ( !(cr_list_j = new int[PP->n*PP->m]) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   if ( !(cr_list_m = new int[PP->n*PP->m]) )
     {
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
     }
   count = 0;
  }

//**************************************************************************

JSHOP_cr_API_Ngbh::~JSHOP_cr_API_Ngbh()
  {
    delete[] cr_list_j;
    delete[] cr_list_m;
  }

//**************************************************************************

int JSHOP_cr_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i, j, k;
    int numb;
    TIMETYP Cmax;

    if ( typ == RAND )
      {
	// determs the critical operations
	if ( new_solution == TRUE )
	  {
	    P[0]->SetValue(CMAX);
	    Cmax = P[0]->GetValue();
		
	    count = 0;
	    for ( i=1; i<(PP->n+1); i++ )
	      for ( j=1; j<(PP->m+1); j++ )
		if ( (*PP->sij)[i][j] != 0 )
		  {
		    k = P[0]->GetJOsucc( i, j );
		    if ( ( P[0]->GetHead(i,j) + (*PP->time)[i][j] 
			   + P[0]->GetTail(i,j) == Cmax )
			 && ( (*PP->sij)[k][j] != 0 )
			 && ( P[0]->GetTail( k, j ) 
			      + (*PP->time)[k][j] 
			      == P[0]->GetTail( i, j ) )
			 )
		      {
			cr_list_j[count] = i;
			cr_list_m[count] = j;
			count++;
		      }
		  }
	    count--;
	    new_solution = FALSE;
	  }
	// determs randomly an element
	numb = lisa_random( 0, count, &seed );
	machine1 = cr_list_m[numb];
	job1     = cr_list_j[numb];
	job2 = P[0]->GetJOsucc( job1, machine1 );
 
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
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_crpi.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_cr_API_Ngbh::do_move()
 {
   int help;
   *P[1]=*P[0];

   P[1]->exclude( job1, machine1 );
   if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
     return !OK;

   // the following swap means, that in next time the re-move is set 
   // to be tabu
   help = tabu_param[0][2];
   tabu_param[0][2] = tabu_param[0][3];
   tabu_param[0][3] = help;
   return OK;

 }

//**************************************************************************

