/*
 * **************Bottleneck.h*******************************
 * 
 * Shifting_Bottleneck heuristic
 *
 * Owner: Christian Schulz
 *
 * 14.5.2001
 *
*/

#ifndef _bottleneck_h 
#define _bottleneck_h 

#include "../../basics/matrix.hpp"
#include "../../basics/graph.hpp"
#include "../../basics/except.hpp"
#include "../../lisa_dt/lvalues.hpp"
#include "../../lisa_dt/schedule.hpp"
#include"../../lisa_dt/schedule/js_sched.hpp"
#include"../../lisa_dt/schedule/sp_sched.hpp"
#include"../../basics/order.hpp"
#include <iostream.h>


/** Shifting_Bottleneck object for Lisa, using the Shifting_Bottleneck 
    heuristic to solve scheduling problems 

    @author Christian Schulz
*/ 
class Shifting_Bottleneck
{
  /// running mode for single machine problem
  bool single_machine_mode;
  Lisa_JsProblem* JsPro;
  Lisa_JsSchedule* JsSch;
  /// stores the machines, for which the schedule is determined
  Lisa_Vector<int>* Done_Machines;
  /// contains the already computed parts of the final schedule
  Lisa_Graph* Plan;
  /// initialize the Lisa_Graph* Plan
  void init_graph();
  /// fast algorithm for shifting_bottleneck heuristic
  void shifting_bottleneck();
public:
  /** initialize the Shifting_Bottleneck object with
      parameters from Lisa_JsSchedule* pJsSch 
  */
  Shifting_Bottleneck(Lisa_JsSchedule* pJsSch, bool m=false);
  ~Shifting_Bottleneck();
  /** start the bottleneck heuristic and ...\\ 
      stores the result in Lisa_Schedule* pSch */
  void run(Lisa_Schedule* pSch);
};
#endif





