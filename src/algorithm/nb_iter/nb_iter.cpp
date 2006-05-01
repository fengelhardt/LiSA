/**
 * @author  Andreas Winkler
 * @version 2.3final
 */ 

#include "nb_iter.hpp"
using namespace std;

//**************************************************************************

  //put all the defs etc. in a struct to make it usable from outside

//**************************************************************************

//this version is a modification using xml file format
//it also removes the multiple problem input option

//hopefully I did not mess up the algorithm - it's kind of confusing


/* at first three procedures for the iteration of 
     - one machine problems
     - open shop problems
     - job shop problems
   then the main procedure   */
int NB_Iteration::one_mach_iter(Lisa_Values& Values,
                                Lisa_List<Lisa_ScheduleNode>& Starters,
                                Lisa_List<Lisa_ScheduleNode>& Results){
                                  
  Lisa_1Problem *m1_Prob = new Lisa_1Problem( &Values );   
  if (!m1_Prob){
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}

      // here could standing precedence-constraints
      Lisa_Schedule* starter = NULL;
      for ( jj=0; jj<NUMB_PLANS; jj++ )
	{
	  starter = Starters.get().actual_schedule;
      if(!Starters.next()) //stop next loop
								jj = NUMB_PLANS;
                                
						if(starter == NULL)
								{
										G_ExceptionList.lthrow("Invalid starting schedule ... aborting");
										exit(7);
								}
	  if ( m1_Prob->n != starter->get_n() )
	    {
	      G_ExceptionList.lthrow("dimension mismatch in plan and problem");
	      exit(7);
	    }
	    
	  
    Lisa_1Schedule *m1_Plan = new Lisa_1Schedule( m1_Prob );
    if (!m1_Plan){
	    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	    exit( 7 );
	  }

	  m1_Plan->ComputeHeadsTails( 1, 1 );
	  // transfering the datas:
	  //for ( i=1; i<m1_Prob->n+1; i++ )
	  //  {
	  //    cout<<"\ni:"<<i<<" "<<(*plan_in->LR)[i-1][0];
	  //    (*m1_Plan->sequ)[(*plan_in->LR)[i-1][0]] = i;
	  //  }
	  Lisa_Order *LROrder;
	  int JOPred;
	  Lisa_Vector<int> *LRV;
	  if ( !(LRV = new Lisa_Vector<int>(m1_Prob->n)) )
	    {
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  if ( !(LROrder = new Lisa_Order(m1_Prob->n)) )
	    {
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  for ( i=0; i<m1_Prob->n; i++ )
	    (*LRV)[i] = (*starter->LR)[i][0];
	  //for ( i=0; i<m1_Prob->n; i++ )
	    LROrder->read( LRV );
	  LROrder->sort();

	  JOPred = 0;
	  for ( i=0; i<m1_Prob->n; i++ )
	    if ( (*starter->LR)[(*LROrder)[i]][0] != 0 )
	      {
		m1_Plan->insert( (*LROrder)[i]+1,JOPred); 
		JOPred = (*LROrder)[i]+1;
	      }
	  delete LROrder;
	  delete LRV;

	  m1_Plan->write_sequ();
	  m1_Plan->get_Ci(1);
	  cout << "Sq: " << *m1_Plan->sequ << "\n";
	  cout << "Ci: " << *m1_Plan->Ci << "\n";
	  // end of transfering
	  
    API_Neighbourhood  *m1_api;
	  switch( NGBH ){
	    case API:
	      m1_api = new API_Neighbourhood(m1_Plan,m1_Prob);
	      break;
	    case SHIFT:
	      m1_api=new shift_Neighbourhood(m1_Plan,m1_Prob);
	      break;
	    case PI:
        m1_api=new PI_Neighbourhood(m1_Plan,m1_Prob);
        break;
      default: 
	      G_ExceptionList.lthrow("The specified Neighbourhood does not exist");
	      exit(7);
	    }

      if(!m1_api){
        G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
        exit( 7 );
      }
      
	  
	    if( !( it = new Lisa_Iterator() ) ){
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }

	  switch ( METHOD )
	    {
	    case II: it->init( II, TYPE ); break;
	    case SA: it->init( SA, PROB, MAX_STUCK ); break;
	    case SA_anti: it->init( SA_anti, PROB, MAX_STUCK ); break;
	    case TA: it->init( TA, PROB, MAX_STUCK ); break;
	    case TS: /*if (NUMB_NGHB < m1_Prob->n)
	               G_ExceptionList.lthrow("Number of neighbours should be greater",Lisa_ExceptionList::WARNING);*/
	             it->init( TS, TABULENGTH, NUMB_NGHB, TYPE ); break;
	    default: G_ExceptionList.lthrow("wrong METHOD specified in ITERATE");
	      exit(7);
	    }
	  if ( NUMB_STUCKS != MAXINT )
	    it->set_abort_at_stuck( NUMB_STUCKS );
	  if ( ABORT_BOUND != -MAXLONG )
	    it->set_abort_at_bound( ABORT_BOUND );
	  m1_Plan->SetValue( OBJ_TYPE );
	  cout << "\nstart objective_value: " << m1_Plan->GetValue() << "\n";
	  it->iterate( m1_api, OBJ_TYPE, STEPS );
	  delete it;
    it = 0;
	  
	  m1_api->return_schedule( m1_Plan );
	  delete m1_api;

	      
	  // append schedule to solutions
	  m1_Plan->SetValue( OBJ_TYPE );
	  cout << "\nbest objective_value: " << m1_Plan->GetValue() << "\n";
	  
	  // transfering the datas:
      Lisa_Schedule* result;
	  if ( !( result = new Lisa_Schedule( m1_Prob->n, 1 ) ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  result->make_LR();
	  result->make_CIJ();
	  for ( i=1; i<m1_Prob->n+1; i++ )
	    {
	      (*result->LR)[(*m1_Plan->sequ)[i]-1][0] = i;
	      (*result->CIJ)[i-1][0] = (*m1_Plan->Ci)[i];
	    }
        
      Results.append(Lisa_ScheduleNode(result));
      
      cout << "Sq: " << *m1_Plan->sequ << "\n";
      
	  //might be dangerous
      delete result;
	  
	  delete m1_Plan;
	} // NUMB_PLANS
      delete m1_Prob;
  return OK;
} // End of one_mach_iter()

//**************************************************************************

int NB_Iteration::osp_iter(Lisa_Values& Values,
                           Lisa_List<Lisa_ScheduleNode>& Starters,
                           Lisa_List<Lisa_ScheduleNode>& Results){
    
  Lisa_OsProblem *os_Prob = new Lisa_OsProblem(&Values);
  if(!os_Prob ){
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}

    Lisa_Schedule* starter = NULL;
      for ( jj=0; jj<NUMB_PLANS; jj++ )
	{
						starter = Starters.get().actual_schedule;
						if(!Starters.next()) //stop next loop
								jj = NUMB_PLANS;
                              
	  
   Lisa_OsSchedule *os_Plan = new Lisa_OsSchedule( os_Prob );
   if (!os_Plan){
	   G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	   exit( 7 );
	 }
	 os_Plan->ComputeHeadsTails( 1, 1 );

	  if ( os_Prob->n != starter->get_n() )
	    {
	      G_ExceptionList.lthrow("dimension mismatch in plan and problem");
	      exit(7);
	    }

	  // now construct the start-schedule:
	  // from LR
	  Lisa_Order *LROrder;
	  int* MOPred = new int[os_Prob->n+1];
      int* JOPred = new int[os_Prob->m+1];
	  if ( !(LROrder = new Lisa_Order(os_Prob->n, os_Prob->m)) )
	    {
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  LROrder->read( starter->LR );
	  LROrder->sort();
	  
	  for ( i=1; i<os_Prob->n+1; i++ )
	    MOPred[i] = 0;
	  for ( i=1; i<os_Prob->m+1; i++ )
	    JOPred[i] = 0;
	  for ( i=0; i<os_Prob->n*os_Prob->m; i++ )
	    if ( (*starter->LR)[LROrder->row(i)][LROrder->col(i)] != 0 )
	      {	    
		os_Plan->insert(LROrder->row(i)+1,LROrder->col(i)+1,
				JOPred[LROrder->col(i)+1],
				MOPred[LROrder->row(i)+1]); 
		JOPred[LROrder->col(i)+1] = LROrder->row(i)+1;
		MOPred[LROrder->row(i)+1] = LROrder->col(i)+1;
	      }
	  delete LROrder;

	  // End of the schedule-construction
    OSHOP_Ngbh *os_ngbh;
	  switch ( NGBH ){
      case PI:
        os_ngbh = new OSHOP_PI_Ngbh( os_Plan, os_Prob );
	      break;
	    case API:   
	      os_ngbh = new OSHOP_kAPI_Ngbh( os_Plan, os_Prob,1);
	      break;
	    case k_API:   
	      os_ngbh = new OSHOP_kAPI_Ngbh( os_Plan, os_Prob,k);
	      break;
      case SHIFT: 
	      os_ngbh = new OSHOP_shift_Ngbh(os_Plan,os_Prob);
	      break;
   /* case _3_API: /// broken ... doku says it only works with tabu search ... marc
	      os_ngbh = new OSHOP_3_API_Ngbh(os_Plan,os_Prob);
	      break; */
	    case _3_CR:
	      os_ngbh = new OSHOP_3_CR_Ngbh(os_Plan,os_Prob);
	      break;
	    case CR_API:
	      os_ngbh = new OSHOP_cr_API_Ngbh(os_Plan,os_Prob);
	      break; 
	    case BL_API:
	      os_ngbh = new OSHOP_cr_bl_API_Ngbh(os_Plan,os_Prob);
	      break;   
	    case CR_SHIFT:
	      os_ngbh = new OSHOP_cr_shift_Ngbh(os_Plan,os_Prob);
	      break;
	    case BL_SHIFT:
	      os_ngbh= new OSHOP_cr_bl_shift_Ngbh(os_Plan,os_Prob);
	      break;
	    case CR_TST:
	      os_ngbh = new OSHOP_cr_TST_Ngbh(os_Plan,os_Prob);
	      break;
	    case k_REINSERTION:
	      os_ngbh = new OSHOP_kREINSERTION_Ngbh(os_Plan,os_Prob,k);
	      break;        
	    default: 
	      G_ExceptionList.lthrow("The specified Neighbourhood does not exist");
	      exit(7);
	    }
	  
     if(!os_ngbh){  
		   G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		   exit( 7 );
		 }
      
	   if ( !( it = new Lisa_Iterator() ) ){  
	     G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	     exit( 7 );
	   }
      
	  switch ( METHOD )
	    {
	    case II: it->init( II, TYPE ); break;
	    case SA: it->init( SA, PROB, MAX_STUCK ); break;
	    case SA_anti: it->init( SA_anti, PROB, MAX_STUCK ); break;
	    case TA: it->init( TA, PROB, MAX_STUCK ); break;
	    case TS: /* if (NUMB_NGHB < os_Prob->n)
	               G_ExceptionList.lthrow("Number of neighbours should be greater",Lisa_ExceptionList::WARNING);
                  */   it->init( TS, TABULENGTH, NUMB_NGHB, TYPE );break;
	    default: G_ExceptionList.lthrow("wrong METHOD specified in ITERATE");
	      exit(7);
	    }
	  if ( NUMB_STUCKS != MAXINT )
	    it->set_abort_at_stuck( NUMB_STUCKS );
	  if ( ABORT_BOUND != -MAXLONG )
	    it->set_abort_at_bound( ABORT_BOUND );
	  os_Plan->SetValue( OBJ_TYPE );
	  cout << "\nstart objective_value: " << os_Plan->GetValue() << "\n";
	  it->iterate( os_ngbh, OBJ_TYPE, STEPS );
	  delete it;
    it = 0;
	    
	  os_ngbh->return_schedule( os_Plan );
	  delete os_ngbh;

	  // return the schedule in a file
	  os_Plan->SetValue( CMAX );
	  cout << "\nbest Cmax           : " << os_Plan->GetValue() << "\n";
	  os_Plan->SetValue( OBJ_TYPE );
	  cout << "\nbest objective_value: " << os_Plan->GetValue() << "\n";
	  // transfering the datas:
	  Lisa_Schedule* result;
	  if ( !( result = new Lisa_Schedule( os_Prob->n, os_Prob->m ) ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  
	  result->make_LR();
	  os_Plan->write_LR( result->LR );
	     
      Results.append(Lisa_ScheduleNode(result));
      
	  delete result;
	  
	  delete os_Plan;
      delete[]  MOPred;
      delete[] JOPred;
	} // NUMB_PLANS
      delete os_Prob;
  return OK;
} // End of osp_iter()

//**************************************************************************

int NB_Iteration::jsp_iter(Lisa_Values& Values,
                           Lisa_List<Lisa_ScheduleNode>& Starters,
                           Lisa_List<Lisa_ScheduleNode>& Results){
  if ( !(Values.MO)){
	  G_ExceptionList.lthrow("you must define a MO for a job-shop-problem !");
	  exit( 7 );
	}

  Lisa_JsProblem *js_Prob = new Lisa_JsProblem(&Values);
  if (!js_Prob){  
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}

      Lisa_Schedule* starter = NULL;
      for ( jj=0; jj<NUMB_PLANS; jj++ )
	{
						starter = Starters.get().actual_schedule;
						if(!Starters.next()) //stop next loop
								jj = NUMB_PLANS;
            
		Lisa_JsSchedule *js_Plan = new Lisa_JsSchedule( js_Prob );
	  if (!js_Plan){  
	    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	    exit( 7 );
	  }
	  js_Plan->ComputeHeadsTails( 1, 1 );
	  
	  if ( js_Prob->n != starter->get_n() )
	    {
	      G_ExceptionList.lthrow("dimension mismatch in plan and problem");
	      exit(7);
	    }

	  // test, wether LR and MO are match
	  

	  // now construct the start-schedule:
	  // from LR
	  Lisa_Order *LROrder;
	  int* JOPred = new int[js_Prob->m+1];
	  int* MOPred = new int[js_Prob->n+1];
	  if ( !(LROrder = new Lisa_Order(js_Prob->n, js_Prob->m)) )
	    {
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  LROrder->read( starter->LR );
	  LROrder->sort();
	  
	  for ( i=1; i<js_Prob->m+1; i++ )
	    JOPred[i] = 0;

	  for ( i=1; i<js_Prob->n+1; i++ )
	    MOPred[i] = 0;
	    
	  for ( i=0; i<js_Prob->n*js_Prob->m; i++ )
	    if ( (*starter->LR)[LROrder->row(i)][LROrder->col(i)] != 0 )
	      {
		if ( (*js_Prob->MOpred)[LROrder->row(i)+1][LROrder->col(i)+1]
		     != MOPred[LROrder->row(i)+1] )
		  {
		    G_ExceptionList.lthrow("Mismatch between LR and MO !");
		    exit( 7 );
		  }
		js_Plan->insert(LROrder->row(i)+1,LROrder->col(i)+1,
				JOPred[LROrder->col(i)+1]); 
		JOPred[LROrder->col(i)+1] = LROrder->row(i)+1;
		MOPred[LROrder->row(i)+1] = LROrder->col(i)+1;
	      }
	  delete LROrder;
	  // end of schedule-construction

    JSHOP_API_Ngbh   *js_api;
	  switch ( NGBH ){
	    case API:   
	      js_api = new JSHOP_API_Ngbh( js_Plan, js_Prob );
	      break;
	    case PI:   
	      js_api = new JSHOP_PI_Ngbh( js_Plan, js_Prob );
	      break;
	    case SHIFT: 
	      js_api = new JSHOP_shift_Ngbh(js_Plan,js_Prob);
	      break;
	    case TRANS: 
	      js_api = new JSHOP_trans_Ngbh(js_Plan,js_Prob);
	      break;
	    case CR_TRANS: 
	      js_api = new JSHOP_cr_trans_Ngbh(js_Plan,js_Prob);
	      break;
	    case CR_TRANS_MIX: 
	      js_api = new JSHOP_cr_trans_mix_Ngbh(js_Plan,js_Prob);
	      break;
	    case SC_TRANS: 
	      js_api= new JSHOP_semi_trans_Ngbh(js_Plan,js_Prob);
	      break;
	    case _3_API:
	      js_api = new JSHOP_3_API_Ngbh(js_Plan,js_Prob);
	      break;
	    case _3_CR:
	      js_api = new JSHOP_3_CR_Ngbh(js_Plan,js_Prob);
	      break;	      
	    case CR_API:
	      js_api = new JSHOP_cr_API_Ngbh(js_Plan,js_Prob);
	      break; 
	    case SC_API:
	      js_api = new JSHOP_semi_API_Ngbh(js_Plan,js_Prob);
	      break; 
	    case BL_API:
	      js_api=new JSHOP_cr_bl_API_Ngbh(js_Plan,js_Prob);
	      break; 
	    case CR_SHIFT:
	      js_api = new JSHOP_cr_shift_Ngbh(js_Plan,js_Prob);
	      break;
	    case CR_SHIFT_MIX:
	      js_api = new JSHOP_cr_shift_mix_Ngbh(js_Plan,js_Prob);
	      break;
	    case BL_SHIFT:
	      js_api = new JSHOP_cr_bl_shift_Ngbh(js_Plan,js_Prob);
	      break;
	    default: 
	      G_ExceptionList.lthrow("The specified Neighbourhood does not exist");
	      exit(7);
	  }

    if(!js_api){  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
      
	  if ( !( it = new Lisa_Iterator() ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  switch ( METHOD )
	    {
	    case II: it->init( II, TYPE ); break;
	    case SA: it->init( SA, PROB, MAX_STUCK ); break;
	    case SA_anti: it->init( SA_anti, PROB, MAX_STUCK ); break;
	    case TA: it->init( TA, PROB, MAX_STUCK ); break;
	    case TS: /* if (NUMB_NGHB < js_Prob->n)
	               G_ExceptionList.lthrow("Number of neighbours should be greater",Lisa_ExceptionList::WARNING);
                  */   it->init( TS, TABULENGTH, NUMB_NGHB, TYPE );break;
	    default: G_ExceptionList.lthrow("wrong METHOD specified in ITERATE");
	      exit(7);
	    }
	  if ( NUMB_STUCKS != MAXINT )
	    it->set_abort_at_stuck( NUMB_STUCKS );
	  if ( ABORT_BOUND != -MAXLONG )
	    it->set_abort_at_bound( ABORT_BOUND );
	  js_Plan->SetValue( OBJ_TYPE );
	  cout << "\nstart objective_value: " << js_Plan->GetValue() << "\n";
	  it->iterate( js_api, OBJ_TYPE, STEPS );
	  delete it;
    it = 0;
	 
	  js_api->return_schedule( js_Plan );
	  delete js_api;

	  // return the schedule in the file
	  js_Plan->SetValue( OBJ_TYPE );
	  cout << "\nbest objective_value: " << js_Plan->GetValue() << "\n";
	  
	  // transfering the datas:
      Lisa_Schedule *result;
	  if ( !( result = new Lisa_Schedule( js_Prob->n, js_Prob->m ) ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  
	  // constructing LR
	  result->make_LR();
	  js_Plan->write_LR( result->LR );
	
      Results.append(Lisa_ScheduleNode(result));
	     
	  //js_Plan->print();

	  printf("\n");

	  delete js_Plan;
      delete[] JOPred;
	  delete[] MOPred;
	} // NUMB_PLANS
      delete js_Prob; 
  return OK;
} // end of jsp_iter()


bool NB_Iteration::configure(Lisa_ProblemType& Problem,
			                       Lisa_ControlParameters& Parameter,
			                       Lisa_Values& Values){
  // start-values
  NGBH = 0;
  METHOD = 0;
  PROB = 1;
  MAX_STUCK = MAXLONG;
  TABULENGTH = 1;
  NUMB_NGHB = 1;
  TYPE = 1;
  OBJ_TYPE = 1;
  STEPS = 1;
  NUMB_PROBLEMS = 1;
  NUMB_PLANS = 1;
  NUMB_STUCKS = MAXINT;
  ABORT_BOUND = -MAXLONG;
  k = 1;
  
  it = 0;

  OBJ_TYPE = Problem.get_property(OBJECTIVE);
  PROB_TYPE = Problem.get_property(M_ENV);

  if (!(Parameter.defined("NGBH"))){ 
    G_ExceptionList.lthrow("you must define a neighbourhood in the input file");
    return false;//exit(7);
  }
  NGBH_St = Parameter.get_string( "NGBH" );
  
  if (!(Parameter.defined("METHOD"))){ 
    G_ExceptionList.lthrow("you must define a method in the input file");
    return false;//exit(7);
  }
  METHOD_St = Parameter.get_string( "METHOD" );
  
  if ( Parameter.defined("PROB") ) PROB = Parameter.get_long( "PROB" );
  if ( PROB < 0 ){
      G_ExceptionList.lthrow("PROB must be nonnegative");
      return false;//exit(7);
  }
  
  if ( Parameter.defined("MAX_STUCK") ) MAX_STUCK = Parameter.get_long( "MAX_STUCK" );
  if ( MAX_STUCK < 1 ){
    G_ExceptionList.lthrow("MAX_STUCK must be positive");
    return false;//exit(7);
  }
  
  if ( Parameter.defined("TABULENGTH") ) TABULENGTH = Parameter.get_long( "TABULENGTH" );
  if ( TABULENGTH < 1 ){
    G_ExceptionList.lthrow("TABULENGTH must be positive");
    return false;//exit(7);
  }
  
  if ( Parameter.defined("NUMB_NGHB") ) NUMB_NGHB = Parameter.get_long( "NUMB_NGHB" );
  if ( NUMB_NGHB < 1 ) {
    G_ExceptionList.lthrow("NUMB_NGHB must be positive");
    return false;//exit(7);
  }
    
  if ( Parameter.defined("TYPE") ) TYPE_St = Parameter.get_string( "TYPE" );

  if ( Parameter.defined("STEPS") ) STEPS = Parameter.get_long( "STEPS" );
  if ( STEPS < 1 ){
    G_ExceptionList.lthrow("STEPS must be positive");
    return false;//exit(7);
  }
  
  if ( Parameter.defined("NUMB_PROBLEMS") ) NUMB_PROBLEMS = Parameter.get_long( "NUMB_PROBLEMS" );  
  if ( NUMB_PROBLEMS < 1 ){
    G_ExceptionList.lthrow("NUMB_PROBLEMS must be positive");
    return false;//exit(7);
  }
  
  if ( Parameter.defined("NUMB_PLANS") ) NUMB_PLANS = Parameter.get_long( "NUMB_PLANS" );  
  if ( NUMB_PLANS < 1 ){
    G_ExceptionList.lthrow("NUMB_PLANS must be positive");
    return false;//exit(7);
  }
  
  if ( Parameter.defined("NUMB_STUCKS") ) NUMB_STUCKS = Parameter.get_long( "NUMB_STUCKS" );
  if ( NUMB_STUCKS < 1 ){
    G_ExceptionList.lthrow("NUMB_STUCKS must be positive");
    return false;//exit(7);
  }
  
  if ( Parameter.defined("ABORT_BOUND") ) ABORT_BOUND = Parameter.get_double( "ABORT_BOUND" );

  if ( Parameter.defined("k") ) k = Parameter.get_long( "k" ); 
  
  //set flags for algorithms
  cout << "Neighborhood : \"" << NGBH_St << "\"" << endl;
    
       if ( NGBH_St     == "API"              ) NGBH = API;
  else if ( NGBH_St     == "k_API"            ) NGBH = k_API;
  else if ( NGBH_St     == "SHIFT"            ) NGBH = SHIFT;
  else if ( NGBH_St     == "CR_TRANS"         ) NGBH = CR_TRANS;
  else if ( NGBH_St     == "CR_TRANS_MIX"     ) NGBH = CR_TRANS_MIX;
  else if ( NGBH_St     == "SC_TRANS"         ) NGBH = SC_TRANS;
  else if ( NGBH_St     == "TRANS"            ) NGBH = TRANS;
  else if ( NGBH_St     == "3_API"            ) NGBH = _3_API;
  else if ( NGBH_St     == "3_CR"             ) NGBH = _3_CR;
  else if ( NGBH_St     == "CR_API"           ) NGBH = CR_API;
  else if ( NGBH_St     == "SC_API"           ) NGBH = SC_API;
  else if ( NGBH_St     == "BL_API"           ) NGBH = BL_API;
  else if ( NGBH_St     == "CR_SHIFT"         ) NGBH = CR_SHIFT;
  else if ( NGBH_St     == "CR_SHIFT_MIX"     ) NGBH = CR_SHIFT_MIX;
  else if ( NGBH_St     == "BL_SHIFT"         ) NGBH = BL_SHIFT;
  else if ( NGBH_St     == "CR_TST"           ) NGBH = CR_TST;
  else if ( NGBH_St     == "PI"               ) NGBH = PI;
  else if ( NGBH_St     == "k_REINSERTION"    ) NGBH = k_REINSERTION;
  else{
    G_ExceptionList.lthrow("Neighbourhood "+NGBH_St+" unknown.");
    return false;//exit(7);
  }
  
  if      ( METHOD_St   == "IterativeImprovement"       ) METHOD   = II;
  else if ( METHOD_St   == "SimulatedAnnealing"       ) METHOD   = SA;
  else if ( METHOD_St   == "SA_anti"  ) METHOD   = SA_anti;
  else if ( METHOD_St   == "ThresholdAccepting"       ) METHOD   = TA;
  else if ( METHOD_St   == "TabuSearch"       ) METHOD   = TS;
  else{
    G_ExceptionList.lthrow("Method "+METHOD_St+" unknown.");
    return false;//exit(7);
  }
  
  if      ( TYPE_St     == "ENUM"     ) TYPE     = ENUM;
  else if ( TYPE_St     == "RAND"     ) TYPE     = RAND;
  else{
    G_ExceptionList.lthrow("TYPE "+TYPE_St+" unknown.");
    return false;//exit(7);
  }

  if ( METHOD == II )
    cout<<"parameters: "<< STEPS <<" STEPS ";
  if ( (METHOD==SA) || (METHOD==TA) || (METHOD==SA_anti) )
    cout<<"parameters: "<< STEPS <<" STEPS "<< PROB <<" PROB "<< MAX_STUCK <<" MAX_STUCK";
  if ( METHOD == TS )
    cout<<"parameters: "<<STEPS<<" STEPS "<<TABULENGTH<<" TABULENGTH "<<NUMB_NGHB<<" NUMB_NGHB";

  return true;
}


//**************************************************************************
