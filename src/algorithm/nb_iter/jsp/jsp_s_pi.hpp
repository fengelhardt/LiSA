/*
 * ******************** jsp_s_pi.hpp ******************************
 * 
 * description:      job shop semicritical-API neighbourhood
 * 
 * @author            Andreas Winkler
 *
 * date:             17.10.2000
 *
 */

#ifndef _jshp_s_api_h
#define _jshp_s_api_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop semicritical-API Neighbourhood.
    This is a problem dependent API neighbourhood class for the 
    job shop problem, where we interchange two noncritical adjacent operations 
    or we move a critical operation to the critical block end.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_semi_API_Ngbh: public JSHOP_API_Ngbh
     {
                Lisa_Vector<int> *JOrd;
	public:	
		/** construct JSHOP_semi_API_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_semi_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
		~JSHOP_semi_API_Ngbh();
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbour;
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move  it returns OK or !OK
		int   do_move();
     };

#endif



