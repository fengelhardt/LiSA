/**
 * @author  Andreas Winkler
 * @version 2.3rc1
 */
 
#include <stdlib.h>
#include "jsp_s_pi.hpp"

//**************************************************************************

JSHOP_semi_API_Ngbh::JSHOP_semi_API_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    if ( !( JOrd = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",2);
	exit( 7 );
      }
  }

//**************************************************************************

JSHOP_semi_API_Ngbh::~JSHOP_semi_API_Ngbh()
  {
    delete JOrd;
  }

//**************************************************************************

int JSHOP_semi_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    bool critical;
    TIMETYP Cmax;
    int k, succ, pred, pos=0, direction;
    int count, count_l, count_r, l_end, r_end;

    critical = FALSE;
    
    if ( typ == RAND )
      {
	P[0]->SetValue(CMAX);
	Cmax = P[0]->GetValue();

	// determs randomly a machine
	machine1 = lisa_random( 1, PP->m, &seed );
	// determs randomly a job and look if he whether is noncritical and 
	// has a successor or he is critical,
	// if job1 is noncritical, he will be reinsert after his successor,
	// if job1 is critical, he will be reinsert dependently from the 
	// block structure (see manual)
	do
	  {
	    do
	      job1 = lisa_random( 1, PP->n, &seed );
	    while ( (*PP->sij)[job1][machine1]==0 );
	    if ( P[0]->GetHead(job1,machine1) + (*PP->time)[job1][machine1] 
		 + P[0]->GetTail(job1,machine1) == Cmax )
	      {
		critical = TRUE;
		job2 = job1;
	      }
	    else
	      job2 = P[0]->GetJOsucc( job1, machine1 );
	  }
	while (( job2 == 0 ) || ( (*PP->sij)[job2][machine1]==0 ));
	//cout<<"op.("<<job1<<","<<machine1<<";"<<job2<<")\n";

	if ( critical == TRUE )
	  {
	    // determs the critical block containing job1
	    // first, the operations leftside from job1
	    pred = job1;
	    k = P[0]->GetJOpred( pred, machine1 );
	    l_end = pred;
	    count_l = 0;
	    //cout<<"left block: ";
	    while( ( k != SOURCE ) 
		   && ( P[0]->GetHead( k, machine1 ) 
			+ (*PP->time)[k][machine1] 
			== P[0]->GetHead( pred, machine1 ) ) )
	      {
		pred = k;
		k = P[0]->GetJOpred( pred, machine1 );
		//(*block_l)[count_l] = pred;
		l_end = pred;
		//cout<<pred<<" ";
		count_l++;
	      }
	    //cout<<"\n";

	    // second, the operations rightside from job1
	    succ = job1;
	    k = P[0]->GetJOsucc( succ, machine1 );
	    r_end = succ;
	    count_r = 0;
	    //cout<<"right block: ";
	    while( ( k != SINK ) 
		   && ( P[0]->GetTail( k, machine1 ) 
			+ (*PP->time)[k][machine1] 
			== P[0]->GetTail( succ, machine1 ) ) )
	      {
		succ = k;
		k = P[0]->GetJOsucc( succ, machine1 );
		//(*block_r)[count_r] = succ;
		r_end = succ;
		//cout<<succ<<" ";
		count_r++;
	      }
	    //cout<<"\n";
	    
	    // determs a second job dependently from the block structure
	    if ( ( count_l == 0 ) && ( count_r == 0 ) ) // job1 is a single 
	                                                // critical operation
	      {
		// determs the successor of job1;
		job2 = P[0]->GetJOsucc( job1, machine1 );
		//cout<<"single cr. operation, job2="<<job2<<"\n";
		if ( job2 == SINK )
		  return !OK;
	      }
	    else if ( count_l == 0 ) // job1 is left block end
	      {
		// determs the job order rightside from job1
		//cout<<"left block end, rightside JOrd=( ";
		succ = job1; count = 0;
		do
		  {
		    succ = P[0]->GetJOsucc( succ, machine1 );
		    (*JOrd)[count] = succ;
		    //cout<<succ<<" ";
		    count++;
		  }
		while (succ != 0);
		count--;
		//cout<<"), count="<<count<<", job2=";

		if ( count > 0 )
		  {
		    // reinsert job1 after a second job rightside
		    job2 = (*JOrd)[ lisa_random( 1, count, &seed )-1 ];
		    //cout<<job2<<"\n";
		  }
		else
		  return !OK;
	      }
	    else if ( count_r == 0 ) // job1 is right block end
	      {
		// determs the job order leftside from job1
		//cout<<"right block end, leftside JOrd=( ";
		pred = job1; count = 0;
		do
		  {
		    pred = P[0]->GetJOpred( pred, machine1 );
		    (*JOrd)[count] = pred;
		    //cout<<pred<<" ";
		    count++;
		  }
		while (pred != 0);
		count--;
		//cout<<" ), count="<<count<<", job2=";

		if ( count > 0 )
		  {
		    // reinsert job1 before a second job leftside
		    job2 = (*JOrd)[ lisa_random( 1, count, &seed )-1 ];
		    job2 = P[0]->GetJOpred( job2, machine1 );
		    //cout<<job2<<"\n";
		  }
		else
		  return !OK;
	      }
	    else // job1 is within a critical block
	      {
		// reinsert right or left ?
		//cout<<"mittle block op., "; 
		direction = lisa_random( 1, 2, &seed ); // 1 = right
		                                        // 2 = left
		if ( direction == 1 )
		  {
		    // determs the job order rightside from job1
		    //cout<<"right end = "<<r_end<<", rightside JOrd=( ";
		    succ = job1; count = 0;
		    do
		      {
			succ = P[0]->GetJOsucc( succ, machine1 );
			(*JOrd)[count] = succ;
			//cout<<succ<<" ";
			count++;
			if ( succ == r_end )
			  {
			    pos = count;
			    //cout<<"(endpos="<<count<<") ";
			  }
		      }
		    while (succ != 0);
		    count--;
		    //cout<<"), count="<<count<<", job2=";

		    // reinsert job1 after a second job rightsidefrom the 
		    // right block end
		    job2 = (*JOrd)[ lisa_random( pos, count, &seed )-1 ];
		    //cout<<job2<<"\n";
		  }
		else
		  {
		    // determs the job order leftside from job1
		    //cout<<"left end = "<<l_end<<", leftside JOrd=( ";
		    pred = job1; count = 0;
		    do
		      {
			pred = P[0]->GetJOpred( pred, machine1 );
			(*JOrd)[count] = pred;
			//cout<<pred<<" ";
			count++;
			if ( pred == l_end )
			  {
			    //cout<<"(endpos="<<count<<") ";
			    pos = count;
			  }
		      }
		    while (pred != 0);
		    count--;
		    //cout<<"), count="<<count<<", job2=";

		    // reinsert job1 after a second job rightsidefrom the 
		    // right block end
		    job2 = (*JOrd)[ lisa_random( pos, count, &seed )-1 ];
		    job2 = P[0]->GetJOpred( job2, machine1 );
		    //cout<<job2<<"\n";
		  }
	      }
	  } // if ( critical == TRUE )
	//cout<<"<next>";
	//cin>>succ;
    
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
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_trans.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_semi_API_Ngbh::do_move()
 {
   int help;
   *P[1]=*P[0];
   P[1]->exclude( job1, machine1 );
   if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
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

//**************************************************************************

