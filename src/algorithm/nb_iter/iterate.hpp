
#ifndef _iterate_h
#define _iterate_h

#include "neighbour.hpp"
#include "../../main/global.hpp"
#include "../../misc/except.hpp"

/// available algorithms       
enum{NOMETHOD /** none  */ =0
    ,II /** iterative improvement */
    ,SA /** simulated annealing */
    ,TA /** treshold accepting */
    ,TS /** tabu search */
    ,SA_anti /** simulated annealing with antineighbour */
    };

/// Problem independent neighbourhood iteration class.
/** This class provides several neighbourhood search algorithms. 
    They can be used with any class inherited from Lisa_Neighbourhood.

    The names for the search methods are the follows:
    - II : iterative improvement
    - SA : simulated annealing
    - TA : treshold acception
    - TS : tabu search
    - SA_anti : simulated annealing with antineighbour
    
    A start solution is given within the neighbourhood.

    How to use Lisa_Iterator:
    
    - create a class inherited from Lisa_Neighbourhood describing your 
      problem and of course the neighbourhood you want to use
      - My_Neighbourhood nbh(my_problem,my_start_solution);

    - create an instance of Lisa_Iterator
      - Lisa_Iterator it;
    
    - initialize that instance with the algorithm you want and further 
      parameters
      - it.init(SA,50,3000);
      - it.set_abort_at_stuck(100000);
      - it.set_abort_at_bound(0);
    
    - run the algorithm with your neighbourhood, an objective type and the 
      number of steps
      - it.iterate(&nbh,my_objective,100000);

    - compile and run that ... it's really easy ;)

    An example showing how to implement your own Neighbourhood and how to use 
    Lisa_Iterator can be found in LiSA/src/utility/travel/

    @see Lisa_Neighbourhood
    @see Lisa_Tabu

    @author Andreas Winkler
    @version 2.3rc1
*/
class Lisa_Iterator {
private: 
  
  /// algorithm parameter, as given by init() methods
  int   method, max_stuck;
  
  /// algorithm parameter as given by set_abort_at_stuck()
  int abort_stuck;
  
  /// algorithm parameter as given by set_abort_at_bound()
  TIMETYP abort_at_bound;
  
  /// parameter for SA and TS  
  float factor0;
  
  /// random number generator seed
  long  seed;
  
  /// RAND or ENUM
  int search_type;
  
  /// number of neighbours to generate as given by init() methods
  int number_neighbours;
  
  /// length of tabu list as given by init() method
  int tabu_lenght;
  
  /// sa with anti neighbours ?
  bool  anti_neighbour;
  
  void run_start();
  void run_stop();

public: 
  
  /// contructor
  /** Create a new iteration object. */
  Lisa_Iterator(); 

  /// init function for iterative improvement II

  /** Please note that the Neighbourhood you intend to use has to support the 
      type of generation you intend to use.
      
      - init( II, type ); 
      
      @param method II
      @param type ENUM or RAND for enumerative or random generation of 
      Neighbours.
   */
  void init(int method, unsigned int type);
  
  /// init function for SA, SA_anti and TA 
  /** Please note that if you intend to use SA_anit your neighbourhood should support
      anti neighbours. 

      init( SA, prob0, max_stuck); 
      init( SA_anti, prob0, max_stuck); 
      init( TA, threshold, max_stuck); 
      
      @param method SA,SA_anti or TA
      @param prob0 Probability for accepting a neighbour with 1% worse 
             objective value in the first step. / Start threshold for accepting
             a neighbour with with a worse objective value.
      @param max_stuck If iteration is stuck for so many steps, then increase 
             temperature or treshold, respectively. */
  void  init(int method, unsigned int prob0, unsigned int max_stuck);
  
  ///init function for tabu search TS
  /** Please note that the neighbourhood you intend to use has to support the 
      type of generation you intend to use.
     
      init( TS, lenght, nn, type ); 
      
      @param method TS
      @param length The length of your tabu list.
      @param nn number of random neighbours to generate in each step or ENUM 
                for full enumeration. 
      @param type ENUM or RAND for enumerative or random generation of 
             neighbours.
   */
  void  init(int method, unsigned int length, unsigned int nn, unsigned int type);

  /// initialize additional abort criteria
  /** Force algorithm to stop if it is stuck for given number of steps,
      default is do not abort. */
  void  set_abort_at_stuck(int steps);

  /// initialize additional abort criteria
  /**  Force algorithm to stop if objective function value reached 
      given lower bound, default is no bound. */
  void set_abort_at_bound(TIMETYP bound);

  /// start the algorithm
  /** Run the initialized metaheuristic with given neighbourhood and 
      objective function. The third parameter is the maximal number of 
      solutions to be generated. A start solution is given within the
      neighbourhood. */
  void  iterate( Lisa_Neighbourhood*, int objective_type, long steps);
};

#endif

