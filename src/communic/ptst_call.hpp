/*
 * ***************** ptst_call.hpp ******************************
 * 
 * description:        functions which was called as tcl/tk command
 *                     parametertest of the neighborhood iteration
 *
 * owner:              Andreas Winkler
 *
 * date:               14.4.1999
 *
 * version:            V 1.0
*/ 

/** @name TCL/TK Commands

    include tk.h

    file: LiSA/src/desktop/ptst_call.hpp

    contains functions, which was called by the TCL/TK interpreter

    @author Andreas Winkler
    @version 2.3pre2
*/ 

//@{ 
#ifndef _ptst_call_hpp 
#define _ptst_call_hpp
 
#include "../basics/matrix.hpp"
#include "../misc/except.hpp"
#include "../lisa/ptype.hpp"
#include "../scheduling/schedule.hpp"
#include "../lisa/lvalues.hpp"
#include "../lisa/ctrlpara.hpp"
// some definitions in: ProblemTupel
#include "../main/global.hpp"
#include "../main/lisapref.hpp"

#include "../misc/int2str.hpp"

#include <tk.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

//  **************  Function Definitions ******************

/// starts external algorithm parameter test
int TC_startparam(ClientData /* clientData */,
		Tcl_Interp *interp,
		int /*argc*/, const char *argv[]);

/// call external algorithm parameter test
int start_paratest(Tcl_Interp *interp, string name_of_algo, string algo_call,
	       string output_file, string result_file,
	       Lisa_Preferences & G_Preferences,
	       Lisa_ProblemType & G_ProblemType,
	       Lisa_ControlParameters & parameter,
	       Lisa_Schedule & G_Schedule, Lisa_Values & G_Values);
 //@}
#endif









