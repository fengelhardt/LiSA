/**
 * @author  Andreas Winkler
 * @version 2.3pre3
 */

#include "jsp_swap.hpp"

//**************************************************************************

JSHOP_swap_Ngbh::JSHOP_swap_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    JOrd = new Lisa_Vector<int>( PP->n+1 );   // JOrd[i] is the job on position i on the machine
  }

//**************************************************************************

JSHOP_swap_Ngbh::~JSHOP_swap_Ngbh()
  {
    delete JOrd;
  }

//**************************************************************************

int JSHOP_swap_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int test, succ, i;
    test = OK;

    if ( typ == RAND )
      {
	// determs randomly a machine
	machine1 = lisa_random( 1, PP->m, &seed );
	// determs the order on the machine
	succ = 0; i = 0;
	do
	  {
	    succ = P[0]->GetJOsucc( succ, machine1 );
	    (*JOrd)[i] = succ;
	    i++;
	  }
	while (succ != 0);
	// determs randomly two positions in JOrd
	do
	  {
	    pos1 = lisa_random( 1, i-1, &seed );
	    do
	      pos2 = lisa_random( 1, i-1, &seed );
	    while ( pos1 == pos2 );
	    if ( pos1 > pos2 )
	      {
		succ = pos2;
		pos2 = pos1;
		pos1 = succ;
	      }
	    job1 = (*JOrd)[pos1-1];
	    job2 = (*JOrd)[pos2-1];
	  }
	while (0); // here could stay precedence constraints
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
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_swap.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_swap_Ngbh::do_move()
 {
   int predJ1;
   int predJ2;
   int help;
   *P[1]=*P[0];
   
   predJ1 = P[1]->GetJOpred(job1, machine1);
   predJ2 = P[1]->GetJOpred(job2, machine1);
   if ( pos2 == pos1 + 1 )
     predJ2 = job2;
   P[1]->exclude( job1, machine1 );
   P[1]->exclude( job2, machine1 );
   if ( P[1]->insert( job2, machine1, predJ1 ) == CYCLE )
     return !OK;
   if ( P[1]->insert( job1, machine1, predJ2 ) == CYCLE )
     return !OK;

   // the following means, that in next time the re-move is set 
   // to be tabu
   help = tabu_param[0][2];
   tabu_param[0][2] = tabu_param[0][3];
   tabu_param[0][3] = help;
   return OK;

 }

//**************************************************************************

