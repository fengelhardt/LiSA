
#ifndef _callback_h 
#define _callback_h 

#include "../scheduling/os_sched.hpp"
#include "../main/global.hpp"

/// to be called when Lisa_ProblemType has changed
/** @author Per Willenius
    @version 2.3final
 */
void new_problemtype();

/// to be called if no valid schedule exist 
void no_schedule();

/// to be called if no valid values exist
void no_values();

/// to be called if the Lisa_Schedule has changed
void new_schedule();

/// to be called if Lisa_Values has changed
void new_values();

/// to be called if the number of machines or jobs has changed
void new_mn();

/// to be called if the number of maschines (or  jobs) is <= 0
void no_mn();

/// refresh the main window
void show_output();

/// construct new objective value
TIMETYP update_objective(Lisa_OsSchedule &myOsSchedule );

/// update the latin rectangle
void update_LR();

/// generate MO with Flow Shop Order
void gen_fs( );

#endif

