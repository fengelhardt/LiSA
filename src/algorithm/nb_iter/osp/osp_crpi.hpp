/*
 * ******************** osp_crpi.hpp ******************************
 * 
 * description:      open shop critical-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _oshp_cr_api_h
#define _oshp_cr_api_h

#include "../neighbor.hpp"
#include "../../../scheduling/os_sched.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/** Open-Shop critical-API Neighborhood.
    This is a problem dependent API neighborhood class for the 
    open shop problem, where we take a swap only for a critical operation.\\
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre1
    @see Lisa_Neighborhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_API_Ngbh: public OSHOP_API_Ngbh
     {
                int   *cr_list_j;
		int   *cr_list_m;  // set of critical operations
		int   count;       // number of critical operations
	public:	
		/** construct OSHOP_API_Ngbh with a start schedule
		    and specified problem datas   */
		OSHOP_cr_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		~OSHOP_cr_API_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbour;\\
		    it propose an interchange of two adjacent operations (in 
		    the job-order or in the machine-order) on the Cmax-
	 	    critical way;\\
 		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move\\ it returns OK or !OK
		int   do_move();
     };

#endif




