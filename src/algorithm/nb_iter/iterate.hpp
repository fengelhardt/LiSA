
#ifndef _iterate_h
#define _iterate_h

#include "neighbour.hpp"
#include "../../main/global.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ctrlpara.hpp"

//*****************************************************************************

/// available algorithms       
enum{NOMETHOD /** none  */ =0
    ,II /** iterative improvement */
    ,SA /** simulated annealing */
    ,TA /** treshold accepting */
    ,TS /** tabu search */
    ,SA_new /** new implementation of simulated annealing */
    };

//*****************************************************************************

class Lisa_Iter{
protected:

  /// random number generator seed
  long  seed;
  
  ///stop algorithm now ??
  bool abort_algorithm;
  
  /// algorithm parameter as given by set_abort_at_stuck()
  int abort_stuck;
  
  /// algorithm parameter as given by set_abort_at_bound()
  TIMETYP abort_at_bound;
  
  //default constructor
  Lisa_Iter(Lisa_ControlParameters *CP);
  
public:
  
  ///destructor
  virtual ~Lisa_Iter(){}
  
  /// start the algorithm
  /** Run the initialized metaheuristic with given neighbourhood and 
      objective function. The third parameter is the maximal number of 
      solutions to be generated. A start solution is given within the
      neighbourhood. */
  virtual void iterate(Lisa_Neighbourhood*, int objective_type, long steps) = 0;
  
  /// tell the algorithm to stop now
  void inline abort(){abort_algorithm=true;}
};

//*****************************************************************************

/// Problem independent neighbourhood iteration class.
class Lisa_ThresholdAccepting:public Lisa_Iter{
private: 
    ///enumerate neightbours or generate them randomly ?
  int gen_nb;
  
  /** Probability for accepting a neighbour with 1% worse 
      objective value in the first step. / Start threshold for accepting
      a neighbour with with a worse objective value. */
  int prob0;
  double factor0;
  
  /** If iteration is stuck for so many steps, then increase 
      temperature or treshold, respectively. */
  int max_stuck;
  
public: 
  
  Lisa_ThresholdAccepting(Lisa_ControlParameters* CP);
  

  void  iterate(Lisa_Neighbourhood *ngbh, int objective_type, long maxsteps);
 
};

//*****************************************************************************

class Lisa_OldSimulatedAnnealing: public Lisa_Iter{
private:
  ///enumerate neightbours or generate them randomly ?
  int gen_nb;
  
  /** Probability for accepting a neighbour with 1% worse 
      objective value in the first step. / Start threshold for accepting
      a neighbour with with a worse objective value. */
  int prob0;
  double factor0;
  
  /** If iteration is stuck for so many steps, then increase 
      temperature or treshold, respectively. */
  int max_stuck;
  
  
public:
  Lisa_OldSimulatedAnnealing(Lisa_ControlParameters* CP);
    
  void iterate(Lisa_Neighbourhood *ngbh, int objective_type, long maxsteps);

};

//*****************************************************************************

class Lisa_TabuSearch:public Lisa_Iter{
private:

  ///enumerate neightbours or generate them randomly ?
  int gen_nb;
  
  ///number if neighbours to generate in each step
  int number_of_neighbours;
  
  ///length of tabulist
  int tl_length;
  
public:
  Lisa_TabuSearch(Lisa_ControlParameters* CP);
    
  void iterate(Lisa_Neighbourhood *ngbh, int objective_type, long maxsteps);
};

//*****************************************************************************

class Lisa_SimulatedAnnealing:public Lisa_Iter{
public:
  //how to reduce temperature
  /** GEOMETRIC:    T = cp*T             ( 0 < cp < 1 )
      LUNDYANDMEES: T = T/(1+cp*T)       ( 0 < cp )
      LINEAR:       T = T - cp*Tstart    ( 0 < cp ) */
  enum COOLING_SCHEME {GEOMETRIC,LUNDYANDMEES,LINEAR};
  
private:

  //the start and end temperature
  double Tstart,Tend;
  
  //the cooling scheme to apply
  COOLING_SCHEME cs;
  
  //cooling parameter
  double cp;
  
  //how many steps before reducing temperature
  long epochlength;
  
  //how many neighbours to generate in each step
  int numberneighbours;
public:
  Lisa_SimulatedAnnealing(Lisa_ControlParameters* CP);

  void iterate(Lisa_Neighbourhood *ngbh, int objective_type, long maxsteps);  
};

//*****************************************************************************

class Lisa_IterativeImprovement:public Lisa_Iter{
private:

  ///enumerate neightbours or generate them randomly ?
  int gen_nb;
public:
  Lisa_IterativeImprovement(Lisa_ControlParameters* CP);
    
  void iterate(Lisa_Neighbourhood *ngbh, int objective_type, long maxsteps);
};

//*****************************************************************************

#endif

