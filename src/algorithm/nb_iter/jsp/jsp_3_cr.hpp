
#ifndef _jshp_3_cr_h
#define _jshp_3_cr_h

#include "../../../basics/order.hpp"
#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop 3-time-critical-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    open shop problem, where we swap two adjacent operations on the critical 
    way and additional we swap also the direct predecence and the direct 
    sucsessive operations on the critical way.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_3_CR_Ngbh: public JSHOP_API_Ngbh{       
private: 
  /// private data, needs documentation
  Lisa_Order *ROrd;
  /// private data, needs documentation
  int   OrdCount;
  /// private data, needs documentation
	int   NewOrder;
  /// for the 3 proposed swaps
  /** for JO-swap:
	    - swaps[][0]=JO
			- swaps[][1] machine1
			- swaps[][2] job1
			- swaps[][3] job2
			- swaps[][4] OK ? */
  int   swaps[3][5];
  /// head and tail for a operation
	TIMETYP head, tail;
	/// machines
  int   machine2, machine3;
  /// job
	int   job3;
	/// basic problem data
  Lisa_JsSchedule  *temp_schedule;
public:	
	/// construct JSHOP_3_CR_Ngbh 
  /** with a start schedule and specified problem data */
	JSHOP_3_CR_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_3_CR_Ngbh();

	int   prepare_move(int);
	int   do_move();
	int   use_tabulist();
	int   set_tabulist();
	void clean_tabu_param();
};

#endif




