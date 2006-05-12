/*
*  ba_env.h
*  beamaco
*
*  Created by Per Willenius on 03.04.06.
*  Copyright 2006 __MyCompanyName__. All rights reserved.
*
*/


#ifndef _lisa_ba_env_h_
#define _lisa_ba_env_h_

#include <iostream>

#include "../../basics/list.hpp"
#include "../../basics/order.hpp"

//**************************************************************************

/// flag to store whether current abort signal (in unix SIGINT) was received
extern bool abort_algorithm;

//**************************************************************************

/// insertion order flag for bb
enum{RANDOM_ORDER=0,LPT_ORDER};

//**************************************************************************

/// abort signal catching method
void set_abort(int i); 

//**************************************************************************

/// Basic Class for Beam ACO Procedures.
/** Provides some environment for a branch and bound algorithm to run like 
list of results, step counters, control output and so on.
@author Thomas Tautenhahn
@version 2.3final
*/
class Lisa_GenericBeamACO {
protected:
	//@{ 
	/// upper bounds for optimal objective value:
	TIMETYP upper_bound;
	TIMETYP given_upper_bound;
	TIMETYP given_lower_bound;
	//@}
	/// upper bound for number of optimal schedules in output:
	int output_bound;
	/// the insertion order:
	Lisa_Order * order;
	/// which kind of insertion order do we use?
	int ins_order_type;
	/// number of operations to insert:
	int number_ops;
	/// pointer to access the result list given by user:
	Lisa_List<Lisa_Matrix<int> * > * my_list;
	/// objective function 
	int zfn_type;
	/// some statistics
	void count_steps();
	/// soma statistics
	long steps_ok, steps;
	/// call at beginning of run to set signal and exception handling
	void run_start();
	/// call at end of run
	void run_stop();   
public:   
	/// constructor
	Lisa_GenericBeamACO();
	/// impose upper bound on number of schedules in output (default 30000)
	void inline set_output_bound(int nr) { output_bound=nr; }
	/// feed the algorithm an upper bound obtained elsewhere:
	void inline set_upper_bound(TIMETYP bound) { given_upper_bound=bound; }
	/// feed the algorithm an lower bound obtained elsewhere:
	void inline set_lower_bound(TIMETYP bound) { given_lower_bound=bound; }
	/// get number of successfull insertions in last run:
	long inline get_nr_insert_ok() { return steps_ok; }
	/// get total number of insertions in last run:
	long inline get_nr_insert() { return steps; }
	/// set insertion order:
	void inline set_ins_order(int ord){ ins_order_type=ord; }  
};            

//**************************************************************************

#endif

