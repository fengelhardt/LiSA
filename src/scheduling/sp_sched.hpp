/*
 * ************** sp_sched.cpp *******************************
 * 
 * description:  basic class for shop schedules used in fast algorithms
 *
 * @author        Thomas Tautenhahn
 *
 * date:         11.12.1998
 *
 * 
 * history:      26. 10. 98 inline functions, more objective functions,
 *                          documentation added 
 *                4. 11. 98 short replaced by int
 *               11. 12. 98 plan replaced by schedule, own functionality
*/

#ifndef _sp_sched_h
#define _sp_sched_h

#include "../main/global.hpp"
#include "../basics/matrix.hpp"
#include "../lisa/lvalues.hpp"
#include "../lisa/lsaobjct.hpp"

const int SOURCE=0;
const int SINK=0;

#define CYCLE 0                                  
#define PERFEKT 2                                
#define ERROR -1

/** @name shop schedules
    Special implementation of schedules for schop problems based on the block 
    matrices modell. Can represent only full schedules on (partial) operation 
    sets. These schedules can be used only in connection with a Lisa_ShpProblem
    instance. */
//@{ 

/** class for representing an instance of a shop problem.
    This class allows fast access with Lisa_ShpSchedule, but is restricted to 
    problems without repetion, preemption, precedence constraints or batching.
    WARNING: jobs and machines are internally indexed 1,2, ...
    @author Thomas Tautenhahn and Andreas Winkler
    @version 2.3pre3
    @see Lisa_ShpSchedule
*/
class Lisa_ShpProblem 
   { 
     public: /// processing times
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
	     /// needs a given instance of Lisa_Values to construct
             Lisa_ShpProblem(Lisa_Values * Pin);
             ~Lisa_ShpProblem();
   };
       
/** basic class for Lisa_OsSchedule and Lisa_JsSchedule. 
    Cannot be used explicitely. 
    @author Thomas Tautenhahn and Andreas Winkler
    @version 2.3pre3
    @see Lisa_OsSchedule 
    @see Lisa_JsSchedule
    @see Lisa_ShpProblem
*/ 
class Lisa_ShpSchedule: public Lisa_GenericObject 
  { 
    public:
       virtual ~Lisa_ShpSchedule() {}			  
       /// pointer to the problem instance
       Lisa_ShpProblem * P;
       /// exclude operation from schedule 
       virtual void    exclude(int,int) = 0;
       /// test whether operation belongs to schedule
       bool exists(int,int); 
       /// several get-functions, no need to explain ...
       int GetJOsucc(int i,int j) { return (*JOsucc)[i][j]; } 	
       int GetMOsucc(int i,int j) { return (*MOsucc)[i][j]; } 
       int GetJOpred(int i,int j) { return (*JOpred)[i][j]; } 
       int GetMOpred(int i,int j) { return (*MOpred)[i][j]; }
       TIMETYP GetHead(int i,int j) { return (*head)[i][j]; }
       TIMETYP GetTail(int i,int j) { return (*tail)[i][j]; }
       /// get objective function value 
       TIMETYP GetValue() { return value; }
       /// compute objective function value
       void    SetValue(int);       
       /** switch computation of heads/tails on or off for all 
           following insert and exclude operations. ONE OF THE 
           PARAMETERS HAS TO BE TRUE! */   
       void    ComputeHeadsTails(bool, bool);
       /// shift all operations to latest possible times 
       void    HeadsFromTails(void); 
       /// shift all operations to earliest possible times
       void    TailsFromHeads(void);
       /// own print function for debugging
       void write(std::ostream& strm) const; 
       /// writes itself as LR in given Lisa_Matrix
       void    write_LR(Lisa_Matrix<int> *) const; 
       /// increase starting time of operation 
       void pushstart(int, int, TIMETYP);
       /** force operation to be completed at MAXTIME-x (Warning: this may 
           push other operations before their release date!) **/
       void pushstop(int, int, TIMETYP);        
    protected:
       Lisa_Matrix<int> * JOsucc;
       Lisa_Matrix<int> * JOpred;
       Lisa_Matrix<int> * MOsucc;
       Lisa_Matrix<int> * MOpred;
       Lisa_Matrix<TIMETYP> * head;
       Lisa_Matrix<TIMETYP> * tail;
       TIMETYP value; 
       bool  ScheduleOK;
       bool  ComputeTails, ComputeHeads;
       int   lastinsi, lastinsj;
       int   pushhead(int, int, TIMETYP);
       int   pushtail(int, int, TIMETYP);
       int   pullhead(int, int);
       int   pulltail(int, int);
       int   sti, stj, started;
       void    push_LR(Lisa_Matrix<int>*,int,int,int) const;
  };


  //@Include: os_sched.hpp
  //@Include: js_sched.hpp
//@}
#endif

