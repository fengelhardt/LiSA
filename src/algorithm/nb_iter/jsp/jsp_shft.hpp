
#ifndef _jsp_shft_h
#define _jsp_shft_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop SHIFT neighbourhood
/** This is the problem dependent SHIFT neighbourhood class for the 
    job shop problem (i.e. these neighbourhood shifts an arbitrary operation 
    on an other position on machine).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_shift_Ngbh: public JSHOP_API_Ngbh{
private:
  /// positions
  int   pos1, pos2, n;
  /// job order
  Lisa_Vector<int> *JOrd;     
public:	
  /// tabu positions
  Lisa_Vector<int> *tabu_0,*tabu_1;
  /// construct JSHOP_shift_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_shift_Ngbh(); 

  int   prepare_move(int);
  int   do_move();
  int   init_tabulist( unsigned int ); 
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
};

#endif

