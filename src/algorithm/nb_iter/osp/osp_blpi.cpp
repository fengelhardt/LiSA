/*
 * ******************** osp_blpi.cpp ******************************
 * 
 * description:      open shop critical-block-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             21.10.1998
 *
 */
#include <stdlib.h>

#include "osp_blpi.hpp"

OSHOP_cr_bl_API_Ngbh::OSHOP_cr_bl_API_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
    if ( !(cr_list_j = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !(cr_list_m = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !(direction = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    count = 0;
  }

OSHOP_cr_bl_API_Ngbh::~OSHOP_cr_bl_API_Ngbh()
  {
    delete[] cr_list_j;
    delete[] cr_list_m;
    delete[] direction;
  }

int OSHOP_cr_bl_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i, j, k, l;
    int numb;
    //TIMETYP ZFV;
    TIMETYP Cmax;

    for ( i=0; i<(PP->n)*(PP->m)+1; i++ )
      cr_list_j[i] = cr_list_m[i] = direction[i] = 0;
    
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
		    cr_list_j[count] = i;
		    cr_list_m[count] = j;
		    direction[count] = 1;
		    //cout<<"("<<i<<","<<j<<") ";
		    count++;
		  }
		if ( (P[0]->GetHead(k,j)+(*PP->time)[k][j]
		                     +P[0]->GetTail(k,j)!=Cmax) &&
		     (P[0]->GetHead(l,j)+(*PP->time)[l][j]
		                     +P[0]->GetTail(l,j)==Cmax) )
		  {
		    // (i,j) is a right block end
		    cr_list_j[count] = i;
		    cr_list_m[count] = j;
		    direction[count] = -1;
		    //cout<<"("<<i<<","<<j<<") ";
		    count++;
		  }
	      }
	  }
    if ( count == 0 )
      {
	do
	  {
	    cr_list_j[count] = lisa_random( 1, PP->n, &seed );
	    cr_list_m[count] = lisa_random( 1, PP->m, &seed );
	  }
	while ( (*PP->sij)[cr_list_j[count]][cr_list_m[count]] == 0 );
	direction[count] = 1;
	count++;
      }

    count--;
    // determs randomly an element
    numb = lisa_random( 0, count, &seed );
    machine1 = cr_list_m[numb];
    job1     = cr_list_j[numb];
    //cout<<"\nnow ("<<job1<<","<<machine1<<","<<direction[numb]<<")";
    
    if ( typ == RAND )
      {
	// determs randomly if swap in JO or MO
	witch_swap = lisa_random( 1, 2, &seed );
	//witch_swap = JO;
	if ( witch_swap == JO )
	  {
	    //cout<<" JO";
	    if ( direction[numb]==1 )
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
		job2 = P[0]->GetJOpred( job1, machine1 );
		if (( job2 == 0 ) || ( (*PP->sij)[job2][machine1]==0 ))
		  return !OK;
		// is this move setting to be tabu ? :
		tabu_param[0][0] = JO;
		tabu_param[0][1] = machine1;
		tabu_param[0][2] = job2;
		tabu_param[0][3] = job1;
		job1 = job2; job2 = tabu_param[0][3]; 
		// here could stay precedence constraints
		return OK;
	      }
	  }
	else
	  {
	    //cout<<" MO";
	    if ( direction[numb]==-1 )
	      {
		machine2 = P[0]->GetMOsucc( job1, machine1 );
		if (( machine2==0 ) || ( (*PP->sij)[job1][machine2]==0 ))
		  return !OK;
		// is this move setting to be tabu ? :
		tabu_param[0][0] = MO;
		tabu_param[0][1] = job1;
		tabu_param[0][2] = machine1;
		tabu_param[0][3] = machine2;
		return OK;
	      }
	    else
	      {
		machine2 = P[0]->GetMOpred( job1, machine1 );
		if (( machine2==0 ) || ( (*PP->sij)[job1][machine2]==0 ))
		  return !OK;
		// is this move setting to be tabu ? :
		tabu_param[0][0] = MO;
		tabu_param[0][1] = job1;
		tabu_param[0][2] = machine2;
		tabu_param[0][3] = machine1;
		machine1 = machine2; machine2 = tabu_param[0][3];
		return OK;
	      }
	  }
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in osp_blpi.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

int OSHOP_cr_bl_API_Ngbh::do_move()
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








