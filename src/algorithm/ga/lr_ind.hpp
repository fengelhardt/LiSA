#ifndef GA_LR_INDIVIDUUM_H
#define GA_LR_INDIVIDUUM_H

#include "../../main/global.hpp"
#include "../../scheduling/os_sched.hpp"

#include "./ga_setup.hpp"
typedef int Op;

//an  individuum represented by rank matrix
class LR_Individuum {
public:

  static void init(const Lisa_OsProblem& P, int objective);
  static Lisa_OsProblem * P;
  static Lisa_OsSchedule* S;
  static Lisa_Order* Order;
  static Lisa_Matrix<int>* Cross_Mask;
  static int Objective;
  
  //constructor/assignment
  LR_Individuum();
  ~LR_Individuum();
  LR_Individuum(const LR_Individuum& i);
  //crossover_constructor
  LR_Individuum(const LR_Individuum& i1, const LR_Individuum& i2);
  LR_Individuum(LR_Individuum& i);
  
  LR_Individuum& operator=(const LR_Individuum& i);
  LR_Individuum& operator=(LR_Individuum& i);
  
  //comparison
  bool operator <  (const LR_Individuum& i) const {return getFitness() < i.getFitness(); }
  bool operator >  (const LR_Individuum& i) const {return getFitness() > i.getFitness(); }
  bool operator >= (const LR_Individuum& i) const {return !(*this < i); }
  bool operator <= (const LR_Individuum& i) const {return !(*this > i); }

  bool operator == (const LR_Individuum& i){
    if(c && i.c)
      return (*c == *(i.c)); 
    return false;
  }
 
  
  void initialize(GA_Setup&);
  void initialize(const Lisa_ScheduleNode&);
  void makePlan(Lisa_OsSchedule& plan) const{
    plan.clear();
    plan.read_LR(c);
  }

  static void setCrossMask();
  //crossover
  template <typename OutputIterator>
  static void combine(const LR_Individuum& i1, 
		      const LR_Individuum& i2, 
		      const GA_Setup& setup,
		      OutputIterator result)
	
  {
    setCrossMask();
    *result = LR_Individuum(i1,i2);++result;
    *result = LR_Individuum(i2,i1);++result;
  }

  //mutation
  void mutate();
  void mutate_swap();
  void mutate_rotate();

  void improve(GA_Setup&);

  void forgetFitness(){
    f_valid = false;
  }

  TIMETYP getFitness() const{
    if(!f_valid) eval();
    return fitness;
  }
  
  void eval() const;
  
  typedef Lisa_Matrix<int>  LR;
  typedef LR Chromosome;
  
private :
  Chromosome *c;
  friend std::ostream& operator<<(std::ostream& out, const LR_Individuum&);
  mutable TIMETYP fitness;
  mutable bool f_valid;

  void latinize();
  
};

std::ostream& operator<<(std::ostream& out, const LR_Individuum&);


#endif
