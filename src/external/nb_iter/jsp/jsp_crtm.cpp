/*
 * ******************** jsp_crtm.cpp ******************************
 * 
 * description:      job shop critical-TRANSPOSE-mixed-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             16.10.2000
 *
 */

#include "jsp_crtm.hpp"
//#include <math.h>

JSHOP_cr_trans_mix_Ngbh::JSHOP_cr_trans_mix_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    JOrd = new Lisa_Vector<int>( PP->n+1 );       
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
    if ( !( LR = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !( cij = new Lisa_Matrix<TIMETYP>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !( krv = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !( krr = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !( kr = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    if ( !( machines = new Lisa_Vector<int>( PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }

  };

JSHOP_cr_trans_mix_Ngbh::~JSHOP_cr_trans_mix_Ngbh()
  {
    delete JOrd;
    delete tabu_0;
    delete tabu_1;
    delete LR;
    delete cij;
    delete krv;
    delete krr;
    delete kr;
    delete machines;
  };

int JSHOP_cr_trans_mix_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int succ, i, j, k, l, r, count, sum;
    int m_count, rand;
    TIMETYP Cmax;
    
    if ( typ == RAND )
      {
	// determs randomly whether API or CR_TRANS
	rand = lisa_random( 1, 100000, &seed );
	if ( rand <= 25000 )
	  {
	    // make an API neighbor swap
	    
	    // determs randomly a machine
	    api = TRUE;
	    machine1 = lisa_random( 1, PP->m, &seed );
	    // determs randomly a job and look if it have a predecessor
	    do
	      {
		do
		  job2 = lisa_random( 1, PP->n, &seed );
		while ( (*PP->sij)[job2][machine1]==0 );
		job1 = P[0]->GetJOpred( job2, machine1 );
	      }
	    while (( job1 == 0 ) || ( (*PP->sij)[job1][machine1]==0 ));

	    (*tabu_0)[0] = machine1;
	    (*tabu_0)[1] = job1;
	    (*tabu_0)[2] = job2;
	    for (i=3; i<=PP->n; i++)
	      (*tabu_0)[i] = (*JOrd)[i-1];
	  }
	else
	  {
	    // make a CR-TRANS neighbor swap

	    api = FALSE;
	    // determs the number of cr. ways over each operation
	    if ( new_solution == TRUE )
	      {
		P[0]->SetValue(CMAX);
		Cmax = P[0]->GetValue();

		// first, determs the outgoing critical ways
		P[0]->write_LR( LR );
		r = 0;
		for (i=0; i<PP->n; i++)
		  for (j=0; j<PP->m; j++)
		    {
		      r = MAX( r, (*LR)[i][j] );
		      (*cij)[i][j] = P[0]->GetHead(i+1,j+1)+(*PP->time)[i+1][j+1];
		      if ( (*cij)[i][j] == Cmax )
			(*krv)[i][j] = 1;
		      else
			(*krv)[i][j] = 0;
		    }
		for (k=1; k<=r; k++)
		  for (i=0; i<PP->n; i++)
		    for (j=0; j<PP->m; j++)
		      if ( (*LR)[i][j] == r-k+1 )
			if ( (*cij)[i][j] + P[0]->GetTail(i+1,j+1) == Cmax )
			  {
			    l = P[0]->GetMOsucc( i+1, j+1 );
			    if ( ( l!=SINK ) && 
				 ( (*cij)[i][l-1] == 
				   (*cij)[i][j] + (*PP->time)[i+1][l] ) )
			      (*krv)[i][j] += (*krv)[i][l-1];
			    l = P[0]->GetJOsucc( i+1, j+1 );
			    if ( ( l!=SINK ) && 
				 ( (*cij)[l-1][j] == 
				   (*cij)[i][j] + (*PP->time)[l][j+1] ) )
			      (*krv)[i][j] += (*krv)[l-1][j];
			  }
		// second, determs the incoming critical ways
		for (i=0; i<PP->n; i++)
		  for (j=0; j<PP->m; j++)
		    {
		      (*cij)[i][j] = (*PP->time)[i+1][j+1] 
			+ P[0]->GetTail(i+1,j+1);
		      if ( (*cij)[i][j] == Cmax )
			(*krr)[i][j] = 1;
		      else
			(*krr)[i][j] = 0;
		    }
		for (k=1; k<=r; k++)
		  for (i=0; i<PP->n; i++)
		    for (j=0; j<PP->m; j++)
		      if ( (*LR)[i][j] == k )
			if ( (*cij)[i][j] + P[0]->GetHead(i+1,j+1) == Cmax )
			  {
			    l = P[0]->GetMOpred( i+1, j+1 );
			    if ( ( l!=SINK ) && 
				 ( (*cij)[i][l-1] == 
				   (*cij)[i][j] + (*PP->time)[i+1][l] ) )
			      (*krr)[i][j] += (*krr)[i][l-1];
			    l = P[0]->GetJOpred( i+1, j+1 );
			    if ( ( l!=SINK ) && 
				 ( (*cij)[l-1][j] == 
				   (*cij)[i][j] + (*PP->time)[l][j+1] ) )
			      (*krr)[i][j] += (*krr)[l-1][j];
			  }
		// all critical ways 
		for (i=0; i<PP->n; i++)
		  for (j=0; j<PP->m; j++)
		    (*kr)[i][j] = (*krv)[i][j] * (*krr)[i][j];

		new_solution = FALSE;
	      }
		 
	    // determs the number of cr. operations on each machine
	    m_count = 0;
	    for (j=0; j<PP->m; j++)
	      {
		sum = 0;
		for (i=0; i<PP->n; i++)
		  if ( (*kr)[i][j] > 0 )
		    sum += 1;
		if ( sum > 1 )
		  {
		    (*machines)[m_count] = j+1;
		    m_count++;
		  }
	      }
	    m_count--;

	    if ( m_count < 0 )
	      {
		G_ExceptionList.lthrow("This solution is optimal !",WARNING);
		return !OK;
	      }
	
	    // determs randomly a machine
	    machine1 = (*machines)[ lisa_random( 0, m_count, &seed ) ];
	    
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

	    // the sum of numbers of critical ways
	    sum = 0;
	    for (i=1; i<=count; i++)
	      sum += (*kr)[(*JOrd)[i-1]-1][machine1-1];
	
	    // determs randomly two positions in JOrd
	    // it will transpose the sequence between these positions
	    k = lisa_random( 1, sum, &seed );
	    j = 0;
	    for (i=1; i<=count; i++)
	      {
		j += (*kr)[(*JOrd)[i-1]-1][machine1-1];
		if ( k <= j )
		  {
		    pos1 = i;
		    i = count;
		  }
	      }

	    do
	      {
		k = lisa_random( 1, sum, &seed );
		j = 0;
		for (i=1; i<=count; i++)
		  {
		    j += (*kr)[(*JOrd)[i-1]-1][machine1-1];
		    if ( k <= j )
		      {
			pos2 = i;
			i = count;
		      }
		  }
	      }
	    while ( pos1 == pos2 );
	    if ( pos1 > pos2 )
	      {
		succ = pos2;
		pos2 = pos1;
		pos1 = succ;
	      }
	    job1 = (*JOrd)[pos1-1];
	    job2 = (*JOrd)[pos2-1];

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
	  } // end of CR-TRANS neighbor swap
	
	return OK;
          
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_crtr.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+string(typ)+")");
    return !OK;
  };

int JSHOP_cr_trans_mix_Ngbh::do_move()
 {
   short predJ1;
   int i, help;
   *P[1]=*P[0];
   
   if ( api == TRUE )
     {
       P[1]->exclude( job1, machine1 );
       if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
	 return !OK;

       help = (*tabu_0)[1];
       (*tabu_0)[1] = (*tabu_0)[2];
       (*tabu_0)[2] = help;
     }
   else
     {
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
     }

   return OK;
 }

int JSHOP_cr_trans_mix_Ngbh::init_tabulist( unsigned int length )
  {
    // the tabulist contains the number of the maschine 
    // and the order of the jobs on this maschine
    if ( !(tabulist = new Lisa_Tabu( length, PP->n+1 )) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
    return OK;
  };

int JSHOP_cr_trans_mix_Ngbh::use_tabulist()
  {
   return tabulist->use_vector( tabu_0 );
  };

int JSHOP_cr_trans_mix_Ngbh::set_tabulist()
  {
    tabulist->set_vector( tabu_1 );
    return OK;
  };

void JSHOP_cr_trans_mix_Ngbh::store_tabu_param()
  {
    *tabu_1 = *tabu_0;
  };

void JSHOP_cr_trans_mix_Ngbh::clean_tabu_param()
  {
    tabu_0->fill(0);
  };
    



