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
#include "../basics/lisa_str.hpp"
#include <tcl.h>

//  ********************** LiSA Includes ***************************
// Lisa_ProblemType, Lisa_Values, Lisa_Schedule, Lisa_ControlParameters
// and Lisa_Preferences as parameters
#include "../lisa_dt/ctrlpara.hpp"
#include "../lisa_dt/ptype.hpp"
#include "../lisa_dt/lvalues.hpp"
#include "../lisa_dt/schedule.hpp"
#include "../lisa_dt/lisapref.hpp"

// sorting used for priority rules
#include "../basics/order.hpp"

// Lisa_Status used as parameter for priority rules
#include "../basics/status.hpp"

/// priority rules
void start_prior(Tcl_Interp *interp,Lisa_ProblemType & G_ProblemType,
		 Lisa_Schedule & G_Schedule, Lisa_Values & G_Values,
		 Lisa_Status & G_Status, string type);

/// call external algorithm
int start_ext_algo(Tcl_Interp *interp, string name_of_algo, string algo_call,
	       string output_file, string result_file,
	       Lisa_Preferences & G_Preferences,
	       Lisa_ProblemType & G_ProblemType,
	       Lisa_ControlParameters & parameter,
	       Lisa_Schedule & G_Schedule, Lisa_Values & G_Values);

/// call internal algorithm
int start_int_algo(string name_of_algo, string algo_call,
	       Lisa_Preferences & G_Preferences,
	       Lisa_ProblemType & G_ProblemType,
	       Lisa_ControlParameters & parameter,
	       Lisa_Schedule & G_Schedule, Lisa_Values & G_Values);

//@}

#endif





