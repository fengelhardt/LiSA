/*
 * ************** js_plan.h *******************************
 * 
 * inherit for a schedule for the job shop problem
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
*/

/** @name class Lisa_JsSchedule and class Lisa_JsProblem 
    These are the general problem and schedule classes for the 
    job shop problem.
    
    @author Thomas Tautenhahn and Andreas Winkler
    @version 2.3pre1
    @see Lisa_ShpSchedule
*/  

//@{

#ifndef __cplusplus
#error Must use C++ for these types.
#endif

#ifndef _js_plan_h
#define _js_plan_h

#include<stdio.h>
#include"sp_sched.hpp"

/// special problem class for job shop problems, it numerated from 1
class Lisa_JsProblem : public Lisa_ShpProblem
    { 
    public:
      /// successors in machine orders
      Lisa_Matrix<int> * MOsucc;	
      /// machine orders as predecessor lists
      Lisa_Matrix<int> * MOpred;	      
      /// construct Lisa_OsProblem with a specified Lisa_Values
      Lisa_JsProblem( Lisa_Values *Pin );
      ~Lisa_JsProblem();
    };
       
/// special schedule class for job shop problems
class Lisa_JsSchedule  : public Lisa_ShpSchedule
           { 
              public:
               /// pointer to corresponding problem instance
               Lisa_JsProblem * JSP;
	       /// construct Lisa_OsSchedule with the specified problem
               Lisa_JsSchedule(Lisa_JsProblem *); 
	       /// compute heads and tails for empty schedule
	       void  InitHeadsTails();
	       /** insert(i,j,k) inserts (i,j) after (k,j), return value is
	           OK, PERFEKT (if no other operation is moved) or 
	           CYCLE (schedule is destroyed) */
	       int   insert(int,int,int);
	       /// exclude the operation (i,j) from the schedule
	       void    exclude(int,int);
               /// assignment between schedules
               void    operator=(Lisa_JsSchedule&);  
               /// fills itself according to given LR (no LR testing)
               int read_LR(Lisa_Matrix<int> *);
               /// fills itself according to given C_ij
               int read_Cij(Lisa_Matrix<TIMETYP> *);    
               ~Lisa_JsSchedule();
	    };

#endif

//@}







