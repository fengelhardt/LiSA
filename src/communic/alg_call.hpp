 /*
 * ****************** alg_call.hpp ******************************
 * 
 * description:      manage the calling of internal and external Algorithms
 *
 * @author            Per Willenius
 *
 * date:             8.10.1999
 *
 *
*/ 

/**@name Calling internal and external algorithms 
   @version 2.3pre3
  */
  
//@{
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
int start_ext_algo(Tcl_Interp *interp, 
                   std::string name_of_algo, 
                   std::string algo_call,
                   std::string output_file,
                   std::string result_file,
                   Lisa_Preferences & G_Preferences,
                   Lisa_ProblemType & G_ProblemType,
                   Lisa_ControlParameters & parameter,
                   Lisa_Schedule & G_Schedule,
                   Lisa_Values & G_Values);

#endif
//@}

