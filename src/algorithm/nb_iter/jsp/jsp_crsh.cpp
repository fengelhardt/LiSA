/*
 * ******************** jsp_crsh.cpp ******************************
 * 
 * description:      job shop critical shift neighborhood
 * 
 * @author            Andreas Winkler
 * @version 2.3pre3
 *
 * date:             18.10.2000
 *
 */
#include <stdlib.h>

#include "jsp_crsh.hpp"

JSHOP_cr_shift_Ngbh::JSHOP_cr_shift_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    if ( !( JOrd = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !( cr_list_j = new Lisa_Vector<int>( PP->n*PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !( cr_list_m = new Lisa_Vector<int>( PP->n*PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    cr_count = 0;
  }

JSHOP_cr_shift_Ngbh::~JSHOP_cr_shift_Ngbh()
  {
    delete JOrd;
    delete cr_list_j;
    delete cr_list_m;
  }

int JSHOP_cr_shift_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    TIMETYP Cmax;
    int i, j, k, succ, pred, pos=0, direction;
    int count, count_l, count_r, l_end, r_end;

    if ( typ == RAND )
      {
	// make critical shift
	if ( new_solution == TRUE )
	  {
	    P[0]->SetValue(CMAX);
	    Cmax = P[0]->GetValue();
	    
	    // determs the critical operations
	    cr_count = 0;
	    for ( i=1; i<(PP->n+1); i++ )
	      for ( j=1; j<(PP->m+1); j++ )
		if ( (*PP->sij)[i][j] != 0 )
		  {
		    if ( P[0]->GetHead(i,j) + (*PP->time)[i][j] 
			 + P[0]->GetTail(i,j) == Cmax )
		      {
			(*cr_list_j)[cr_count] = i;
			(*cr_list_m)[cr_count] = j;
			cr_count++;
		      }
		  }
	    new_solution = FALSE;
	  }

	// determs a critical operation
	k = lisa_random( 0, cr_count-1, &seed );
	machine1 = (*cr_list_m)[k];
	job1     = (*cr_list_j)[k];
	
	// determs the critical block containing job1
	// first, the operations leftside from job1
	pred = job1;
	k = P[0]->GetJOpred( pred, machine1 );
	l_end = pred;
	count_l = 0;
	while( ( k != SOURCE ) 
	       && ( P[0]->GetHead( k, machine1 ) 
		    + (*PP->time)[k][machine1] 
		    == P[0]->GetHead( pred, machine1 ) ) )
	  {
	    pred = k;
	    k = P[0]->GetJOpred( pred, machine1 );
	    l_end = pred;
	    count_l++;
	  }
	
	// second, the operations rightside from job1
	succ = job1;
	k = P[0]->GetJOsucc( succ, machine1 );
	r_end = succ;
	count_r = 0;
	while( ( k != SINK ) 
	       && ( P[0]->GetTail( k, machine1 ) 
		    + (*PP->time)[k][machine1] 
		    == P[0]->GetTail( succ, machine1 ) ) )
	  {
	    succ = k;
	    k = P[0]->GetJOsucc( succ, machine1 );
	    r_end = succ;
	    count_r++;
	  }
	
	// determs a second job dependently from the block structure
	if ( ( count_l == 0 ) && ( count_r == 0 ) ) // job1 is a single 
	                                            // critical operation
	  return !OK;
	else if ( count_l == 0 ) // job1 is left block end
	  {
	    direction = 1; // shift right
	    // determs the job order rightside from job1
	    succ = job1; count = 0;
	    do
	      {
		succ = P[0]->GetJOsucc( succ, machine1 );
		(*JOrd)[count] = succ;
		count++;
	      }
	    while (succ != 0);
	    count--;
		
	    if ( count > 0 )
	      {
		// reinsert job1 after a second job rightside
		job2 = (*JOrd)[ lisa_random( 1, count, &seed )-1 ];
	      }
	    else
	      return !OK;
	  }
	else if ( count_r == 0 ) // job1 is right block end
	  {
	    direction = -1; // shift left
	    // determs the job order leftside from job1
	    pred = job1; count = 0;
	    do
	      {
		pred = P[0]->GetJOpred( pred, machine1 );
		(*JOrd)[count] = pred;
		count++;
	      }
	    while (pred != 0);
	    count--;

	    if ( count > 0 )
	      {
		// reinsert job1 before a second job leftside
		job2 = (*JOrd)[ lisa_random( 1, count, &seed )-1 ];
		job2 = P[0]->GetJOpred( job2, machine1 );
	      }
	    else
	      return !OK;
	  }
	else // job1 is within a critical block
	  {
	    // reinsert right or left ?
	    direction = lisa_random( 1, 2, &seed ); // 1  = right
	    if ( direction == 2 )                   // -1 = left
	      direction = -1;
	    if ( direction == 1 )
	      {
		// determs the job order rightside from job1
		succ = job1; count = 0;
		do
		  {
		    succ = P[0]->GetJOsucc( succ, machine1 );
		    (*JOrd)[count] = succ;
		    count++;
		    if ( succ == r_end )
		      {
			pos = count;
		      }
		  }
		while (succ != 0);
		count--;
		
		// reinsert job1 after a second job rightsidefrom the 
		// right block end
		job2 = (*JOrd)[ lisa_random( pos, count, &seed )-1 ];
		  }
	    else // if ( direction == -1 )
	      {
		// determs the job order leftside from job1
		pred = job1; count = 0;
		do
		  {
		    pred = P[0]->GetJOpred( pred, machine1 );
		    (*JOrd)[count] = pred;
		    count++;
		    if ( pred == l_end )
		      {
			pos = count;
		      }
		  }
		while (pred != 0);
		count--;
		
		// reinsert job1 after a second job rightsidefrom the 
		// right block end
		job2 = (*JOrd)[ lisa_random( pos, count, &seed )-1 ];
		job2 = P[0]->GetJOpred( job2, machine1 );
	      }
	  }

	// is this move setting to be tabu ? :
	tabu_param[0][0] = 0;
	tabu_param[0][1] = machine1;
	tabu_param[0][2] = job1;
	tabu_param[0][3] = direction;
	// here could stay precedence constraints
	return OK;
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_trans.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

int JSHOP_cr_shift_Ngbh::do_move()
 {
   *P[1]=*P[0];
   P[1]->exclude( job1, machine1 );
   if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
       return !OK;

   // the following swap means, that in next time the re-move is set 
   // to be tabu
   tabu_param[0][3] *= -1;
   return OK;
 }





