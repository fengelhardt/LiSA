/*
 * ************** os_bb.h *******************************
 * 
 * branch and bound implementation for open shop
 *
 * @author Thomas Tautenhahn
 * @version 2.3pre3
 *
 * 19.9.1998
*/

#ifndef _os_bb_h
#define _os_bb_h

#include "../../scheduling/os_sched.hpp"
#include "bb_env.hpp"

class OS_BB: public Lisa_GenericBb
  {
    public:  
      OS_BB();
      /// exclude reverse schedules (only for O||C_max)
      void set_exclude_reverse(bool i) { exclude_reverse=i; }  
      /// start of the recursion
      void run(Lisa_OsProblem * Pi, int zfn, 
               Lisa_List<Lisa_Matrix<int>*> *results);
    private: 
      bool exclude_reverse;
      // the real recursion, stripped of unnecessary parameters
      void _run(int);
      // the schedule to work on:
      Lisa_OsProblem * P;
      // the second schedule used as a backup copy:
      Lisa_OsSchedule * Schedule;  
  };           
      
#endif


















