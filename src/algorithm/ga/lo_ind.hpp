#ifndef GA_LO_INDIVIDUUM_H
#define GA_LO_INDIVIDUUM_H

#include "../../main/global.hpp"
#include "../../scheduling/os_sched.hpp"
#include <vector>
#include "./ga_setup.hpp"
typedef int Op;

//a linear order individuum represented by a permutation of operations
class LO_Individuum {
public:

  static std::vector<Op> OPS;
  static std::vector<short> PERM;
  static int N_OPS,N,M,P_SIZE;
  static void init(const Lisa_OsProblem& P);
  
  //constructor/assignment
  LO_Individuum();
  LO_Individuum(const LO_Individuum& i);
  LO_Individuum(LO_Individuum& i);
  
  LO_Individuum& operator=(const LO_Individuum& i);
  LO_Individuum& operator=(LO_Individuum& i);
  
  //comparison
  bool operator <  (const LO_Individuum& i) const {return fitness < i.fitness; }
  bool operator >  (const LO_Individuum& i) const {return fitness > i.fitness; }
  bool operator >= (const LO_Individuum& i) const {return !(*this < i); }
  bool operator <= (const LO_Individuum& i) const {return !(*this > i); }

  bool operator == (const LO_Individuum& i){return (c == i.c); }
 
  
  void initialize();
  void makePlan(Lisa_OsSchedule& plan) const;

  //crossover
  void combine(const LO_Individuum& i2);

  //mutation
  void mutate();
  void mutate_swap();
  void mutate_rotate();

  void improve(GA_Setup& );
  
  typedef std::vector<Op> Chromosome;
  TIMETYP fitness;
  
private :
  Chromosome c;
  friend std::ostream& operator<<(std::ostream& out, const LO_Individuum&);
};

std::ostream& operator<<(std::ostream& out, const LO_Individuum&);


#endif
