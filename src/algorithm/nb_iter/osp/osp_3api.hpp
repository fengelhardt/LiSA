
#ifndef _oshp_3_api_h
#define _oshp_3_api_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/// Open-Shop 3-time-API Neighbourhood.
/** This is a problem dependent API neighbourhood class for the open shop 
    problem where we take a swap of adjacent operations on 3 different machines
    or jobs.
    
    It works only with tabusearch method !
    
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_3_API_Ngbh: public OSHOP_API_Ngbh{
private:
  /// for the 3 proposed swaps
  /** for JO-swap:
	    - swaps[][0]=JO
			- swaps[][1] machine1
			- swaps[][2] job1
			- swaps[][3] job2
			- swaps[][4] OK ? */
  int swaps[3][5];
public:	
  /// construct OSHOP_3_API_Ngbh
  /** with a start schedule and specified problem datas   */
	OSHOP_3_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
  
	int   prepare_move(int);
	int   do_move();
	int   use_tabulist();
	int   set_tabulist();
};

#endif

