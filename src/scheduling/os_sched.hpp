/*
 * ************** os_sched.h *******************************
 * 
 * inherit for a schedule for the open shop problem
 *
 * Owner: Thomas Tautenhahn
 *
 * 11.12.1998
*/

/** @name Schedules for Open Shop
    These the general problem and schedule classes for the 
    open shop problem.
    
    @author Thomas Tautenhahn and Andreas Winkler
    @version 2.3pre1
    @see Lisa_ShpSchedule
*/  

//@{

#ifndef __cplusplus
#error Must use C++ for these types.
#endif

#ifndef _osplan_h
#define _osplan_h

#include<stdio.h>
#include"sp_sched.hpp"
#include<iostream.h>

/** Class for representing an instance of open shop to be used by Lisa_OsSchedule.
    Warning: jobs and machines are internally numbered 1,2, ... */
class Lisa_OsProblem : public Lisa_ShpProblem
  { 
    public:
      /// construct Lisa_OsProblem with a specified Lisa_Values
      Lisa_OsProblem( Lisa_Values *Pin );
  };
       
/// special schedule class for open shop problems
class Lisa_OsSchedule  : public Lisa_ShpSchedule
  { 
    public:
      /// pointer to corresponding problem instance 
      Lisa_OsProblem * OSP;
      /// construct empty schedule for given problem instance
      Lisa_OsSchedule(Lisa_OsProblem *);    
      /** insert(i,j,k,l) inserts (i,j) after (k,j) and (i,l), 
          respectively, return value is OK, PERFEKT (if no other 
          operation is moved) or  CYCLE (schedule is destroyed)*/
      int   insert(int,int,int,int);
      /// exclude the operation (i,j) from the schedule
      void    exclude(int,int);
      /// set all orders according to a given LR (does not test input) 
      int     read_LR(Lisa_Matrix<int> * lr);
      /// fills itself according to given C_ij
      int read_Cij(Lisa_Matrix<TIMETYP> *);   
      void    operator=(Lisa_OsSchedule&);    
      ~Lisa_OsSchedule();
  };

#endif

//@}







