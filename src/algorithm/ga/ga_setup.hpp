#ifndef GA_SETUP_H
#define GA_SETUP_H


#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include <vector>
//#include "../../misc/except.hpp"
//#include "../../basics/pair.hpp"

#include "../nb_iter/nb_iter.hpp"

static const int RANDOM_BOUND = (1 << (sizeof(int)*8 -2)) -1 ;

struct LisaRandGenerator : public std::unary_function<int,int> {

  LisaRandGenerator(long* ext_seed){ seed = ext_seed; }
  
  int operator()(int min,int max){
    return lisa_random(min,max,seed);
  }

  int operator()(int max){
    return (*this)(0,max-1);
  }


  double operator()(){
    return ((double)lisa_random(0,RANDOM_BOUND-1,seed))/((double)RANDOM_BOUND);
  }

  bool yes_no(double prob){
    return (*this)() < prob;
  }

  void setSeed(long s){
    *seed = s;
  }

private:
  long* seed;
};


enum GA_MODE {
  IM_POP, //intermediate population (standard form)
  IM_POP_PARTIAL, //parially copy population
  STEADY_STATE //replace <parial> worst induviduals
};

enum SELECT_MODE {
  ROULLETE_WHEEL,
  TOURNAMENT_2_4
};

struct selection_params {
  GA_MODE ga_mode;
  SELECT_MODE sel_mode;
  
  int partial;
  double p_mutate;
  double p_combine;

  static const double p_op_crossing = 0.3;

  selection_params();
  
};

struct GA_Setup {

  //local improvement
  bool apply_LocalImpr;

  NB_Iteration improver;

  int pop_size;
  int n_gen;

  selection_params sel_params;

  static long SEED;
  static LisaRandGenerator* random;

  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;
  Lisa_Values Values;

  Lisa_OsProblem *problem;
  Lisa_OsSchedule *schedule;

  GA_Setup();
  ~GA_Setup(){clear();}
  
  bool init(LisaXmlFile& input);

  void clear();

};



#endif
