/*
 * ******************** jsp_blpi.hpp ******************************
 * 
 * description:      job shop critical-block-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             5.01.1999
 *
 */

#ifndef _jshp_bl_api_h
#define _jshp_bl_api_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop critical-block-API Neighborhood.
    This is the problem dependent API neighborhood class for the 
    open shop problem, where we take a swap only for a critical block 
    operation.\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_bl_API_Ngbh: public JSHOP_API_Ngbh
     {
                int   *cr_list_j;
		int   *cr_list_m;  
		int   *direction;  // set of critical operations
		int   count;       // number of critical operations
	public:	
		/** construct JSHOP_cr_bl_API_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_cr_bl_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
		~JSHOP_cr_bl_API_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbour;\\
		    it propose an exchange of a block-end-operation on the
		    Cmax-critical way with a block-internal-operation (in the 
		    job-order of a machine);\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif



