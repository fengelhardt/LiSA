
#ifndef _jshp_bl_shift_h
#define _jshp_bl_shift_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop critical-block-SHIFT neighbourhood
/** This is the problem dependent SHIFT neighbourhood class for the 
    job shop problem, where we take a swap only for a critical block 
    operation.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_bl_shift_Ngbh: public JSHOP_API_Ngbh{
private:
  /// set of critical operations
  int   *cr_list_j;
  /// set of critical operations
  int   *cr_list_m;
  /// set of critical operations
  int   *direction;
  /// number of critical operations
  int   count;
  /// where to swap ?  
  int   pos1, pos2;
  /// job order
  Lisa_Vector<int> *JOrd;
public:	
  /// construct JSHOP_cr_bl_shift_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_cr_bl_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor 
  ~JSHOP_cr_bl_shift_Ngbh();
  
  int   prepare_move(int);
  int   do_move();
};

#endif

