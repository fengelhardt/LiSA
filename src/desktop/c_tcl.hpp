/*
 * ************** c_tcl.hpp ******************************
 * 
 * description: C++ functions to call tcl/tk routines
 *
 * owner: Per Willenius
 * date: 28.9.1999
 * version: V 1.0
*/ 

/** 
  @version:
  */

#ifndef _c_tcl_h 
#define _c_tcl_h 

//  ************** System Includes  ******************
#include <tk.h>
#include "../misc/lisa_str.hpp"

//  ************** Functions  ***********************

int Tcl_myAppInit(int argc, char **argv, 
    Tcl_Interp *interp );

int Tk_myMain(int argc, char **argv );   
 
int Tcl_AppInit(Tcl_Interp *interp);

/// open a message window and display a message
void message(string title,string text);

/// open a message window and display a message
void message(string text);

/// Translate the word in the actual language.
/// The function returns the value of $Name(word) which has to defined
/// in the file: LiSA/tcl/<actual_language>/main.tcl. 
string translate(string word);

/// open main window:
void open_main_window();

/// open dialog to problem input
void open_problem_window();

/// print a message on the label position in the main window:
void mw_set_label(string text);

/// print a message on the objective value position in the main window:
void mw_set_objective(string text);

/// print error to stdout or in a message window
void print_error();

/// return the height of the .lisa Window
int mw_height();


/// return the width of the .lisa Window
int mw_width();


#endif





