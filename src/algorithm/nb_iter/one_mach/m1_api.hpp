
#ifndef _m1_nb_h
#define _m1_nb_h

#include "../../../scheduling/m1_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"

/// single machine API neighbourhood
/** This is the problem dependent API neighbourhood class for the single machine
    problem (i.e. this neighbourhood swaps two arbitrary adjacent jobs).
   
    @author Andreas Winkler
    @version 2.3rc1
    @see Lisa_Neighbourhood  
 */
class API_Neighbourhood: public Lisa_Neighbourhood{
protected: 
  /// a tabu list
  Lisa_Tabu *tabulist;
  /// copy solution source to solution destination
	int   copy_schedule(int source, int destination);
public:
  /// seed for random number generation
  long seed;
	/// underlying problem data
	Lisa_1Problem *PP;
	/// internal data, needs documentation
  int num,pos;
  /// tabu parameters
	int tabu_param[2][4];
	/// the class contains 4 schedules
	Lisa_1Schedule *P [4];
	/// construct API_Neighbourhood with a start schedule and specified problem data
	API_Neighbourhood(Lisa_1Schedule*,Lisa_1Problem*);
	/// destructor
  ~API_Neighbourhood();


  void  set_objective_type(int objective);
	void  set_objective(int objective, int schedule);
  TIMETYP get_objective_value(int schedule);
  virtual int   accept_solution();
	virtual int   accept_best_ngh();
	virtual int   put_orig_to_best();
	virtual int   put_work_to_best_ngh();
	int prepare_move(int param);
	int do_move();
  int anti_neighbour();
	int   init_tabulist( unsigned int );
	int   use_tabulist();
	int   set_tabulist();
	void  store_tabu_param();
	void  clean_tabu_param();
  
	/// return the generated schedule in the specified schedule
	void  return_schedule( Lisa_1Schedule* );
};

#endif

