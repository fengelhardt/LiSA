
#ifndef lisa_openshop_neighbourhoods_h
#define lisa_openshop_neighbourhoods_h

//**************************************************************************

#include "../../basics/order.hpp"
#include "../../scheduling/os_sched.hpp"
#include "neighbour.hpp"
#include "tabu.hpp"

//**************************************************************************

/// open shop API neighbourhood.
/** This is the problem dependent API neighbourhood class for the open shop 
    problem (i.e. these neighbourhood swaps two arbitrary adjacent operations on
    a machine or for a job). It is inherited from the class Lisa_Neighbourhood.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
*/
class OSHOP_API_Ngbh: public Lisa_Neighbourhood{
protected:
  /// tabu list
  Lisa_Tabu *tabulist;
  /// copy source schedule to destionation schedule
	int copy_schedule(int source, int destination);
public:	
  /// seed for random number generation
  long seed;
  /// tabu parameters
  int  tabu_param[2][4];
  /// underlying problem data
  Lisa_OsProblem *PP;
  /// swap where ?
  int witch_swap;
  /// machines to swap
  int machine1, machine2;
  /// jobs to swap
  int job1, job2;
  /// the class contains 4 schedules:
  Lisa_OsSchedule  *P [4];
  /// construct OSHOP_API_Ngbh
  /** with a start schedule and specified problem datas   */
  OSHOP_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
  /// destructor
  ~OSHOP_API_Ngbh();
  
  void  set_objective_type(int objective);
  void  set_objective(int objective,int schedule);
  TIMETYP get_objective_value(int schedule);
  virtual int   accept_solution();
  virtual int   accept_best_ngh();
  virtual int   put_orig_to_best();
  virtual int   put_work_to_best_ngh();
  int prepare_move(int type);
  int   do_move();
  int   anti_neighbour();
  int   init_tabulist( unsigned int );
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
  
  /// return the generated schedule in the specified plan
  void  return_schedule( Lisa_OsSchedule* );
};

//**************************************************************************

/// Open-Shop 3-time-API Neighbourhood.
/** This is a problem dependent API neighbourhood class for the open shop 
    problem where we take a swap of adjacent operations on 3 different machines
    or jobs.
    
    It works only with tabusearch method !
    
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_3_API_Ngbh: public OSHOP_API_Ngbh{
private:
  /// for the 3 proposed swaps
  /** for JO-swap:
	    - swaps[][0]=JO
			- swaps[][1] machine1
			- swaps[][2] job1
			- swaps[][3] job2
			- swaps[][4] OK ? */
  int swaps[3][5];
public:	
  /// construct OSHOP_3_API_Ngbh
  /** with a start schedule and specified problem datas   */
	OSHOP_3_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
  
	int   prepare_move(int);
	int   do_move();
	int   use_tabulist();
	int   set_tabulist();
};

//**************************************************************************

/// open shop 3 time critical API Neighbourhood.
/** This is the problem dependent API neighbourhood class for the 
    open shop problem, where we swap two adjacent operations on the
    critical way and additional we swap also the direct predecence 
    and the direct sucsessive operations on the critical way.
    
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_3_CR_Ngbh: public OSHOP_API_Ngbh{       
  private:
    //@{
    /// private data, needs documentation
    Lisa_Order *ROrd;
		int   OrdCount;
		int   NewOrder;
		TIMETYP head, tail;
		int   machine3;
		int   job3;
	  Lisa_OsSchedule  *temp_schedule;
    //@}
    /// for the 3 proposed swaps
    /** for JO-swap:
	    - swaps[][0]=JO
			- swaps[][1] machine1
			- swaps[][2] job1
			- swaps[][3] job2
			- swaps[][4] OK ? */   
    int   swaps[3][5];
	public:	
		/// construct OSHOP_3_CR_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_3_CR_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
	  /// destructor
    ~OSHOP_3_CR_Ngbh();

		int   prepare_move(int);
		int   do_move();
		int   use_tabulist();
		int   set_tabulist();
		void clean_tabu_param();
};

//**************************************************************************

/// open shop critical-block-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    open shop problem, where we take a swap only for a critical block  
    end operation.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_bl_API_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// set of critical operations
    int   *cr_list_j;
    /// set of critical operations
		int   *cr_list_m;  
		/// private data, needs documentation
    int   *direction; 
    /// number of critical operations
    int   count;     
	public:	
		/// construct OSHOP_cr_bl_API_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_cr_bl_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_cr_bl_API_Ngbh();

		int   prepare_move(int);

		int   do_move();
};

//**************************************************************************

/// Open-Shop critical_block-SHIFT Neighbourhood.
/** This is the problem dependent SHIFT neighbourhood class for the 
    open shop problem, where we take a swap only for a critical block 
    operation.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_bl_shift_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// set of critical operations
		Lisa_Vector<int> *cr_list_j, *cr_list_m, *direction; 
    /// number of critical operations
		int   count;
    /// private data needs documentation
		int   pos1, pos2;
		/// job oder and machine order
    Lisa_Vector<int> *JOrd, *MOrd;
	public:	
		/// construct OSHOP_cr_bl_shift_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_cr_bl_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		/// destructor
    ~OSHOP_cr_bl_shift_Ngbh();

		int   prepare_move(int);

		int   do_move();
};

//**************************************************************************

/// Open-Shop critical-API Neighbourhood (with a fix order of the operations). 
/** This is the problem dependent API neighbourhood class for the 
    open shop problem where we take a swap only for critical operations.
    This class works like OSHOP_cr_API_Ngbh, but here we create a fixed
    order of the critical operations.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
    @see OSHOP_cr_API_Ngbh
*/
class OSHOP_cr_TST_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// private data, needs documentation
    Lisa_Order *ROrd;
    /// private data, needs documentation
		int   OrdCount;
    /// private data, needs documentation
		int   NewOrder;
	public:	
		/// construct OSHOP_cr_TST_Ngbh
    /** with a start schedule and specified problem data   */
		OSHOP_cr_TST_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_cr_TST_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

//**************************************************************************

/// open shop critical-API neighbourhood
/** This is a problem dependent API neighbourhood class for the 
    open shop problem, where we take a swap only for a critical operation.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_API_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// set of critical operations
    int   *cr_list_j;
    /// set of critical operations
		int   *cr_list_m; 
		/// number of critical operations
    int   count;     
	public:	
		/// construct OSHOP_API_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_cr_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		/// destructor
    ~OSHOP_cr_API_Ngbh();

		int   prepare_move(int);

		int   do_move();
};

//**************************************************************************

/// open shop critical-SHIFT neighbourhood
/** This is a problem dependent SHIFT neighbourhood class for the 
    open shop problem where we take a swap only for critical operations.
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
*/
class OSHOP_cr_shift_Ngbh: public OSHOP_API_Ngbh{
  
    /// set of critical operations
		Lisa_Vector<int> *cr_list_j, *cr_list_m;
		/// machine order and job order
    Lisa_Vector<int> *JOrd, *MOrd;
    /// number of critical operations
		int   count;
	public:	
		/// construct OSHOP_API_Ngbh
    /** with a start schedule and specified problem datas   */
		OSHOP_cr_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_cr_shift_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

//**************************************************************************

/// open shop SHIFT neighbourhood
/** This is the problem dependent SHIFT neighbourhood class for the open shop
    problem, these neighbourhood shifts an arbitrary operation on one machine
    or one job.
    
    It is inherited from the class OSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
 */
class OSHOP_shift_Ngbh: public OSHOP_API_Ngbh{
  private:
    /// private data needs documentation
		Lisa_Vector<int> *JOrd, *MOrd, *JOpos, *MOpos;
	public:	
		/// construct OSHOP_API_Ngbh 
    /** with a start schedule and specified problem datas   */
		OSHOP_shift_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_shift_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

//**************************************************************************

/// open shop PI neighbourhood
/** This is the problem dependent PI neighbourhood class for the open shop
    problem, it swaps two arbitrary operations on one machine or one job.
    
    The operations to swap are determined randomly, enumeration is not
    supported.
    
    It is inherited from the class OSHOP_API_Ngbh.

    @author Marc Moerig
    @version 2.3final
    @see Lisa_Neighbourhood
    @see OSHOP_API_Ngbh
 */
class OSHOP_PI_Ngbh: public OSHOP_API_Ngbh{
	public:	
		/// construct OSHOP_PI_Ngbh 
    /** with a start schedule and specified problem datas   */
		OSHOP_PI_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
    /// destructor
		~OSHOP_PI_Ngbh();

		int   prepare_move(int);
		int   do_move();
};

//**************************************************************************

#endif
