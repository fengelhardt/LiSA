/*
 * ***************** setreadv.hpp ******************************
 * 
 * description:        functions to connect tcl and C++ varaibles
 *
 * @author              Per Willenius
 *
 * date:               28.9.1999
 *
 * @version 2.3pre3
 */ 

#ifndef _setreadv_h 
#define _setreadv_h
 
#include <tk.h>
#include "tclhack.hpp"

/** @name TCL/TK Commands

    include tk.h

    file: LiSA/src/desktop/setreadv.hpp

    contains functions, which was called by the TCL/TK interpreter
    and set or read C++ variables

    @author Per Willenius
    @version 2.3pre3
*/ 

//@{ 

//**************************************************************************

/// set entry in G_Schedule->LR
int TC_set_p(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// get entry in G_Schedule->LR
int TC_get_p(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// set entry in G_Values.PT
int TC_set_pt(ClientData clientData,
	     Tcl_Interp *interp,
	     int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// get entry in G_Values.PT
int TC_get_pt(ClientData clientData,
	     Tcl_Interp *interp,
	     int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// set in Lisa_Values the new maschine or job number
int TC_set_prob(ClientData clientData,
	       Tcl_Interp *interp,
	       int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// set an entry in Lisa_Problen.tupel
int TC_set_Tupel(ClientData clientData,
		 Tcl_Interp *interp,
		 int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// returns the values of C++ variables
int TC_getvar(ClientData clientData,
	     Tcl_Interp *interp,
	     int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Set C++ variables 
int TC_setvar(ClientData clientData,
	     Tcl_Interp *interp,
	     int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

//@}

#endif

