/*
 * ************** dispatch.hpp *******************************
 * 
 * priority rules for jobshop
 *
 * @author Thomas Tautenhahn
 *
 * 10.9.1999
*/

#ifndef _dispatch_h
#define _dispatch_h

#include "../../scheduling/js_sched.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"

const int NUMBER_RULES=9;
const int RAND=0;
const int FCFS=1;
const int EDD=2;
const int LQUE=3;
const int SPT=4;
const int WSPT=5;
const int ECT=6;
const int WI=7;
const int LPT=8;

/// some strings to describe the dispatching rules
const char RULE_NAMES[NUMBER_RULES][5]= { "RAND","FCFS","EDD","LQUE","SPT",
					  "WSPT","ECT","WI","LPT"};

/** Priority dispatching rules for different problem types. The output of the 
    algorithm for semiactive schedules is only written as an LR!

    @author Thomas Tautenhahn
    @version 2.3pre3
    @see Lisa_List
    @see Lisa_GenericList */ 
class Lisa_Dispatcher
  { 	
    private: 
      int rule, n, m;
      long myseed;
      int my_problem;
      Lisa_Vector<TIMETYP> * jstart;
      Lisa_Vector<TIMETYP> * mstart;
      Lisa_ShpProblem * problem;
      Lisa_ShpSchedule * schedule;
      TIMETYP priority(int, int); 
      Lisa_Schedule * LSchedule;
      void os_dispatch_active();
      void js_dispatch_active();
      void os_dispatch();
      void js_dispatch();
   
    public:
      Lisa_Dispatcher();
      /// setting up problem and target schedule (needed to run the algorithm)
      bool SetProblem(Lisa_ProblemType*, Lisa_Values*, Lisa_Schedule*);
      /// chooses the priority rule (default SPT)
      void SetRule(string r);
      /** At each time a machine or job becomes available, schedule the 
          operation with the highest priority among the available ones. The 
          resulting schedule will be active. **/ 
      void dispatch_active();
      /** Among all unscheduled operations (with no unscheduled predecessors), 
          schedule one with the highest priority. The resulting schedule is 
          semiactive, but in general not active. **/
      void dispatch();
      ~Lisa_Dispatcher();  
   };
      
Lisa_JsSchedule * dispatch_active(Lisa_JsProblem * Pi, int rule);
      
#endif


















