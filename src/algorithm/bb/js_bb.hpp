
#ifndef _js_bb_h
#define _js_bb_h

#include "../../scheduling/js_sched.hpp"
#include "bb_env.hpp"

/// branch and bound implementation for job shop
/**
    @author Thomas Tautenhahn
    @version 2.3final
 */
class JS_BB: public Lisa_GenericBb{
private: 
  /// the real recursion, stripped of unnecessary parameters
  void _run(int);
  /// the schedule to work on:
  Lisa_JsProblem * P;
  /// the second schedule used as a backup copy:
  Lisa_JsSchedule * Schedule;    
public:  
  /// start of the recursion
  void run(Lisa_JsProblem * Pi, int zfn, Lisa_List<Lisa_Matrix<int>*> *results);
};           
      
#endif

