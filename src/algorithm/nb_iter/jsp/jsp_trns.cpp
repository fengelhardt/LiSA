/*
 * ******************** jsp_trns.cpp ******************************
 * 
 * description:      job shop TRANSPOSE neighborhood
 * 
 * @author            Andreas Winkler
 * @version 2.3pre3
 *
 * date:             02.10.2000
 *
 */
 
#include <math.h>
#include "jsp_trns.hpp"

JSHOP_trans_Ngbh::JSHOP_trans_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    JOrd = new Lisa_Vector<int>( PP->n+1 );   // JOrd[i] is the job on position i on the machine
    if ( !( tabu_0 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !( tabu_1 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
  }

JSHOP_trans_Ngbh::~JSHOP_trans_Ngbh()
  {
    delete JOrd;
    delete tabu_0;
    delete tabu_1;
  }

int JSHOP_trans_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int succ, i, count;
    
    if ( typ == RAND )
      {
	// determs randomly a machine
	machine1 = lisa_random( 1, PP->m, &seed );
	
	// determs the order on the machine
	succ = 0; count = 0;
	do
	  {
	    succ = P[0]->GetJOsucc( succ, machine1 );
	    (*JOrd)[count] = succ;
	    count++;
	  }
	while (succ != 0);
	count--;

	// determs randomly two positions in JOrd
	// it will transpose the sequence between these positions
	do
	  {
	    pos1 = lisa_random( 1, count, &seed );
	    do
	      pos2 = lisa_random( 1, count, &seed );
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
	(*tabu_0)[0] = machine1;
	if ( pos1 > 1 )
	  for (i=1; i<pos1; i++)
	    (*tabu_0)[i] = (*JOrd)[i-1];
	for (i=0; i<=pos2-pos1; i++)
	  (*tabu_0)[pos1+i] = (*JOrd)[pos2-i-1];
	if ( pos2 < PP->n )
	  for (i=pos2+1; i<=PP->n; i++)
	    (*tabu_0)[i] = (*JOrd)[i-1];
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

int JSHOP_trans_Ngbh::do_move()
 {
   int predJ1;
   int i;
   *P[1]=*P[0];
   
   predJ1 = P[1]->GetJOpred(job1, machine1);
   for (i=pos1; i<=pos2; i++)
     P[1]->exclude( (*JOrd)[i-1], machine1 );
   for (i=0; i<=pos2-pos1; i++)
     {
       if ( P[1]->insert( (*JOrd)[pos2-i-1], machine1, predJ1 ) == CYCLE )
	 return !OK;
       predJ1 = (*JOrd)[pos2-i-1];
     }

   // the following means, that in next time the re-move is set 
   // to be tabu
   (*tabu_0)[0] = machine1;
   for (i=1; i<=PP->n; i++)
     (*tabu_0)[i] = (*JOrd)[i-1];
   return OK;
 }

int JSHOP_trans_Ngbh::init_tabulist( unsigned int length )
  {
    // the tabulist contains the number of the maschine 
    // and the order of the jobs on this maschine
    if ( !(tabulist = new Lisa_Tabu( length, PP->n+1 )) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    return OK;
  }

int JSHOP_trans_Ngbh::use_tabulist()
  {
   return tabulist->use_vector( tabu_0 );
  }

int JSHOP_trans_Ngbh::set_tabulist()
  {
    tabulist->set_vector( tabu_1 );
    return OK;
  }

void JSHOP_trans_Ngbh::store_tabu_param()
  {
    *tabu_1 = *tabu_0;
  }

void JSHOP_trans_Ngbh::clean_tabu_param()
  {
    tabu_0->fill(0);
  }
    



