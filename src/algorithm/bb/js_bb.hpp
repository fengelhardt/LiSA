/*
 * ************** js_bb.h *******************************
 * 
 * branch and bound implementation for job shop
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
*/

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _js_bb_h
#define _js_bb_h

#include"bb_env.hpp"
#include"../../lisa_dt/schedule/js_sched.hpp"

class JS_BB: public Lisa_GenericBb
  {
    public:  
      /// start of the recursion
      void run(Lisa_JsProblem * Pi, int zfn, 
               Lisa_List<Lisa_Matrix<int>*> *results);
    private: 
      // the real recursion, stripped of unnecessary parameters
      void _run(int);
      // the schedule to work on:
      Lisa_JsProblem * P;
      // the second schedule used as a backup copy:
      Lisa_JsSchedule * Schedule;  
  };           
      
#endif


















