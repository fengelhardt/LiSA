/*
 * ******************** jsp_3_cr.hpp ******************************
 * 
 * description:      job shop 3-critical-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _jshp_3_cr_h
#define _jshp_3_cr_h

#include "../neighbor.hpp"
#include "../../../scheduling/js_sched.hpp"
#include "../tabu.hpp"
#include "../../../basics/order.hpp"
#include "jsp_api.hpp"

/** Job-Shop 3-time-critical-API Neighborhood.
    This is a problem dependent API neighborhood class for the 
    open shop problem, where we swap two adjacent operations on the critical 
    way and additional we swap also the direct predecence and the direct 
    sucsessive operations on the critical way.\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre2
    @see Lisa_Neighborhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_3_CR_Ngbh: public JSHOP_API_Ngbh
     {       
       private: Lisa_Order *ROrd;
		int   OrdCount;
		int   NewOrder;
                int   swaps[3][5]; // that the 3 proposed swaps:
				   // for JO-swap:
	                           // swaps[][0]=JO
				   // swaps[][1] machine1
				   // swaps[][2] job1
				   // swaps[][3] job2
				   // swaps[][4] OK ?
		TIMETYP head, tail;
		int   machine2, machine3;
		int   job3;
	        Lisa_JsSchedule  *temp_schedule;
	public:	
		/** construct JSHOP_3_CR_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_3_CR_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
	        ~JSHOP_3_CR_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbor;\\
		    it propose an interchange of two adjacent operations (in 
		    the job-order) on the Cmax-critical way, and additional, 
		    if possible and useful, an interchange of the successive 
		    operations and an interchange of the predecence operations
		    on the critical way;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
		/// search in the tabulist \\ it returns OK or !OK
		int   use_tabulist();
		/// set a tabulist-entry
		int   set_tabulist();
		/// set initial tabu_param datas
		void clean_tabu_param();
     };

#endif




