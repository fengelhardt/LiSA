
#ifndef _oshp_shft_h
#define _oshp_shft_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "osp_api.hpp"

/// open shop SHIFT neighbourhood
/** This is the problem dependent SHIFT neighbourhood class for the 
    open shop problem (i.e. these neighbourhood swaps two arbitrary
    opererations on one machine or for one job).
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
 */
class OSHOP_shift_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// where to swap  
    int   pos1, pos2;
    /// private data needs documentation
		Lisa_Vector<int> *JOrd, *MOrd, *JOpos, *MOpos;
	public:	
		/// construct OSHOP_API_Ngbh 
    /** with a start schedule and specified problem datas   */
		OSHOP_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_shift_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

#endif

