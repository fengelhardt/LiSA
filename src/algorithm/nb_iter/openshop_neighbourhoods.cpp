/**
 * @author  Andreas Winkler
 * @version 2.3final
 */
 
#include <cstdlib>
#include "openshop_neighbourhoods.hpp"

//**************************************************************************

OSHOP_API_Ngbh::OSHOP_API_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi ){
  machine1 = 1;
  witch_swap = JO;
  job1 = 0;
  seed = 247639875L;
  PP = PPi;

  if ( !( P[0] = new Lisa_OsSchedule( PP ) )){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
  
  *(P[0])=*Plan;

  if ( !( P[1] = new Lisa_OsSchedule( PP ) )){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }

  if ( !( P[2] = new Lisa_OsSchedule( PP ) )){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }

  if ( !( P[3] = new Lisa_OsSchedule( PP ) )){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
   
   tabulist = 0;
}

//**************************************************************************

OSHOP_API_Ngbh::~OSHOP_API_Ngbh(){
    if( P[0] ) delete P[0];
    if( P[1] ) delete P[1];
    if( P[2] ) delete P[2];
    if( P[3] ) delete P[3];
    if( tabulist ) delete tabulist;
}

//**************************************************************************

int OSHOP_API_Ngbh::copy_schedule( int a , int b ){
  *P[b]=*P[a];
  return OK;
}

//**************************************************************************

int OSHOP_API_Ngbh::accept_solution(){
  return copy_schedule( WORK_SOLUTION, ORIG_SOLUTION );
}

//**************************************************************************
  
int OSHOP_API_Ngbh::accept_best_ngh(){
  return copy_schedule( BEST_NGH_SOLUTION, ORIG_SOLUTION );
}

//**************************************************************************

int OSHOP_API_Ngbh::put_orig_to_best(){
  return copy_schedule( ORIG_SOLUTION, BEST_SOLUTION );
}

//**************************************************************************

int OSHOP_API_Ngbh::put_work_to_best_ngh(){
  return copy_schedule( WORK_SOLUTION, BEST_NGH_SOLUTION );
}

//**************************************************************************

int OSHOP_API_Ngbh::prepare_move( int typ ){
  
  // determs a possible move
  // typ=ENUM : enumerativ
  // typ=RAND : randomly
  int test = OK;

  if ( typ == RAND ){
    // determs randomly if swap in JO oder MO
    witch_swap = lisa_random( 1, 2, &seed );
    if ( witch_swap == JO ){
      
      // determs randomly a machine:
      machine1 = lisa_random( 1, PP->m, &seed );
      
      // determs randomly a job and look, if it has a predecessor
      do{
	    
        do{
          job2 = lisa_random( 1, PP->n, &seed );
        }while ( (*PP->sij)[job2][machine1] == 0 );
	       
        job1 = P[0]->GetJOpred( job2, machine1 );
	    
      }while (( job1 == 0 ) || ( (*PP->sij)[job1][machine1]==0 ));
	     
       // is this move setting to be tabu ? :
	     tabu_param[0][0] = JO;
	     tabu_param[0][1] = machine1;
	     tabu_param[0][2] = job1;
	     tabu_param[0][3] = job2;
       
       // here could standing precedence constraints
      return OK;
    
    }else{
	    
      //determs randomly a job:
      job1 = lisa_random( 1, PP->n, &seed );
      // determs randomly a machine and look, if it have a predecessor
      do{
	    
        do{
          machine2 = lisa_random( 1, PP->m, &seed );
        }while ( (*PP->sij)[job1][machine2] == 0 );
	      
        machine1 = P[0]->GetMOpred( job1, machine2 );
      
      }while (( machine1 == 0 ) || ( (*PP->sij)[job1][machine1]==0 ));
	     
      // is this move setting to be tabu ? :
	    tabu_param[0][0] = MO;
	    tabu_param[0][1] = job1;
	    tabu_param[0][2] = machine1;
	    tabu_param[0][3] = machine2;
      
      // here could stay precedence constraints
     return OK;
    
    }
      
  }else if ( typ == ENUM ){

    int return_OK = !OK;
    
    while ( return_OK == !OK ){
      
      test = OK;
      if (witch_swap == JO){ // swap in JO
        job1 = P[0]->GetJOsucc( job1, machine1 );
	      job2 = P[0]->GetJOsucc( job1, machine1 );
	      
        if (( job1 == 0 ) || ( job2 == 0 )){
          machine1++;
          
          if ( machine1 == PP->m+1 ){
            
            witch_swap = MO;
            job1 = 1;
            machine1 = 0;
            
            //return !OK;
            test = !OK;
          }
		  
          if ( test==OK ){
            job1 = P[0]->GetJOsucc( 0, machine1 );
            job2 = P[0]->GetJOsucc( job1, machine1 );
          }
        }
	      
        if ( test==OK ){
		   
          //return !OK;
          if (( (*PP->sij)[job1][machine1]==0 ) || ( (*PP->sij)[job2][machine1]==0 )) continue;
		   
          // is this move setting to be tabu ? :
          tabu_param[0][0] = JO;
          tabu_param[0][1] = machine1;
          tabu_param[0][2] = job1;
          tabu_param[0][3] = job2;
          
          //return !OK;
          if ( use_tabulist() != OK ) continue;
		   
          return_OK = OK;
        }
      }else if (witch_swap == MO){ // swap in MO 
	     
        machine1 = P[0]->GetMOsucc( job1, machine1 );
        machine2 = P[0]->GetMOsucc( job1, machine1 );
        
        if (( machine1 == 0 ) || ( machine2 == 0 )){
          
          job1++;
          
          if ( job1 == PP->n+1 ){
            witch_swap = JO;
            job1 = 0;
            machine1 = 1;
            //return NO_NGHBOURS;
            test = !OK;
          }
          
          if ( test==OK ){
            machine1 = P[0]->GetMOsucc( job1, 0 );
            machine2 = P[0]->GetMOsucc( job1, machine1 );
          }
        }
	      
        //return !OK;
        if (( (*PP->sij)[job1][machine1]==0 ) || ( (*PP->sij)[job1][machine2]==0 )) continue;
	      
        // is this move setting to be tabu ? :
	      tabu_param[0][0] = MO;
	      tabu_param[0][1] = job1;
	      tabu_param[0][2] = machine1;
	      tabu_param[0][3] = machine2;
	      
        //return !OK;   
        if ( use_tabulist() != OK ) continue;
	      
        return_OK = OK;
      }

    }
    
    return OK;

  }

  G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
  return !OK;

}

//**************************************************************************

int OSHOP_API_Ngbh::do_move(){

  int predJ1,predM1;
  
  *P[1]=*P[0];
  
  if (witch_swap==JO){ // swap in JO
    predM1 = P[1]->GetMOpred(job1,machine1);
    P[1]->exclude( job1, machine1 );
    
    if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE ){
	    //printf("\nnot OK by insert Job1");
	    return !OK;
    }
    
    // the following swap means, that in next time the re-move is set 
    // to be tabu
    const int help = tabu_param[0][2];
    tabu_param[0][2] = tabu_param[0][3];
    tabu_param[0][3] = help;
    return OK;
  }else if (witch_swap==MO){   // swap in MO
    predJ1 = P[1]->GetJOpred(job1,machine1);
    P[1]->exclude( job1, machine1 );
    
    if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE ){
	    //printf("\nnot OK by insert machine1");
	    return !OK;
    }
    
    // the following swap means, that in next time the re-move is set 
    // to be tabu
    const int help = tabu_param[0][2];
    tabu_param[0][2] = tabu_param[0][3];
    tabu_param[0][3] = help;
    return OK;
  }

  return !OK;
}

//**************************************************************************

int OSHOP_API_Ngbh::anti_neighbour(){
  return OK;
}

//**************************************************************************

void OSHOP_API_Ngbh::set_objective_type( int o ){
  objective_type = o;
}

//**************************************************************************

void OSHOP_API_Ngbh::set_objective( int z, int a){
#ifdef LISA_DEBUG
  if ((a<0) || (a>=4)){
    G_ExceptionList.lthrow("wrong plan in set_objective_type("+ztos(a)+")");
    exit( 7 );
  }
#endif
  P[a]->SetValue(z);
}

//**************************************************************************

TIMETYP OSHOP_API_Ngbh::get_objective_value( int a){
#ifdef LISA_DEBUG
  if ((a<0) || (a>=4)){
    G_ExceptionList.lthrow("wrong plan in get_objective_type("+ztos(a)+")");
    exit( 7 );
  }
#endif

  return P[a]->GetValue();
}

//**************************************************************************

int OSHOP_API_Ngbh::init_tabulist( unsigned int length ){
  if( tabulist ) delete tabulist;
  if( !(tabulist = new Lisa_Tabu( length )) ){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
  return OK;
}

//**************************************************************************

int OSHOP_API_Ngbh::use_tabulist(){
  return tabulist->use(tabu_param[0][0],tabu_param[0][1],
                       tabu_param[0][2],tabu_param[0][3]);
}

//**************************************************************************

int OSHOP_API_Ngbh::set_tabulist(){
  tabulist->set(tabu_param[1][0],tabu_param[1][1],
		            tabu_param[1][2],tabu_param[1][3]);
  return OK;
}

//**************************************************************************

void OSHOP_API_Ngbh::store_tabu_param(){
  for (int i=0; i<=3; i++ ) tabu_param[1][i] = tabu_param[0][i];
}

//**************************************************************************

void OSHOP_API_Ngbh::clean_tabu_param(){
  for (int i=0; i<4; i++ ) tabu_param[0][i] = 0;
}

//**************************************************************************

void OSHOP_API_Ngbh::return_schedule( Lisa_OsSchedule *Plan ){
  *Plan = *(P[BEST_SOLUTION]);
}

//**************************************************************************
//**************************************************************************
//**************************************************************************

OSHOP_3_API_Ngbh::OSHOP_3_API_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
  }

//**************************************************************************

int OSHOP_3_API_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int i;

   if ( typ == RAND )
     {
       // determs randomly, if swap in JO or MO, equal for the 3 machines 
       // or jobs
       witch_swap = lisa_random( 1, 2, &seed );
       swaps[0][0] = swaps[1][0] = swaps[2][0] = witch_swap;
       swaps[0][4] = swaps[1][4] = swaps[2][4] = !OK;
       swaps[0][1] = swaps[1][1] = swaps[2][1] = 0;
       //witch_swap = JO;
       if ( witch_swap == JO )
	 {
	   //cout << "\nJO  ";
	   for ( i=0; i<3; i++ )
	     {
	       // determs randomly a machine
	       do
		 machine1 = lisa_random( 1, PP->m, &seed );
	       while ( (machine1==swaps[0][1])||(machine1==swaps[1][1])||
		       (machine1==swaps[2][1]) );
	       // determs randomly a job and look, if it have a predecessor
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
       else
	 {
	   //cout << "\nMO  ";
	   for ( i=0; i<3; i++ )
	     {
	       //determs randomly a job
	       do
		 job1 = lisa_random( 1, PP->n, &seed );
	       while ( (job1==swaps[0][1])||(job1==swaps[1][1])||
		       (job1==swaps[2][1]) );
	       // determs randomly a machine and look, if it have a predec.
	       do
		 {
		   machine2 = lisa_random( 1, PP->m, &seed );
		   machine1 = P[0]->GetMOpred( job1, machine2 );
		 }
	       while ( machine1 == 0 );
	      if (( (*PP->sij)[job1][machine1]==0 ) 
		   || ( (*PP->sij)[job1][machine2]==0 ))
		 return !OK; 
	       swaps[i][1] = job1;
	       swaps[i][2] = machine1; swaps[i][3] = machine2;
	     }

           // here could stay precedence constraints
           return OK;
	 }
     }
   if ( typ == ENUM )
     {
       G_ExceptionList.lthrow("enumerativ method not implemented in osp_3api.cpp");
       exit(7);
     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return !OK;
 }

//**************************************************************************

int OSHOP_3_API_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
   int i;
   *P[1]=*P[0];
   if (witch_swap==JO)  
     {
       //cout << "\nJO  ";
       for ( i=0; i<3; i++ )
	 if ( swaps[i][4] == OK )
	   {
	     machine1 = swaps[i][1];
	     job1 = swaps[i][2]; job2 = swaps[i][3];
	     predM1 = P[1]->GetMOpred(job1,machine1);
	     P[1]->exclude( job1, machine1 );
	     if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	       {
		 //printf("\nnot OK by insert Job1");
		 return !OK;
	       }
	   }
       return OK;
     }

   if (witch_swap==MO)  
     {
       //cout << "\nMO  ";
       for ( i=0; i<3; i++ )
	 if ( swaps[i][4] == OK )
	   {
	     job1 = swaps[i][1];
	     machine1 = swaps[i][2]; machine2 = swaps[i][3];
	     predJ1 = P[1]->GetJOpred(job1,machine1);
	     P[1]->exclude( job1, machine1 );
	     if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	       {
		 //printf("\nnot OK by insert machine1");
		 return !OK;
	       }
	   }
       return OK;
     }
   return !OK;
 }

//**************************************************************************
 
int OSHOP_3_API_Ngbh::use_tabulist()
  {
    int i, ret;
    ret = !OK;
    for ( i=0; i<3; i++ )
      if (tabulist->use(swaps[i][0],swaps[i][1],
			swaps[i][2],swaps[i][3]) == OK)
	{
	  ret = OK;
	  swaps[i][4] = OK;
	}
    return ret;
  }

//**************************************************************************

int OSHOP_3_API_Ngbh::set_tabulist()
  {
    int i;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK )
	tabulist->set(swaps[i][0],swaps[i][1],swaps[i][3],swaps[i][2]);
    return OK;
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

OSHOP_3_CR_Ngbh::OSHOP_3_CR_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
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
    if ( !(temp_schedule = new Lisa_OsSchedule(PP)) )
      {
	G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
  }

//**************************************************************************

OSHOP_3_CR_Ngbh::~OSHOP_3_CR_Ngbh()
  {
    delete ROrd;
    delete temp_schedule;
  }

//**************************************************************************

int OSHOP_3_CR_Ngbh::prepare_move( int typ )
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
	    //cout<<"new order\n";
	    for ( i=0; i<PP->n*PP->m; i++ )
	      ROrd->read_one_key( i, float(lisa_random( 1, 1000, &seed )) );
	    ROrd->sort();
	    NewOrder = 0;
	    OrdCount = 0;
	  }
	
	// look at all elements of the order and take only critical operations
       
	P[0]->SetValue(CMAX);
	Cmax = P[0]->GetValue();
	while ( (P[0]->GetHead( ROrd->row(OrdCount)+1, ROrd->col(OrdCount)+1 )
		 + (*PP->time)[ ROrd->row(OrdCount)+1][ROrd->col(OrdCount)+1] 
		 + P[0]->GetTail(ROrd->row(OrdCount)+1,ROrd->col(OrdCount)+1) 
		 != Cmax ) 
	    || ((*PP->sij)[ROrd->row(OrdCount)+1][ROrd->col(OrdCount)+1]==0)) 
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
	//cout<<" ("<<job1<<","<<machine1<<") ";
	OrdCount++;
	if ( OrdCount == PP->n * PP->m )
	  {
	    OrdCount = 0;
	    NewOrder++;
	  }
	
	// determs randomly if swap in JO or MO, equal for all proposals
	witch_swap = lisa_random( 1, 2, &seed );
	//witch_swap = JO;
	swaps[0][0] = swaps[1][0] = swaps[2][0] = witch_swap;
	swaps[0][4] = swaps[1][4] = swaps[2][4] = OK;
	if ( witch_swap == JO )
	  {
	    swaps[0][1] = machine1;
	    swaps[0][2] = job1;
	    job2        = P[0]->GetJOsucc( job1, machine1 );
	    if (( job2==0 )||( (*PP->sij)[job2][machine1]==0 ))
	      return !OK;
	    swaps[0][3] = job2;
	    machine2    = P[0]->GetMOsucc( job1, machine1 );
	    if ( (*PP->sij)[job1][machine2]==0 )
	      swaps[1][4] = !OK;
	    swaps[1][1] = machine2;
	    swaps[1][2] = job1;
	    swaps[1][3] = P[0]->GetJOsucc( job1, machine2 );
	    if ( (*PP->sij)[swaps[1][3]][machine2]==0 )
	      swaps[1][4] =  !OK;
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
	else
	  {
	    swaps[0][1] = job1;
	    swaps[0][2] = machine1;
	    machine2    = P[0]->GetMOsucc( job1, machine1 );
	    if (( machine2 == 0 )||( (*PP->sij)[job1][machine2]==0 ))
	      return !OK;
	    swaps[0][3] = machine2;
	    job2        = P[0]->GetJOsucc( job1, machine1 );
	    if ( (*PP->sij)[job2][machine1]==0 )
	      swaps[1][4] = !OK;
	    swaps[1][1] = job2;
	    swaps[1][2] = machine1;
	    swaps[1][3] = P[0]->GetMOsucc( job2, machine1 );
	    if ( (*PP->sij)[job2][swaps[1][3]]==0 )
	      swaps[1][4] = !OK;
	    job3        = P[0]->GetJOpred( job1, machine2 );
	    if ( (*PP->sij)[job3][machine2]==0 )
	      swaps[2][4] = !OK;
	    swaps[2][1] = job3;
	    swaps[2][2] = P[0]->GetMOpred( job3, machine2 );
	    if ( (*PP->sij)[job3][swaps[2][2]]==0 )
	      swaps[2][4] = !OK;
	    swaps[2][3] = machine2;
	    head = P[0]->GetHead( job2, machine1 );
	    tail = P[0]->GetTail( job3, machine2 )
	           + (*PP->time)[job3][machine2];

	    // here could stay precedence constraints
	    return OK;
	  } 
      }
    if ( typ == ENUM )
      {
	G_ExceptionList.lthrow("enumerativ method not implemented in osp_3_cr.cpp");
	exit(7);
      }
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;
  }

//**************************************************************************

int OSHOP_3_CR_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
   *P[1] = *P[0];
   *temp_schedule = *P[0];
   //swaps[1][4]=!OK; 
   //swaps[2][4]=!OK;
   if (witch_swap==JO)
     {
       if ( (swaps[0][4]==OK) && (swaps[0][2]!=0) && (swaps[0][3]!=0) )
	 {
	   machine1 = swaps[0][1];
	   job1 = swaps[0][2]; job2 = swaps[0][3];
	   predM1 = P[1]->GetMOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	     {
	       swaps[0][4] = !OK;
	       return !OK;
	     }
	   *temp_schedule = *P[1];
	 }
       else return !OK;
       if ( (P[1]->GetHead(swaps[0][2],swaps[0][1])
	    + (*PP->time)[ swaps[0][2] ][ swaps[0][1] ] >= head)
	    && (swaps[1][4]==OK) && (swaps[1][2]!=0) && (swaps[1][3]!=0) )
	 {
	   machine1 = swaps[1][1];
	   job1 = swaps[1][2]; job2 = swaps[1][3];
	   predM1 = P[1]->GetMOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	     {
	       swaps[1][4] = !OK;
	       *P[1] = *temp_schedule;
	       //return !OK;
	     }
	 } 
       else swaps[1][4] = !OK;
       if ( (P[1]->GetHead(swaps[0][3],swaps[0][1])<=tail)
	    && (swaps[2][4]==OK) && (swaps[2][2]!=0) && (swaps[2][3]!=0) )
	 {
	   machine1 = swaps[2][1];
	   job1 = swaps[2][2]; job2 = swaps[2][3];
	   predM1 = P[1]->GetMOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,job2,predM1) == CYCLE )
	     {
	       swaps[2][4] = !OK;
	       *P[1] = *temp_schedule; 
	       //return !OK;
	     }
	 } 
       else swaps[2][4] = !OK;
       return OK;
     }
   if (witch_swap==MO) 
     {
       if ( (swaps[0][4]==OK) && (swaps[0][2]!=0) && (swaps[0][3]!=0) )
	 {
	   job1 = swaps[0][1];
	   machine1 = swaps[0][2]; machine2 = swaps[0][3];
	   predJ1 = P[1]->GetJOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	     {
	       swaps[0][4] = !OK;
	       return !OK;
	     }
	   *temp_schedule = *P[1];
	 } 
       else return !OK;
       if ( (P[1]->GetHead(swaps[0][1],swaps[0][2])
	     + (*PP->time)[ swaps[0][1] ][ swaps[0][2] ] >= head)
	    && (swaps[1][4]==OK) && (swaps[1][2]!=0) && (swaps[1][3]!=0) )
	 {
	   job1 = swaps[1][1];
	   machine1 = swaps[1][2]; machine2 = swaps[1][3];
	   predJ1 = P[1]->GetJOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
	     {
	       swaps[1][4] = !OK;
	       *P[1] = *temp_schedule;
	       //return !OK;
	     }
	 }   
       else swaps[1][4] = !OK;
       if ( (P[1]->GetHead(swaps[0][1],swaps[0][3])<=tail)
	    && (swaps[2][4]==OK) && (swaps[2][2]!=0) && (swaps[2][3]!=0) )
	 {
	   job1 = swaps[2][1];
	   machine1 = swaps[2][2]; machine2 = swaps[2][3];
	   predJ1 = P[1]->GetJOpred(job1,machine1);
	   P[1]->exclude( job1, machine1 );
	   if ( P[1]->insert(job1,machine1,predJ1,machine2) == CYCLE )
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
 
int OSHOP_3_CR_Ngbh::use_tabulist()
  {
    int i, ret;
    ret = !OK;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK ) 
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

int OSHOP_3_CR_Ngbh::set_tabulist()
  {
    int i;
    for ( i=0; i<3; i++ )
      if ( swaps[i][4] == OK )
	tabulist->set(swaps[i][0],swaps[i][1],swaps[i][3],swaps[i][2]);
    return OK;
  }

//**************************************************************************

void OSHOP_3_CR_Ngbh::clean_tabu_param()
  {
    int i;
    for ( i=0; i<3; i++ )
      swaps[i][0] = swaps[i][1] = swaps[i][2] = swaps[i][3] = 0;
  }

//**************************************************************************
//**************************************************************************
//**************************************************************************

OSHOP_cr_bl_API_Ngbh::OSHOP_cr_bl_API_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
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

OSHOP_cr_bl_API_Ngbh::~OSHOP_cr_bl_API_Ngbh()
  {
    delete[] cr_list_j;
    delete[] cr_list_m;
    delete[] direction;
  }

//**************************************************************************

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

//**************************************************************************

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

//**************************************************************************
//**************************************************************************
//**************************************************************************

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

//**************************************************************************

OSHOP_cr_bl_shift_Ngbh::~OSHOP_cr_bl_shift_Ngbh()
  {
    delete cr_list_j;
    delete cr_list_m;
    delete direction;
    delete JOrd;
    delete MOrd;
  }

//**************************************************************************

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

//**************************************************************************

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

//**************************************************************************
//**************************************************************************
//**************************************************************************

OSHOP_cr_TST_Ngbh::OSHOP_cr_TST_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
    // create a randomly order of the operatins
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
//**************************************************************************
//**************************************************************************

OSHOP_cr_API_Ngbh::OSHOP_cr_API_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
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
//**************************************************************************
//**************************************************************************

OSHOP_cr_shift_Ngbh::OSHOP_cr_shift_Ngbh(Lisa_OsSchedule *Plan,Lisa_OsProblem *PPi)
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
    cr_list_j = new Lisa_Vector<int>(PP->n*PP->m);
    cr_list_m = new Lisa_Vector<int>(PP->n*PP->m);
    count = 0;
    JOrd = new Lisa_Vector<int>(PP->n+1);   // JOrd[i] is the job on position i on the machine
    MOrd = new Lisa_Vector<int>(PP->m+1);   // MOrd[i] is ...
  }

//**************************************************************************

OSHOP_cr_shift_Ngbh::~OSHOP_cr_shift_Ngbh()
  {
    delete cr_list_j;
    delete cr_list_m;
    delete JOrd;
    delete MOrd;
  }

//**************************************************************************

int OSHOP_cr_shift_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int pos_job1=0, pos_job2, pos_mach1=0, pos_mach2;
    int test, succ, i, j, numb;
    test = OK;
    TIMETYP Cmax;

   if ( typ == RAND )
     {

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
		   (*cr_list_j)[count] = i;
		   (*cr_list_m)[count] = j;
		   count++;
		 }
	     }
       count--;

       // determs randomly an element
       numb = lisa_random( 0, count, &seed );
       machine1 = (*cr_list_m)[numb];
       job1     = (*cr_list_j)[numb];

       // determs the order on the machine1
       succ = 0; i = 0;
       do
	 {
	   succ = P[0]->GetJOsucc( succ, machine1 );
	   (*JOrd)[i] = succ;
	   if ( succ == job1 )
	     pos_job1 = i+1;
	   i++;
	 }
       while (succ != 0);

       // determs randomly a second position in JOrd
       do
	 {
	   do
	     pos_job2 = lisa_random( 1, i-1, &seed );
	   while ( pos_job1 == pos_job2 );
	   job2 = (*JOrd)[pos_job2-1];
	 }
       while (0); // here could stay precedence constraints

       // determs the order on the job1
       succ = 0; i = 0;
       do
	 {
	   succ = P[0]->GetMOsucc( job1, succ );
	   (*MOrd)[i] = succ;
	   if ( succ == machine1 )
	     pos_mach1 = i+1;
	   i++;
	 }
       while (succ != 0);	

       // determs randomly a second position in Ord
       do
	 {
	   do
	     pos_mach2 = lisa_random( 1, i-1, &seed );
	   while ( pos_mach1 == pos_mach2 );
	   machine2 = (*MOrd)[pos_mach2-1];
	 }
       while (0); // here could stay prcedence constraints

       if (( (*PP->sij)[job1][machine2]==0 ) 
	   || ( (*PP->sij)[job2][machine1]==0 ))
	 return !OK;
       // is this move setting to be tabu ? :
       tabu_param[0][0] = job1;
       tabu_param[0][1] = machine1;
       if (pos_job1<pos_job2) 
	 tabu_param[0][2] = 1;
       else
	 tabu_param[0][2] = -1;
       if (pos_mach1<pos_mach2) 
	 tabu_param[0][3] = 1;
       else
	 tabu_param[0][3] = -1;
       return OK;
     }
   if ( typ == ENUM )
     {
       G_ExceptionList.lthrow("enumerativ method not implemented in osp_crsh.cpp"); 
       exit(7);
      }

   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return !OK;
 }

//**************************************************************************

int OSHOP_cr_shift_Ngbh::do_move()
 {
   int predJ1;
   int predM1;
   *P[1]=*P[0];

   if (tabu_param[0][2]==1)
     predJ1 = job2;
   else
     predJ1 = P[1]->GetJOpred(job2,machine1);

   if (tabu_param[0][3]==1)
     predM1 = machine2; 
   else
     predM1 = P[1]->GetMOpred(job1,machine2);

   P[1]->exclude( job1, machine1 );
   if ( P[1]->insert(job1,machine1,predJ1,predM1) == CYCLE )
     return !OK;
   // the following means, that in next time the re-move is set 
   // to be tabu
   tabu_param[0][2] *= -1;
   tabu_param[0][3] *= -1;
   return OK;
 }

//**************************************************************************
//**************************************************************************
//**************************************************************************

OSHOP_shift_Ngbh::OSHOP_shift_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                  :OSHOP_API_Ngbh( Plan, PPi ){
  witch_swap = JO;
  job1 = 1; job2 = 1;
  machine1 = 1; machine2 = 0;
  JOrd = new Lisa_Vector<int>( PP->n+1 );   // JOrd[i] is the job on position i on the machine
  MOrd = new Lisa_Vector<int>( PP->m+1 );   // MOrd[i] is ...
  JOpos = new Lisa_Vector<int>( PP->n+1 );  // JOpos[i] is the position of job i on the machine
  MOpos = new Lisa_Vector<int>( PP->m+1 );  // MOpos[i] is ...
}

//**************************************************************************

OSHOP_shift_Ngbh::~OSHOP_shift_Ngbh(){
  delete JOrd;
  delete MOrd;
  delete JOpos;
  delete MOpos;
}

//**************************************************************************

int OSHOP_shift_Ngbh::prepare_move( int typ ){
  // determs a possible move
  // typ=ENUM : enumerativ
  // typ=RAND : randomly
  int pos_job1=0, pos_job2, pos_mach1=0, pos_mach2;
  int test, succ, i;
  test = OK;

  if ( typ == RAND ){ // determs randomly a machine and a job
  
    do{
      
      machine1 = lisa_random( 1, PP->m, &seed );
      job1     = lisa_random( 1, PP->n, &seed );
	 
    }while ( (*PP->sij)[job1][machine2]==0 ); 
    
    // determs the order on the machine1
    succ = 0; i = 0;
    
    do{
      succ = P[0]->GetJOsucc( succ, machine1 );
      (*JOrd)[i] = succ;
      if ( succ == job1 ) pos_job1 = i+1;
      i++;
	 
    }while (succ != 0);

    // determs randomly a second position in JOrd
    do{
	   
     do{
       pos_job2 = lisa_random( 1, i-1, &seed );
	   }while ( pos_job1 == pos_job2 );
	   
     job2 = (*JOrd)[pos_job2-1];
    
    }while (0); // here could stay precedence constraints

    // determs the order on the job1
    succ = 0; i = 0;
    
    do{
	   
     succ = P[0]->GetMOsucc( job1, succ );
	   (*MOrd)[i] = succ;
	   if ( succ == machine1 ) pos_mach1 = i+1;
	   i++;
	 
    }while (succ != 0);	

    // determs randomly a second position in MOrd
    do{
	   
      do{
        pos_mach2 = lisa_random( 1, i-1, &seed );
      }while ( pos_mach1 == pos_mach2 );
	   
      machine2 = (*MOrd)[pos_mach2-1];
    }while (0); // here could stay prcedence constraints

    
    if (( (*PP->sij)[job1][machine2]==0 ) || ( (*PP->sij)[job2][machine1]==0 )) 
      return !OK;
       
    // is this move setting to be tabu ? :
    tabu_param[0][0] = job1;
    tabu_param[0][1] = machine1;
    
    if (pos_job1<pos_job2) tabu_param[0][2] = 1;
    else tabu_param[0][2] = -1;
    
    if (pos_mach1<pos_mach2) tabu_param[0][3] = 1;
    else tabu_param[0][3] = -1;
    
    return OK;
  }

  if ( typ == ENUM ){

    int return_OK = !OK;
    
    while ( return_OK == !OK ){
	   
      machine2 += 1;
      if ( machine2 == machine1 ) machine2 += 1;
	   
      if ( machine2 >= PP->m+1 ){
       
        job2 += 1;
        machine2 = 1;
	     
        if ( job2 == job1 ) job2 += 1;
	     
        if ( job2 >= PP->n+1 ){
		    
          job1 += 1;
          job2 = 1;
		    
          if ( job1 >= PP->n+1 ){
            machine1 += 1;
            job1 = 1;
		    
            if ( machine1 >= PP->m+1 ){
              machine1 = 1;
              machine2 = 2;
            }
          }
        }
      } 	   
    
      if (   ( (*PP->sij)[job1][machine1]==0 ) 
	        || ( (*PP->sij)[job2][machine1]==0 )
	        || ( (*PP->sij)[job1][machine2]==0 ) ) continue;	   

      // determs the order on machine1:
      succ = 0;
      i = 0;
	   
      do{
        succ = P[0]->GetJOsucc( succ, machine1 );
        (*JOpos)[succ] = i + 1;
        i++;
      }while (succ != 0);
	   
      pos_job1 = (*JOpos)[job1];
	    pos_job2 = (*JOpos)[job2];

      // determs the order on job1:
      succ = 0;
      i = 0;
	   
      do{
        succ = P[0]->GetMOsucc( job1, succ );
        (*MOpos)[succ] = i + 1;
        i++;
      }while (succ != 0);
     
      pos_mach1 = (*MOpos)[machine1];
      pos_mach2 = (*MOpos)[machine2];

	    //cout<<" ("<<job1<<","<<machine1<<")-("
	    //    <<job2<<","<<machine2<<")";

	    // is this move setting to be tabu ? :
	    tabu_param[0][0] = job1;
	    tabu_param[0][1] = machine1;
	   
      if (pos_job1<pos_job2) tabu_param[0][2] = 1;
	    else tabu_param[0][2] = -1;
	   
      if (pos_mach1<pos_mach2) tabu_param[0][3] = 1;
	    else tabu_param[0][3] = -1;

	    if ( use_tabulist() != OK ) continue;
	    return_OK = OK;
    } 

    return OK;
  }

  G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
  return !OK;
}

//**************************************************************************

int OSHOP_shift_Ngbh::do_move(){
  
  int predJ1;
  int predM1;
  *P[1]=*P[0];

  if (tabu_param[0][2]==1) predJ1 = job2;
  else predJ1 = P[1]->GetJOpred(job2,machine1);

  if (tabu_param[0][3]==1) predM1 = machine2; 
  else predM1 = P[1]->GetMOpred(job1,machine2);

  P[1]->exclude( job1, machine1 );
   
  if ( P[1]->insert(job1,machine1,predJ1,predM1) == CYCLE ) return !OK;
   
  // the following means, that in next time the re-move is set 
  // to be tabu
  tabu_param[0][2] *= -1;
  tabu_param[0][3] *= -1;
  return OK;

}

//**************************************************************************
//**************************************************************************
//**************************************************************************

OSHOP_PI_Ngbh::OSHOP_PI_Ngbh(Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi):
                            OSHOP_API_Ngbh( Plan, PPi ),
                            order(std::max(PP->n,PP->m)+1){
  witch_swap = JO;
  job1 = job2 = 1;
  pos1=pos2=0;
  machine1 = 1;
  machine2 = 0;
}

//**************************************************************************

OSHOP_PI_Ngbh::~OSHOP_PI_Ngbh(){

}

//**************************************************************************

int OSHOP_PI_Ngbh::prepare_move( int typ ){
#ifdef LISA_DEBUG
  if(typ != ENUM && typ != RAND){
    G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
    return !OK;   
  }
#endif

  // we only support random neighbours, enum parameter gets ignored
  witch_swap = lisa_random(0,1,&seed) ? MO : JO;
  if(witch_swap == JO){ // swap two operations in job order on fixed machine1
    
    //find random machine
    int i,succ;
    int count=0;
    const int stopcount = 10*PP->m;
    do{
      machine1 = lisa_random(1,PP->m,&seed);
      machine2 = 0;
    
      //get order on this machine
      i=succ=0;
      do{
        succ = P[0]->GetJOsucc( succ, machine1 );
        order[i++] = succ;
      }while(succ);
   
      if(count++ > stopcount) return NO_NGHBOURS;
    }while(i<3);
    
    //find two distinct random positions in job order
    pos1 = lisa_random(1,i-1,&seed);
    do{
      pos2 = lisa_random(1,i-1,&seed);
    }while(pos1==pos2);
    
    //wlog pos1 < pos2
    if(pos1 > pos2) std::swap(pos1,pos2);
    
    job1 = order[pos1-1];    
    job2 = order[pos2-1];

#ifdef LISA_DEBUG
    // can this happen ? -marc-
    if( (*PP->sij)[job1][machine1]==0){
      G_ExceptionList.lthrow("Found nonexisting operation ("+ztos(job1)+","
                             +ztos(machine1)+") in prepare_move().");
      return !OK;
    }
    if( (*PP->sij)[job2][machine1]==0 ){
      G_ExceptionList.lthrow("Found nonexisting operation ("+ztos(job2)+","
                             +ztos(machine1)+") in prepare_move().");
      return !OK;     
    }
#endif

  }else{ //swap two operations in machine order of fixed job1
    
    //find random job
    int i,succ;
    int count=0;
    const int stopcount = 10*PP->n;
    do{
    
      job1 = lisa_random(1,PP->n,&seed);
      job2 = 0;
    
      //get order of this job
      i=succ=0;
      do{
        succ = P[0]->GetMOsucc(job1,succ);
        order[i++] = succ;
      }while(succ);
      
      if(count++ > stopcount) return NO_NGHBOURS;
    }while(i<3);
    
    //find two distinct random positions in machine order
    pos1 = lisa_random(1,i-1,&seed);
    do{
      pos2 = lisa_random(1,i-1,&seed);
    }while(pos1==pos2);
    
    //wlog pos1 < pos2
    if(pos1 > pos2) std::swap(pos1,pos2);
    
    machine1 = order[pos1-1];    
    machine2 = order[pos2-1];

#ifdef LISA_DEBUG
    // can this happen ? -marc-
    if( (*PP->sij)[job1][machine1]==0){
      G_ExceptionList.lthrow("Found nonexisting operation ("+ztos(job1)+","
                             +ztos(machine1)+") in prepare_move().");
      return !OK;
    }
    if( (*PP->sij)[job1][machine2]==0 ){
      G_ExceptionList.lthrow("Found nonexisting operation ("+ztos(job1)+","
                             +ztos(machine2)+") in prepare_move().");
      return !OK;     
    }
#endif
  }
  
  //save move parameters for tabu check 
  tabu_param[0][0] = job1;
  tabu_param[0][1] = job2;
  tabu_param[0][2] = machine1;
  tabu_param[0][3] = machine2;
  
  return OK;
}

//**************************************************************************

int OSHOP_PI_Ngbh::do_move(){
  *P[1] = *P[0];
  
  if(witch_swap == JO){// swap two operations in job order of fixed machine1
    const int predJ1 = P[1]->GetJOpred(job1,machine1);
    const int predM1 = P[1]->GetMOpred(job1,machine1);
          int predJ2 = P[1]->GetJOpred(job2,machine1);
    const int predM2 = P[1]->GetMOpred(job2,machine1);
  
    if ( pos2 == pos1+1 ) predJ2 = job2;
  
    P[1]->exclude(job1,machine1);
    P[1]->exclude(job2,machine1);
  
    if ( P[1]->insert(job2,machine1,predJ1,predM2) == CYCLE ) return !OK;
    if ( P[1]->insert(job1,machine1,predJ2,predM1) == CYCLE ) return !OK;
  }else{ // swap two operations in machine order of fixed job1
    const int predJ1 = P[1]->GetJOpred(job1,machine1);
    const int predM1 = P[1]->GetMOpred(job1,machine1);
    const int predJ2 = P[1]->GetJOpred(job1,machine2);
          int predM2 = P[1]->GetMOpred(job1,machine2);
  
    if ( pos2 == pos1+1 ) predM2 = machine2;
  
    P[1]->exclude(job1,machine1);
    P[1]->exclude(job1,machine2);
  
    if ( P[1]->insert(job1,machine2,predJ2,predM1) == CYCLE ) return !OK;
    if ( P[1]->insert(job1,machine1,predJ1,predM2) == CYCLE ) return !OK;

    return !OK;
  }

  return OK;
}

//**************************************************************************

