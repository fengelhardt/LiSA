
#ifndef _osplan_h
#define _osplan_h

#include <stdio.h>
#include <iostream>

#include "sp_sched.hpp"

/// class for representing an instance of open shop
/** To be used by Lisa_OsSchedule.
    Warning: jobs and machines are internally numbered 1,2,...
    
    @author Thomas Tautenhahn, Andreas Winkler
    @version 2.3pre3
  */
class Lisa_OsProblem : public Lisa_ShpProblem{ 
public:
  /// construct Lisa_OsProblem with a specified Lisa_Values
  Lisa_OsProblem( Lisa_Values *Pin );
};

//**************************************************************************
       
/// special schedule class for open shop problems
/**
    @author Thomas Tautenhahn, Andreas Winkler
    @version 2.3pre3
    @see Lisa_OsProblem
  */
class Lisa_OsSchedule  : public Lisa_ShpSchedule{ 
public:

  /// pointer to corresponding problem instance 
  Lisa_OsProblem* OSP;
  
  /// construct empty schedule for given problem instance
  Lisa_OsSchedule(Lisa_OsProblem* other);
    
  /// insert(i,j,woi,woj) inserts (i,j) after (woi,j) and (i,woj)
  /** respectively, return value is OK, PERFEKT (if no other operation is 
      moved) or CYCLE (schedule is destroyed) */
  int   insert(int i,int j,int woi,int woj);
  
  /// exclude the operation (i,j) from the schedule
  void    exclude(int i,int j);
  
  /// set all orders according to a given LR
  /** warning: this does not test input for validity */ 
  int     read_LR(Lisa_Matrix<int> * lr);
  
  /// fills itself according to given C_ij
  int read_Cij(Lisa_Matrix<TIMETYP> * cij);   

  /// assignment operator
  void operator=(Lisa_OsSchedule& other);

  /// destructor     
  ~Lisa_OsSchedule();
};

#endif

