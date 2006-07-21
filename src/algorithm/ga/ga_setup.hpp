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
#include "../dispatch/dispatcher.hpp"

static const int RANDOM_BOUND = (1 << (sizeof(int)*8 -2)) -1 ;

//static const int SUM_CI_2 = SUM_CI + 0xfdec;

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

struct GA_Setup;

enum GA_INIT_MODE {
  INIT_RANDOM,
  INIT_DISP_ACTIVE,
  INIT_DISP_NON_DELAY
};

class GA_Initializer : public Lisa_Dispatcher {
public:

  GA_Initializer(const GA_Setup*);

  void getPlan(Lisa_Matrix<int>& p, GA_INIT_MODE m);
  Lisa_Schedule* target_schedule;

  ~GA_Initializer();
};


struct selection_params {
  double p_mutate;
  double p_combine;

  selection_params();
  
};

struct GA_Setup {

  //local improvement
  bool apply_LocalImpr;

  NB_Iteration improver;
  GA_Initializer *initializer;

  GA_INIT_MODE mode;
  
  int pop_size;
  int n_gen;
  int objective;

  selection_params sel_params;

  static long SEED;
  static LisaRandGenerator* random;

  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;
  Lisa_Values Values;

  Lisa_OsProblem *problem;
  Lisa_OsSchedule *schedule;

  GA_Setup();
  ~GA_Setup(){
    clear();
  }
  
  bool init(LisaXmlFile& input);

  void clear();

};



#endif
