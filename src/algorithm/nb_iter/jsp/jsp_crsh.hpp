/*
 * ******************** jsp_crsh.hpp ******************************
 * 
 * description:      job shop Critical-Shift neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             18.10.2000
 *
 */

#ifndef _jshp_cr_shift_h
#define _jshp_cr_shift_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop critical shift Neighborhood.
    This is a problem dependent API neighborhood class for the 
    job shop problem, where we shift an operation dependently of the critical 
    block structure.\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_shift_Ngbh: public JSHOP_API_Ngbh
     {
                Lisa_Vector<int>   *cr_list_j;
		Lisa_Vector<int>   *cr_list_m; // set of critical operations
		int   cr_count;    // number of critical operations
                Lisa_Vector<int>   *JOrd;
	public:	
		/** construct JSHOP_cr_shift_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_cr_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
		~JSHOP_cr_shift_Ngbh();
		/** propose a possible move,
		    parameter is only RAND for random
		    generation of neighbour;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif



