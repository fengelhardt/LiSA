
#ifndef _m1_sched_h
#define _m1_sched_h

#include <stdlib.h>

#include "../main/global.hpp"
#include "../basics/matrix.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "schedule.hpp"
#include "sp_sched.hpp"


/// problem data class for single machine problems
/** This is the problem dependent schedule class for single machine problems.
    WARNING: jobs are internally indexed 1,2,3, ...
    @see Lisa_1Schedule
    @author Andreas Winkler
    @version 2.3rc1
*/  
class Lisa_1Problem {
public: 

  //@{
  ///problem data
  Lisa_Vector<TIMETYP> *time, *di, *ri;
  Lisa_Vector<float>   *wi;
  //@}
  
  /// adjacence-matrix of precedence-graph
  /** prec[i][j] means, that job i must be completed before job j can start */
  Lisa_Matrix<int> *prec;
  
  /// size, number of operations
  int n;
  
  /// consturctor
  /** construct Lisa_SingleMProblem with specified dimension 
  and a arbitrary param to construct start values */
  Lisa_1Problem(Lisa_Values* Pin);
  
  /// destructor
  ~Lisa_1Problem();
};



/// schedule for single machine problem,
/** WARNING: the numeration is here beginning at 0 
    @see Lisa_1Problem
    @author Andreas Winkler
    @version 2.3rc1
*/
class Lisa_1Schedule{
private: 

  // objective value
  TIMETYP  value;

  /// successor for each operation 
  Lisa_Vector<int> *JOsucc;
  /// predecessor for each operation
  Lisa_Vector<int> *JOpred;
  /// head for each operation
  Lisa_Vector<TIMETYP> * head;
  /// tail for each operation
  Lisa_Vector<TIMETYP> * tail;
  /// compute heads/tails ?
  bool  ComputeTails, ComputeHeads;
  
  /// internal flags
  /** needs documentation */
  int   sti, started;

  //@{
  /// operations on heads/tails
  /** needs documentation */
  int pushhead(int i, TIMETYP newhead);
  int pushtail(int i, TIMETYP newtail);
  int pullhead(int i);
  int pulltail(int i);
  //@}
  
public:
  /// completion times for each operation
  Lisa_Vector<TIMETYP>    *Ci;
  
  /// Pointer to the problem instance
  Lisa_1Problem *PP;

  /// constructor
  /** create Lisa_1Schedule with specified problem data */
  Lisa_1Schedule(Lisa_1Problem* PPi);
  
  /// destructor
  ~Lisa_1Schedule();
  
  /// get successor of specified operation
  int inline GetJOsucc(int i) { return (*JOsucc)[i]; }
  /// get predecessor of specified operation 	
  int inline GetJOpred(int i) { return (*JOpred)[i]; }
  
  /// returns head for a given oeration
  TIMETYP inline GetHead(int i) { return (*head)[i]; }
  /// returns tail for a given operation
  TIMETYP inline GetTail(int i) { return (*tail)[i]; }

  /// switch computation of heads/tails on or off 
  /** for all following insert and exclude operations. ONE OF THE PARAMETERS 
      HAS TO BE TRUE! */ 
  void ComputeHeadsTails(bool heads, bool tails);
  
  /// test whether operation belongs to schedule
  bool exists(int i);
  
  /// insert(i,j) inserts job i after after job k on the machine
  /** return value is OK, PERFEKT (if no other operation is moved) */
  int insert(int i, int woi);
  /// exclude job from schedule
  void exclude(int i);
  /// sets the specified objectiv function
  int SetValue(int ZF);
  /// returns the objective value
  TIMETYP  GetValue();
  /// shift( pos1, pos2 ) shifts job at pos1 to pos2
  int shift( int a, int b);
  /// returns the operation on the specified position
  int get_sequ(int i);
  ///assignment operator
  void operator = (Lisa_1Schedule &other);
  /// the sequence of the jobs
  Lisa_Vector<int>  *sequ;
  /// determs the Ci from specified number to n
  void get_Ci(int pos);
  /// writes the current sequence to sequ
  void write_sequ();
  /// own print function for debugging
  void print(void);
};

#endif

