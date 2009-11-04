/**
 * @author  Andreas Winkler
 * @version 3.0pre1
 */
 
#include <cstdlib>
#include <cmath>

#include "jobshop_neighbourhoods.hpp"

//**************************************************************************
//**************************************************************************

JSHOP_API_Ngbh::JSHOP_API_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
  {
    machine1 = 1;
    job1 = 0;
    seed = 247639875L;
    PP = PPi;
    new_solution = true;
    
    if ( !( P[0] = new Lisa_JsSchedule( PP ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    P[0]->ComputeHeadsTails( 1, 1 );
    *(P[0])=*Plan;

    if ( !( P[1] = new Lisa_JsSchedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    P[1]->ComputeHeadsTails( 1, 1 );


    if ( !( P[2] = new Lisa_JsSchedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    P[2]->ComputeHeadsTails( 1, 1 );

    if ( !( P[3] = new Lisa_JsSchedule( PP ) ))
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    P[3]->ComputeHeadsTails( 1, 1 );

    tabulist = NULL;
  }

//**************************************************************************

JSHOP_API_Ngbh::~JSHOP_API_Ngbh()
  {
    delete P[0];
    delete P[1];
    if ( P[2] != NULL )
      delete P[2];
    if ( P[3] != NULL )
      delete P[3];
    if ( tabulist != NULL )
      delete tabulist;
  }

//**************************************************************************

int JSHOP_API_Ngbh::copy_schedule( int a , int b )
  {
   *P[b]=*P[a];
   return OK;
  }

//**************************************************************************

int JSHOP_API_Ngbh::accept_solution()
{
  new_solution = true;
  return copy_schedule( WORK_SOLUTION, ORIG_SOLUTION );
}

//**************************************************************************
  
int JSHOP_API_Ngbh::accept_best_ngh()
{
  new_solution = true;
  return copy_schedule( BEST_NGH_SOLUTION, ORIG_SOLUTION );
}

//**************************************************************************

int JSHOP_API_Ngbh::put_orig_to_best()
{
  return copy_schedule( ORIG_SOLUTION, BEST_SOLUTION );
}

//**************************************************************************

int JSHOP_API_Ngbh::put_work_to_best_ngh()
{
  return copy_schedule( WORK_SOLUTION, BEST_NGH_SOLUTION );
}

//**************************************************************************

int JSHOP_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int test;
    test = OK;
    
    if ( typ == RAND )
      {
	// determs randomly a machine
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
	job1 = P[0]->GetJOsucc( job1, machine1 );
	job2 = P[0]->GetJOsucc( job1, machine1 );
	if ( (job2 == 0) || (job1 == 0) )
	  {
	    machine1++;
	    if ( machine1 == PP->m+1 )
	      {
		job1 = 0;
		machine1 = 1;
		return !OK;
	      }
	    job1 = P[0]->GetJOsucc( 0, machine1 );
	    job2 = P[0]->GetJOsucc( job1, machine1 );
	  }
	if ( test==OK )
	  {
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
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_API_Ngbh::do_move()
 {
   int predM1;
   int help;
   *P[1]=*P[0];
   predM1 = P[1]->GetMOpred(job1,machine1);
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

int JSHOP_API_Ngbh::anti_neighbour()
 {
   Lisa_Vector<int>     *cr_job;
   Lisa_Vector<int>     *cr_mach;
   Lisa_Vector<int>     *cr_rand;
   Lisa_Vector<int>     *JOrd;
   Lisa_Order           *cr_order;
   Lisa_JsSchedule      *copied;
   int i, j, cr_count, succ, count;
   int job, machine, pos, test;
   TIMETYP Cmax;

   if ( !( copied = new Lisa_JsSchedule( PP ) ))
     {
       G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   
   if ( !( cr_job = new Lisa_Vector<int>( PP->m * PP->n ) ) )
     {
       G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   if ( !( cr_mach = new Lisa_Vector<int>( PP->m * PP->n ) ) )
     {
       G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   if ( !( JOrd = new Lisa_Vector<int>( PP->n+2 ) ) )
     {
       G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }

   P[0]->SetValue(CMAX);
   Cmax = P[0]->GetValue();

   // determs the critical operations
   cr_count = 0;

   for (i=1; i<=PP->n; i++)
     for (j=1; j<=PP->m; j++)
       //if ( (*PP->sij)[i][j] == 1 )
       if ( ( (*PP->sij)[i][j] == 1 ) 
	    && ( P[0]->GetHead(i,j) + (*PP->time)[i][j] 
		 + P[0]->GetTail(i,j) == Cmax ) )
	 {
	   (*cr_job)[cr_count] = i;
	   (*cr_mach)[cr_count] = j;
	   cr_count++;
	 }
   
   // determs the order to insert the operations
   if ( !( cr_rand = new Lisa_Vector<int>( cr_count ) ) )
     {
       G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   if ( !( cr_order = new Lisa_Order( cr_count ) ) )
     {
       G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   for (i=0; i<cr_count; i++)
     (*cr_rand)[i] = lisa_random( 1, 1000, &seed );

   cr_order->read( cr_rand );
   cr_order->sort();

   // exclude all critical operations
   for (i=0; i<cr_count; i++)
     P[0]->exclude( (*cr_job)[i], (*cr_mach)[i] );

   // now include the excluded operations
   for (i=0; i<cr_count; i++)
     {
       job     = (*cr_job) [ (*cr_order)[i] ];
       machine = (*cr_mach)[ (*cr_order)[i] ];

       // determs the order on the machine
       (*JOrd)[0] = 0;
       succ = 0; count = 1;
       do
	 {
	   succ = P[0]->GetJOsucc( succ, machine );
	   (*JOrd)[count] = succ;
	   count++;
	 }
       while (succ != 0);
       count--;
       
       // determs randomly an insertion position 
       // and test if the insertion is OK
       do
	 {
	   *copied = *P[0];
	   pos = lisa_random( 0, count-1, &seed );
	   test = OK;
	   if ( copied->insert( job, machine, (*JOrd)[pos] ) == CYCLE )
	     test = !OK;
	 }
       while ( test != OK );
       *P[0] = *copied;
     }
   
   // attantion! we have a new solution
   new_solution = true; 

   delete cr_job;
   delete cr_mach;
   delete cr_rand;
   delete cr_order;
   delete JOrd;
   delete copied;

   return OK;
 }

//**************************************************************************

void JSHOP_API_Ngbh::set_objective_type( int o )
 {
   objective_type = o;
 }

//**************************************************************************

void JSHOP_API_Ngbh::set_objective( int z, int a)
 {
   if ((a<0) || (a>=4))
     {
       G_ExceptionList.lthrow("wrong plan in set_objective_type("+ztos(a)+")");
      exit( 7 );
     }
   P[a]->SetValue(z);
 }

//**************************************************************************

TIMETYP JSHOP_API_Ngbh::get_objective_value( int a)
 {
  if ((a<0) || (a>=4))
    {
      G_ExceptionList.lthrow("wrong plan in get_objective_type("+ztos(a)+")");
     exit( 7 );
    }
  return P[a]->GetValue();
 }

//**************************************************************************

int JSHOP_API_Ngbh::init_tabulist( unsigned int length )
  {
   if ( !(tabulist = new Lisa_Tabu( length )) )
     {
       G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
      exit( 7 );
     }
   return OK;
  }

//**************************************************************************

int JSHOP_API_Ngbh::use_tabulist()
  {
   return tabulist->use(tabu_param[0][0],tabu_param[0][1],
			tabu_param[0][2],tabu_param[0][3]);
  }

//**************************************************************************

int JSHOP_API_Ngbh::set_tabulist()
  {
    tabulist->set(tabu_param[1][0],tabu_param[1][1],
		  tabu_param[1][2],tabu_param[1][3]);
    return OK;
  }

//**************************************************************************

void JSHOP_API_Ngbh::store_tabu_param()
  {
    int i;
    for ( i=0; i<=3; i++ )
      tabu_param[1][i] = tabu_param[0][i];
  }

//**************************************************************************

void JSHOP_API_Ngbh::clean_tabu_param()
  {
    int i;
    for ( i=0; i<4; i++ )
      tabu_param[0][i] = 0;
  }

//**************************************************************************

void JSHOP_API_Ngbh::return_schedule( Lisa_JsSchedule *Plan )
  {
    *Plan = *(P[BEST_SOLUTION]);
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_3_CR_Ngbh::JSHOP_3_CR_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // make a order for randomly choice the operations
    if ( !(ROrd = new Lisa_Order(PP->n, PP->m)) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    OrdCount = 0;
    NewOrder = 0;
    int i;
    for ( i=0; i<PP->n*PP->m; i++ )
      ROrd->read_one_key( i, float(lisa_random( 1, 1000, &seed )) );
    ROrd->sort();
    if ( !(temp_schedule = new Lisa_JsSchedule(PP)) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
  }

//**************************************************************************

JSHOP_3_CR_Ngbh::~JSHOP_3_CR_Ngbh()
  {
    delete ROrd;
    delete temp_schedule;
  }

//**************************************************************************

int JSHOP_3_CR_Ngbh::prepare_move( int typ )
  {
   // determs a possible move
   // typ=ENUM : enumerativ
   // typ=RAND : randomly
   int i;

   TIMETYP Cmax;

   clean_tabu_param();
   if ( typ == RAND )
     {
       // make a new order for choice the operations
       if ( NewOrder > 2 )
	 {
	   for ( i=0; i<PP->n*PP->m; i++ )
	     ROrd->read_one_key( i, float(lisa_random( 1, 1000, &seed )) );
	   ROrd->sort();
	   NewOrder = 0;
	   OrdCount = 0;
	 }
       
       // look at all elements of the order and take only critical operations
       P[0]->SetValue(CMAX);
       Cmax = P[0]->GetValue();
       while ( (P[0]->GetHead(  ROrd->row(OrdCount)+1, ROrd->col(OrdCount)+1 )
		+ (*PP->time)[  ROrd->row(OrdCount)+1][ROrd->col(OrdCount)+1] 
		+ P[0]->GetTail(ROrd->row(OrdCount)+1, ROrd->col(OrdCount)+1) 
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

       // only swaps in JO
       witch_swap = JO;
       swaps[0][0] = swaps[1][0] = swaps[2][0] = witch_swap;
       swaps[0][4] = swaps[1][4] = swaps[2][4] = OK;
       swaps[0][1] = machine1;
       swaps[0][2] = job1;
       job2        = P[0]->GetJOsucc( job1, machine1 );
       if (( job2 == 0 ) || ( (*PP->sij)[job2][machine1]==0 ))
	 return !OK;
       swaps[0][3] = job2;
       machine2    = P[0]->GetMOsucc( job1, machine1 );
       if ( (*PP->sij)[job1][machine2]==0 )
	 swaps[1][4] = !OK;
       swaps[1][1] = machine2;
       swaps[1][2] = job1;
       swaps[1][3] = P[0]->GetJOsucc( job1, machine2 );
       if ( (*PP->sij)[swaps[1][3]][machine2]==0 )
	 swaps[1][4] = !OK;
       machine3    = P[0]->GetMOpred( job2, machine1 );
       if ( (*PP->sij)[job2][machine3]==0 )
	 swaps[2][4] = !OK;
       swaps[2][1] = machine3;
       swaps[2][2] = P[0]->GetJOpred( job2, machine3 );
       if ( (*PP->sij)[swaps[2][2]][machine3]==0 )
	 swaps[2][4] = !OK;
       swaps[2][3] = job2;
       head = P[0]->GetHead( job1, machine2 );
       tail = P[0]->GetHead( job2, machine3 )
	      + (*PP->time)[job2][machine3];
       
       // here could stay precedence constraints
       return OK;
     }
   if ( typ == ENUM )
     {
       G_ExceptionList.lthrow("enumerativ method not implemented in osp_3tst.cpp");
       exit(7);
     }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_3_CR_Ngbh::do_move()
 {
   *P[1]=*P[0];
   *temp_schedule = *P[0];
   if (witch_swap==JO)
     {
       if ( (swaps[0][4]==OK) && (swaps[0][2]!=0) && (swaps[0][3]!=0) )
	 {
	   machine1 = swaps[0][1];
	   job1 = swaps[0][2]; job2 = swaps[0][3];
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2) == CYCLE )
	     {
	       swaps[0][4] = !OK;
	       return !OK;
	     }
	   *temp_schedule = *P[1];
	 } 
       else return !OK;
       if ( (P[1]->GetHead(swaps[0][2],swaps[0][1])
	     + (*PP->time)[ swaps[0][2] ][ swaps[0][1] ] >= head)
	    &&(swaps[1][4]==OK) && (swaps[1][2]!=0) && (swaps[1][3]!=0) )
	 {
	   machine1 = swaps[1][1];
	   job1 = swaps[1][2]; job2 = swaps[1][3];
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2) == CYCLE )
	     {
	       swaps[1][4] = !OK;
	       *P[1] = *temp_schedule;
	       //return !OK;
	     }
	 } 
       else swaps[1][4] = !OK;
       if ( (P[1]->GetTail(swaps[0][3],swaps[0][1]) < tail)
	    &&(swaps[2][4]==OK) && (swaps[2][2]!=0) && (swaps[2][3]!=0) )
	 {
	   machine1 = swaps[2][1];
	   job1 = swaps[2][2]; job2 = swaps[2][3];
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2) == CYCLE )
	     {
	       swaps[2][4] = !OK;
	       *P[1] = *temp_schedule;
	       //return !OK;
	     }
	 } 
       else swaps[2][4] = !OK;
       return OK;
     }
   return !OK;
 }

//**************************************************************************
 
int JSHOP_3_CR_Ngbh::use_tabulist()
  {
    int i, ret;
    ret = !OK;
    for ( i=0; i<3; i++ )
      {
	if (tabulist->use(swaps[i][0],swaps[i][1],
			  swaps[i][2],swaps[i][3]) == OK)
	  {
	    ret = OK;
	    swaps[i][4] = OK;
	  }
	else
	  swaps[i][4] = !OK;
      }
    return ret;
  }

//**************************************************************************

int JSHOP_3_CR_Ngbh::set_tabulist()
  {
    int i;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK )
	tabulist->set(swaps[i][0],swaps[i][1],swaps[i][3],swaps[i][2]);
    return OK;
  }

//**************************************************************************

void JSHOP_3_CR_Ngbh::clean_tabu_param()
  {
    int i;
    for ( i=0; i<3; i++ )
      swaps[i][0] = swaps[i][1] = swaps[i][2] = swaps[i][3] = 0;
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_3_API_Ngbh::JSHOP_3_API_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
  }

//**************************************************************************

int JSHOP_3_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i;

   if ( typ == RAND )
     {
       swaps[0][4] = swaps[1][4] = swaps[2][4] = !OK;
       swaps[0][1] = swaps[1][1] = swaps[2][1] = 0;

       for ( i=0; i<3; i++ )
	 {
	   // determs randomly a machine
	   do
	     machine1 = lisa_random( 1, PP->m, &seed );
	   while ( (machine1==swaps[0][1])||(machine1==swaps[1][1])||
		   (machine1==swaps[2][1]) );
	   // determs randomly a job and look if it have a predecessor
	   do
	     {
	       job2 = lisa_random( 1, PP->n, &seed );
	       job1 = P[0]->GetJOpred( job2, machine1 );
	     }
	   while ( job1 == 0 );
	   if (( (*PP->sij)[job1][machine1]==0 ) 
	       || ( (*PP->sij)[job2][machine1]==0 ))
	     return !OK;
	   swaps[i][1] = machine1;
	   swaps[i][2] = job1; swaps[i][3] = job2;
	 }
       
       // here could stay precedence constraints
       return OK;
     }
   if ( typ == ENUM )
     {
       G_ExceptionList.lthrow("enumerativ method not implemented in jsp_3api.cpp"); 
       exit(7);
     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return !OK;
 }

//**************************************************************************

int JSHOP_3_API_Ngbh::do_move()
 {
   int predM1;
   int i;
   *P[1]=*P[0];

   for ( i=0; i<3; i++ )
     if ( swaps[i][4] == OK )
       {
	 machine1 = swaps[i][1];
	 job1 = swaps[i][2]; job2 = swaps[i][3];
	 predM1 = P[1]->GetMOpred(job1,machine1);
	 P[1]->exclude( job1, machine1 );
	 if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
	   {
	     //printf("\nnot OK by insert Job1");
	     return !OK;
	   }
       }
   return OK;
 
 }

//**************************************************************************
 
int JSHOP_3_API_Ngbh::use_tabulist()
  {
    int i, ret;
    ret = !OK;
    for ( i=0; i<3; i++ )
      if (tabulist->use(0,swaps[i][1],
			swaps[i][2],swaps[i][3]) == OK)
	{
	  ret = OK;
	  swaps[i][4] = OK;
	}
    return ret;
  }

//**************************************************************************

int JSHOP_3_API_Ngbh::set_tabulist()
  {
    int i;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK )
	tabulist->set(0,swaps[i][1],swaps[i][3],swaps[i][2]);
    return OK;
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_cr_bl_API_Ngbh::JSHOP_cr_bl_API_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    if ( !(cr_list_j = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !(cr_list_m = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !(direction = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    count = 0;
  }

//**************************************************************************

JSHOP_cr_bl_API_Ngbh::~JSHOP_cr_bl_API_Ngbh()
  {
    delete[] cr_list_j;
    delete[] cr_list_m;
    delete[] direction;
  }

//**************************************************************************

int JSHOP_cr_bl_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i, j, k, l;
    int numb;
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
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in osp_crpi.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_cr_bl_API_Ngbh::do_move()
 {
   int help;
   *P[1]=*P[0];
   P[1]->exclude( job1, machine1 );
   if ( P[1]->insert(job1,machine1,job2) == CYCLE )
     return !OK;
   // the following swap means, that in next time the re-move is set 
   // to be tabu
   help = tabu_param[0][2];
   tabu_param[0][2] = tabu_param[0][3];
   tabu_param[0][3] = help;
   return OK;
 }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_cr_bl_shift_Ngbh::JSHOP_cr_bl_shift_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    JOrd = new Lisa_Vector<int>(PP->n+1);
    if ( !(cr_list_j = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !(cr_list_m = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !(direction = new int[PP->n*PP->m+1]) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    count = 0;
  }

//**************************************************************************

JSHOP_cr_bl_shift_Ngbh::~JSHOP_cr_bl_shift_Ngbh()
  {
    delete[] cr_list_j;
    delete[] cr_list_m;
    delete[] direction;
    delete JOrd;
  }

//**************************************************************************

int JSHOP_cr_bl_shift_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i, j, k, l;
    int numb, succ;
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
	if ( direction[numb]==1 )
	  {
	    // determs randomly a second position in JOrd
	    if ( pos1 == i-1 )
	      return !OK;
	    pos2 = lisa_random( pos1+1, i-1, &seed );
	    job2 = (*JOrd)[pos2-1];
	    if ( (*PP->sij)[job2][machine1]==0 )
	      return !OK;
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
	    if ( (*PP->sij)[job2][machine1]==0 )
	      return !OK;
	    // is this move setting to be tabu ? :
	    tabu_param[0][0] = JO;
	    tabu_param[0][1] = machine1;
	    tabu_param[0][2] = job1;
	    tabu_param[0][3] = -1;
	    // here could stay precedence constraints
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

int JSHOP_cr_bl_shift_Ngbh::do_move()
 {
   int predJ1;
   *P[1]=*P[0];
   if (pos1<pos2)      // shift right
     {
       P[1]->exclude( job1, machine1 );
       if ( P[1]->insert(job1,machine1,job2) == CYCLE )
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
       predJ1 = P[1]->GetJOpred(job2,machine1);
       P[1]->exclude( job1, machine1 );
       if ( P[1]->insert(job1,machine1,predJ1) == CYCLE )
	 {
	   //printf("\nnot OK by insert Job1");
	   return !OK;
	 }
       // the following means, that in next time the re-move is set 
       // to be tabu
       tabu_param[0][3] *= -1;
       return OK;
     }
   return !OK;
 }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_cr_API_Ngbh::JSHOP_cr_API_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
   if ( !(cr_list_j = new int[PP->n*PP->m]) )
     {
       G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   if ( !(cr_list_m = new int[PP->n*PP->m]) )
     {
      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
      exit( 7 );
     }
   count = 0;
  }

//**************************************************************************

JSHOP_cr_API_Ngbh::~JSHOP_cr_API_Ngbh()
  {
    delete[] cr_list_j;
    delete[] cr_list_m;
  }

//**************************************************************************

int JSHOP_cr_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i, j, k;
    int numb;
    TIMETYP Cmax;

    if ( typ == RAND )
      {
	// determs the critical operations
	if ( new_solution == true )
	  {
	    P[0]->SetValue(CMAX);
	    Cmax = P[0]->GetValue();
		
	    count = 0;
	    for ( i=1; i<(PP->n+1); i++ )
	      for ( j=1; j<(PP->m+1); j++ )
		if ( (*PP->sij)[i][j] != 0 )
		  {
		    k = P[0]->GetJOsucc( i, j );
		    if ( ( P[0]->GetHead(i,j) + (*PP->time)[i][j] 
			   + P[0]->GetTail(i,j) == Cmax )
			 && ( (*PP->sij)[k][j] != 0 )
			 && ( P[0]->GetTail( k, j ) 
			      + (*PP->time)[k][j] 
			      == P[0]->GetTail( i, j ) )
			 )
		      {
			cr_list_j[count] = i;
			cr_list_m[count] = j;
			count++;
		      }
		  }
	    count--;
	    new_solution = false;
	  }
	// determs randomly an element
	numb = lisa_random( 0, count, &seed );
	machine1 = cr_list_m[numb];
	job1     = cr_list_j[numb];
	job2 = P[0]->GetJOsucc( job1, machine1 );
 
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
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_crpi.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_cr_API_Ngbh::do_move()
 {
   int help;
   *P[1]=*P[0];

   P[1]->exclude( job1, machine1 );
   if ( P[1]->insert( job1, machine1, job2 ) == CYCLE )
     return !OK;

   // the following swap means, that in next time the re-move is set 
   // to be tabu
   help = tabu_param[0][2];
   tabu_param[0][2] = tabu_param[0][3];
   tabu_param[0][3] = help;
   return OK;

 }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_cr_shift_Ngbh::JSHOP_cr_shift_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    if ( !( JOrd = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( cr_list_j = new Lisa_Vector<int>( PP->n*PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( cr_list_m = new Lisa_Vector<int>( PP->n*PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    cr_count = 0;
  }

//**************************************************************************

JSHOP_cr_shift_Ngbh::~JSHOP_cr_shift_Ngbh()
  {
    delete JOrd;
    delete cr_list_j;
    delete cr_list_m;
  }

//**************************************************************************

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
	if ( new_solution == true )
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
	    new_solution = false;
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

//**************************************************************************

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

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_cr_shift_mix_Ngbh::JSHOP_cr_shift_mix_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    if ( !( JOrd = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( cr_list_j = new Lisa_Vector<int>( PP->n*PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( cr_list_m = new Lisa_Vector<int>( PP->n*PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    cr_count = 0;
  }

//**************************************************************************

JSHOP_cr_shift_mix_Ngbh::~JSHOP_cr_shift_mix_Ngbh()
  {
    delete JOrd;
    delete cr_list_j;
    delete cr_list_m;
  }

//**************************************************************************

int JSHOP_cr_shift_mix_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    TIMETYP Cmax;
    int i, j, k, succ, pred, pos=0, direction;
    int count, count_l, count_r, l_end, r_end;

    if ( typ == RAND )
      {
	// determs randomly whether API or CR_SHIFT
	k = lisa_random( 1, 100000, &seed );
	if ( k <= 25000 )
	  {
	    // make an API neighbour swap
	    // determs randomly a machine
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
	    direction = 1;
	  }
	else
	  {
	    // make critical shift
	    if ( new_solution == true )
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
		new_solution = false;
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
	  }  // end of critical shift  

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

//**************************************************************************

int JSHOP_cr_shift_mix_Ngbh::do_move()
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

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_cr_trans_mix_Ngbh::JSHOP_cr_trans_mix_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    JOrd = new Lisa_Vector<int>( PP->n+1 );       
    if ( !( tabu_0 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( tabu_1 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( LR = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( cij = new Lisa_Matrix<TIMETYP>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( krv = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( krr = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( kr = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( machines = new Lisa_Vector<int>( PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }

  }

//**************************************************************************

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
  }

//**************************************************************************

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
	    // make an API neighbour swap
	    
	    // determs randomly a machine
	    api = true;
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
	    // make a CR-TRANS neighbour swap

	    api = false;
	    // determs the number of cr. ways over each operation
	    if ( new_solution == true )
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

		new_solution = false;
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

	    if( m_count < 0 ){
        /* this is only true fpr cmax i think -marc-
        G_ExceptionList.lthrow("This solution is optimal !",WARNING);
        */
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
	  } // end of CR-TRANS neighbour swap
	
	return OK;
          
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_crtr.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_cr_trans_mix_Ngbh::do_move()
 {
   int predJ1;
   int i, help;
   *P[1]=*P[0];
   
   if ( api == true )
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

//**************************************************************************

int JSHOP_cr_trans_mix_Ngbh::init_tabulist( unsigned int length )
  {
    // the tabulist contains the number of the maschine 
    // and the order of the jobs on this maschine
    if ( !(tabulist = new Lisa_Tabu( length, PP->n+1 )) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    return OK;
  }

//**************************************************************************

int JSHOP_cr_trans_mix_Ngbh::use_tabulist()
  {
   return tabulist->use_vector( tabu_0 );
  }

//**************************************************************************

int JSHOP_cr_trans_mix_Ngbh::set_tabulist()
  {
    tabulist->set_vector( tabu_1 );
    return OK;
  }

//**************************************************************************

void JSHOP_cr_trans_mix_Ngbh::store_tabu_param()
  {
    *tabu_1 = *tabu_0;
  }

//**************************************************************************

void JSHOP_cr_trans_mix_Ngbh::clean_tabu_param()
  {
    tabu_0->fill(0);
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_cr_trans_Ngbh::JSHOP_cr_trans_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    JOrd = new Lisa_Vector<int>( PP->n+1 );       
    if ( !( tabu_0 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( tabu_1 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( LR = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( cij = new Lisa_Matrix<TIMETYP>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( krv = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( krr = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( kr = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( machines = new Lisa_Vector<int>( PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }

  }

//**************************************************************************

JSHOP_cr_trans_Ngbh::~JSHOP_cr_trans_Ngbh()
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
  }

//**************************************************************************

int JSHOP_cr_trans_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int succ, i, j, k, l, r, count, sum;
    int m_count;
    TIMETYP Cmax;
    
    if ( typ == RAND )
      {
	// make a CR-TRANS neighbour swap

	// determs the number of cr. ways over each operation
	if ( new_solution == true )
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
	    
	    new_solution = false;
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

	if( m_count < 0 ){
    /* this is only true fpr cmax i think -marc-
	  G_ExceptionList.lthrow("This solution is optimal !",WARNING);
    */
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
	
	return OK;
          
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_crtr.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_cr_trans_Ngbh::do_move()
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

//**************************************************************************

int JSHOP_cr_trans_Ngbh::init_tabulist( unsigned int length )
  {
    // the tabulist contains the number of the maschine 
    // and the order of the jobs on this maschine
    if ( !(tabulist = new Lisa_Tabu( length, PP->n+1 )) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    return OK;
  }

//**************************************************************************

int JSHOP_cr_trans_Ngbh::use_tabulist()
  {
   return tabulist->use_vector( tabu_0 );
  }

//**************************************************************************

int JSHOP_cr_trans_Ngbh::set_tabulist()
  {
    tabulist->set_vector( tabu_1 );
    return OK;
  }

//**************************************************************************

void JSHOP_cr_trans_Ngbh::store_tabu_param()
  {
    *tabu_1 = *tabu_0;
  }

//**************************************************************************

void JSHOP_cr_trans_Ngbh::clean_tabu_param()
  {
    tabu_0->fill(0);
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_semi_API_Ngbh::JSHOP_semi_API_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    if ( !( JOrd = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
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

    critical = false;
    
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
		critical = true;
		job2 = job1;
	      }
	    else
	      job2 = P[0]->GetJOsucc( job1, machine1 );
	  }
	while (( job2 == 0 ) || ( (*PP->sij)[job2][machine1]==0 ));
	//cout<<"op.("<<job1<<","<<machine1<<";"<<job2<<")\n";

	if ( critical == true )
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
	  } // if ( critical == true )
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
//**************************************************************************
//**************************************************************************

JSHOP_semi_trans_Ngbh::JSHOP_semi_trans_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    JOrd = new Lisa_Vector<int>( PP->n+1 );   
    if ( !( tabu_0 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( tabu_1 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( LR = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( cij = new Lisa_Matrix<TIMETYP>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( krv = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( krr = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( kr = new Lisa_Matrix<int>( PP->n, PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( machines = new Lisa_Vector<int>( PP->m ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }

  }

//**************************************************************************

JSHOP_semi_trans_Ngbh::~JSHOP_semi_trans_Ngbh()
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
  }

//**************************************************************************

int JSHOP_semi_trans_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int succ, i, j, k, l, r, count, sum;
    TIMETYP Cmax;
    
    if ( typ == RAND )
      {
	// determs the number of cr. ways over each operation
	if ( new_solution == true )
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

	    new_solution = false;
	  }
		 
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

	// the sum of numbers of critical ways
	sum = 0;
	for (i=1; i<=count; i++)
	  sum += int( exp(2*log( (double) (*kr)[(*JOrd)[i-1]-1][machine1-1]+1 )) );
		    
	// determs randomly two positions in JOrd
	// it will transpose the sequence between these positions
	k = lisa_random( 1, sum, &seed );
	j = 0;
	for (i=1; i<=count; i++)
	  {
	    j += int( exp(2*log( (double) (*kr)[(*JOrd)[i-1]-1][machine1-1]+1 )) );
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
		j += int( exp(2*log( (double) (*kr)[(*JOrd)[i-1]-1][machine1-1]+1 )) );
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
	return OK;
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in jsp_s_tr.cpp"); 
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int JSHOP_semi_trans_Ngbh::do_move()
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

//**************************************************************************

int JSHOP_semi_trans_Ngbh::init_tabulist( unsigned int length )
  {
    // the tabulist contains the number of the maschine 
    // and the order of the jobs on this maschine
    if ( !(tabulist = new Lisa_Tabu( length, PP->n+1 )) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    return OK;
  }

//**************************************************************************

int JSHOP_semi_trans_Ngbh::use_tabulist()
  {
   return tabulist->use_vector( tabu_0 );
  }

//**************************************************************************

int JSHOP_semi_trans_Ngbh::set_tabulist()
  {
    tabulist->set_vector( tabu_1 );
    return OK;
  }

//**************************************************************************

void JSHOP_semi_trans_Ngbh::store_tabu_param()
  {
    *tabu_1 = *tabu_0;
  }

//**************************************************************************

void JSHOP_semi_trans_Ngbh::clean_tabu_param()
  {
    tabu_0->fill(0);
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_shift_Ngbh::JSHOP_shift_Ngbh( Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    // JOrd[i] is the job on position i on the machine
    if ( !( JOrd = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( tabu_0 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( tabu_1 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    n = PP->n;
  }

//**************************************************************************

JSHOP_shift_Ngbh::~JSHOP_shift_Ngbh()
  {
    delete JOrd;
    delete tabu_0;
    delete tabu_1;
  }

//**************************************************************************

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
       G_ExceptionList.lthrow("enumerativ method not implemented in jsp_shft.cpp"); 
       exit(7);
     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return !OK;
 }

//**************************************************************************

int JSHOP_shift_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
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

//**************************************************************************

int JSHOP_shift_Ngbh::init_tabulist( unsigned int length )
  {
    // the tabulist contains the number of the maschine 
    // and the order of the jobs on this maschine
    if ( !(tabulist = new Lisa_Tabu( length, n+1 )) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    return OK;
  }

//**************************************************************************

int JSHOP_shift_Ngbh::use_tabulist()
  {
   return tabulist->use_vector( tabu_0 );
  }

//**************************************************************************

int JSHOP_shift_Ngbh::set_tabulist()
  {
    tabulist->set_vector( tabu_1 );
    return OK;
  }

//**************************************************************************

void JSHOP_shift_Ngbh::store_tabu_param()
  {
    *tabu_1 = *tabu_0;
  }

//**************************************************************************

void JSHOP_shift_Ngbh::clean_tabu_param()
  {
    tabu_0->fill(0);
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_PI_Ngbh::JSHOP_PI_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi)
                               :JSHOP_API_Ngbh(Plan, PPi){
  JOrd = new Lisa_Vector<int>( PP->n+1 );   // JOrd[i] is the job on position i on the machine
  
  if(JOrd == 0){
    G_ExceptionList.lthrow("Out of memory.",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit(7);
  }
}

//**************************************************************************

JSHOP_PI_Ngbh::~JSHOP_PI_Ngbh(){
  delete JOrd;
}

//**************************************************************************

int JSHOP_PI_Ngbh::prepare_move( int typ ){
#ifdef LISA_DEBUG
  if( typ != RAND && typ != ENUM){
    G_ExceptionList.lthrow("Wrong Parameter in prepare_move()");
    exit(7);
  }
#endif
  // we only support random neighbours and ignore the ENUM parameter

  int succ = 0;
  int i = 0;
	
  // determs randomly a machine
  machine1 = lisa_random( 1, PP->m, &seed );
  
  // get the order on the machine
  do{
    succ = P[0]->GetJOsucc( succ, machine1 );
	  (*JOrd)[i] = succ;
	  i++;
	}while (succ != 0);
	
  // determs randomly two positions in JOrd
  do{
    pos1 = lisa_random( 1, i-1, &seed );
	  
    do{
	    pos2 = lisa_random( 1, i-1, &seed );
	  }while( pos1 == pos2 );
	  
    if ( pos1 > pos2 ){
      succ = pos2;
      pos2 = pos1;
      pos1 = succ;
	  }
	    
    job1 = (*JOrd)[pos1-1];
	  job2 = (*JOrd)[pos2-1];
	  
  }while (0); // here could stay precedence constraints
	
  if(   (*PP->sij)[job1][machine1]==0  
	   || (*PP->sij)[job2][machine1]==0 ) return !OK;
     
   // is this move setting to be tabu ? :
   tabu_param[0][0] = 0;
   tabu_param[0][1] = machine1;
   tabu_param[0][2] = job1;
   tabu_param[0][3] = job2;
   return OK;
}

//**************************************************************************

int JSHOP_PI_Ngbh::do_move(){

  *P[1]=*P[0];
   
  int predJ1 = P[1]->GetJOpred(job1, machine1);
  int predJ2 = P[1]->GetJOpred(job2, machine1);
  
  if ( pos2 == pos1 + 1 ) predJ2 = job2;
  
  P[1]->exclude( job1, machine1 );
  P[1]->exclude( job2, machine1 );
  
  if ( P[1]->insert( job2, machine1, predJ1 ) == CYCLE ) return !OK;
  if ( P[1]->insert( job1, machine1, predJ2 ) == CYCLE ) return !OK;

  // the following means, that in next time the re-move is set 
  // to be tabu
  const int help = tabu_param[0][2];
  tabu_param[0][2] = tabu_param[0][3];
  tabu_param[0][3] = help;
  return OK;
}

//**************************************************************************
//**************************************************************************
//**************************************************************************

JSHOP_trans_Ngbh::JSHOP_trans_Ngbh(Lisa_JsSchedule *Plan, Lisa_JsProblem *PPi )
                                    : JSHOP_API_Ngbh( Plan, PPi )
  {
    JOrd = new Lisa_Vector<int>( PP->n+1 );   // JOrd[i] is the job on position i on the machine
    if ( !( tabu_0 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    if ( !( tabu_1 = new Lisa_Vector<int>( PP->n+1 ) ) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
  }

//**************************************************************************

JSHOP_trans_Ngbh::~JSHOP_trans_Ngbh()
  {
    delete JOrd;
    delete tabu_0;
    delete tabu_1;
  }

//**************************************************************************

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

//**************************************************************************

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

//**************************************************************************

int JSHOP_trans_Ngbh::init_tabulist( unsigned int length )
  {
    // the tabulist contains the number of the maschine 
    // and the order of the jobs on this maschine
    if ( !(tabulist = new Lisa_Tabu( length, PP->n+1 )) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    return OK;
  }

//**************************************************************************

int JSHOP_trans_Ngbh::use_tabulist()
  {
   return tabulist->use_vector( tabu_0 );
  }

//**************************************************************************

int JSHOP_trans_Ngbh::set_tabulist()
  {
    tabulist->set_vector( tabu_1 );
    return OK;
  }

//**************************************************************************

void JSHOP_trans_Ngbh::store_tabu_param()
  {
    *tabu_1 = *tabu_0;
  }

//**************************************************************************

void JSHOP_trans_Ngbh::clean_tabu_param()
  {
    tabu_0->fill(0);
  }

//**************************************************************************
//**************************************************************************
