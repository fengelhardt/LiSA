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

//tcl version hack
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
