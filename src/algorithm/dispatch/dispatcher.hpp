
#ifndef _dispatch_h
#define _dispatch_h

#include "../../scheduling/js_sched.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"

/// Priority dispatching rules for different problem types.
/** The output of the algorithm for semiactive schedules is only written as a 
    LR!

    @author Thomas Tautenhahn
    @version 3.0
    @see Lisa_List
 */ 
class Lisa_Dispatcher{ 	

public:
  
  //******************************************************************************
  // put this here to avoid name clashes
  
/// which priority rules are implemented + how many are implemented
  enum{RAND=0,FCFS,EDD,LQUE,SPT,WSPT,ECT,WI,LPT
       ,NUMBER_RULES /// number of rules
  };
  
  //******************************************************************************
  
    /// some strings to describe the dispatching rules
    static const char* const RULE_NAMES[NUMBER_RULES];// = { "RAND","FCFS","EDD","LQUE","SPT","WSPT","ECT","WI","LPT"};
  
  //******************************************************************************
  
  
private:
  /// by which rule are we dispatching ? 
  int rule;
  /// problem size
  int n, m;
  /// seed for random number generation
  long myseed;
  /// machine enviroment
  /** open shop or job shop ? */
  int my_problem;
  
  /// when is a job free again
  Lisa_Vector<TIMETYP> * jstart;
  /// when is a machine free again ?
  Lisa_Vector<TIMETYP> * mstart;
  
  /// returns ECT of unscheduled operation (i,j) 
  TIMETYP /*Lisa_Dispatcher::*/getECT(int i, int j);

  /// returns earliest start time of unscheduled operation (i,j) 
  TIMETYP /*Lisa_Dispatcher::*/getEST(int i, int j);

  /// returns earliest ECT of any unscheduled operation
  TIMETYP /*Lisa_Dispatcher::*/getEECT();

  /// returns priority for operation (i,j) according to current rule
  TIMETYP priority(int i, int j); 
  
  /// pointer to schedule given by SetProblem()
  Lisa_Schedule * LSchedule;
  /// internal object holding problem data
  Lisa_ShpProblem * problem;
  /// internal object holding schedule for problem
  Lisa_ShpSchedule * schedule;
  
  /// internal nondelay schedule dispatcher for open shop
  void os_dispatch_nondelay();
  /// internal nondelay schedule dispatcher for job shop
  void js_dispatch_nondelay();
  /// internal active schedule dispatcher for open shop
  void os_dispatch_active();
  /// internal active schedule dispatcher for job shop
  void js_dispatch_active();
  /// internal semiactive schedule dispatcher for open shop  
  void os_dispatch();
  /// internal semiactive schedule dispatcher for job shop  
  void js_dispatch();
   
public:
  /// constructor
  Lisa_Dispatcher();
  /// destructor
  ~Lisa_Dispatcher();
  
  /// setting up problem and target schedule (needed to run the algorithm)
  bool SetProblem(const Lisa_ProblemType* pt, const Lisa_Values* val, Lisa_Schedule* sched);
  /// chooses the priority rule (default SPT)
  void SetRule(std::string rule);
  
  /// non-delay schedule dispatcher
  /** At each time a machine or job becomes available, schedule the operation 
      with the highest priority among the available ones. The resulting 
      schedule will be active. */ 
  void dispatch_nondelay();

  /// active schedule dispatcher
  /** Among all unscheduled operations which can start before the nect ECT 
      operation would complete, schedule the one with highest priority. */
  void dispatch_active();
  
  /// semiactive schedule dispatcher
  /** Among all unscheduled operations (with no unscheduled predecessors), 
      schedule one with the highest priority. The resulting schedule is 
      semiactive, but in general not active. */
  void dispatch();

};

//******************************************************************************
      
#endif

