/*
 * ******************** jsp_crsh.hpp ******************************
 * 
 * description:      job shop critical SHIFT neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _shp_cr_shift_h
#define _shp_cr_shift_h

#include "../neighbor.hpp"
#include "../../../lisa_dt/schedule/js_sched.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop critical-SHIFT Neighborhood.
    This is a problem dependent SHIFT neighborhood class for the 
    job shop problem where we take a swap only for critical operations.\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 1.0a1
    @see Lisa_Neighborhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_shift_Ngbh: public JSHOP_API_Ngbh
     {
                int   *cr_list_j;
		int   *cr_list_m; // set of critical operations
		int   count;      // number of critical operations
                Lisa_Vector<int> *JOrd; 
	public:	
		/** construct OSHOP_API_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_cr_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
		~JSHOP_cr_shift_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbor;\\
		    it propose an interchange of two not necessary adjacent 
		    operations (in the job-order of a machine) on the 
		    Cmax-critical way;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
     };

#endif




