/*
 * ******************** osp_blsh.cpp ******************************
 * 
 * description:      open shop critical-block-SHIFT neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             11.01.1999
 *
 */
#include <stdlib.h>

#include "osp_blsh.hpp"

OSHOP_cr_bl_shift_Ngbh::OSHOP_cr_bl_shift_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
    cr_list_j = new Lisa_Vector<int>(PP->n*PP->m+1);
    cr_list_m = new Lisa_Vector<int>(PP->n*PP->m+1);
    direction = new Lisa_Vector<int>(PP->n*PP->m+1);
    count = 0;
    JOrd = new Lisa_Vector<int>(PP->n+1);
    MOrd = new Lisa_Vector<int>(PP->m+1);
  }

OSHOP_cr_bl_shift_Ngbh::~OSHOP_cr_bl_shift_Ngbh()
  {
    delete cr_list_j;
    delete cr_list_m;
    delete direction;
    delete JOrd;
    delete MOrd;
  }

int OSHOP_cr_bl_shift_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i, j, k, l;
    int numb, succ;
    //TIMETYP ZFV;
    TIMETYP Cmax;

    cr_list_j->fill(0);
    cr_list_m->fill(0);
    direction->fill(0);
    
    // determs the critical operations
    P[0]->SetValue(CMAX);
    Cmax = P[0]->GetValue();
    count = 0;
    for ( i=1; i<(PP->n+1); i++ )
      for ( j=1; j<(PP->m+1); j++ )
	{
	  if ( P[0]->GetHead(i,j) + (*PP->time)[i][j] 
	       + P[0]->GetTail(i,j) == Cmax ) 
	         // i.e. (i,j) is critical operation
	    {
	      //cout<<"("<<i<<","<<j<<") ";
	      k = P[0]->GetJOsucc(i,j); l = P[0]->GetJOpred(i,j);
	      if ( (P[0]->GetHead(k,j)+(*PP->time)[k][j]
		                     +P[0]->GetTail(k,j)==Cmax) &&
		   (P[0]->GetHead(l,j)+(*PP->time)[l][j]
		                     +P[0]->GetTail(l,j)!=Cmax) )
		{
		  // (i,j) is a left block end
		  (*cr_list_j)[count] = i;
		  (*cr_list_m)[count] = j;
		  (*direction)[count] = 1;
		  //cout<<"("<<i<<","<<j<<") ";
		  count++;
		}
	      if ( (P[0]->GetHead(k,j)+(*PP->time)[k][j]
		                     +P[0]->GetTail(k,j)!=Cmax) &&
		   (P[0]->GetHead(l,j)+(*PP->time)[l][j]
		                     +P[0]->GetTail(l,j)==Cmax) )
		{
		  // (i,j) is a right block end
		  (*cr_list_j)[count] = i;
		  (*cr_list_m)[count] = j;
		  (*direction)[count] = -1;
		  //cout<<"("<<i<<","<<j<<") ";
		  count++;
		}
	    }
	}
    if ( count == 0 )
      {
	(*cr_list_j)[count] = lisa_random( 1, PP->n, &seed );
	(*cr_list_m)[count] = lisa_random( 1, PP->m, &seed );
	(*direction)[count] = 1;
	count++;
      }

    count--;
    // determs randomly an element
    numb = lisa_random( 0, count, &seed );
    machine1 = (*cr_list_m)[numb];
    job1     = (*cr_list_j)[numb];
    
    //cout<<"\nnow ("<<job1<<","<<machine1<<","<<(*direction)[numb]<<")";
    
    if ( typ == RAND )
      {
	// determs randomly if swap in JO or MO
	witch_swap = lisa_random( 1, 2, &seed );
	//witch_swap = JO;
	if ( witch_swap == JO )
	  {
	    // determs the order on the machine1
	    succ = 0; i = 0;
	    do
	      {
		succ = P[0]->GetJOsucc( succ, machine1 );
		(*JOrd)[i] = succ;
		if ( succ == job1 )
		  pos1 = i+1;
		i++;
	      }
	    while (succ != 0);
	    //cout<<" JO";
	    if ( (*direction)[numb]==1 )
	      {
		// determs randomly a second position in JOrd
		if ( pos1 == PP->n )
		  return !OK;
		pos2 = lisa_random( pos1+1, PP->n, &seed );
		job2 = (*JOrd)[pos2-1];
		
		// is this move setting to be tabu ? :
		tabu_param[0][0] = JO;
		tabu_param[0][1] = machine1;
		tabu_param[0][2] = job1;
		tabu_param[0][3] = 1;
		// here could stay precedence constraints
		return OK;
	      }
	    else
	      {
		// determs randomly a second position in JOrd
		if ( pos1 == 1 )
		  return !OK;
		pos2 = lisa_random( 1, pos1-1, &seed );
		job2 = (*JOrd)[pos2-1];

		// is this move setting to be tabu ? :
		tabu_param[0][0] = JO;
		tabu_param[0][1] = machine1;
		tabu_param[0][2] = job1;
		tabu_param[0][3] = -1;
		// here could stay precedence constraints
		return OK;
	      }
	  }
	else
	  {
	    //cout<<" MO";
	    // determs the order on the job1
	    succ = 0; i = 0;
	    do
	      {
		succ = P[0]->GetMOsucc( job1, succ );
		(*MOrd)[i] = succ;
		if ( succ == machine1 )
		  pos1 = i+1;
		i++;
	      }
	    while (succ != 0);
	    if ( (*direction)[numb]==-1 )
	      {
		// determs randomly a second position in JOrd
		if ( pos1 == PP->m )
		  return !OK;
		pos2 = lisa_random( pos1+1, PP->m, &seed );
		machine2 = (*MOrd)[pos2-1];
       
		// is this move setting to be tabu ? :
		tabu_param[0][0] = MO;
		tabu_param[0][1] = job1;
		tabu_param[0][2] = machine1;
		tabu_param[0][3] = 1;
		return OK;
	      }
	    else
	      {
		// determs randomly a second position in JOrd
		if ( pos1 == 1 )
		  return !OK;
		pos2 = lisa_random( 1, pos1-1, &seed );
		machine2 = (*MOrd)[pos2-1];

		// is this move setting to be tabu ? :
		tabu_param[0][0] = MO;
		tabu_param[0][1] = job1;
		tabu_param[0][2] = machine1;
		tabu_param[0][3] = -1;
		return OK;
	      }
	  }
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in osp_blsh.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

int OSHOP_cr_bl_shift_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
   *P[1]=*P[0];
   if (witch_swap==JO)
     if (pos1<pos2)      // shift right
       {
	 predM1 = P[1]->GetMOpred(job1,machine1);
	 P[1]->exclude( job1, machine1 );
	 if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	   {
	     //printf("\nnot OK by insert Job1");
	     return !OK;
	   }
	 // the following means, that in next time the re-move is set 
	 // to be tabu
	 tabu_param[0][3] *= -1;
	 return OK;
       }
     else                // shift left
       {
	 predM1 = P[1]->GetMOpred(job1,machine1);
	 predJ1 = P[1]->GetJOpred(job2,machine1);
	 P[1]->exclude( job1, machine1 );
	 if ( P[1]->insert(job1,machine1,predJ1,predM1) == CYCLE )
	   {
	     //printf("\nnot OK by insert Job1");
	     return !OK;
	   }
	 // the following means, that in next time the re-move is set 
	 // to be tabu
	 tabu_param[0][3] *= -1;
	 return OK;
       }
 
   if (witch_swap==MO) 
     if (pos1<pos2)      // shift right
       {
	 predJ1 = P[1]->GetJOpred(job1,machine1);
	 P[1]->exclude( job1, machine1 );
	 if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	   {
	     //printf("\nnot OK by insert machine1");
	     return !OK;
	   }
	 // the following means, that in next time the re-move is set 
	 // to be tabu
	 tabu_param[0][3] *= -1;
	 return OK;
       }
     else                // shift left
       {
	 predJ1 = P[1]->GetJOpred(job1,machine1);
	 predM1 = P[1]->GetMOpred(job1,machine2);
	 P[1]->exclude( job1, machine1 );
	 if ( P[1]->insert(job1,machine1,predJ1,predM1) == CYCLE )
	   {
	     //printf("\nnot OK by insert machine1");
	     return !OK;
	   }
	 // the following means, that in next time the re-move is set 
	 // to be tabu
	 tabu_param[0][3] *= -1;
	 return OK;
       }
   return !OK;
 }







