/*
 * ***************** ptst_call.hpp ******************************
 * 
 * description:        functions which was called as tcl/tk command
 *                     parametertest of the neighbourhood iteration
 *
 * @author              Andreas Winkler
 *
 * date:               14.4.1999
 *
 * version:            V 1.0
*/ 
  
#ifndef _ptst_call_hpp 
#define _ptst_call_hpp

#include <stdlib.h>

#include <fstream>
#include <iomanip>

#include <tk.h>
 
#include "../basics/matrix.hpp"
#include "../misc/except.hpp"
#include "../misc/int2str.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../lisa/ctrlpara.hpp"
#include "../main/global.hpp"
#include "../main/lisapref.hpp"
#include "../scheduling/schedule.hpp"
#include "../desktop/tclhack.hpp"

//**************************************************************************

/// starts external algorithm parameter test
/** @author Andreas Winkler
    @version 2.3final
*/ 
int TC_startparam(ClientData /* clientData */,
		Tcl_Interp *interp,
		int /*argc*/, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// call external algorithm parameter test
/** @author Andreas Winkler
    @version 2.3final
*/ 
int start_paratest(Tcl_Interp *interp, 
                   std::string name_of_algo,
                   std::string algo_call,
                   std::string output_file, 
                   std::string result_file,
                   Lisa_Preferences & G_Preferences,
                   Lisa_ProblemType & G_ProblemType,
                   Lisa_ControlParameters & parameter,
                   Lisa_Schedule & G_Schedule, 
                   Lisa_Values & G_Values);

//**************************************************************************

#endif

