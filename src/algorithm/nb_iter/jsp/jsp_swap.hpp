
#ifndef _jsp_swap_h
#define _jsp_swap_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop SWAP neighbourhood
/** This is the problem dependent SWAP neighbourhood class for the 
    job shop problem (i.e. these neighbourhood swaps two arbitrary operations
    on a machine).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood
*/
class JSHOP_swap_Ngbh: public JSHOP_API_Ngbh{
private:
  /// positions
  int   pos1, pos2;
  /// job order
  Lisa_Vector<int> *JOrd;
public:	
  /// construct JSHOP_swap_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_swap_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_swap_Ngbh(); 

  int   prepare_move(int);
  int   do_move();
};

#endif

