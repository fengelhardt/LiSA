
#ifndef _jsp_crtr_mix_h
#define _jsp_crtr_mix_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/// job shop critical-TRANSPOSE-mixed-API neighbourhood
/** This is the problem dependent critical-TRANSPOSE neighbourhood class for 
    the job shop problem mixed with 25% API (i.e. these neighbourhood transpose 
    a partial sequence of operations with critical end operations 
    on a machine).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_cr_trans_mix_Ngbh: public JSHOP_API_Ngbh{
private:
  //@{
  /// private data, needs documentation
  int   pos1, pos2;
  bool  api;
  Lisa_Vector<int> *JOrd,*machines;
  Lisa_Matrix<TIMETYP> *cij;
  Lisa_Matrix<int> *krv,*krr,*kr,*LR;
  //@}
public:	
  /// tabu positions
  Lisa_Vector<int> *tabu_0,*tabu_1;
  
  /// construct JSHOP_cr_trans_mix_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_cr_trans_mix_Ngbh(Lisa_JsSchedule*, Lisa_JsProblem*);
  /// destructor 
  ~JSHOP_cr_trans_mix_Ngbh(); 
  
  int   prepare_move(int);
  int   do_move();
  int   init_tabulist( unsigned int ); 
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
};

#endif

