/**
 * @author  Andreas Winkler
 * @version 2.3final
 */
 
#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <time.h>

#include "../../basics/matrix.hpp"
#include "../../scheduling/m1_sched.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../scheduling/js_sched.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../misc/except.hpp"

#include "one_mach/m1_api.hpp"
#include "one_mach/m1_shft.hpp"

#include "osp/osp_api.hpp"
#include "osp/osp_3api.hpp"
#include "osp/osp_3_cr.hpp"
#include "osp/osp_crpi.hpp"
#include "osp/osp_blpi.hpp"
#include "osp/osp_blsh.hpp"
#include "osp/osp_shft.hpp"
#include "osp/osp_crsh.hpp"
#include "osp/osp_cr_r.hpp"

#include "jsp/jsp_api.hpp"
#include "jsp/jsp_swap.hpp"
#include "jsp/jsp_3api.hpp"
#include "jsp/jsp_crpi.hpp"
#include "jsp/jsp_s_pi.hpp"
#include "jsp/jsp_blpi.hpp"
#include "jsp/jsp_shft.hpp"
#include "jsp/jsp_trns.hpp"
#include "jsp/jsp_crtr.hpp"
#include "jsp/jsp_crtm.hpp"
#include "jsp/jsp_s_tr.hpp"
#include "jsp/jsp_crsh.hpp"
#include "jsp/jsp_crsm.hpp"
#include "jsp/jsp_blsh.hpp"
#include "jsp/jsp_3_cr.hpp"

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
  const int SWAP         = 20;
  const int TRANS        = 21;
  const int CR_TRANS     = 22;
  const int SC_TRANS     = 23;
  const int SC_API       = 24;
  const int CR_SHIFT_MIX = 25;
  const int CR_TRANS_MIX = 26;
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
  Lisa_Neighbourhood        *ngbh;
  API_Neighbourhood         *m1_api;
  shift_Neighbourhood       *m1_shift;
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
  JSHOP_swap_Ngbh          *js_swap;
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

/* at first three procedures for the iteration of 
     - one machine problems
     - open shop problems
     - job shop problems
   then the main procedure   */
int one_mach_iter( ifstream& strm, ofstream& fplan_o )
{
  for ( ii=0; ii<NUMB_PROBLEMS; ii++ )
    {
	
      // reading problem data
      Lisa_Values *problem_in;
      if ( !( problem_in = new Lisa_Values() ) )
	{  
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	} 
      strm >> (*problem_in);
      fplan_o << (*problem_in);
      if ( !( m1_Prob = new Lisa_1Problem( problem_in ) ) )
	{
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}

      // here could standing precedence-constraints
      delete problem_in;
	
      for ( jj=0; jj<NUMB_PLANS; jj++ )
	{
	  // start-schedule-datas
	  Lisa_Schedule *plan_in;
	  if ( !( plan_in = new Lisa_Schedule() ) )
	    {
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	    
	  strm >> (*plan_in);
	  if ( m1_Prob->n != plan_in->get_n() )
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
	    (*LRV)[i] = (*plan_in->LR)[i][0];
	  //for ( i=0; i<m1_Prob->n; i++ )
	    LROrder->read( LRV );
	  LROrder->sort();

	  JOPred = 0;
	  for ( i=0; i<m1_Prob->n; i++ )
	    if ( (*plan_in->LR)[(*LROrder)[i]][0] != 0 )
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
	  delete plan_in;
	  
	  switch ( NGBH )
	    {
	    case API:
	      if (!(ngbh = m1_api = new API_Neighbourhood(m1_Plan,m1_Prob)))
		{
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case SHIFT:
	      if(!(ngbh=m1_shift=new shift_Neighbourhood(m1_Plan,m1_Prob)))
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
	    case TS: if (NUMB_NGHB < m1_Prob->n)
	               G_ExceptionList.lthrow("Number of neighbours should be greater",Lisa_ExceptionList::WARNING);
	             it->init( TS, TABULENGTH, NUMB_NGHB, TYPE );break;
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
	  
	  switch ( NGBH )
	    {
	    case API:
	      m1_api->return_schedule( m1_Plan );
	      delete m1_api;
	      break;
	    case SHIFT:
	      m1_shift->return_schedule( m1_Plan );
	      delete m1_shift;
	      break;
	    }
	    
	    
	  // return the schedule in a file
	  m1_Plan->SetValue( OBJ_TYPE );
	  cout << "\nbest objective_value: " << m1_Plan->GetValue() << "\n";
	  
	  // transfering the datas:
	  if ( !( plan_in = new Lisa_Schedule( m1_Prob->n, 1 ) ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  plan_in->make_LR();
	  plan_in->make_CIJ();
	  for ( i=1; i<m1_Prob->n+1; i++ )
	    {
	      (*plan_in->LR)[(*m1_Plan->sequ)[i]-1][0] = i;
	      (*plan_in->CIJ)[i-1][0] = (*m1_Plan->Ci)[i];
	    }
	  fplan_o << (*plan_in);

	  fplan_o << "\n<OBJECTIVE>\n";
	  fplan_o << "OBJECTIVE= " << m1_Plan->GetValue() << "\n";
	  fplan_o << "</OBJECTIVE>\n";

	  cout << "Sq: " << *m1_Plan->sequ << "\n";

	  delete plan_in;
	  
	  delete m1_Plan;
	} // NUMB_PLANS
      delete m1_Prob;
    } // NUMB_PROBLEMS
  return OK;
} // End of one_mach_iter()

//**************************************************************************

int osp_iter( ifstream& strm, ofstream& fplan_o )
{
  for ( ii=0; ii<NUMB_PROBLEMS; ii++ )
    {
      // problemdatas
      Lisa_Values *problem_in;
      if ( !( problem_in = new Lisa_Values() ) )
	{
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	} 
      strm >> (*problem_in);
      fplan_o << (*problem_in);
      if ( !( os_Prob = new Lisa_OsProblem(problem_in) ) )
	{
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}
      delete problem_in;

      for ( jj=0; jj<NUMB_PLANS; jj++ )
	{
	  if ( !( os_Plan = new Lisa_OsSchedule( os_Prob ) ) )
	    {
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  os_Plan->ComputeHeadsTails( 1, 1 );
	  
	  // start-schedule-datas
	  Lisa_Schedule *plan_in;
	  if ( !( plan_in = new Lisa_Schedule() ) )
	    {
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  strm >> (*plan_in);
	  if ( os_Prob->n != plan_in->get_n() )
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
	  LROrder->read( plan_in->LR );
	  LROrder->sort();
	  
	  for ( i=1; i<os_Prob->n+1; i++ )
	    MOPred[i] = 0;
	  for ( i=1; i<os_Prob->m+1; i++ )
	    JOPred[i] = 0;
	  for ( i=0; i<os_Prob->n*os_Prob->m; i++ )
	    if ( (*plan_in->LR)[LROrder->row(i)][LROrder->col(i)] != 0 )
	      {	    
		os_Plan->insert(LROrder->row(i)+1,LROrder->col(i)+1,
				JOPred[LROrder->col(i)+1],
				MOPred[LROrder->row(i)+1]); 
		JOPred[LROrder->col(i)+1] = LROrder->row(i)+1;
		MOPred[LROrder->row(i)+1] = LROrder->col(i)+1;
	      }
	  delete LROrder;

	  // End of the schedule-construction

	  delete plan_in;

	  switch ( NGBH )
	    {
	    case API:   
	      if(!(ngbh = os_api = new OSHOP_API_Ngbh( os_Plan, os_Prob )))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case SHIFT: 
	      if(!(ngbh = os_shift = new OSHOP_shift_Ngbh(os_Plan,os_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case _3_API: /// broken -marc-
	   /*   if(!(ngbh = os_api_3 = new OSHOP_3_API_Ngbh(os_Plan,os_Prob)))
	      	{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
	        } */ 
	      if(!(ngbh = os_cr_3 = new OSHOP_3_CR_Ngbh(os_Plan,os_Prob)))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		} 
	      break;
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
	    case TS: if (NUMB_NGHB < os_Prob->n)
	               G_ExceptionList.lthrow("Number of neighbours should be greater",Lisa_ExceptionList::WARNING);
                     it->init( TS, TABULENGTH, NUMB_NGHB, TYPE );break;
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
	    
	  if ( !( plan_in = new Lisa_Schedule( os_Prob->n, os_Prob->m ) ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  
	  //os_Plan->print();

	  plan_in->make_LR();
	  os_Plan->write_LR( plan_in->LR );
	     
	  // write the datas
	  fplan_o << (*plan_in);

	  fplan_o << "\n<OBJECTIVE>\n";
	  fplan_o << "OBJECTIVE= " << os_Plan->GetValue() << "\n";
	  fplan_o << "</OBJECTIVE>\n";

	  delete plan_in;
	  
	  delete os_Plan;
      delete[]  MOPred;
      delete[] JOPred;
	} // NUMB_PLANS
      delete os_Prob;
    } // NUMB_PROBLEMS
  return OK;
} // End of osp_iter()

//**************************************************************************

int jsp_iter( ifstream& strm, ofstream& fplan_o )
{
  for ( ii=0; ii<NUMB_PROBLEMS; ii++ )
    {
      // problemdatas
      Lisa_Values *problem_in;
      if ( !( problem_in = new Lisa_Values() ) )
	{  
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}
      strm >> (*problem_in);
      if ( !(problem_in->MO))
	{
	  G_ExceptionList.lthrow("you must define a MO for a job-shop-problem !");
	  exit( 7 );
	}

      fplan_o << (*problem_in);
      if ( !( js_Prob = new Lisa_JsProblem(problem_in) ) )
	{  
	  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	  exit( 7 );
	}

      delete problem_in;

      for ( jj=0; jj<NUMB_PLANS; jj++ )
	{
	  if ( !( js_Plan = new Lisa_JsSchedule( js_Prob ) ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  js_Plan->ComputeHeadsTails( 1, 1 );
	  
	  // start-schedule-datas
	  Lisa_Schedule *plan_in;
	  if ( !( plan_in = new Lisa_Schedule() ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }

	  strm >> (*plan_in);
	  if ( js_Prob->n != plan_in->get_n() )
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
	  LROrder->read( plan_in->LR );
	  LROrder->sort();
	  
	  for ( i=1; i<js_Prob->m+1; i++ )
	    JOPred[i] = 0;

	  for ( i=1; i<js_Prob->n+1; i++ )
	    MOPred[i] = 0;
	    
	  for ( i=0; i<js_Prob->n*js_Prob->m; i++ )
	    if ( (*plan_in->LR)[LROrder->row(i)][LROrder->col(i)] != 0 )
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
	  
	  delete plan_in;

	  switch ( NGBH )
	    {
	    case API:   
	      if(!(ngbh = js_api = new JSHOP_API_Ngbh( js_Plan, js_Prob )))
		{  
		  G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
		  exit( 7 );
		}
	      break;
	    case SWAP:   
	      if(!(ngbh = js_swap = new JSHOP_swap_Ngbh( js_Plan, js_Prob )))
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
	    case TS: if (NUMB_NGHB < js_Prob->n)
	               G_ExceptionList.lthrow("Number of neighbours should be greater",Lisa_ExceptionList::WARNING);
                     it->init( TS, TABULENGTH, NUMB_NGHB, TYPE );break;
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
	    case SWAP:
	      js_swap->return_schedule( js_Plan );
	      delete js_swap;
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

	  if ( !( plan_in = new Lisa_Schedule( js_Prob->n, js_Prob->m ) ) )
	    {  
	      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
	      exit( 7 );
	    }
	  
	  // constructing LR
	  plan_in->make_LR();
	  js_Plan->write_LR( plan_in->LR );
	     
	  //js_Plan->print();

	  printf("\n");
	   
	  // writing in the file
	  fplan_o << (*plan_in);

	  fplan_o << "\n<OBJECTIVE>\n";
	  fplan_o << "OBJECTIVE= " << js_Plan->GetValue() << "\n";
	  fplan_o << "</OBJECTIVE>\n";

	  delete plan_in;
	    
	  delete js_Plan;
      delete[] JOPred;
	  delete[] MOPred;
	} // NUMB_PLANS
      delete js_Prob;
    } // NUMB_PROBLEMS   
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

  ifstream strm(argv[1]);    // this file contains the problem, schedule and 
                             // some parameters
  ofstream fplan_o(argv[2]); // this file returns the best computed schedule

  if (!strm)
    {
      G_ExceptionList.lthrow("could not open file "+string(argv[1]));
      exit(7);
    }
  if (!fplan_o)
    {
      G_ExceptionList.lthrow("could not open file "+string(argv[2]));
      exit(1);
    }
  cout << "Problem " << argv[1] << "\n";

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

  Lisa_ProblemType *prob_type;
  if ( !( prob_type = new Lisa_ProblemType ) )
    {
      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
      exit( 7 );
    }
  strm.seekg(0);
  strm >> (*prob_type);
  OBJ_TYPE = prob_type->get_property(OBJECTIVE);
  PROB_TYPE = prob_type->get_property(M_ENV);
  //cout << "OBJ_TYPE = " << OBJ_TYPE << "\n";

  Lisa_ControlParameters *my_special;
  if ( !( my_special = new Lisa_ControlParameters ) )
    {
      G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
      exit( 7 );
    }

  // read the special-datas
  strm >> (*my_special);
  if (!(my_special->defined("NGBH")))
    { 
      G_ExceptionList.lthrow("you must define a neighbourhood in the input file");
      exit(7);
    }
  NGBH_St = my_special->get_string( "NGBH" );
  if (!(my_special->defined("METHOD")))
    { 
      G_ExceptionList.lthrow("you must define a method in the input file");
      exit(7);
    }
  METHOD_St = my_special->get_string( "METHOD" );
  if ( my_special->defined("PROB") )
    PROB = my_special->get_long( "PROB" );
  if ( PROB < 0 )
    {
      G_ExceptionList.lthrow("PROB must be nonnegative");
      exit(7);
    }
  if ( my_special->defined("MAX_STUCK") )
    MAX_STUCK = my_special->get_long( "MAX_STUCK" );
  if ( MAX_STUCK < 1 )
    {
      G_ExceptionList.lthrow("MAX_STUCK must be positive");
      exit(7);
    } 
  if ( my_special->defined("TABULENGTH") )
    TABULENGTH = my_special->get_long( "TABULENGTH" );
  if ( TABULENGTH < 1 )
    {
      G_ExceptionList.lthrow("TABULENGTH must be positive");
      exit(7);
    }
  if ( my_special->defined("NUMB_NGHB") )
    NUMB_NGHB = my_special->get_long( "NUMB_NGHB" );
  if ( NUMB_NGHB < 1 )
    {
      G_ExceptionList.lthrow("NUMB_NGHB must be positive");
      exit(7);
    }
  if ( my_special->defined("TYPE") )
    TYPE_St = my_special->get_string( "TYPE" );
  //  if ( my_special->defined("OBJ_TYPE") )
  //  OBJ_TYPE_St = my_special->get_string( "OBJ_TYPE" );
  if ( my_special->defined("STEPS") )
    STEPS = my_special->get_long( "STEPS" );
  if ( STEPS < 1 )
    {
      G_ExceptionList.lthrow("STEPS must be positive");
      exit(7);
    }
  if ( my_special->defined("NUMB_PROBLEMS") )
    { 
      NUMB_PROBLEMS = my_special->get_long( "NUMB_PROBLEMS" );
    }
  if ( my_special->defined("NUMB_PLANS") )
    { 
      NUMB_PLANS = my_special->get_long( "NUMB_PLANS" );
    }
  if ( NUMB_PROBLEMS < 1 )
    {
      G_ExceptionList.lthrow("NUMB_PROBLEMS must be positive");
      exit(7);
    }
  if ( NUMB_PLANS < 1 )
    {
      G_ExceptionList.lthrow("NUMB_PLANS must be positive");
      exit(7);
    }
  if ( my_special->defined("NUMB_STUCKS") )
    NUMB_STUCKS = my_special->get_long( "NUMB_STUCKS" );
  if ( NUMB_STUCKS < 1 )
    {
      G_ExceptionList.lthrow("NUMB_STUCKS must be positive");
      exit(7);
    }
  if ( my_special->defined("ABORT_BOUND") )
    ABORT_BOUND = my_special->get_double( "ABORT_BOUND" );

  // write the special-datas in fplan_o
  fplan_o << (*prob_type);
  fplan_o << (*my_special);
  delete prob_type;
  delete my_special;

  if ( NGBH_St     == "M1_API"           ) NGBH = API;
  if ( NGBH_St     == "M1_SHIFT"         ) NGBH = SHIFT;
  if ( NGBH_St     == "OSP_API"          ) NGBH = API;
  if ( NGBH_St     == "OSP_SHIFT"        ) NGBH = SHIFT;
  if ( NGBH_St     == "OSP_3_API"        ) NGBH = _3_API;
  if ( NGBH_St     == "OSP_3_CR"         ) NGBH = _3_CR;
  if ( NGBH_St     == "OSP_CR_API"       ) NGBH = CR_API;
  if ( NGBH_St     == "OSP_BL_API"       ) NGBH = BL_API;
  if ( NGBH_St     == "OSP_CR_SHIFT"     ) NGBH = CR_SHIFT;
  if ( NGBH_St     == "OSP_BL_SHIFT"     ) NGBH = BL_SHIFT;
  if ( NGBH_St     == "OSP_CR_TST"       ) NGBH = CR_TST;
  if ( NGBH_St     == "JSP_API"          ) NGBH = API;
  if ( NGBH_St     == "JSP_SWAP"         ) NGBH = SWAP;
  if ( NGBH_St     == "JSP_SHIFT"        ) NGBH = SHIFT;
  if ( NGBH_St     == "JSP_TRANS"        ) NGBH = TRANS;
  if ( NGBH_St     == "JSP_CR_TRANS"     ) NGBH = CR_TRANS;
  if ( NGBH_St     == "JSP_CR_TRANS_MIX" ) NGBH = CR_TRANS_MIX;
  if ( NGBH_St     == "JSP_SC_TRANS"     ) NGBH = SC_TRANS;
  if ( NGBH_St     == "JSP_3_API"        ) NGBH = _3_API;
  if ( NGBH_St     == "JSP_3_CR"         ) NGBH = _3_CR;
  if ( NGBH_St     == "JSP_CR_API"       ) NGBH = CR_API;
  if ( NGBH_St     == "JSP_SC_API"       ) NGBH = SC_API;
  if ( NGBH_St     == "JSP_BL_API"       ) NGBH = BL_API;
  if ( NGBH_St     == "JSP_CR_SHIFT"     ) NGBH = CR_SHIFT;
  if ( NGBH_St     == "JSP_CR_SHIFT_MIX" ) NGBH = CR_SHIFT_MIX;
  if ( NGBH_St     == "JSP_BL_SHIFT"     ) NGBH = BL_SHIFT;
  if ( NGBH_St     == "API"              ) NGBH = API;
  if ( NGBH_St     == "SWAP"             ) NGBH = SWAP;
  if ( NGBH_St     == "SHIFT"            ) NGBH = SHIFT;
  if ( NGBH_St     == "CR_TRANS"         ) NGBH = CR_TRANS;
  if ( NGBH_St     == "CR_TRANS_MIX"     ) NGBH = CR_TRANS_MIX;
  if ( NGBH_St     == "SC_TRANS"         ) NGBH = SC_TRANS;
  if ( NGBH_St     == "TRANS"            ) NGBH = TRANS;
  if ( NGBH_St     == "3_API"            ) NGBH = _3_API;
  if ( NGBH_St     == "3_CR"             ) NGBH = _3_CR;
  if ( NGBH_St     == "CR_API"           ) NGBH = CR_API;
  if ( NGBH_St     == "SC_API"           ) NGBH = SC_API;
  if ( NGBH_St     == "BL_API"           ) NGBH = BL_API;
  if ( NGBH_St     == "CR_SHIFT"         ) NGBH = CR_SHIFT;
  if ( NGBH_St     == "CR_SHIFT_MIX"     ) NGBH = CR_SHIFT_MIX;
  if ( NGBH_St     == "BL_SHIFT"         ) NGBH = BL_SHIFT;
  if ( NGBH_St     == "CR_TST"           ) NGBH = CR_TST;
  if ( METHOD_St   == "IterativeImprovement"       ) METHOD   = II;
  if ( METHOD_St   == "SimulatedAnnealing"       ) METHOD   = SA;
  if ( METHOD_St   == "SA_anti"  ) METHOD   = SA_anti;
  if ( METHOD_St   == "ThresholdAccepting"       ) METHOD   = TA;
  if ( METHOD_St   == "TabuSearch"       ) METHOD   = TS;
  if ( TYPE_St     == "ENUM"     ) TYPE     = ENUM;
  if ( TYPE_St     == "RAND"     ) TYPE     = RAND;

  /*
  switch ( NGBH )
    {
    case M1_API:       art_of_problem = SingleMachine; break;
    case M1_SHIFT:     art_of_problem = SingleMachine; break;
    case OSP_API:      art_of_problem = OSP; break;
    case OSP_SHIFT:    art_of_problem = OSP; break;
    case OSP_3_API:    art_of_problem = OSP; break;
    case OSP_3_CR:     art_of_problem = OSP; break;  
    case OSP_CR_API:   art_of_problem = OSP; break;
    case OSP_BL_API:   art_of_problem = OSP; break;  
    case OSP_CR_SHIFT: art_of_problem = OSP; break;
    case OSP_BL_SHIFT: art_of_problem = OSP; break;
    case OSP_CR_TST:   art_of_problem = OSP; break;
    case JSP_API:      art_of_problem = JSP; break;
    case JSP_SHIFT:    art_of_problem = JSP; break;
    case JSP_3_API:    art_of_problem = JSP; break;
    case JSP_3_CR:     art_of_problem = JSP; break;
    case JSP_CR_API:   art_of_problem = JSP; break;
    case JSP_BL_API:   art_of_problem = JSP; break;
    case JSP_CR_SHIFT: art_of_problem = JSP; break;
    case JSP_BL_SHIFT: art_of_problem = JSP; break;
    default: G_ExceptionList.lthrow("unknown Neighbourhood specified in ITERATE");
             exit(7);  
    }
    */
  // end of special-data-input

  if ( METHOD == II )
    cout<<"parameters: "<<STEPS<<" STEPS ";
  if ( (METHOD==SA) || (METHOD==TA) || (METHOD==SA_anti) )
    cout<<"parameters: "<<STEPS<<" STEPS "<<PROB<<" PROB "<<MAX_STUCK<<" MAX_STUCK";
  if ( METHOD == TS )
    cout<<"parameters: "<<STEPS<<" STEPS "<<TABULENGTH<<" TABULENGTH "<<NUMB_NGHB<<" NUMB_NGHB";

  // starting time:
  //clock_t time1, time2;
  time_t time1, time2;
  time1 = time(&time1);

  // ####  SINGLE-MACHINE   ####
  if ( PROB_TYPE == ONE )
    one_mach_iter( strm, fplan_o );
  // ####  END OF SINGLE-MACHINE  ####

  // ####  OPEN-SHOP  ####
  if ( PROB_TYPE == O )
    osp_iter( strm, fplan_o );
  // ####  END OF OPEN-SHOP  ####

  // ####  JOB-SHOP ####
  if ( (PROB_TYPE==J) || (PROB_TYPE==F) )
    jsp_iter( strm, fplan_o );
  // ####  END OF JOB-SHOP  ####

  // end time:
  time2 = time(&time2);

  // run time:

  //time2 = (time2 - time1)/CLK_TCK/10000;
  double elapsed;
  //elapsed = ( double( time2 - time1 ) ) / CLOCKS_PER_SEC;
  elapsed = difftime( time2, time1 );
  fplan_o << "<TIME>\n";
  fplan_o << "TIME= " << elapsed << "\n";
  fplan_o << "</TIME>\n";

  my_special = new Lisa_ControlParameters;
  int i = 1;
  while (!(G_ExceptionList.empty()))
    my_special->add_key("EXCEPTION"+ztos(i++), G_ExceptionList.lcatch());
  fplan_o << (*my_special);
  delete my_special;

  return OK;
 }

//**************************************************************************

