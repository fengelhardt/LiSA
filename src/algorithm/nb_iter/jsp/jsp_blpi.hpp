
#ifndef _jshp_bl_api_h
#define _jshp_bl_api_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop critical-block-API neighbourhood
/** This is the problem dependent API neighbourhood class for the 
    open shop problem, where we take a swap only for a critical block 
    operation.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_bl_API_Ngbh: public JSHOP_API_Ngbh{
private:
  /// set of critical operations
  int   *cr_list_j;
  /// set of critical operations
  int   *cr_list_m;
  /// set of critical operations
  int   *direction;
  /// number of critical operations 
  int   count;      
public:	
  /// construct JSHOP_cr_bl_API_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_cr_bl_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_cr_bl_API_Ngbh();

  int   prepare_move(int);
  int   do_move();
};

#endif

