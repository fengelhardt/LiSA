/*
 * ******************** osp_3api.hpp ******************************
 * 
 * description:      open shop 3-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef _oshp_3_api_h
#define _oshp_3_api_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/** Open-Shop 3-time-API Neighborhood.
    This is a problem dependent API neighborhood class for the 
    open shop problem where we take a swap of adjacent operations 
    on 3 different machines or jobs.\\
    It works only with tabusearch method !\\
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_3_API_Ngbh: public OSHOP_API_Ngbh
     {
                int   swaps[3][5];
	public:	
		/** construct OSHOP_3_API_Ngbh with a start schedule
		 and specified problem datas   */
		OSHOP_3_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbor;\\
		    it propose three interchanges of two adjacent operations 
		    in the job-orders of three machines or in the machine-
		    orders of three jobs;\\
	            it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move\\it returns OK or !OK
		int   do_move();
		/// search in the tabulist\\it returns OK or !OK
		int   use_tabulist();
		/// set a tabulist-entry
		int   set_tabulist();
     };

#endif




