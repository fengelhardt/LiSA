/*
 * ******************** osp_blsh.hpp ******************************
 * 
 * description:      open shop critical-block-SHIFT neighborhood
 * 
 * @author            Andreas Winkler
 *
 * date:             11.01.1999
 *
 */

#ifndef _oshp_bl_shift_h
#define _oshp_bl_shift_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/** Open-Shop critical_block-SHIFT Neighborhood.
    This is the problem dependent SHIFT neighborhood class for the 
    open shop problem, where we take a swap only for a critical block 
    operation.\\
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_bl_shift_Ngbh: public OSHOP_API_Ngbh
     {
                // set of critical operations
		Lisa_Vector<int> *cr_list_j, *cr_list_m, *direction; 
		int   count;       // number of critical operations
		int   pos1, pos2;
		Lisa_Vector<int> *JOrd, *MOrd;
	public:	
		/** construct OSHOP_cr_bl_shift_Ngbh with a start schedule
		    and specified problem datas   */
		OSHOP_cr_bl_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		~OSHOP_cr_bl_shift_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbour;\\
		    it propose an exchange of a block-end-operation on the
		    Cmax-critical way (in a job-order or a machine-order) 
		    with an other operation on the same machine or job;\\ 
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif




