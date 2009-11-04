
#ifndef _js_plan_h
#define _js_plan_h

#include <stdio.h>
#include "sp_sched.hpp"

/// special problem class for job shop problems
/** it numerates from 1
    @author Thomas Tautenhahn, Andreas Winkler
    @version 3.0pre1
    @see Lisa_JsSchedule
 */
class Lisa_JsProblem : public Lisa_ShpProblem{ 
public:
  /// successors in machine orders
  Lisa_Matrix<int> * MOsucc;	
  /// machine orders as predecessor lists
  Lisa_Matrix<int> * MOpred;	      
  /// construct Lisa_JsProblem with a specified Lisa_Values
  Lisa_JsProblem( const Lisa_Values *Pin );
  ///destructor
  ~Lisa_JsProblem();
};
       
/// special schedule class for job shop problems
/** it numerates from 1
    @author Thomas Tautenhahn, Andreas Winkler
    @version 3.0pre1
    @see Lisa_JsProblem
 */
class Lisa_JsSchedule  : public Lisa_ShpSchedule{ 
public:
  /// pointer to corresponding problem instance
  const Lisa_JsProblem * JSP;
  /// construct Lisa_JsSchedule with the specified problem
  Lisa_JsSchedule(const Lisa_JsProblem *); 
  /// compute heads and tails for empty schedule
  void  InitHeadsTails();
  /// insert(i,j,k) inserts (i,j) after (k,j)
  /** return value is OK, PERFEKT (if no other operation is moved) or 
      CYCLE (schedule is destroyed) */
  int   insert(int i,int j,int k);
  /// exclude the operation (i,j) from the schedule
  void    exclude(int i,int j);
  /// assignment operator between schedules
  void operator=(Lisa_JsSchedule&);  
  /// fills itself according to given LR (no LR testing)
  int read_LR(Lisa_Matrix<int> *);
  /// fills itself according to given C_ij
  int read_Cij(Lisa_Matrix<TIMETYP> *);
  /// destructor    
  ~Lisa_JsSchedule();
};

#endif

