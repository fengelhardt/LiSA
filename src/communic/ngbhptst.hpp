/*
 * ***************** ngbhtst.hpp ******************************
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

    file: LiSA/src/desktop/ngbhtst.hpp

    contains functions, which was called by the TCL/TK interpreter

    @author Andreas Winkler
    @version 2.2a0
*/ 

//@{ 
#ifndef _ngbhtst_hpp 
#define _ngbhtst_hpp
 
#include "../basics/matrix.hpp"
#include "../basics/except.hpp"
#include "../lisa_dt/ptype.hpp"
#include "../lisa_dt/schedule.hpp"
#include "../lisa_dt/lvalues.hpp"
#include "../lisa_dt/ctrlpara.hpp"
#include "../external/nb_iter/iterate.hpp"
// some definitions in: ProblemTupel
#include "../basics/global.hpp"
#include "../lisa_dt/lisapref.hpp"

#include "../misc/int2str.hpp"

#include <tk.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdlib.h>


//  **************  Function Definitions ******************

/// starts external algorithm parameter test
int TC_startparam(ClientData /* clientData */,
		Tcl_Interp *interp,
		int /*argc*/, char *argv[]);

/// call external algorithm parameter test
int start_paratest(Tcl_Interp *interp, string name_of_algo, string algo_call,
	       string output_file, string result_file,
	       Lisa_Preferences & G_Preferences,
	       Lisa_ProblemType & G_ProblemType,
	       Lisa_ControlParameters & parameter,
	       Lisa_Schedule & G_Schedule, Lisa_Values & G_Values);
 //@}
#endif









