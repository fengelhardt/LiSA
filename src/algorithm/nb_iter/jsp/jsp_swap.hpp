/*
 * ******************** jsp_swap.hpp ******************************
 * 
 * description:      job shop SWAP neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             19.09.2000
 *
 */

#ifndef _jsp_swap_h
#define _jsp_swap_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop SWAP Neighborhood.
    This is the problem dependent SWAP neighborhood class for the 
    job shop problem (i.e. these neighborhood swaps two arbitrary operations
    on a machine).\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
*/
class JSHOP_swap_Ngbh: public JSHOP_API_Ngbh
     {
                int   pos1, pos2;
                Lisa_Vector<int> *JOrd;
	public:	
		/** construct JSHOP_swap_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_swap_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
                ~JSHOP_swap_Ngbh(); 
		/** propose a possible move,
		    parameter is only RAND for random
		    generation of a neighbor;\\
		    it propose an interchange of two arbitrary not necessary 
   		    adjacent operations in the job-order of one machine;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif




