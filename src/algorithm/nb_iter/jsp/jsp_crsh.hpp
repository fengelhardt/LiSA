
#ifndef _jshp_cr_shift_h
#define _jshp_cr_shift_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop critical shift neighbourhood
/** This is a problem dependent API neighbourhood class for the job shop 
    problem, where we shift an operation dependently of the critical block
    structure.
    
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_shift_Ngbh: public JSHOP_API_Ngbh{
private:
  /// set of critical operations
  Lisa_Vector<int>   *cr_list_j,*cr_list_m;
  /// number of critical operations
  int   cr_count;    
  /// job order
  Lisa_Vector<int>   *JOrd;
public:	
  /// construct JSHOP_cr_shift_Ngbh 
  /** with a start schedule and specified problem data */
  JSHOP_cr_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_cr_shift_Ngbh();
  
  int   prepare_move(int);
  int   do_move();
};

#endif

