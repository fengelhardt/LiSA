
#ifndef _oshp_cr_api_h
#define _oshp_cr_api_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/// open shop critical-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    open shop problem, where we take a swap only for a critical operation.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_API_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// set of critical operations
    int   *cr_list_j;
    /// set of critical operations
		int   *cr_list_m; 
		/// number of critical operations
    int   count;     
	public:	
		/// construct OSHOP_API_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_cr_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		/// destructor
    ~OSHOP_cr_API_Ngbh();

		int   prepare_move(int);

		int   do_move();
};

#endif




