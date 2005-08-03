
#ifndef _tcl_c_h 
#define _tcl_c_h
 
#include <tk.h>
#include "tclhack.hpp"


//**************************************************************************

/// write MO as Matrix in Lisa_MO
/** @author Per Willenius
    @version 2.3final
 */
int TC_adopt_MO(ClientData clientData, Tcl_Interp *interp,
	              int argc, TCL_HACK_CHAR *argv[]);  

//**************************************************************************

/// write Lisa_MO in Matrix in G_TclVar
int TC_make_MO_Matrix(ClientData,Tcl_Interp *,
		                  int , TCL_HACK_CHAR **);

//**************************************************************************

/// read file
int TC_open(ClientData clientData,Tcl_Interp *interp,
	          int argc, TCL_HACK_CHAR *argv[]);  

//**************************************************************************

/// read only the schedule
int TC_open_schedule(ClientData clientData, Tcl_Interp *interp,
	                   int argc, TCL_HACK_CHAR *argv[]);  

//**************************************************************************

/// write file
int TC_save(ClientData clientData, Tcl_Interp *interp,
	          int argc, TCL_HACK_CHAR *argv[]);  

//**************************************************************************

/// generate randomly processing time matrix 
/** with parameters min,max,step, seed1,seed2 */
int TC_genpt(ClientData clientData, Tcl_Interp *interp,
	           int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// generate randomly column
/** with parameters min,max,step,seed1,name */
int TC_gen_column(ClientData clientData, Tcl_Interp *interp,
	                int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// generate randomly Lisa_Schedule.LR
int TC_gen_seq(ClientData clientData, Tcl_Interp *interp,
	             int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// generate randomly Lisa_Values.SIJ
int TC_gen_sij(ClientData clientData, Tcl_Interp *interp,
	             int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// generate randomly Lisa_Values.MO
int TC_gen_mo(ClientData clientData, Tcl_Interp *interp,
	            int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// reverse  Lisa_Schedule.LR
int TC_rev_seq(ClientData clientData, Tcl_Interp *interp,
	             int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// update a schedule
int TC_update_seq(ClientData clientData, Tcl_Interp *interp,
	                int argc, TCL_HACK_CHAR *argv[]);  

//**************************************************************************

/// update CIJ matrix
int TC_update_cij(ClientData clientData, Tcl_Interp *interp,
	                int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// refresh the output
int TC_draw_output(ClientData clientData, Tcl_Interp *interp,
	                 int argc, TCL_HACK_CHAR *argv[]); 

//**************************************************************************

/// Draw Value Window
int TC_draw_dat(ClientData clientData, Tcl_Interp *interp,
	              int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Update an entry in the value Window
int TC_draw_dat_entry(ClientData clientData, Tcl_Interp *interp,
	                    int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Mark an entry in the Value Window
int TC_mark_value_entry(ClientData clientData, Tcl_Interp *interp,
	                      int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Draw Schedule Window
int TC_draw_schedule(ClientData clientData, Tcl_Interp *interp,
	                   int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Draw List of Schedules
int TC_draw_schedule_list(ClientData clientData, Tcl_Interp *interp,
	                        int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Update an entry in the schedule Window
int TC_draw_schedule_entry(ClientData clientData, Tcl_Interp *interp,
			                     int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// if new Lisa_ProblemType defined
int TC_new_problem(ClientData clientData, Tcl_Interp *interp,
	                 int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// if new Lisa_Values defined
int TC_new_values(ClientData clientData, Tcl_Interp *interp,
	                int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Mark an entry in the Schedule Window
int TC_mark_schedule_entry(ClientData clientData, Tcl_Interp *interp,
	                         int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

// call the extern branch and bound routine
// with parameter: 
// number of solutions:no_of_solutions 
// insertion order: ins_order 
// type of lower bound lower_bound 
// upper bound: upper_bound
// int TC_start_bb(ClientData clientData,
// 	    Tcl_Interp *interp,
// 	    int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

// call the extern neigborhood-search module
// int TC_start_nb(ClientData clientData,
// 	    Tcl_Interp *interp,
// 	    int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// control the end of the LiSA programm
int TC_exit(ClientData clientData, Tcl_Interp *interp,
	          int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// problem classification
int TC_classify(ClientData clientData,Tcl_Interp *interp,
	              int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// write bibtex reference for the corresponding classification
int TC_references(ClientData clientData, Tcl_Interp *interp,
	                int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// if an error in the Tk- application occours
int TC_error(ClientData clientData, Tcl_Interp *interp,
		         int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Delete all global objects
int TC_new(ClientData clientData, Tcl_Interp *interp,
		       int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// write G_Schedule.LR to G_TCLVar.LR
int  TC_save_old_sequence(ClientData clientData, Tcl_Interp *interp,
			                    int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// move operation
int TC_move_operation(ClientData /* clientData */, Tcl_Interp * /*interp*/,
	                    int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Choose new Sequence
int TC_choose_sequence(ClientData /* clientData */, Tcl_Interp * interp,
		                   int /*argc*/, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// Sort list of sequences
int TC_sort_sl(ClientData /* clientData */, Tcl_Interp * interp,
		           int /*argc*/, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// starts external algorithm
int TC_startalg(ClientData /* clientData */, Tcl_Interp *interp,
		            int /*argc*/, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// returns 1, if the actual problem is solvable
int TC_problem_reduction(ClientData /* clientData */, Tcl_Interp *interp,
			                   int /*argc*/, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// clear list of schedules
int TC_exclude(ClientData /* clientData */, Tcl_Interp *interp,
		           int argc, TCL_HACK_CHAR *argv[]);

//**************************************************************************

/// save G_Preferences in default.lsa
int TC_save_options(ClientData,	Tcl_Interp *, int , TCL_HACK_CHAR **);

//**************************************************************************

///send <ctrl-C> Signal to external Algorithm
int TC_stop_extalg(ClientData,	Tcl_Interp *, int , TCL_HACK_CHAR **);

//**************************************************************************

/// a TCL wrapper for the UNIX kill command
int TC_kill(ClientData /* clientData */, Tcl_Interp * /*interp*/,
	          int /*argc*/, TCL_HACK_CHAR *argv[]);

//**************************************************************************

#endif

