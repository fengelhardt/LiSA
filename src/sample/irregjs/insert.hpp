
#ifndef _insert_h
#define _insert_h

#include "../../scheduling/js_sched.hpp"
#include "../../basics/order.hpp"

//**************************************************************************

/// needs documentation
void rattle_js( Lisa_JsSchedule * sched);

//**************************************************************************

/// Job Shop insertion class
/**
  @version 2.3rc1
  @author Thomas Tautenhahn */
class JS_Inserter{
public:
//@{
  ///public method, needs documentation
  void init(Lisa_JsSchedule * in_sched, int in_zfn);
  void rattle( int job );
  void insert();
  void simple_order();
//@}

  /// destructor
  ~JS_Inserter();
private:   
//@{
  /// private data, needs documentation
  Lisa_JsSchedule * sched;
  Lisa_JsSchedule * sched2;
  int zfn;
  Lisa_JsProblem * P;
  Lisa_Order * order;
//@}
};
  
//**************************************************************************  

#endif

