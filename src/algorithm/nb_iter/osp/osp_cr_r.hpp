
#ifndef _oshp_cr_tst_h
#define _oshp_cr_tst_h

#include "../../../basics/order.hpp"
#include "../../../scheduling/os_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/// Open-Shop critical-API Neighbourhood (with a fix order of the operations). 
/** This is the problem dependent API neighbourhood class for the 
    open shop problem where we take a swap only for critical operations.
    This class works like OSHOP_cr_API_Ngbh, but here we create a fixed
    order of the critical operations.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
    @see OSHOP_cr_API_Ngbh
*/
class OSHOP_cr_TST_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// private data, needs documentation
    Lisa_Order *ROrd;
    /// private data, needs documentation
		int   OrdCount;
    /// private data, needs documentation
		int   NewOrder;
	public:	
		/// construct OSHOP_cr_TST_Ngbh
    /** with a start schedule and specified problem data   */
		OSHOP_cr_TST_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_cr_TST_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

#endif


