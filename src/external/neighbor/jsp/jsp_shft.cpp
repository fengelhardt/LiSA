/*
 * ******************** jsp_shft.C ******************************
 * 
 * description:      job shop SHIFT neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             21.10.1998
 *
 */

#include "jsp_shft.hpp"

JSHOP_shift_Ngbh::JSHOP_shift_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    if ( !( JOrd = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
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
    n = PP->n;
  };

JSHOP_shift_Ngbh::~JSHOP_shift_Ngbh()
  {
    delete JOrd;
    delete tabu_0;
    delete tabu_1;
  };

int JSHOP_shift_Ngbh::prepare_move( int typ )
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
	   job1 = (*JOrd)[pos1-1];
	   job2 = (*JOrd)[pos2-1];
	 }
       while (0); // here could stay precedence constraints
       if (( (*PP->sij)[job1][machine1]==0 ) 
	       || ( (*PP->sij)[job2][machine1]==0 ))
	 return !OK;

       // is this move setting to be tabu ? :
       (*tabu_0)[0] = machine1;
       if ( pos1 < pos2 )
	 {
	   if ( pos1 > 1 )
	     for (i=1; i<pos1; i++)
	       (*tabu_0)[i] = (*JOrd)[i-1];
	   for (i=pos1; i<pos2; i++)
	     (*tabu_0)[i] = (*JOrd)[i];
	   (*tabu_0)[pos2] = job1;
	   if ( pos2 < n )
	     for (i=pos2+1; i<=n; i++)
	       (*tabu_0)[i] = (*JOrd)[i-1];
	 }
       else
	 {
	   if ( pos2 > 1 )
	     for (i=1; i<pos2; i++)
	       (*tabu_0)[i] = (*JOrd)[i-1];
	   (*tabu_0)[pos2] = job1;
	   for (i=pos2+1; i<=pos1; i++)
	     (*tabu_0)[i] = (*JOrd)[i-2];
	   if (pos1 < n )
	     for (i=pos1+1; i<=n; i++)
	       (*tabu_0)[i] = (*JOrd)[i-1];
	 }
       return OK;
     }
   if ( typ == ENUM )
     {
       G_ExceptionList.lthrow("enumerativ method not implemented in jsp_shft.C"); 
       exit(7);
     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+string(typ)+")");
   return !OK;
 };

int JSHOP_shift_Ngbh::do_move()
 {
   short predJ1;
   short predM1;
   int i;

   // the following means, that in next time the re-move is set 
   // to be tabu
   (*tabu_0)[0] = machine1;
   for (i=1; i<=n; i++)
     (*tabu_0)[i] = (*JOrd)[i-1];
   *P[1]=*P[0];
   if (pos1<pos2)      // shift right
     {
       predM1 = P[1]->GetMOpred(job1,machine1);
       P[1]->exclude( job1, machine1 );
       if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
	 return !OK;
       return OK;
     }
   else                // shift left
     {
       predM1 = P[1]->GetMOpred(job1,machine1);
       predJ1 = P[1]->GetJOpred(job2,machine1);
       P[1]->exclude( job1, machine1 );
       if ( P[1]->insert( job1, machine1, predJ1) == CYCLE )
	 return !OK;
       return OK;
     }

 }


int JSHOP_shift_Ngbh::init_tabulist( unsigned int length )
  {
    // the tabulist contains the number of the maschine 
    // and the order of the jobs on this maschine
    if ( !(tabulist = new Lisa_Tabu( length, n+1 )) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    return OK;
  };

int JSHOP_shift_Ngbh::use_tabulist()
  {
   return tabulist->use_vector( tabu_0 );
  };

int JSHOP_shift_Ngbh::set_tabulist()
  {
    tabulist->set_vector( tabu_1 );
    return OK;
  };

void JSHOP_shift_Ngbh::store_tabu_param()
  {
    *tabu_1 = *tabu_0;
  };

void JSHOP_shift_Ngbh::clean_tabu_param()
  {
    tabu_0->fill(0);
  };
    



