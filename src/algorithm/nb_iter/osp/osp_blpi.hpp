
#ifndef _oshp_bl_api_h
#define _oshp_bl_api_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/// open shop critical-block-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    open shop problem, where we take a swap only for a critical block  
    end operation.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_bl_API_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// set of critical operations
    int   *cr_list_j;
    /// set of critical operations
		int   *cr_list_m;  
		/// private data, needs documentation
    int   *direction; 
    /// number of critical operations
    int   count;     
	public:	
		/// construct OSHOP_cr_bl_API_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_cr_bl_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_cr_bl_API_Ngbh();

		int   prepare_move(int);

		int   do_move();
};

#endif




