/*
 * ******************** jsp_crpi.hpp ******************************
 * 
 * description:      job shop critical API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _shp_cr_api_h
#define _shp_cr_api_h

#include "../neighbor.hpp"
#include "../../../lisa_dt/schedule/js_sched.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop critical-API Neighborhood.
    This is a problem dependent API neighborhood class for the 
    job shop problem where we take a swap only for critical operations.\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre1
    @see Lisa_Neighborhood
    @see JSHOP_API_Ngbh
*/  
class JSHOP_cr_API_Ngbh: public JSHOP_API_Ngbh
     {
                int   *cr_list_j;
		int   *cr_list_m; 
		int   count;     
	public:	
		/** construct OSHOP_API_Ngbh with a start schedule
	            and specified problem datas   */
		JSHOP_cr_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
		~JSHOP_cr_API_Ngbh();
		/** propose a possible move,
		    parameter is only RAND for random
		    generation of neighbor;\\
		    it propose an interchange of two adjacent operations (in 
		    the job-order of a machine) on the Cmax-critical way;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif




