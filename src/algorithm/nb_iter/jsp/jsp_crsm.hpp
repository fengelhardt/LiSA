/*
 * ******************** jsp_crsm.hpp ******************************
 * 
 * description:      job shop Critical-Shift-mixed-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             18.10.2000
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _jshp_cr_shift_mix_h
#define _jshp_cr_shift_mix_h

#include "../neighbor.hpp"
#include "../../../scheduling/js_sched.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop critical-shift-mixed-API Neighborhood.
    This is a problem dependent SHIFT neighborhood class for the 
    job shop problem mixed with 25% API. We shift an operation dependently on 
    his position in the critical block structure.\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre2
    @see Lisa_Neighborhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_shift_mix_Ngbh: public JSHOP_API_Ngbh
     {
                Lisa_Vector<int>   *cr_list_j;
		Lisa_Vector<int>   *cr_list_m; // set of critical operations
		int   cr_count;    // number of critical operations
                Lisa_Vector<int>   *JOrd;
	public:	
		/** construct JSHOP_cr_shift_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_cr_shift_mix_Ngbh(Lisa_JsSchedule*, Lisa_JsProblem*);
		~JSHOP_cr_shift_mix_Ngbh();
		/** propose a possible move,
		    parameter is only RAND for random 
		    generation of a neighbour;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif



