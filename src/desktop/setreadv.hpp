/*
 * ***************** setreadv.hpp ******************************
 * 
 * description:        functions to connect tcl and C++ varaibles
 *
 * owner:              Per Willenius
 *
 * date:               28.9.1999
 *
 * version:            V 1.0
*/ 

/** @name TCL/TK Commands

    include tk.h

    file: LiSA/src/desktop/setreadv.hpp

    contains functions, which was called by the TCL/TK interpreter
    and set or read C++ variables

    @author Per Willenius
    @version 2.3pre2
*/ 

//@{ 
#ifndef _setreadv_h 
#define _setreadv_h
 
//  **************  System Includes  ******************
#include <tk.h>

//  **************  Function Definitions ******************


/// set entry in G_Schedule->LR
int TC_set_p(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, const char *argv[]); 

/// get entry in G_Schedule->LR
int TC_get_p(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, const char *argv[]); 

/// set entry in G_Values.PT
int TC_set_pt(ClientData clientData,
	     Tcl_Interp *interp,
	     int argc, const char *argv[]); 

/// get entry in G_Values.PT
int TC_get_pt(ClientData clientData,
	     Tcl_Interp *interp,
	     int argc, const char *argv[]); 

/// set in Lisa_Values the new maschine or job number
int TC_set_prob(ClientData clientData,
	       Tcl_Interp *interp,
	       int argc, const char *argv[]); 

/// set an entry in Lisa_Problen.tupel
int TC_set_Tupel(ClientData clientData,
		 Tcl_Interp *interp,
		 int argc, const char *argv[]); 

/// returns the values of C++ variables
int TC_getvar(ClientData clientData,
	     Tcl_Interp *interp,
	     int argc, const char *argv[]);

/// Set C++ variables 
int TC_setvar(ClientData clientData,
	     Tcl_Interp *interp,
	     int argc, const char *argv[]);

#endif

//@}



