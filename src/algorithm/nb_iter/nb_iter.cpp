/**
 * @author  Andreas Winkler
 * @version 2.3final
 */ 
#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <time.h>

#include "../../xml/LisaXmlFile.hpp"

#include "../../basics/matrix.hpp"
#include "../../scheduling/m1_sched.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../scheduling/js_sched.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../misc/except.hpp"

#include "single_neighbourhoods.hpp"
#include "openshop_neighbourhoods.hpp"
#include "jobshop_neighbourhoods.hpp"

#include "iterate.hpp"

using namespace std;

//**************************************************************************

const int MAXINT  = 214748000;
const int MAXLONG = 214748000;

// definition of parameters for the file-controlling
// following parameters are possible:
  int    NGBH,       /* NGBH = M1_API, M1_SHIFT, OSP_API, OSP_SHIFT,
		         OSP_3_API, OSP_CR_API, OSP_BL_API, OSP_CR_SHIFT, 
			 OSP_BL_SHIFT, OSP_CR_TST, OSP_3_CR, JSP_API, JSP_SWAP,
			 JSP_SHIFT, JSP_TRANS, JSP_CR_TRANS, JSP_CR_TRANS_MIX,                          
       JSP_SC_TRANS, JSP_3_API, JSP_CR_API, JSP_SC_API, 
                         JSP_BL_API, JSP_CR_SHIFT, JSP_CR_SHIFT_MIX, 
			 JSP_BL_SHIFT, JSP_3_CR */
         METHOD,     // METHOD = II, SA, TA, TS, SA_anti
         PROB,       // PROB = int-Value in [ 0, 100 ]
         MAX_STUCK,  // MAX_STUCK = int-Value
         TABULENGTH, // TABULENGTH = int-Value
         NUMB_NGHB,  // NUMB_NGHB = int-Value
         NUMB_PROBLEMS, // NUMB_PROBLEMS = int-Value
         NUMB_PLANS, // NUMB_PROBLEMS = int-Value
         TYPE,	     // TYPER = ENUM, RAND
         OBJ_TYPE,   // OBJ_TYPE = CMAX, LMAX, SUM_CI, SUM_WICI, SUM_UI,
		     // SUM_WIUI, SUM_TI, SUM_WITI
	 NUMB_STUCKS,// NUMB_STUCKS = int value
	 PROB_TYPE,
         ii, jj;
  long   STEPS;      // STEPS = long-Value
  std::string NGBH_St,    //  M1_API, M1_SHIFT, OSP_API, OSP_SHIFT, OSP_3_API, 
                     //  OSP_CR_API, OSP_BL_API, OSP_CR_SHIFT, OSP_BL_SHIFT, 
		     //	 OSP_CR_TST, OSP_3_CR, JSP_API, JSP_SHIFT, JSP_3_API,
		     //  JSP_CR_API, JSP_BL_API, JSP_CR_SHIFT, JSP_BL_SHIFT,
		     //  JSP_3_CR
         METHOD_St,
         TYPE_St,    //  ENUM, RAND
         OBJ_TYPE_St;//  CMAX, LMAX, SUM_CI, SUM_WICI, SUM_UI,
		     //  SUM_WIUI, SUM_TI, SUM_WITI
  TIMETYP ABORT_BOUND; // ABORT_BOUND = TIMETYP value

  //const int API        =  1;
  //const int SHIFT      =  2;
  const int API          = 11;
  const int SHIFT        = 12;
  const int _3_API       = 13;
  const int CR_API       = 14;
  const int BL_API       = 15;
  const int CR_SHIFT     = 16;
  const int BL_SHIFT     = 17;
  const int CR_TST       = 18;
  const int _3_CR        = 19;
  //const int SWAP         = 20;
  const int TRANS        = 21;
  const int CR_TRANS     = 22;
  const int SC_TRANS     = 23;
  const int SC_API       = 24;
  const int CR_SHIFT_MIX = 25;
  const int CR_TRANS_MIX = 26;
  const int PI           = 27;
  //const int API       = 21;
  //const int SHIFT     = 22;
  //const int 3_API     = 23;
  //const int CR_API    = 24;
  //const int BL_API    = 25;
  //const int CR_SHIFT  = 26;
  //const int BL_SHIFT  = 27;
  //const int 3_CR      = 28;
  const int SingleMachine = 1;
  const int OSP           = 2;
  const int JSP           = 3;

  int art_of_problem;
  int n;  // number of jobs
  int i, j, k;
  int count, succ;

  Lisa_1Problem            *m1_Prob;
  Lisa_1Schedule 	         *m1_Plan;
  Lisa_OsProblem           *os_Prob;
  Lisa_OsSchedule          *os_Plan;
  Lisa_JsProblem           *js_Prob;
  Lisa_JsSchedule          *js_Plan;
  Lisa_Neighbourhood       *ngbh;
  API_Neighbourhood        *m1_api;
  shift_Neighbourhood      *m1_shift;
  PI_Neighbourhood         *m1_pi;
  OSHOP_PI_Ngbh            *os_pi;
  OSHOP_API_Ngbh           *os_api;
  OSHOP_shift_Ngbh         *os_shift;
  OSHOP_3_API_Ngbh         *os_api_3;
  OSHOP_3_CR_Ngbh          *os_cr_3;
  OSHOP_cr_API_Ngbh        *os_cr_api;
  OSHOP_cr_bl_API_Ngbh     *os_bl_api;
  OSHOP_cr_shift_Ngbh      *os_cr_shift;
  OSHOP_cr_bl_shift_Ngbh   *os_bl_shift;
  OSHOP_cr_TST_Ngbh        *os_cr_tst;
  JSHOP_API_Ngbh           *js_api;
  JSHOP_PI_Ngbh            *js_pi;
  JSHOP_shift_Ngbh         *js_shift;
  JSHOP_trans_Ngbh         *js_trans;
  JSHOP_cr_trans_Ngbh      *js_cr_trans;
  JSHOP_cr_trans_mix_Ngbh  *js_cr_trans_mix;
  JSHOP_semi_trans_Ngbh    *js_sc_trans;
  JSHOP_3_API_Ngbh         *js_api_3;
  JSHOP_cr_API_Ngbh        *js_cr_api;
  JSHOP_semi_API_Ngbh      *js_sc_api;
  JSHOP_cr_bl_API_Ngbh     *js_bl_api;
  JSHOP_cr_shift_Ngbh      *js_cr_shift;
  JSHOP_cr_shift_mix_Ngbh  *js_cr_shift_mix;
  JSHOP_cr_bl_shift_Ngbh   *js_bl_shift;
  JSHOP_3_CR_Ngbh          *js_cr_3;
  Lisa_Iterator	           *it;

//**************************************************************************

//this version is a modification using xml file format
//it also removes the multiple problem input option

//hopefully I did not mess up the algorithm - it's kind of confusing


/* at first three procedures for the iteration of 
     - one machine problems
     - open shop problems
     - job shop problems
   then the main procedure   */
int one_mach_iter(Lisa_Values& Values,Lisa_List<Lisa_ScheduleNode>& Starters,Lisa_List<Lisa_ScheduleNode>& Results)
{
      if ( !( m1_Prob = new Lisa_1Problem( &Values ) ) )
	{
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
	    
	  if ( !( m1_Plan = new Lisa_1Schedule( m1_Prob ) ) )
	    {
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
	  
	  switch( NGBH ){
	    case API:
	      if (!(ngbh = m1_api = new API_Neighbourhood(m1_Plan,m1_Prob))){
          G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
          exit( 7 );
        }
	      break;
	    case SHIFT:
	      if(!(ngbh=m1_shift=new shift_Neighbourhood(m1_Plan,m1_Prob))){
          G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
          exit( 7 );
        }
	      break;
	    case PI:
        if(!(ngbh=m1_pi=new PI_Neighbourhood(m1_Plan,m1_Prob))){
          G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
          exit( 7 );
        }
        break;
      default: 
	      G_ExceptionList.lthrow("The specified Neighbourhood does not exist");
	      exit(7);
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
	  it->iterate( ngbh, OBJ_TYPE, STEPS );
	  delete it;
	  
	  switch( NGBH ){
	    case API:
	      m1_api->return_schedule( m1_Plan );
	      delete m1_api;
	      break;
	    case SHIFT:
	      m1_shift->return_schedule( m1_Plan );
	      delete m1_shift;
	      break;
	    case PI:
        m1_pi->return_schedule( m1_Plan );
	      delete m1_pi;
	      break;       
      }
	    
	    
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

int osp_iter(Lisa_Values& Values,Lisa_List<Lisa_ScheduleNode>& Starters,Lisa_List<Lisa_ScheduleNode>& Results)
{
      if ( !( os_Prob = new Lisa_OsProblem(&Values) ) )
	{
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}

    Lisa_Schedule* starter = NULL;
      for ( jj=0; jj<NUMB_PLANS; jj++ )
	{
						starter = Starters.get().actual_schedule;
						if(!Starters.next()) //stop next loop
								jj = NUMB_PLANS;
                              
	  if ( !( os_Plan = new Lisa_OsSchedule( os_Prob ) ) )
	    {
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
	  switch ( NGBH ){
      case PI:
        if(!(ngbh = os_pi = new OSHOP_PI_Ngbh( os_Plan, os_Prob ))){  
          G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
          exit( 7 );
        }
	      break;
	    case API:   
	      if(!(ngbh = os_api = new OSHOP_API_Ngbh( os_Plan, os_Prob ))){  
          G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
          exit( 7 );
        }
	      break;
      case SHIFT: 
	      if(!(ngbh = os_shift = new OSHOP_shift_Ngbh(os_Plan,os_Prob))){  
          G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
          exit( 7 );
        }
	      break;
   /* case _3_API: /// broken ... doku says it only works with tabu search ... marc
	      if(!(ngbh = os_api_3 = new OSHOP_3_API_Ngbh(os_Plan,os_Prob))){  
		      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		      exit( 7 );
	      }
	      break; */
	    case _3_CR:
	      if(!(ngbh = os_cr_3 = new OSHOP_3_CR_Ngbh(os_Plan,os_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case CR_API:
	      if(!(ngbh=os_cr_api = new OSHOP_cr_API_Ngbh(os_Plan,os_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break; 
	    case BL_API:
	      if(!(ngbh=os_bl_api=new OSHOP_cr_bl_API_Ngbh(os_Plan,os_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;   
	    case CR_SHIFT:
	      if(!(ngbh = os_cr_shift 
		   = new OSHOP_cr_shift_Ngbh(os_Plan,os_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case BL_SHIFT:
	      if(!(ngbh = os_bl_shift 
		   = new OSHOP_cr_bl_shift_Ngbh(os_Plan,os_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case CR_TST:
	      if(!(ngbh=os_cr_tst = new OSHOP_cr_TST_Ngbh(os_Plan,os_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    default: 
	      //G_ExceptionList.lthrow("wrong Neighbourhood specified in ITERATE");
	      G_ExceptionList.lthrow("The specified Neighbourhood does not exist");

	      exit(7);
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
	  it->iterate( ngbh, OBJ_TYPE, STEPS );
	  delete it;
	  
	  switch ( NGBH )
	    {
	    case PI:
        os_pi->return_schedule( os_Plan );
        delete os_pi;
        break;
      case API:   
	      os_api->return_schedule( os_Plan );
	      delete os_api;
	      break;
	    case SHIFT:
	      os_shift->return_schedule( os_Plan );
	      delete os_shift;
	      break;
	    case _3_API:
	      os_cr_3->return_schedule( os_Plan );
	      delete os_cr_3;
	      break;
	    case _3_CR:
	      os_cr_3->return_schedule( os_Plan );
	      delete os_cr_3;
	      break;
	    case CR_API:
	      os_cr_api->return_schedule( os_Plan );
	      delete os_cr_api;
	      break; 
	    case BL_API:
	      os_bl_api->return_schedule( os_Plan );
	      delete os_bl_api;
	      break; 
	    case CR_SHIFT:
	      os_cr_shift->return_schedule( os_Plan );
	      delete os_cr_shift;
	      break;
	    case BL_SHIFT:
	      os_bl_shift->return_schedule( os_Plan );
	      delete os_bl_shift;
	      break;
	    case CR_TST:
	      os_cr_tst->return_schedule( os_Plan );
	      delete os_cr_tst;
	      break; 
	    }

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

int jsp_iter(Lisa_Values& Values,Lisa_List<Lisa_ScheduleNode>& Starters,Lisa_List<Lisa_ScheduleNode>& Results)
{
      if ( !(Values.MO))
	{
	  G_ExceptionList.lthrow("you must define a MO for a job-shop-problem !");
	  exit( 7 );
	}

      if ( !( js_Prob = new Lisa_JsProblem(&Values) ) )
	{  
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}

      Lisa_Schedule* starter = NULL;
      for ( jj=0; jj<NUMB_PLANS; jj++ )
	{
						starter = Starters.get().actual_schedule;
						if(!Starters.next()) //stop next loop
								jj = NUMB_PLANS;
						
	  if ( !( js_Plan = new Lisa_JsSchedule( js_Prob ) ) )
	    {  
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

	  switch ( NGBH )
	    {
	    case API:   
	      if(!(ngbh = js_api = new JSHOP_API_Ngbh( js_Plan, js_Prob )))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case PI:   
	      if(!(ngbh = js_pi = new JSHOP_PI_Ngbh( js_Plan, js_Prob )))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case SHIFT: 
	      if(!(ngbh= js_shift = new JSHOP_shift_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		} 
	      break;
	    case TRANS: 
	      if(!(ngbh= js_trans = new JSHOP_trans_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case CR_TRANS: 
	      if(!(ngbh= js_cr_trans 
		   = new JSHOP_cr_trans_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case CR_TRANS_MIX: 
	      if(!(ngbh= js_cr_trans_mix 
		   = new JSHOP_cr_trans_mix_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case SC_TRANS: 
	      if(!(ngbh= js_sc_trans 
		   = new JSHOP_semi_trans_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case _3_API:
	      if(!(ngbh= js_api_3 = new JSHOP_3_API_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case _3_CR:
	      if(!(ngbh= js_cr_3 = new JSHOP_3_CR_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;	      
	    case CR_API:
	      if(!(ngbh=js_cr_api = new JSHOP_cr_API_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break; 
	    case SC_API:
	      if(!(ngbh=js_sc_api = new JSHOP_semi_API_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break; 
	    case BL_API:
	      if(!(ngbh=js_bl_api=new JSHOP_cr_bl_API_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break; 
	    case CR_SHIFT:
	      if(!(ngbh = js_cr_shift 
		   = new JSHOP_cr_shift_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case CR_SHIFT_MIX:
	      if(!(ngbh = js_cr_shift_mix
		   = new JSHOP_cr_shift_mix_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case BL_SHIFT:
	      if(!(ngbh = js_bl_shift 
		   = new JSHOP_cr_bl_shift_Ngbh(js_Plan,js_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    default: 
	      G_ExceptionList.lthrow("The specified Neighbourhood does not exist");
	      exit(7);
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
	  it->iterate( ngbh, OBJ_TYPE, STEPS );
	  delete it;
	 
	  switch ( NGBH )
	    {
	    case API:
	      js_api->return_schedule( js_Plan );
	      delete js_api;
	      break;
	    case PI:
	      js_pi->return_schedule( js_Plan );
	      delete js_pi;
	      break;
	    case SHIFT:
	      js_shift->return_schedule( js_Plan );
	      delete js_shift;
	      break;
	    case TRANS:
	      js_trans->return_schedule( js_Plan );
	      delete js_trans;
	      break;
	    case CR_TRANS:
	      js_cr_trans->return_schedule( js_Plan );
	      delete js_cr_trans;
	      break;
	    case CR_TRANS_MIX:
	      js_cr_trans_mix->return_schedule( js_Plan );
	      delete js_cr_trans_mix;
	      break;
	    case SC_TRANS:
	      js_sc_trans->return_schedule( js_Plan );
	      delete js_sc_trans;
	      break;
	    case _3_API:
	      js_api_3->return_schedule( js_Plan );
	      delete js_api_3;
	      break;
	    case _3_CR:
	      js_cr_3->return_schedule( js_Plan );
	      delete js_cr_3;
	      break;      
	    case CR_API:
	      js_cr_api->return_schedule( js_Plan );
	      delete js_cr_api;
	      break;
	    case SC_API:
	      js_sc_api->return_schedule( js_Plan );
	      delete js_sc_api;
	      break;
	    case BL_API:
	      js_bl_api->return_schedule( js_Plan );
	      delete js_bl_api;
	      break;
	    case CR_SHIFT:
	      js_cr_shift->return_schedule( js_Plan );
	      delete js_cr_shift;
	      break;
	    case CR_SHIFT_MIX:
	      js_cr_shift_mix->return_schedule( js_Plan );
	      delete js_cr_shift_mix;
	      break;
	    case BL_SHIFT:
	      js_bl_shift->return_schedule( js_Plan );
	      delete js_bl_shift;
	      break;
	    }

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

//**************************************************************************

int main(int argc, char *argv[])
 {
   G_ExceptionList.set_output_to_cout();

   // now we read the file:

   // print a message that the programm started:
   cout << "This is the LiSA-Neighbourhood-Search Module Version 05.01.1999" << endl;
  if (argc != 3) 
    {
      cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
      exit(7);
    }
 
 cout << "PID= " << getpid() << endl;  
 
 ifstream i_strm(argv[1]);
 ofstream o_strm(argv[2]);
 if (!i_strm)
 {
   cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
   exit(1);
 }
 if (!o_strm)
 {
   cout << "ERROR: cannot find output file " << argv[1] << "." << endl;
   exit(1);
 }
 i_strm.close();
 o_strm.close();
 
 LisaXmlFile::initialize();
 LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
 
 Lisa_ProblemType Problem;
 Lisa_ControlParameters Parameter;   
 Lisa_Values Values;
 Lisa_List<Lisa_ScheduleNode> Starters;
 Lisa_List<Lisa_ScheduleNode> Results;
 
 xmlInput.read(argv[1]);
 LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
 
 if (!xmlInput || type != LisaXmlFile::SOLUTION)
 {
   cout << "ERROR: cannot read input , aborting program." << endl;
   exit(1);
 }
 if( !(xmlInput >> Problem))
 {
   cout << "ERROR: cannot read ProblemType , aborting program." << endl;
   exit(1);
 }
 if( !(xmlInput >> Parameter))
 {
   cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
   exit(1);
 }
 if( !(xmlInput >> Values))
 {
   cout << "ERROR: cannot read Values , aborting program." << endl;
   exit(1);
 }
 if( !(xmlInput >> Starters))
 {
   cout << "ERROR: cannot read starting schedule , aborting program." << endl;
   exit(1);
 }
 if (!G_ExceptionList.empty())
 {
   cout << "ERROR: cannot read input , aborting program." << endl;
   exit(1);
 }
			
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


  OBJ_TYPE = Problem.get_property(OBJECTIVE);
  PROB_TYPE = Problem.get_property(M_ENV);

  if (!(Parameter.defined("NGBH")))
    { 
      G_ExceptionList.lthrow("you must define a neighbourhood in the input file");
      exit(7);
    }
  NGBH_St = Parameter.get_string( "NGBH" );
  if (!(Parameter.defined("METHOD")))
    { 
      G_ExceptionList.lthrow("you must define a method in the input file");
      exit(7);
    }
  METHOD_St = Parameter.get_string( "METHOD" );
  if ( Parameter.defined("PROB") )
    PROB = Parameter.get_long( "PROB" );
  if ( PROB < 0 )
    {
      G_ExceptionList.lthrow("PROB must be nonnegative");
      exit(7);
    }
  if ( Parameter.defined("MAX_STUCK") )
    MAX_STUCK = Parameter.get_long( "MAX_STUCK" );
  if ( MAX_STUCK < 1 )
    {
      G_ExceptionList.lthrow("MAX_STUCK must be positive");
      exit(7);
    } 
  if ( Parameter.defined("TABULENGTH") )
    TABULENGTH = Parameter.get_long( "TABULENGTH" );
  if ( TABULENGTH < 1 )
    {
      G_ExceptionList.lthrow("TABULENGTH must be positive");
      exit(7);
    }
  if ( Parameter.defined("NUMB_NGHB") )
    NUMB_NGHB = Parameter.get_long( "NUMB_NGHB" );
  if ( NUMB_NGHB < 1 )
    {
      G_ExceptionList.lthrow("NUMB_NGHB must be positive");
      exit(7);
    }
  if ( Parameter.defined("TYPE") )
    TYPE_St = Parameter.get_string( "TYPE" );
  //  if ( Parameter.defined("OBJ_TYPE") )
  //  OBJ_TYPE_St = Parameter.get_string( "OBJ_TYPE" );
  if ( Parameter.defined("STEPS") )
    STEPS = Parameter.get_long( "STEPS" );
  if ( STEPS < 1 )
    {
      G_ExceptionList.lthrow("STEPS must be positive");
      exit(7);
    }
  if ( Parameter.defined("NUMB_PROBLEMS") )
    { 
      NUMB_PROBLEMS = Parameter.get_long( "NUMB_PROBLEMS" );
    }
  if ( NUMB_PROBLEMS < 1 )
    {
      G_ExceptionList.lthrow("NUMB_PROBLEMS must be positive");
      exit(7);
    }
    if ( Parameter.defined("NUMB_PLANS") )
    { 
      NUMB_PLANS = Parameter.get_long( "NUMB_PLANS" );
    }
  if ( NUMB_PLANS < 1 )
    {
      G_ExceptionList.lthrow("NUMB_PLANS must be positive");
      exit(7);
    }
  if ( Parameter.defined("NUMB_STUCKS") )
    NUMB_STUCKS = Parameter.get_long( "NUMB_STUCKS" );
  if ( NUMB_STUCKS < 1 )
    {
      G_ExceptionList.lthrow("NUMB_STUCKS must be positive");
      exit(7);
    }
  if ( Parameter.defined("ABORT_BOUND") )
    ABORT_BOUND = Parameter.get_double( "ABORT_BOUND" );

  //set flags for algorithms
  cout << "Neighborhood : \"" << NGBH_St << "\"" << endl;
    
       if ( NGBH_St     == "API"              ) NGBH = API;
  //else if ( NGBH_St     == "SWAP"             ) NGBH = SWAP;
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
  else{
    G_ExceptionList.lthrow("Neighbourhood "+NGBH_St+" unknown.");
    exit(7);
  }
  
  if      ( METHOD_St   == "IterativeImprovement"       ) METHOD   = II;
  else if ( METHOD_St   == "SimulatedAnnealing"       ) METHOD   = SA;
  else if ( METHOD_St   == "SA_anti"  ) METHOD   = SA_anti;
  else if ( METHOD_St   == "ThresholdAccepting"       ) METHOD   = TA;
  else if ( METHOD_St   == "TabuSearch"       ) METHOD   = TS;
  else{
    G_ExceptionList.lthrow("Method "+METHOD_St+" unknown.");
    exit(7);
  }
  
  if      ( TYPE_St     == "ENUM"     ) TYPE     = ENUM;
  else if ( TYPE_St     == "RAND"     ) TYPE     = RAND;
  else{
    G_ExceptionList.lthrow("TYPE "+TYPE_St+" unknown.");
    exit(7);
  }


  if ( METHOD == II )
    cout<<"parameters: "<< STEPS <<" STEPS ";
  if ( (METHOD==SA) || (METHOD==TA) || (METHOD==SA_anti) )
    cout<<"parameters: "<< STEPS <<" STEPS "<< PROB <<" PROB "<< MAX_STUCK <<" MAX_STUCK";
  if ( METHOD == TS )
    cout<<"parameters: "<<STEPS<<" STEPS "<<TABULENGTH<<" TABULENGTH "<<NUMB_NGHB<<" NUMB_NGHB";

  // ####  SINGLE-MACHINE   ####
  if ( PROB_TYPE == ONE )
    one_mach_iter(Values, Starters, Results);
  // ####  END OF SINGLE-MACHINE  ####

  // ####  OPEN-SHOP  ####
  if ( PROB_TYPE == O )
    osp_iter(Values, Starters, Results);
  // ####  END OF OPEN-SHOP  ####

  // ####  JOB-SHOP ####
  if ( (PROB_TYPE==J) || (PROB_TYPE==F) )
    jsp_iter(Values, Starters, Results);
  // ####  END OF JOB-SHOP  ####

  
  //output solution ....
  
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  xmlOutput << Problem << Values << Parameter << Results;
  xmlOutput.write(argv[2]);

  return OK;
 }

//**************************************************************************

