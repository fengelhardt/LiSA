
#ifndef _jsp_3_api_h
#define _jsp_3_api_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop 3-time-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    job shop problem where we take a swap of adjacent operatins on 3 different
    machines.
    It works only with tabusearch method !
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_3_API_Ngbh: public JSHOP_API_Ngbh{
private:
  /// for the 3 proposed swaps
  /** for JO-swap:
	    - swaps[][0]=JO
			- swaps[][1] machine1
			- swaps[][2] job1
			- swaps[][3] job2
			- swaps[][4] OK ? */
  int   swaps[3][5];
public:	
	/// construct JSHOP_3_API_Ngbh 
  /** with a start schedule and specified problem data */
	JSHOP_3_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );

	int   prepare_move(int);
	int   do_move();
	int   use_tabulist();
	int   set_tabulist();
};

#endif

