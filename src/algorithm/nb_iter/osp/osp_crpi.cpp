/**
 * @author  Andreas Winkler
 * @version 2.3pre3
 */
 
#include <stdlib.h>

#include "osp_crpi.hpp"

//**************************************************************************

OSHOP_cr_API_Ngbh::OSHOP_cr_API_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
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

OSHOP_cr_API_Ngbh::~OSHOP_cr_API_Ngbh()
  {
    delete[] cr_list_j;
    delete[] cr_list_m;
  }

//**************************************************************************

int OSHOP_cr_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i, j;
    int numb;
    TIMETYP Cmax;

    // determs the critical operations
    P[0]->SetValue(CMAX);
    Cmax = P[0]->GetValue();
    count = 0;
    for ( i=1; i<(PP->n+1); i++ )
      for ( j=1; j<(PP->m+1); j++ )
	if ( (*PP->sij)[i][j] != 0 )
	  {
	    if ( P[0]->GetHead(i,j) + (*PP->time)[i][j] 
		 + P[0]->GetTail(i,j) == Cmax )
	      {
		cr_list_j[count] = i;
		cr_list_m[count] = j;
		count++;
	      }
	  }
    count--;
    // determs randomly an element
    numb = lisa_random( 0, count, &seed );
    machine1 = cr_list_m[numb];
    job1     = cr_list_j[numb];
    //cout<<" ("<<job1<<","<<machine1<<")";

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
	G_ExceptionList.lthrow("enumerativ method not implemented in osp_crpi.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int OSHOP_cr_API_Ngbh::do_move()
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

