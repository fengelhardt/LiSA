/*
 * ******************** osp_crsh.hpp ******************************
 * 
 * description:      open shop critical SHIFT neighborhood
 * 
 * @author            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef _oshp_cr_shift_h
#define _oshp_cr_shift_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/** Open-Shop critical-SHIFT Neighborhood.
    This is a problem dependent SHIFT neighborhood class for the 
    open shop problem where we take a swap only for critical operations.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_shift_Ngbh: public OSHOP_API_Ngbh
     {
               	// set of critical operations
		Lisa_Vector<int> *cr_list_j, *cr_list_m;
		Lisa_Vector<int> *JOrd, *MOrd;
		int   count;      // number of critical operations
	public:	
		/** construct OSHOP_API_Ngbh with a start schedule
		    and specified problem datas   */
		OSHOP_cr_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		~OSHOP_cr_shift_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbor;
		    it propose an interchange of two not necessary adjacent 
		    operations (in the job-order or in the machine-order) on 
		    the Cmax-critical way;
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move it returns OK or !OK
		int   do_move();
     };

#endif




