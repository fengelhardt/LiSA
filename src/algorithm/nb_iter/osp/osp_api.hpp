
#ifndef _oshp_api_h
#define _oshp_api_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"

/// open shop API neighbourhood.
/** This is the problem dependent API neighborhood class for the open shop 
    problem (i.e. these neighborhood swaps two arbitrary adjacent operations on
    a machine or for a job). It is inherited from the class Lisa_Neighbourhood.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
*/
class OSHOP_API_Ngbh: public Lisa_Neighborhood{
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
  int   anti_neighbor();
  int   init_tabulist( unsigned int );
  int   use_tabulist();
  int   set_tabulist();
  void  store_tabu_param();
  void  clean_tabu_param();
  
  /// return the generated schedule in the specified plan
  void  return_schedule( Lisa_OsSchedule* );
};

#endif





