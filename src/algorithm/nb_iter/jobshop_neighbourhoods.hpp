
#ifndef lisa_jobshop_neighbourhoods_h
#define lisa_jobshop_neighbourhoods_h

#include "../../basics/order.hpp"
#include "../../scheduling/js_sched.hpp"
#include "neighbour.hpp"
#include "tabu.hpp"

//**************************************************************************

/// job shop API neighbourhood
/** This is the problem dependent API neighbourhood class for the job shop 
    problem (i.e. these neighbourhood swaps two arbitrary adjacent operations on
    a machine). It is inherited from the class Lisa_Neighbourhood.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_API_Ngbh: public Lisa_Neighbourhood{
protected:
  /// tabu list
  Lisa_Tabu *tabulist;
	/// copy source schedule to destination schedule	
  int copy_schedule(int source,int destination);
public:
  /// seed for random number generation
  long seed;
  /// underlying problem data
  Lisa_JsProblem *PP;
  /// swap where ?
  int witch_swap;
  /// machine to swap  
  int machine1;
  /// jobs to swap
  int job1, job2;
  /// needs documentation
  bool new_solution;
  /// tabu parameters 
  int tabu_param[2][4];
  /// the class contains 4 schedules:
  Lisa_JsSchedule  *P [4];
  
  /// construct OSHOP_API_Ngbh
  /** with a start schedule and specified problem datas   */
  JSHOP_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_API_Ngbh();
  
  void  set_objective_type( int );
  void  set_objective( int, int );
  TIMETYP get_objective_value( int );
  virtual int   accept_solution();
  virtual int   accept_best_ngh();
  virtual int   put_orig_to_best();
  virtual int   put_work_to_best_ngh();
  int   prepare_move(int);
  int   do_move();
  int   anti_neighbour();
  int   init_tabulist( unsigned int );
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
  
  /// return the generated schedule in the specified schedule
  void  return_schedule( Lisa_JsSchedule* );  
};

//**************************************************************************

/// job shop 3-time-critical-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    open shop problem, where we swap two adjacent operations on the critical 
    way and additional we swap also the direct predecence and the direct 
    sucsessive operations on the critical way.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_3_CR_Ngbh: public JSHOP_API_Ngbh{       
private: 
  /// private data, needs documentation
  Lisa_Order *ROrd;
  /// private data, needs documentation
  int   OrdCount;
  /// private data, needs documentation
	int   NewOrder;
  /// for the 3 proposed swaps
  /** for JO-swap:
	    - swaps[][0]=JO
			- swaps[][1] machine1
			- swaps[][2] job1
			- swaps[][3] job2
			- swaps[][4] OK ? */
  int   swaps[3][5];
  /// head and tail for a operation
	TIMETYP head, tail;
	/// machines
  int   machine2, machine3;
  /// job
	int   job3;
	/// basic problem data
  Lisa_JsSchedule  *temp_schedule;
public:	
	/// construct JSHOP_3_CR_Ngbh 
  /** with a start schedule and specified problem data */
	JSHOP_3_CR_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_3_CR_Ngbh();

	int   prepare_move(int);
	int   do_move();
	int   use_tabulist();
	int   set_tabulist();
	void clean_tabu_param();
};

//**************************************************************************

/// job shop 3-time-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    job shop problem where we take a swap of adjacent operatins on 3 different
    machines.
    It works only with tabusearch method !
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_3_API_Ngbh: public JSHOP_API_Ngbh{
private:
  /// for the 3 proposed swaps
  /** for JO-swap:
	    - swaps[][0]=JO
			- swaps[][1] machine1
			- swaps[][2] job1
			- swaps[][3] job2
			- swaps[][4] OK ? */
  int   swaps[3][5];
public:	
	/// construct JSHOP_3_API_Ngbh 
  /** with a start schedule and specified problem data */
	JSHOP_3_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );

	int   prepare_move(int);
	int   do_move();
	int   use_tabulist();
	int   set_tabulist();
};

//**************************************************************************

/// job shop critical-block-API neighbourhood
/** This is the problem dependent API neighbourhood class for the 
    open shop problem, where we take a swap only for a critical block 
    operation.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_bl_API_Ngbh: public JSHOP_API_Ngbh{
private:
  /// set of critical operations
  int   *cr_list_j;
  /// set of critical operations
  int   *cr_list_m;
  /// set of critical operations
  int   *direction;
  /// number of critical operations 
  int   count;      
public:	
  /// construct JSHOP_cr_bl_API_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_cr_bl_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_cr_bl_API_Ngbh();

  int   prepare_move(int);
  int   do_move();
};

//**************************************************************************

/// job shop critical-block-SHIFT neighbourhood
/** This is the problem dependent SHIFT neighbourhood class for the 
    job shop problem, where we take a swap only for a critical block 
    operation.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_bl_shift_Ngbh: public JSHOP_API_Ngbh{
private:
  /// set of critical operations
  int   *cr_list_j;
  /// set of critical operations
  int   *cr_list_m;
  /// set of critical operations
  int   *direction;
  /// number of critical operations
  int   count;
  /// where to swap ?  
  int   pos1, pos2;
  /// job order
  Lisa_Vector<int> *JOrd;
public:	
  /// construct JSHOP_cr_bl_shift_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_cr_bl_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor 
  ~JSHOP_cr_bl_shift_Ngbh();
  
  int   prepare_move(int);
  int   do_move();
};

//**************************************************************************

/// job shop critical-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    job shop problem where we take a swap only for critical operations.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/  
class JSHOP_cr_API_Ngbh: public JSHOP_API_Ngbh{
  private:
    /// critical operations
    int   *cr_list_j,*cr_list_m;
    /// number of critical operations
		int   count;     
	public:	
		/// construct JSHOP_cr_API_Ngbh
    /** with a start schedule and specified problem data */
		JSHOP_cr_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
    /// destructor
		~JSHOP_cr_API_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

//**************************************************************************

/// job shop critical shift neighbourhood
/** This is a problem dependent API neighbourhood class for the job shop 
    problem, where we shift an operation dependently of the critical block
    structure.
    
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_shift_Ngbh: public JSHOP_API_Ngbh{
private:
  /// set of critical operations
  Lisa_Vector<int>   *cr_list_j,*cr_list_m;
  /// number of critical operations
  int   cr_count;    
  /// job order
  Lisa_Vector<int>   *JOrd;
public:	
  /// construct JSHOP_cr_shift_Ngbh 
  /** with a start schedule and specified problem data */
  JSHOP_cr_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_cr_shift_Ngbh();
  
  int   prepare_move(int);
  int   do_move();
};

//**************************************************************************

/// job shop critical-shift-mixed-API neighbourhood
/** This is a problem dependent SHIFT neighbourhood class for the 
    job shop problem mixed with 25% API. We shift an operation dependently on 
    his position in the critical block structure.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_cr_shift_mix_Ngbh: public JSHOP_API_Ngbh{
private:
  /// set of critical operations
  Lisa_Vector<int>   *cr_list_j,*cr_list_m;
  /// number of critical operations
  int   cr_count;
  /// job order    
  Lisa_Vector<int>   *JOrd;
public:	
  /// construct JSHOP_cr_shift_mix_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_cr_shift_mix_Ngbh(Lisa_JsSchedule*, Lisa_JsProblem*);
  /// destructor
  ~JSHOP_cr_shift_mix_Ngbh();
  
  int   prepare_move(int);
  int   do_move();
};

//**************************************************************************

/// job shop critical-TRANSPOSE-mixed-API neighbourhood
/** This is the problem dependent critical-TRANSPOSE neighbourhood class for 
    the job shop problem mixed with 25% API (i.e. these neighbourhood transpose 
    a partial sequence of operations with critical end operations 
    on a machine).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_cr_trans_mix_Ngbh: public JSHOP_API_Ngbh{
private:
  //@{
  /// private data, needs documentation
  int   pos1, pos2;
  bool  api;
  Lisa_Vector<int> *JOrd,*machines;
  Lisa_Matrix<TIMETYP> *cij;
  Lisa_Matrix<int> *krv,*krr,*kr,*LR;
  //@}
public:	
  /// tabu positions
  Lisa_Vector<int> *tabu_0,*tabu_1;
  
  /// construct JSHOP_cr_trans_mix_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_cr_trans_mix_Ngbh(Lisa_JsSchedule*, Lisa_JsProblem*);
  /// destructor 
  ~JSHOP_cr_trans_mix_Ngbh(); 
  
  int   prepare_move(int);
  int   do_move();
  int   init_tabulist( unsigned int ); 
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
};

//**************************************************************************

/// job shop critical-TRANSPOSE neighbourhood
/** This is the problem dependent critical-TRANSPOSE neighbourhood class for 
    the job shop problem (i.e. these neighbourhood transpose a partial sequence 
    of operations with critical end operations on a machine).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_cr_trans_Ngbh: public JSHOP_API_Ngbh{
private:
  //@{
  /// private data, needs documentation
  int   pos1, pos2;
  Lisa_Vector<int> *JOrd,*machines;
  Lisa_Matrix<TIMETYP> *cij;
  Lisa_Matrix<int> *krv,*krr,*kr,*LR;
  //@}
public:	
  /// tabu positions
  Lisa_Vector<int> *tabu_0,*tabu_1;
  
  /// construct JSHOP_cr_trans_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_cr_trans_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_cr_trans_Ngbh(); 
  
  int   prepare_move(int);
  int   do_move();
  int   init_tabulist( unsigned int );  
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
};

//**************************************************************************

/// job shop semicritical-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    job shop problem, where we interchange two noncritical adjacent operations 
    or we move a critical operation to the critical block end.
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see JSHOP_API_Ngbh
*/
class JSHOP_semi_API_Ngbh: public JSHOP_API_Ngbh{
private:
  /// job order 
  Lisa_Vector<int> *JOrd;
public:	
  /// construct JSHOP_semi_API_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_semi_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_semi_API_Ngbh();

  int   prepare_move(int);
  int   do_move();
};

//**************************************************************************

/// job shop semi-TRANSPOSE neighbourhood
/** This is the problem dependent semicritical-TRANSPOSE neighbourhood class 
    for the job shop problem (i.e. these neighbourhood transpose a partial 
    sequence of operations on a machine but mostly such sequences with 
    critical end operations).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_semi_trans_Ngbh: public JSHOP_API_Ngbh{
private:
  //@{
  /// private data, needs documentation 
  int   pos1, pos2;
  Lisa_Vector<int> *JOrd,*machines,*poss;
  Lisa_Matrix<TIMETYP> *cij;
  Lisa_Matrix<int> *krv,*krr,*kr,*LR;
  //@}
public:	
  /// tabu positions
  Lisa_Vector<int> *tabu_0,*tabu_1;
  
  /// construct JSHOP_semi_trans_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_semi_trans_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_semi_trans_Ngbh(); 

  int   prepare_move(int);
  int   do_move();
  int   init_tabulist( unsigned int ); 
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
};

//**************************************************************************

/// job shop SHIFT neighbourhood
/** This is the problem dependent SHIFT neighbourhood class for the 
    job shop problem (i.e. these neighbourhood shifts an arbitrary operation 
    on an other position on machine).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_shift_Ngbh: public JSHOP_API_Ngbh{
private:
  /// positions
  int   pos1, pos2, n;
  /// job order
  Lisa_Vector<int> *JOrd;     
public:	
  /// tabu positions
  Lisa_Vector<int> *tabu_0,*tabu_1;
  /// construct JSHOP_shift_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_shift_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_shift_Ngbh(); 

  int   prepare_move(int);
  int   do_move();
  int   init_tabulist( unsigned int ); 
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
};

//**************************************************************************

/// job shop SWAP neighbourhood
/** This is the problem dependent SWAP neighbourhood class for the 
    job shop problem (i.e. these neighbourhood swaps two arbitrary operations
    on a machine).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_PI_Ngbh: public JSHOP_API_Ngbh{
private:
  /// positions
  int   pos1, pos2;
  /// job order
  Lisa_Vector<int> *JOrd;
public:	
  /// construct JSHOP_swap_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_PI_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_PI_Ngbh(); 

  int   prepare_move(int);
  int   do_move();
};

//**************************************************************************

/// job shop TRANSPOSE neighbourhood
/** This is the problem dependent TRANSPOSE neighbourhood class for the 
    job shop problem (i.e. these neighbourhood transpose a partial sequence of 
    operations on a machine).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class JSHOP_trans_Ngbh: public JSHOP_API_Ngbh{
private:
  /// positions
  int   pos1, pos2;
  /// job order
  Lisa_Vector<int> *JOrd;
public:
  /// tabu positions
  Lisa_Vector<int> *tabu_0,*tabu_1;
  
  /// construct JSHOP_trans_Ngbh
  /** with a start schedule and specified problem data */
  JSHOP_trans_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
  /// destructor
  ~JSHOP_trans_Ngbh(); 
  
  int   prepare_move(int);
  int   do_move();
  int   init_tabulist( unsigned int ); 
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
};

//**************************************************************************

#endif
