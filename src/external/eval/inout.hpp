/*
 * ************** inout.hpp *************************************************
 * 
 * description:   Dient dazu, in einem externen Modul Ein- und Ausgaben 
 *                auf dem Bildschirm realisieren zu koennen. 
 * owner:         Ines Wasmund
 *
 * date:          10.03.1999
*/ 

#ifndef oberfl_h 
#define oberfl_h 

#include <tk.h>

int Tcl_myAppInit(int argc, char **argv, 
    Tcl_Interp *interp );

int Tk_myMain(int argc, char **argv );   
 
int Tcl_AppInit(Tcl_Interp *interp);

int tcl_hwin(Tcl_Interp *interp);

int tkcan_clear(Tcl_Interp *interp, char* canv);

#endif
