/*
 * ******************** jsp_3api.hpp ******************************
 * 
 * description:      job shop 3-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _jsp_3_api_h
#define _jsp_3_api_h

#include "../neighbor.hpp"
#include "../../../scheduling/js_sched.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop 3-time-API Neighborhood.
    This is a problem dependent API neighborhood class for the 
    job shop problem where we take a swap of adjacent operatins on 3 different
    machines.\\
    It works only with tabusearch method !\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre2
    @see Lisa_Neighborhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_3_API_Ngbh: public JSHOP_API_Ngbh
     {
                int   swaps[3][5];
	public:	
		/** construct OSHOP_API_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_3_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
		/** propose a possible move, 
	            parameter is ENUM or RAND for enumerative or random
		    generation of neighbor;\\
		    it propose three interchanges of two adjacent operations 
		    in the job-orders of three machines;\\
	 	    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
		/// search in the tabulist \\ it returns OK or !OK
		int   use_tabulist();
		/// set a tabulist-entry
		int   set_tabulist();
     };

#endif




