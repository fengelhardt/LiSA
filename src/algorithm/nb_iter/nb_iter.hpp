/*
 * ******************** nb_iter.h ******************************
 * 
 * description:      convention for the main program for iteration
 * 
 * @author            Andreas Winkler
 *
 * date:             5.01.1999
 *
 */

/** @name Main Program for the Neighborhood Search 
    #nb_iter.cpp# is the main program for the iterative neighborhood search.
    Actually are implemented Single-Machine-Schedules, Open-Shop-Schedules 
    and Job-Shop-Shedules with several Neighborhoods. */
//@{
//@Include: doc_iter.hpp
//@}
/*  This program is the main external program for the neighborhood iteration.
    To call this program use: #nb_iter [input file] [output file]# 
    
    @author Andreas Winkler
    @version 2.3pre3
*/  

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

const int MAXINT  = 214748000;
const int MAXLONG = 214748000;

// definition of parameters for the file-controlling
// following parameters are possible:
  int    NGBH,       /* NGBH = M1_API, M1_SHIFT, OSP_API, OSP_SHIFT,
		         OSP_3_API, OSP_CR_API, OSP_BL_API, OSP_CR_SHIFT, 
			 OSP_BL_SHIFT, OSP_CR_TST, OSP_3_CR, JSP_API, JSP_SWAP,
			 JSP_SHIFT, JSP_TRANS, JSP_CR_TRANS, JSP_CR_TRANS_MIX,                          JSP_SC_TRANS, JSP_3_API, JSP_CR_API, JSP_SC_API, 
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
  Lisa_1Schedule 	   *m1_Plan;
  Lisa_OsProblem           *os_Prob;
  Lisa_OsSchedule          *os_Plan;
  Lisa_JsProblem           *js_Prob;
  Lisa_JsSchedule          *js_Plan;
  Lisa_Neighborhood        *ngbh;
  API_Neighborhood         *m1_api;
  shift_Neighborhood       *m1_shift;
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

