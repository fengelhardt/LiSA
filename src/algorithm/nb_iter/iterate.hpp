/*
 * ******************** iterate.hpp ******************************
 * 
 * description:      The class LiSA_Iterator is the problem-independent
 *                   neighborhood-iteration-class
 * 
 * owner:            Andreas Winkler
 *
 * date:             14.12.1998
 *
 */

/** @name Neighborhood Iteration
    These are the problem-independent neighborhood-iteration-methods for 
    arbitrary scheduling problems.\\
    The following methods are implemented:\\
     	- iterative improvement  #II#\\
	- simulated annealing    #SA#\\
	- treshold accepting     #TA#\\
	- tabusearch             #TS#\\
	- simulated annealing with antineighbor #SA_anti# \\
    \\
    To use this methods you must define a neighborhood structure and a 
    corresponding neighborhood class (inherit of the class #Lisa_Neighborhood#) 
    
*/  
//@{

#ifndef _iterate_h
#define _iterate_h

#include "neighbor.hpp"
#include "../../main/global.hpp"
#include "../../misc/except.hpp"

const int NOMETHOD = 0;
// iterative improvement
const int II = 1;
// simulated annealing
const int SA = 2;
// treshold accepting
const int TA = 3;
// tabu search
const int TS = 4;
// simulated annealing with antineighbor
const int SA_anti = 5;

/** Problem-independent Neighborhood-Iteration-Class.\\

    This class provides several neighbourhood search algorithms. 
    They can be used with any class inherited from #Lisa_Neighborhood#.\\

    The names for the search methods are the follows:\\
    - #II# : iterative improvement\\
    - #SA# : simulated annealing\\
    - #TA# : treshold acception\\
    - #TS# : tabu search\\
    - #SA_anti# : simulated annealing with antineighbor\\
    A start solution is given within the neighborhood.\\

    How to use #Lisa_Iterator#:\\
    
    - create a class inherited from #Lisa_Neighborhood# describing your 
    problem and of course the neighbourhood you want to use\\
    #My_Neighbourhood nbh(my_problem,my_start_solution);#\\

    - create an instance of #Lisa_Iterator#\\
    #Lisa_Iterator it;#\\
    
    - initialize that instance with the algorithm you want and further parameters\\
    #it.init(SA,50,3000);#\\
    #it.set_abort_at_stuck(100000);#\\
    #it.set_abort_at_bound(0);#\\
    
    - run the algorithm with your neighbourhood, an objective type and the number of steps\\
    #it.iterate(&nbh,my_objective,100000);#\\

    - compile and run that ... it's really easy ;)\\

    An example showing how to implement your own Neighbourhood and how to use #Lisa_Iterator#
    can be found in #LiSA/src/utility/travel/#

    @see Lisa_Neighborhood
    @see Lisa_Tabu

    @author Andreas Winkler
    @version 2.3pre2
*/
class Lisa_Iterator {
private: 

  int   method, abort_stuck, max_stuck;
  float factor0;
  TIMETYP abort_at_bound;
  long  seed;
  int   search_type;  
  int   number_neighbors, tabu_lenght;
  bool  anti_neighbor;
  void run_start();
  void run_stop();

public: 
  
  /** contructor ...\\

      Create a new iteration object. */
  Lisa_Iterator(); 

  /** init function for iterative improvement #II# ...\\

      #init( II, type )#; \\
      
      - #type#: #ENUM# or #RAND# for enumerative or random generation of Neighbours.
      Please note that the Neighbourhood you intend to use has to support the type 
      of generation you intend to use.*/
  void init(int method, unsigned int type);
  
  /** init function for simmulated annealing #SA#, simmulated annealing with anti 
      neighbour #SA_anti# and threshold accepting #TA# ...\\
      
      Please note that if you intend to use #SA_anit# your neighbourhood should support
      anti neighbours.\\ 

      #init( SA, prob0, max_stuck);# \\
      #init( SA_anti, prob0, max_stuck);# \\
      #init( TA, threshold, max_stuck);# \\
      
      - #prob0#: Probability for accepting a neighbour with 1% worse objective value 
      in the first step.\\
      - #threshold#: Start threshold for accepting a neighbour with with a worse objective value.\\
      - #max_stuck#: If iteration is stuck for so many steps, then increase temperature 
      or treshold, respectively. */
  void  init(int method, unsigned int prob0, unsigned int max_stuck);
  
  /** init function for tabu search #TS# ...\\
      
      #init( TS, lenght, nn, type );# \\
      
      - #length#: The length of your tabu list.\\
      - #nn#: number of random neighbours to generate in each step or #ENUM# for full enumeration.\\ 
      - #type#: #ENUM# or #RAND# for enumerative or random generation of neighbours.
      Please note that the neighbourhood you intend to use has to support the type 
      of generation you intend to use. */
  void  init(int method, unsigned int length, unsigned int nn, unsigned int type);

  /** initialize additional abort criteria ...\\ 

      Force algorithm to stop if it is stuck for given number of steps,
      default is do not abort. */
  void  set_abort_at_stuck(int steps);

  /** initialize additional abort criteria ...\\
      
      Force algorithm to stop if objective function value reached 
      given lower bound, default is no bound. */
  void set_abort_at_bound(TIMETYP bound);

  /** start the algorithm ...\\
      
      Run the initialized metaheuristic with given neighborhood and 
      objective function. The third parameter is the maximal number of 
      solutions to be generated. A start solution is given within the
      neighborhood. */
  void  iterate( Lisa_Neighborhood*, int objective_type, long steps);
};

#endif

//@}




