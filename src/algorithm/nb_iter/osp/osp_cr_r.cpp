/** 
 * @author  Andreas Winkler
 * @version 2.3rc1
 */
 
#include <stdlib.h>

#include "osp_cr_r.hpp"

//**************************************************************************

OSHOP_cr_TST_Ngbh::OSHOP_cr_TST_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
    // create a randomly order of the operatins
    if ( !(ROrd = new Lisa_Order(PP->n, PP->m)) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    OrdCount = 0;
    NewOrder = 0;
    int i;
    for ( i=0; i<PP->n*PP->m; i++ )
      ROrd->read_one_key( i, float(lisa_random( 1, 1000, &seed )) );
    ROrd->sort();
  }

//**************************************************************************

OSHOP_cr_TST_Ngbh::~OSHOP_cr_TST_Ngbh()
  {
    delete ROrd;
  }

//**************************************************************************

int OSHOP_cr_TST_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i;

    TIMETYP Cmax;

    // create a new order of the operations
    if ( NewOrder > 2 )
      {
	for ( i=0; i<PP->n*PP->m; i++ )
	  ROrd->read_one_key( i, float(lisa_random( 1, 1000, &seed )) );
	ROrd->sort();
	NewOrder = 0;
	OrdCount = 0;
      }

    // take a look at all elements of the order and take only critical 
    // operations
    P[0]->SetValue(CMAX);
    Cmax = P[0]->GetValue();
    while ( (P[0]->GetHead(   ROrd->row(OrdCount)+1, ROrd->col(OrdCount)+1 )
	     + (*PP->time)[   ROrd->row(OrdCount)+1][ROrd->col(OrdCount)+1] 
	     + P[0]->GetTail( ROrd->row(OrdCount)+1, ROrd->col(OrdCount)+1 ) 
	     != Cmax ) 
            || ((*PP->sij)[ROrd->row(OrdCount)+1][ROrd->col(OrdCount)+1]==0 ))
      {
	OrdCount++;
	if ( OrdCount == PP->n * PP->m )
	  {
	    OrdCount = 0;
	    NewOrder++;
	  }
      }

    machine1 = ROrd->col( OrdCount ) + 1;
    job1     = ROrd->row( OrdCount ) + 1;
    OrdCount++;
    if ( OrdCount == PP->n * PP->m )
      {
	OrdCount = 0;
	NewOrder++;
      }

    if ( typ == RAND )
      {
       // determs randomly if swap in JO or MO
       witch_swap = lisa_random( 1, 2, &seed );
       //witch_swap = JO;
       if ( witch_swap == JO )
	 {
	   job2 = P[0]->GetJOsucc( job1, machine1 );
	   if (( job2 == 0 ) || ( (*PP->sij)[job2][machine1]==0 ))
	     return !OK;
	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = JO;
	   tabu_param[0][1] = machine1;
	   tabu_param[0][2] = job1;
	   tabu_param[0][3] = job2;
	   // here could stay precedence constraints
	   return OK;
	 }
       else
	 {
	   machine2 = P[0]->GetMOsucc( job1, machine1 );
	   if (( machine2 == 0 ) || ( (*PP->sij)[job1][machine2]==0 ))
	     return !OK;
	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = MO;
	   tabu_param[0][1] = job1;
	   tabu_param[0][2] = machine1;
	   tabu_param[0][3] = machine2;
	   return OK;
	 }
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in osp_cr_r.cpp");
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int OSHOP_cr_TST_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
   int help;
   *P[1]=*P[0];
   if (witch_swap==JO)
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

   if (witch_swap==MO)
     {
       predJ1 = P[1]->GetJOpred(job1,machine1);
       //cout << "\n " << predJ1;
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

