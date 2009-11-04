
#ifndef _alg_call_h 
#define _alg_call_h

#include <string>
#include <tcl.h>

#include "../lisa/ctrlpara.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"
#include "../main/lisapref.hpp"
#include "../basics/order.hpp"
#include "../main/status.hpp"

/// call external algorithm
/** manage the calling of external algorithms
    @author Per Willenius
    @version 3.0pre1
  */
int start_ext_algo(Tcl_Interp *interp, 
                   std::string name_of_algo, 
                   std::string algo_call,
                   std::string output_file,
                   std::string result_file,
                   Lisa_ControlParameters & G_Preferences,
                   Lisa_ProblemType & G_ProblemType,
                   Lisa_ControlParameters & parameter,
                   Lisa_Schedule & G_Schedule,
                   Lisa_Values & G_Values);

#endif

