/*
 * ************** bb_env.h *******************************
 * 
 * common features of the Lisa_*Bb classes 
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
*/

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _bb_env_h
#define _bb_env_h

#include <iostream>

using namespace std;

#include "../../basics/list.hpp"
#include "../../basics/order.hpp"


/// flag to store whether current abort signal (in unix SIGINT) was received
extern bool abort_algorithm;

const int RANDOM_ORDER=0;
const int LPT_ORDER=1;

void set_abort(int i); 

/** Basic Class for Branch and Bound Procedures.
    Provides some environment for a branch and bound algorithm to run like 
    list of results, step counters, control output and so on.
*/
class Lisa_GenericBb 
  {
    public:  
      Lisa_GenericBb();
      /// impose upper bound on number of schedules in output (default 30000)
      void set_output_bound(int nr) { output_bound=nr; }
      /// feed the algorithm an upper bound obtained elsewhere:
      void set_upper_bound(TIMETYP bound) { given_upper_bound=bound; }
      /// feed the algorithm an lower bound obtained elsewhere:
      void set_lower_bound(TIMETYP bound) { given_lower_bound=bound; }
      /// get number of successfull insertions in last run:
      long get_nr_insert_ok() { return steps_ok; }
      /// get total number of insertions in last run:
      long get_nr_insert() { return steps; }
      /// set insertion order:
      void set_ins_order(int ord){ ins_order_type=ord; }
    protected: 
      // upper bounds for optimal objective value:
      TIMETYP upper_bound;
      TIMETYP given_upper_bound;
      TIMETYP given_lower_bound;
      // upper bound for number of optimal schedules in output:
      int output_bound;
      // the insertion order:
      Lisa_Order * order;
      // which kind of insertion order do we use?
      int ins_order_type;
      // number of operations to insert:
      int number_ops;
      // pointer to access the result list given by user:
      Lisa_List<Lisa_Matrix<int> * > * my_list;
      int zfn_type;
      // some statistics:
      void count_steps();
      long steps_ok, steps;
      /// call at beginning of run to set signal and exception handling
      void run_start();
      /// call at end of run
      void run_stop();   
  };            
      
#endif
















