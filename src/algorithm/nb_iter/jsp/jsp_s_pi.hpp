
#ifndef _jshp_s_api_h
#define _jshp_s_api_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop semicritical-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    job shop problem, where we interchange two noncritical adjacent operations 
    or we move a critical operation to the critical block end.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_semi_API_Ngbh: public JSHOP_API_Ngbh{
private:
  /// job order 
  Lisa_Vector<int> *JOrd;
public:	
  /// construct JSHOP_semi_API_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_semi_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_semi_API_Ngbh();

  int   prepare_move(int);
  int   do_move();
};

#endif

