
#ifndef _sp_sched_h
#define _sp_sched_h

#include "../main/global.hpp"
#include "../basics/matrix.hpp"
#include "../lisa/lvalues.hpp"
#include "../lisa/lsaobjct.hpp"

//@{
/// flag, needs documentation
const int SOURCE=0;
const int SINK=0;
const int CYCLE=0;                                
const int PERFEKT=2;                                
const int ERROR=-1;
//@}

/// class representing an instance of a shop problem
/** This class allows fast access with Lisa_ShpSchedule, but is restricted to 
    problems without repetion, preemption, precedence constraints or batching.
    
    WARNING: jobs and machines are internally indexed 1,2,...
    @author Thomas Tautenhahn, Andreas Winkler
    @version 2.3pre3
    @see Lisa_ShpSchedule
*/
class Lisa_ShpProblem { 
public: 
  /// processing times
  Lisa_Matrix<TIMETYP> *time;
  /// operation set
  Lisa_Matrix<bool> *sij;
  /// release times
  Lisa_Vector<TIMETYP> *ri;
  /// due dates / deadlines
  Lisa_Vector<TIMETYP> *di;
  /// weights to be used in the objective function
  Lisa_Vector<float>   *wi;
  /// secondary weights for some objective functions
  Lisa_Vector<float>   *wi_early;
  /// additional weights for maximum earlyness / lateness
  float w_early, w_late;
  /// number of jobs and machines
  int n,m;
  /// constructor
  /** needs a given instance of Lisa_Values to construct */
  Lisa_ShpProblem(Lisa_Values * Pin);
  /// destructor
  ~Lisa_ShpProblem();
};
       
/// basic class and interface for Lisa_OsSchedule and Lisa_JsSchedule 
/**
    @author Thomas Tautenhahn and Andreas Winkler
    @version 2.3pre3
    @see Lisa_OsSchedule 
    @see Lisa_JsSchedule
    @see Lisa_ShpProblem
*/ 
class Lisa_ShpSchedule: public Lisa_GenericObject { 
protected:
  /// successor for each operation in job order
  Lisa_Matrix<int> * JOsucc;
  /// predecessor for each operation in job order
  Lisa_Matrix<int> * JOpred;
  /// successor for each operation in machine order
  Lisa_Matrix<int> * MOsucc;
  /// predecessor for each oderation in machine order
  Lisa_Matrix<int> * MOpred;
  /// heads for alle operations
  Lisa_Matrix<TIMETYP> * head;
  /// tails for all operations
  Lisa_Matrix<TIMETYP> * tail;
  /// objective value
  TIMETYP value;
  /// compute heads or tails ?
  bool  ComputeTails, ComputeHeads;
  //@{
  /// private data, needs documentation
  bool  ScheduleOK;
  int   lastinsi, lastinsj;
  int   sti, stj, started;
  //@}
  
  //@{
  /// private function, needs documentation
  int   pushhead(int i, int j, TIMETYP time);
  int   pushtail(int i, int j, TIMETYP time);
  int   pullhead(int i, int j);
  int   pulltail(int i, int j);
  void  push_LR(Lisa_Matrix<int>* LR,int i,int j,int j) const;
  //@}
public:
  /// destructor
  virtual ~Lisa_ShpSchedule() {}			  
  /// pointer to the problem instance
  Lisa_ShpProblem * P;
  /// exclude operation from schedule 
  virtual void exclude(int i,int j) = 0;
  /// test whether operation belongs to schedule
  bool exists(int i,int j); 
  /// get successor of operation (i,j) in job order
  int inline GetJOsucc(int i,int j) { return (*JOsucc)[i][j]; } 
	/// get successor of operation (i,j) in machine order
  int inline GetMOsucc(int i,int j) { return (*MOsucc)[i][j]; } 
  /// get predecessor of operation (i,j) in job order
  int inline GetJOpred(int i,int j) { return (*JOpred)[i][j]; } 
  /// get predecessor of operation (i,j) in job order
  int inline GetMOpred(int i,int j) { return (*MOpred)[i][j]; }
  /// get head of operation (i,j)
  TIMETYP inline GetHead(int i,int j) { return (*head)[i][j]; }
  /// get head of operation (i,j)
  TIMETYP inline GetTail(int i,int j) { return (*tail)[i][j]; }
  /// get objective function value 
  TIMETYP inline GetValue() { return value; }
  /// compute objective function value
  void    SetValue(int);       
  /// switch computation of heads/tails on or off 
  /** for all following insert and exclude operations. ONE OF THE PARAMETERS 
      HAS TO BE TRUE! */   
  void    ComputeHeadsTails(bool heads, bool tails);
  /// shift all operations to latest possible times 
  void    HeadsFromTails(); 
  /// shift all operations to earliest possible times
  void    TailsFromHeads();
  /// own print function for debugging
  void write(std::ostream& strm) const; 
  /// writes itself as LR in given Lisa_Matrix
  void write_LR(Lisa_Matrix<int> * LR) const; 
  /// increase starting time of operation 
  void pushstart(int i, int j, TIMETYP v);
  /// force operation to be completed at MAXTIME-x 
  /** Warning: this may push other operations before their release date! */
  void pushstop(int i, int j, TIMETYP v);        
};

#endif

