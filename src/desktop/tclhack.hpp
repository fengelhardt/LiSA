
#ifndef _tclhack_h 
#define _tclhack_h
 
#include <tk.h>

//**************************************************************************

/// Tcl version hack.
/** Determine whether we need to use char** oder const char** in our tcl2cpp
    callback functions.
    @author Marc Moerig
    @version 3.0pre1
  */
#if TCL_MAJOR_VERSION < 8
 #define TCL_HACK_CHAR char
#endif

#if TCL_MAJOR_VERSION == 8 
 #if TCL_MINOR_VERSION < 4
  #define TCL_HACK_CHAR char
 #else
  #define TCL_HACK_CHAR const char
 #endif
#endif

#if TCL_MAJOR_VERSION > 8
 #define TCL_HACK_CHAR const char
#endif

//**************************************************************************

#endif

