/*
 * ***************** tcl_c.hpp ******************************
 * 
 * description:        functions which was called as tcl/tk command
 *
 * owner:              Per Willenius
 *
 * date:               28.9.1999
 *
 * version:            V 1.0
*/ 

/** @name TCL/TK Commands

    contains functions, which can be called by the TCL/TK interpreter

    @author Per Willenius
    @version 2.3pre1
*/ 

//@{ 
#ifndef _tcl_c_h 
#define _tcl_c_h
 
//  **************  System Includes  ******************
#include <tk.h>

//  **************  Function Definitions ******************

/// write MO as Matrix in Lisa_MO
int TC_adopt_MO(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, char *argv[]);  

/// write Lisa_MO in Matrix in G_TclVar
int TC_make_MO_Matrix(ClientData,
		      Tcl_Interp *,
		      int , char **);

/// read file
int TC_open(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, char *argv[]);  

/// read only the schedule
int TC_open_schedule(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, char *argv[]);  

/// write file
int TC_save(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);  

/// generate randomly processing time matrix with parameters min,max,step,seed1,seed2
int TC_genpt(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, char *argv[]); 

/// generate randomly column with parameters min,max,step,seed1,name
int TC_gen_column(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]); 

/// generate randomly Lisa_Schedule.LR
int TC_gen_seq(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// generate randomly Lisa_Values.SIJ
int TC_gen_sij(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// generate randomly Lisa_Values.MO
int TC_gen_mo(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// reverse  Lisa_Schedule.LR
int TC_rev_seq(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]); 

/// update a schedule
int TC_update_seq(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);  

/// update  C-matrix
int TC_update_cij(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]); 

/// refresh the output
int TC_draw_output(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]); 


/// Draw Value Window
int TC_draw_dat(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// Update an entry in the value Window
int TC_draw_dat_entry(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// Mark an entry in the Value Window
int TC_mark_value_entry(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// Draw Schedule Window
int TC_draw_schedule(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// Draw List of Schedules
int TC_draw_schedule_list(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// Update an entry in the schedule Window
int TC_draw_schedule_entry(ClientData clientData,
			   Tcl_Interp *interp,
			   int argc, char *argv[]);

/// If new ProblemType defined
int TC_new_problem(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// If new Lisa_Values defined
int TC_new_values(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);

/// Mark an entry in the Schedule Window
int TC_mark_schedule_entry(ClientData clientData,
	      Tcl_Interp *interp,
	      int argc, char *argv[]);
// call the extern branch and bound routine
// with parameter: 
// number of solutions:no_of_solutions 
// insertion order: ins_order 
// type of lower bound lower_bound 
// upper bound: upper_bound
// int TC_start_bb(ClientData clientData,
// 	    Tcl_Interp *interp,
// 	    int argc, char *argv[]);

// call the extern neigborhood-search module
// int TC_start_nb(ClientData clientData,
// 	    Tcl_Interp *interp,
// 	    int argc, char *argv[]);

/// control the end of the Lisa-Programm
int TC_exit(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, char *argv[]);

/// Problem Classification
int TC_classify(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, char *argv[]);

/// write Bibtex reference for the corresponding classification
int TC_references(ClientData clientData,
	    Tcl_Interp *interp,
	    int argc, char *argv[]);

/// If an error in the Tk- Application occour
int TC_error(ClientData clientData,
	    Tcl_Interp *interp,
		int argc, char *argv[]);

/// Delete all global objects
int TC_new(ClientData clientData,
	    Tcl_Interp *interp,
		int argc, char *argv[]);

/// write G_Schedule.LR in G_TCLVar.LR
int  TC_save_old_sequence(ClientData clientData,
			  Tcl_Interp *interp,
			  int argc, char *argv[]);

/// move operation
int TC_move_operation(ClientData /* clientData */,
	   Tcl_Interp * /*interp*/,
	   int argc, char *argv[]);

/// Choose new Sequence
int TC_choose_sequence(ClientData /* clientData */,
		      Tcl_Interp * interp,
		      int /*argc*/, char *argv[]);

/// Sort list of sequences
int TC_sort_sl(ClientData /* clientData */,
		      Tcl_Interp * interp,
		      int /*argc*/, char *argv[]);

/// starts external algorithm
int TC_startalg(ClientData /* clientData */,
		Tcl_Interp *interp,
		int /*argc*/, char *argv[]);

/// returns 1, if the actual problem is solvable
int TC_problem_reduction(ClientData /* clientData */,
			 Tcl_Interp *interp,
			 int /*argc*/, char *argv[]);


/// 
int TC_exclude(ClientData /* clientData */,
		Tcl_Interp *interp,
		int argc, char *argv[]);

/// save Lisa_Preferences in default.lsa
int TC_save_options(ClientData,	Tcl_Interp *, int , char **);

///send <ctrl-C> Signal to external Algorithm
int TC_stop_extalg(ClientData,	Tcl_Interp *, int , char **);

// a TCL wrapper for the UNIX kill command
int TC_kill(ClientData /* clientData */,
	 Tcl_Interp * /*interp*/,
	 int /*argc*/, char *argv[]);

#endif

//@}

