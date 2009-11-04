
#ifndef _os_bb_h
#define _os_bb_h

#include "../../scheduling/os_sched.hpp"
#include "bb_env.hpp"


/// branch and bound implementation for open shop
/** @author Thomas Tautenhahn
    @version 3.0pre1
 */
class OS_BB: public Lisa_GenericBb{
private:
  /// do we exclude reversed schedules ?
  bool exclude_reverse;
  /// the real recursion, stripped of unnecessary parameters
  void _run(int);
  /// the schedule to work on:
  Lisa_OsProblem * P;
  /// the second schedule used as a backup copy:
  Lisa_OsSchedule * Schedule;  
public:  
  /// constructor
  OS_BB();
  /// exclude reverse schedules (only for O||C_max)
  void inline set_exclude_reverse(bool i) { exclude_reverse=i; }  
  /// start of the recursion
  void run(Lisa_OsProblem * Pi, int zfn, Lisa_List<Lisa_Matrix<int>*> *results);
};           
      
#endif

