/*
 * ************** c_tcl.hpp ******************************
 * 
 * description: C++ functions to call tcl/tk routines
 *
 * @author Per Willenius
 * date: 28.9.1999
 * @version 2.3pre3
 */ 

#ifndef _c_tcl_h 
#define _c_tcl_h 

#include <string>
#include <tk.h>

int Tcl_myAppInit(int argc, char **argv, 
    Tcl_Interp *interp );

int Tk_myMain(int argc, char **argv );   
 
int Tcl_AppInit(Tcl_Interp *interp);

/// open a message window and display a message
void message(std::string title,std::string text);

/// open a message window and display a message
void message(std::string text);

/** Translate the word in the actual language.
    The function returns the value of $Name(word) which has to defined
    in the file: LiSA/tcl/<actual_language>/main.tcl. */
std::string translate(std::string word);

/// open main window:
void open_main_window();

/// open dialog to problem input
void open_problem_window();

/// print a message on the label position in the main window:
void mw_set_label(std::string text);

/// print a message on the objective value position in the main window:
void mw_set_objective(std::string text);

/// print error to stdout or in a message window
void print_error();

/// return the height of the .lisa Window
int mw_height();

/// return the width of the .lisa Window
int mw_width();

#endif

