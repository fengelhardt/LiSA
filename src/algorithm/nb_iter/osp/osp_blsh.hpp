
#ifndef _oshp_bl_shift_h
#define _oshp_bl_shift_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/// Open-Shop critical_block-SHIFT Neighbourhood.
/** This is the problem dependent SHIFT neighbourhood class for the 
    open shop problem, where we take a swap only for a critical block 
    operation.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_bl_shift_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// set of critical operations
		Lisa_Vector<int> *cr_list_j, *cr_list_m, *direction; 
    /// number of critical operations
		int   count;
    /// private data needs documentation
		int   pos1, pos2;
		/// job oder and machine order
    Lisa_Vector<int> *JOrd, *MOrd;
	public:	
		/// construct OSHOP_cr_bl_shift_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_cr_bl_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		/// destructor
    ~OSHOP_cr_bl_shift_Ngbh();

		int   prepare_move(int);

		int   do_move();
};

#endif

