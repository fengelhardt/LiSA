
#ifndef _oshp_3_cr_h
#define _oshp_3_cr_h

#include "../../../basics/order.hpp"
#include "../../../scheduling/os_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/// open shop 3 time critical API Neighbourhood.
/** This is the problem dependent API neighbourhood class for the 
    open shop problem, where we swap two adjacent operations on the
    critical way and additional we swap also the direct predecence 
    and the direct sucsessive operations on the critical way.
    
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_3_CR_Ngbh: public OSHOP_API_Ngbh{       
  private:
    //@{
    /// private data, needs documentation
    Lisa_Order *ROrd;
		int   OrdCount;
		int   NewOrder;
		TIMETYP head, tail;
		int   machine3;
		int   job3;
	  Lisa_OsSchedule  *temp_schedule;
    //@}
    /// for the 3 proposed swaps
    /** for JO-swap:
	    - swaps[][0]=JO
			- swaps[][1] machine1
			- swaps[][2] job1
			- swaps[][3] job2
			- swaps[][4] OK ? */   
    int   swaps[3][5];
	public:	
		/// construct OSHOP_3_CR_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_3_CR_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
	  /// destructor
    ~OSHOP_3_CR_Ngbh();

		int   prepare_move(int);
		int   do_move();
		int   use_tabulist();
		int   set_tabulist();
		void clean_tabu_param();
};

#endif

