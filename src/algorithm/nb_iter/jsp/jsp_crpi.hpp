
#ifndef _shp_cr_api_h
#define _shp_cr_api_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop critical-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    job shop problem where we take a swap only for critical operations.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/  
class JSHOP_cr_API_Ngbh: public JSHOP_API_Ngbh{
  private:
    /// critical operations
    int   *cr_list_j,*cr_list_m;
    /// number of critical operations
		int   count;     
	public:	
		/// construct JSHOP_cr_API_Ngbh
    /** with a start schedule and specified problem data */
		JSHOP_cr_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
    /// destructor
		~JSHOP_cr_API_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

#endif

