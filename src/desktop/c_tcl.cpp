/*
 * ************** c_tcl.cpp ******************************
 * 
 * description: C++ functions to call tcl/tk routines
 *
 * owner:       Per Willenius
 * date:        28.9.1999
 * patch        15.02.2000 by TAU
 * patch        02.07.2002 by eldeh
 * version:     V 1.0
*/ 

// ********************* System Includes ********************
// string used in string file
#include <string>

using namespace std;


// ******************** Project Includes ********************

// Tcl/Tk wants to know, where this C++ funktions are
#include "tcl_c.hpp"
#include "setreadv.hpp"
#include "../communic/ptst_call.hpp"

// G_Preferences
#include "../main/lisapref.hpp"

// G_TclVar.alive
#include "../desktop/tclvar.hpp"

// G_ProblemType
#include "../lisa/ptype.hpp"

// Exception handling
#include "../misc/except.hpp"

// ************************ Local Includes ********************
#include "c_tcl.hpp"

// ********************** Global Variables ********************
extern class Lisa_Preferences G_Preferences;
extern Tcl_Interp *interp;
extern class Lisa_TCLVar G_TclVar;
extern class Lisa_ProblemType G_ProblemType;

// *********************** functions *************************

int Tk_myMain(int argc, char **argv) {
  interp = Tcl_CreateInterp();

// Cygwin needs the following line due to a TCL bug therein
  Tcl_FindExecutable(argv[0]);
  
  if (  Tcl_myAppInit( argc, argv, interp ) != TCL_OK ) 
    {
      fprintf( stderr, "\nUnexpected application error!" );
      return 0;
    }
  return 1;
}

int Tcl_myAppInit( int /*argc*/, char ** /*argv[]*/, Tcl_Interp *interp ) {
  int         retcode;
  Tk_Window   mainWindow;
  if( Tcl_Init(interp) == TCL_ERROR ) {
    fprintf(stderr, "Tcl_Init failed: %s\n", interp->result);
    exit(1);
  }
  
  if( Tk_Init(interp) == TCL_ERROR ) {
    fprintf(stderr, "Tk_Init failed: %s\n", interp->result);
    exit(1);
  }
   
  // all C++ functions, which can be called from Tcl/Tk:  
  Tcl_CreateCommand(interp, "TC_adopt_MO", TC_adopt_MO, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_make_MO_Matrix",TC_make_MO_Matrix,(ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_open", TC_open, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_open_schedule", TC_open_schedule,(ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_t", TC_open, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_save", TC_save, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_set_p", TC_set_p, (ClientData)
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_get_p", TC_get_p, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
   Tcl_CreateCommand(interp, "TC_set_prob", TC_set_prob, (ClientData) 
		     Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_set_pt", TC_set_pt, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_get_pt", TC_get_pt, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_gen_seq", TC_gen_seq, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_gen_sij", TC_gen_sij, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_gen_mo", TC_gen_mo, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_rev_seq", TC_rev_seq, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_update_seq", TC_update_seq, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_update_cij", TC_update_cij, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_draw_output", TC_draw_output, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_set_Tupel", TC_set_Tupel, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_genpt", TC_genpt, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_getvar", TC_getvar, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_setvar", TC_setvar, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_draw_dat", TC_draw_dat, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_draw_dat_entry", TC_draw_dat_entry,(ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_draw_schedule", TC_draw_schedule, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_draw_schedule_list", TC_draw_schedule_list, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_draw_schedule_entry", TC_draw_schedule_entry, 
	(ClientData)Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_gen_column", TC_gen_column, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_new_problem", TC_new_problem, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_new_values", TC_new_values, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_mark_value_entry", TC_mark_value_entry, 
       (ClientData) Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
  Tcl_CreateCommand(interp, "TC_mark_schedule_entry", TC_mark_schedule_entry, 
       (ClientData) Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);  
 //  Tcl_CreateCommand(interp, "TC_start_bb", TC_start_bb, (ClientData) 
// 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
//   Tcl_CreateCommand(interp, "TC_start_nb", TC_start_nb, (ClientData) 
// 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateCommand(interp, "TC_exit", TC_exit, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);   
  Tcl_CreateCommand(interp, "TC_classify", TC_classify, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);   
    Tcl_CreateCommand(interp, "TC_references", TC_references, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);  
  Tcl_CreateCommand(interp, "TC_error", TC_error, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);   
 Tcl_CreateCommand(interp, "TC_new", TC_new, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);   
 Tcl_CreateCommand(interp, "TC_save_old_sequence", TC_save_old_sequence , (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);   
 Tcl_CreateCommand(interp, "TC_move_operation", TC_move_operation, (ClientData) 
		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);   
 Tcl_CreateCommand(interp, "TC_choose_sequence", TC_choose_sequence, (ClientData) 
		   Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
 Tcl_CreateCommand(interp, "TC_sort_sl", TC_sort_sl, (ClientData) 
           Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
 Tcl_CreateCommand(interp, "TC_startalg", TC_startalg, (ClientData) 
		   Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
 Tcl_CreateCommand(interp, "TC_startparam", TC_startparam, (ClientData) 
		   Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp, "TC_problem_reduction", TC_problem_reduction, (ClientData) 
		   Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
 Tcl_CreateCommand(interp, "TC_exclude", TC_exclude, (ClientData) 
		   Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
 Tcl_CreateCommand(interp, "TC_save_options",TC_save_options, (ClientData) 
		   Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
 Tcl_CreateCommand(interp, "TC_stop_extalg",TC_stop_extalg, (ClientData) 
		   Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 
 Tcl_CreateCommand(interp, "TC_kill",TC_kill, (ClientData) 
		   Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL); 

 mainWindow = Tk_MainWindow(interp);

 G_Preferences.init(interp);
 string file;
 file = G_Preferences.LISA_HOME+"/tcl/tclinit.tcl";
 // starts the tclinit.tcl file:
 retcode = Tcl_EvalFile( interp,(char*) file.c_str() );
 if(retcode != TCL_OK) {
   fprintf(stderr, "\nGUI: Initialization ERROR: ");
   cerr << Tcl_PosixError(interp) << endl;  
   cerr <<  interp->result<< endl;
   Tcl_BackgroundError(interp);
   //fprintf(stderr, "%s\n", interp->result);
   exit(1);
 }
 // the following instead of:  Tk_MainLoop(); 
 do {
   // waits:
   Tcl_DoOneEvent(TCL_ALL_EVENTS);
   // Exception Handling procedure
   if (G_ExceptionList.empty()==0) print_error();
 } while( G_TclVar.alive == 1 );
 
 //  cout << "LiSA exits correctly.\n";
 Tcl_Eval(interp, "exit");
 return 0;  
}

/// open a message window and display a message
void message(string title,string text) {
  string command="showmessage " + title + " \"" + text+"\"";
  Tcl_Eval(interp,(char*) command.c_str());
}

// open a message window and display a message
void message(string text) {
  message("",text);
}

// Translate the word in the actual language.
// The function returns the value of $Name(word) which has to defined
// in the file: LiSA/tcl/<actual_language>/main.tcl. 
string translate(string word) {
  return Tcl_GetVar2(interp,"Name",(char*) word.c_str(),0);
}

// open main window:
void open_main_window() {
  Tcl_Eval(interp,"Window show .lisa");
}

// open dialog to problem input
void open_problem_window() {
  //  G_ProblemType.valid=TRUE;
  Tcl_Eval(interp,"Window show .dprob");
}

// prints a message on the label position in the main window:
void mw_set_label(string text) {
  string command="mw_set_label \"" + text+"\"";
  Tcl_Eval(interp,(char*) command.c_str());
}

// prints a message on the objective value position in the main window:
void mw_set_objective(string text) {
  string command="mw_set_objective \"" + text+"\"";
  Tcl_Eval(interp,(char*) command.c_str());
}

// print error to stdout or in a message window
void print_error() {
  string errorname=  G_ExceptionList.lcatch();

  // print error message on the button-line of the LiSA-Window:
  // mw_set_label(errorname);
  
  // open errow Window:
  string command="showerrormessage \"" + errorname +"\n\"";
  Tcl_Eval(interp,(char*) command.c_str());
  }

int mw_height() {
  int height=1;
  Tcl_Eval(interp, "set_height");
   string str;
   str= Tcl_GetVar2(interp,"lsa_status","height",TCL_GLOBAL_ONLY);
   sscanf(str.c_str(),"%d",&height);
  return height-153; // magic number corrected from 160 (trail and error)
}

int mw_width() {
  int width=1;
  Tcl_Eval(interp, "set_width");
  string str;
  str= Tcl_GetVar2(interp,"lsa_status","width",TCL_GLOBAL_ONLY);
  sscanf(str.c_str(),"%d",&width);
  return width-75; // magic number corrected from 160 (trail and error)
}












