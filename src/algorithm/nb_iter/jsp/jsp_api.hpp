
#ifndef _jsp_api_h
#define _jsp_api_h

#include "../../../scheduling/js_sched.hpp"
#include "../../../basics/order.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"

/// job shop API neighbourhood
/** This is the problem dependent API neighbourhood class for the job shop 
    problem (i.e. these neighbourhood swaps two arbitrary adjacent operations on
    a machine). It is inherited from the class Lisa_Neighbourhood.

    @author Andreas Winkler
    @version 2.3pre3
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

#endif




