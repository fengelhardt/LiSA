
#ifndef _setreadv_h 
#define _setreadv_h
 
#include <tk.h>
#include "tclhack.hpp"

//**************************************************************************

/// set entry in G_Schedule->LR
/** @author Per Willenius
    @version 2.3pre3
*/ 
int TC_set_p(ClientData clientData, Tcl_Interp *interp,
             int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// get entry in G_Schedule->LR
int TC_get_p(ClientData clientData, Tcl_Interp *interp,
	           int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// set entry in G_Values.PT
int TC_set_pt(ClientData clientData, Tcl_Interp *interp,
	            int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// get entry in G_Values.PT
int TC_get_pt(ClientData clientData, Tcl_Interp *interp,
	            int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// set in G_Values the new machine or job number
int TC_set_prob(ClientData clientData, Tcl_Interp *interp,
	              int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// set an entry in G_ProblemType
int TC_set_Tupel(ClientData clientData, Tcl_Interp *interp,
		             int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// returns the values of c++ variables
int TC_getvar(ClientData clientData, Tcl_Interp *interp,
	            int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// set c++ variables 
int TC_setvar(ClientData clientData, Tcl_Interp *interp,
	            int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

#endif

