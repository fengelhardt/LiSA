/*
 * ******************** m1_sched.hpp ******************************
 * 
 * description:      single machine problem and schedule class
 * 
 * owner:            Andreas Winkler
 *
 * date:             10.11.1998
 *
 */

/** @name class Lisa_1Schedule
    This is the problem dependent schedule class for single machine problems.
    WARNING: jobs are internally indexed 1,2,3, ...

    @author Andreas Winkler
    @version 2.3pre1
*/  

//@{

#ifndef __cplusplus

#error Must use C++ for these types.

#endif
#ifndef _m1_sched_h
#define _m1_sched_h

#include"../../basics/global.hpp"
#include"../../basics/matrix.hpp"
#include"../ptype.hpp"
#include"../schedule.hpp"
#include"../lvalues.hpp"
#include"sp_sched.hpp"
#include <stdlib.h>


/// problem data class for single machine problems
class Lisa_1Problem {
        public: 
                 Lisa_Vector<TIMETYP> *time, *di, *ri;
		 Lisa_Vector<float>   *wi;
		 /*  adjacence-matrix of precedence-graph
		     prec[i][j] means, that job i must be
		     take before job j  */
		 Lisa_Matrix<int> *prec;
		 int n;
		 /** construct Lisa_SingleMProblem with specified dimension 
		     and a arbitrary param to construct start values */
		 Lisa_1Problem( Lisa_Values *Pin );
		 ~Lisa_1Problem();
 };



/** schedule for single machine problem,\\
    attention: the numeration is here beginning by 0 */
class Lisa_1Schedule
  {
   public:  
	    Lisa_Vector<TIMETYP>    *Ci;
	    /// Pointer to the problem instance
	    Lisa_1Problem *PP;
	    int tabu_pos, tabu_pos_to;
	    /// construct Lisa_1Schedule with specified problem datas
	    Lisa_1Schedule( Lisa_1Problem* );
	    ~Lisa_1Schedule();
	    int GetJOsucc(int i) { return (*JOsucc)[i]; } 	
	    int GetJOpred(int i) { return (*JOpred)[i]; }
	    TIMETYP GetHead(int i) { return (*head)[i]; }
            TIMETYP GetTail(int i) { return (*tail)[i]; }
	    /** switch computation of heads/tails on or off for all 
                following insert and exclude operations. ONE OF THE 
                PARAMETERS HAS TO BE TRUE! */ 
	        void ComputeHeadsTails( bool, bool );
	    /// test whether operation belongs to schedule
 	    bool exists( int );
	    /** insert(i,j) inserts job i after after job kon the machine, 
                respectively, return value is OK, PERFEKT (if no other 
                operation is moved) */
	    int insert( int, int );
	    /// exclude job from schedule
	    void exclude( int );
	    /// sets the specified objectiv function
	    int SetValue( int );
	    /// returns the objective value
	    TIMETYP  GetValue();
	    /// shift( pos1, pos2 ) shifts job at pos1 to pos2
	    int shift( int, int );
	    /// returns the operation on the specified position
	    int get_sequ( int );
	    void operator = (Lisa_1Schedule &);
	    /// the sequence of the jobs
            Lisa_Vector<int>  *sequ;
	    /// determs the Ci from specified number to n
	    void get_Ci(int);
	    void write_sequ();
	    /// own print function for debugging
	    void print(void);
   private: TIMETYP  value;
	    Lisa_Vector<int> *JOsucc;
	    Lisa_Vector<int> *JOpred;
	    Lisa_Vector<TIMETYP> * head;
            Lisa_Vector<TIMETYP> * tail;
	    bool  ComputeTails, ComputeHeads;
	    int   sti, started;
	    int pushhead( int, TIMETYP );
	    int pushtail( int, TIMETYP );
	    int pullhead( int );
	    int pulltail( int );
  };

#endif

//@}
