/*
 * ************** callback.hpp ******************************
 * 
 * description: to manage which functions are called, if anything has changed
 * 
 * 
 * @author Per Willenius
 *
 * date: 28.9.1999
 * version: V 1.0
*/ 

/** @name Callback Functions


    @author Per Willenius
    @version 2.3pre3
*/ 

//@{

#ifndef _callback_h 
#define _callback_h 

// To generate the C-Matrix and the objective function
#include "../scheduling/os_sched.hpp"
#include "../main/global.hpp"

//  ************** Functions  ******************

/** If the Lisa_ProblemType has changed  */
void new_problemtype();

/** if no valid schedule exist */
void no_schedule();

/** if no valid values exist */
void no_values();

/** If the Lisa_Schedule has changed  */
void new_schedule();

/** If the Lisa_Values has changed  */
void new_values();

/** If the number of maschines or jobs has changed  */
void new_mn();

/** If the number of maschines (or  jobs) is <= 0 */
void no_mn();

/** refresh the window */
void show_output();

/** construct new objective value */
TIMETYP update_objective(Lisa_OsSchedule &myOsSchedule );

/** updates the latin rectangle */
void update_LR();

/// generate MO with Flow Shop Order
void gen_fs( );

#endif

//@}



