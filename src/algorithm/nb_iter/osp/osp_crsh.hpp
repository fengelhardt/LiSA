
#ifndef _oshp_cr_shift_h
#define _oshp_cr_shift_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/// open shop critical-SHIFT neighbourhood
/** This is a problem dependent SHIFT neighbourhood class for the 
    open shop problem where we take a swap only for critical operations.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_shift_Ngbh: public OSHOP_API_Ngbh{
  
    /// set of critical operations
		Lisa_Vector<int> *cr_list_j, *cr_list_m;
		/// machine order and job order
    Lisa_Vector<int> *JOrd, *MOrd;
    /// number of critical operations
		int   count;
	public:	
		/// construct OSHOP_API_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_cr_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_cr_shift_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

#endif

