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

 
//  ********************** System Includes ***************************
#include <string>
#include <tcl.h>

#include "../lisa/ctrlpara.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"
#include "../main/lisapref.hpp"
#include "../basics/order.hpp"
#include "../main/status.hpp"

using namespace std;

/// call external algorithm
int start_ext_algo(Tcl_Interp *interp, string name_of_algo, string algo_call,
	       string output_file, string result_file,
	       Lisa_Preferences & G_Preferences,
	       Lisa_ProblemType & G_ProblemType,
	       Lisa_ControlParameters & parameter,
	       Lisa_Schedule & G_Schedule, Lisa_Values & G_Values);

//@}

#endif





