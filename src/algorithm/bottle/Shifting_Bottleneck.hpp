
#ifndef _bottleneck_h 
#define _bottleneck_h 

#include <iostream>

#include "../../basics/matrix.hpp"
#include "../../basics/graph.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../scheduling/js_sched.hpp"
#include "../../scheduling/sp_sched.hpp"
#include "../../basics/order.hpp"


/// Shifting_Bottleneck object for Lisa
/** use the Shifting_Bottleneck heuristic to solve job shop problems 
    @version 2.3final
    @author Christian Schulz
*/ 
class Shifting_Bottleneck{
private:
  /// running mode for single machine problem
  bool single_machine_mode;
  /// our problem
  Lisa_JsProblem* JsPro;
  /// our schedule
  Lisa_JsSchedule* JsSch;
  /// stores the machines, for which the schedule is determined
  Lisa_Vector<int>* Done_Machines;
  /// contains the already computed parts of the final schedule
  Lisa_Graph* Plan;
  /// initialize Plan
  void init_graph();
  /// fast algorithm for shifting_bottleneck heuristic
  void shifting_bottleneck();
  /// create the transitive hull
  /** Calculates the transitive hull for this graph and writes it into 
      the target graph. The target graph is assumed to have the same number of 
      vertices as this graph. It must not contain any edges or arcs. */
  bool transitive_hull(Lisa_Graph* source,Lisa_Graph* target);
public:
  /// constructor
  /** initialize the Shifting_Bottleneck object with parameters from 
      Lisa_JsSchedule* pJsSch */
  Shifting_Bottleneck(Lisa_JsSchedule* pJsSch, bool m=false);
  /// destructor
  ~Shifting_Bottleneck();
  /// start the bottleneck heuristic 
  /** store the result in Lisa_Schedule* pSch */
  void run(Lisa_Schedule* pSch);
};

#endif

