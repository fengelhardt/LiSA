/*
 * ******************** osp_shft.hpp ******************************
 * 
 * description:      open shop SHIFT neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _oshp_shft_h
#define _oshp_shft_h

#include "../neighbor.hpp"
#include "../../../lisa_dt/schedule/os_sched.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/** Open-Shop SHIFT Neighborhood.
    This is the problem dependent SHIFT neighborhood class for the 
    open shop problem (i.e. these neighborhood swaps two arbitrary
    opererations on one machine or for one job).\\
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.2a0
    @see Lisa_Neighborhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_shift_Ngbh: public OSHOP_API_Ngbh
     {
                int   pos1, pos2;
		Lisa_Vector<int> *JOrd, *MOrd, *JOpos, *MOpos;
		
	public:	
		/** construct OSHOP_API_Ngbh with a start schedule
		    and specified problem datas   */
		OSHOP_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		~OSHOP_shift_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbor;\\
		    it propose an interchange of two arbitrary not necessary 
   		    adjacent operations in the job-order of one machine or in 
		    the machine-order of one job;\\
	            it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\it returns OK or !OK
		int   do_move();
     };

#endif



