/*
 * ******************** osp_cr_r.hpp ******************************
 * 
 * description:      open shop critical-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef _oshp_cr_tst_h
#define _oshp_cr_tst_h

#include "../../../basics/order.hpp"
#include "../../../scheduling/os_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/** Open-Shop critical-API Neighborhood (with a fix order of the operations). 
    This is the problem dependent API neighborhood class for the 
    open shop problem where we take a swap only for critical operations.\\
    This class works like OSHOP_cr_API_Ngbh, but here we create a fixed
    order of the critical operations.\\
    It is inherited from the class Lisa_Neighborhood.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
    @see OSHOP_API_Ngbh
    @see OSHOP_cr_API_Ngbh
*/
class OSHOP_cr_TST_Ngbh: public OSHOP_API_Ngbh
     {
       private: Lisa_Order *ROrd;
		int   OrdCount;
		int   NewOrder;
	public:	
		/** construct OSHOP_API_Ngbh with a start schedule
		    and specified problem datas   */
		OSHOP_cr_TST_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		~OSHOP_cr_TST_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbor;\\
		    it propose an interchange of two adjacent operations (in 
		    the job-order or in the machine-order) on the Cmax-
	 	    critical way, but the order of the critical operations is 
		    fix, so that we take shure, that all possibilitys are 
		    tested;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif




