/*
 * ******************** jsp_blsh.hpp ******************************
 * 
 * description:      job shop critical-block-SHIFT neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             11.01.1999
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _jshp_bl_shift_h
#define _jshp_bl_shift_h

#include "../neighbor.hpp"
#include "../../../lisa_dt/schedule/js_sched.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop critical-block-SHIFT Neighborhood.
    This is the problem dependent SHIFT neighborhood class for the 
    job shop problem, where we take a swap only for a critical block 
    operation.\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre1
    @see Lisa_Neighborhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_bl_shift_Ngbh: public JSHOP_API_Ngbh
     {
                int   *cr_list_j;
		int   *cr_list_m;  
		int   *direction;  // set of critical operations
		int   count;       // number of critical operations
		int   pos1, pos2;
                Lisa_Vector<int> *JOrd;
	public:	
		/** construct JSHOP_cr_bl_shift_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_cr_bl_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
		~JSHOP_cr_bl_shift_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbour;\\
		    it propose an exchange of a block-end-operation on the
		    Cmax-critical way (in a job-order of a machine) with an 
		    other operation on the same machine;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif




