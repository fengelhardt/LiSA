 /*
 * ****************** alg_call.hpp ******************************
 * 
 * description:      manage the calling of internal and external Algorithms
 *
 * owner:            Per Willenius
 *
 * date:             8.10.1999
 *
 *
*/ 

/**@name Calling internal and external algorithms */
//@{


#ifndef _alg_call_h 
#define _alg_call_h

 
//  ********************** System Includes ***************************
#include <string>
#include <tcl.h>

using namespace std;

//  ********************** LiSA Includes ***************************
// Lisa_ProblemType, Lisa_Values, Lisa_Schedule, Lisa_ControlParameters
// and Lisa_Preferences as parameters
#include "../lisa/ctrlpara.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"
#include "../main/lisapref.hpp"

// sorting used for priority rules
#include "../basics/order.hpp"

// Lisa_Status used as parameter for priority rules
#include "../main/status.hpp"

/// call external algorithm
int start_ext_algo(Tcl_Interp *interp, string name_of_algo, string algo_call,
	       string output_file, string result_file,
	       Lisa_Preferences & G_Preferences,
	       Lisa_ProblemType & G_ProblemType,
	       Lisa_ControlParameters & parameter,
	       Lisa_Schedule & G_Schedule, Lisa_Values & G_Values);

//@}

#endif





