/*
 * ***************** tclhack.hpp ******************************
 *
 * @author              Marc Mörig
 *
 * date:               07.03.2003
 * @version 2.3pre3
 */ 

#ifndef _tclhack_h 
#define _tclhack_h
 
#include <tk.h>

//**************************************************************************

/** Tcl version hack.
    Determine whether we need to use char** oder const char** in our tcl2cpp
    callback functions. */
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

