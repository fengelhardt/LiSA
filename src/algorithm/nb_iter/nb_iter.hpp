#ifndef NB_ITER_HPP
#define NB_ITER_HPP

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

struct NB_Iteration {

  static const int MAXINT  = 214748000;
  static const int MAXLONG = 214748000;

  // definition of parameters for the file-controlling
  // following parameters are possible:
  int    NGBH, /* NGBH = M1_API, M1_SHIFT, OSP_API, OSP_SHIFT,
		                     OSP_3_API, OSP_CR_API, OSP_BL_API, OSP_CR_SHIFT, 
			                   OSP_BL_SHIFT, OSP_CR_TST, OSP_3_CR, JSP_API, JSP_SWAP,
			                   JSP_SHIFT, JSP_TRANS, JSP_CR_TRANS, JSP_CR_TRANS_MIX,                          
                         JSP_SC_TRANS, JSP_3_API, JSP_CR_API, JSP_SC_API, 
                         JSP_BL_API, JSP_CR_SHIFT, JSP_CR_SHIFT_MIX, 
			                   JSP_BL_SHIFT, JSP_3_CR */
         METHOD,        // METHOD = II, SA, TA, TS, SA_anti
         PROB,          // PROB = int-Value in [ 0, 100 ]
         MAX_STUCK,     // MAX_STUCK = int-Value
         TABULENGTH,    // TABULENGTH = int-Value
         NUMB_NGHB,     // NUMB_NGHB = int-Value
         NUMB_PROBLEMS, // NUMB_PROBLEMS = int-Value
         NUMB_PLANS,    // NUMB_PROBLEMS = int-Value
         TYPE,          // TYPER = ENUM, RAND
         OBJ_TYPE,      /* OBJ_TYPE = CMAX, LMAX, SUM_CI, SUM_WICI, SUM_UI,
                                      SUM_WIUI, SUM_TI, SUM_WITI */
	       NUMB_STUCKS,  // NUMB_STUCKS = int value
	       PROB_TYPE, ii, jj;
         
  long   STEPS;      // STEPS = long-Value
  
  std::string NGBH_St,   //  M1_API, M1_SHIFT, OSP_API, OSP_SHIFT, OSP_3_API, 
                         //  OSP_CR_API, OSP_BL_API, OSP_CR_SHIFT, OSP_BL_SHIFT, 
		                     //	 OSP_CR_TST, OSP_3_CR, JSP_API, JSP_SHIFT, JSP_3_API,
		                     //  JSP_CR_API, JSP_BL_API, JSP_CR_SHIFT, JSP_BL_SHIFT,
		                     //  JSP_3_CR
         METHOD_St,
         TYPE_St,        //  ENUM, RAND
         OBJ_TYPE_St;    //  CMAX, LMAX, SUM_CI, SUM_WICI, SUM_UI,
		                     //  SUM_WIUI, SUM_TI, SUM_WITI
  TIMETYP ABORT_BOUND;   // ABORT_BOUND = TIMETYP value

  static const int API          = 11;
  static const int SHIFT        = 12;
  static const int _3_API       = 13;
  static const int CR_API       = 14;
  static const int BL_API       = 15;
  static const int CR_SHIFT     = 16;
  static const int BL_SHIFT     = 17;
  static const int CR_TST       = 18;
  static const int _3_CR        = 19;

  static const int TRANS        = 21;
  static const int CR_TRANS     = 22;
  static const int SC_TRANS     = 23;
  static const int SC_API       = 24;
  static const int CR_SHIFT_MIX = 25;
  static const int CR_TRANS_MIX = 26;
  static const int PI           = 27;

  static const int SingleMachine = 1;
  static const int OSP           = 2;
  static const int JSP           = 3;

  int art_of_problem;
  int n;  // number of jobs
  int i, j, k;
  int count, succ;

  Lisa_1Problem            *m1_Prob;
  Lisa_1Schedule           *m1_Plan;
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

  //methods

  bool configure(Lisa_ProblemType& Problem,
                 Lisa_ControlParameters& Parameter,
		             Lisa_Values& Values);

  int one_mach_iter(Lisa_Values& Values,Lisa_List<Lisa_ScheduleNode>& Starters,Lisa_List<Lisa_ScheduleNode>& Results);
  int osp_iter(Lisa_Values& Values,Lisa_List<Lisa_ScheduleNode>& Starters,Lisa_List<Lisa_ScheduleNode>& Results);
  int jsp_iter(Lisa_Values& Values,Lisa_List<Lisa_ScheduleNode>& Starters,Lisa_List<Lisa_ScheduleNode>& Results);

  int nb_iter(Lisa_Values& Values,Lisa_List<Lisa_ScheduleNode>& Starters,Lisa_List<Lisa_ScheduleNode>& Results){

    if ( PROB_TYPE == ONE ) return one_mach_iter(Values, Starters, Results);
    if ( PROB_TYPE == O ) return osp_iter(Values, Starters, Results);
    if ( (PROB_TYPE==J) || (PROB_TYPE==F) ) return jsp_iter(Values, Starters, Results);

    return -1;
  }

  void inline abort(){ if(it) it->abort(); }  
};

#endif

