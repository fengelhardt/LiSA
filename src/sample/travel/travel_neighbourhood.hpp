
#ifndef _travel_neighbour_h
#define _travel_neighbour_h

#include "../../basics/matrix.hpp"
#include "../../algorithm/nb_iter/neighbour.hpp"
#include "../../algorithm/nb_iter/tabu.hpp"
#include "travel_graph.hpp"

/// just an additional flag indicating a solution
const int POSS_SOLUTION = 4;


/// RPI neighbourhood
/** The RPI neighbourhood for the traveling salesman problem swaps citys 
    at two random positions to generate a neighbour. This class does neither 
    support enumerative generation nor anti neighbours.
    
    For example A = { 0 3 2 1 4 } is a neighbour of B = { 0 1 2 3 4 }.
    
    @version 2.3rc1
    @author Marc Moerig
    @see Travel_Graph
*/
class Travel_RPI_Neighbourhood : public Lisa_Neighbourhood{
private:

  /// our problem
  /** can provide us with an initial solution and is needet to calculate the
      objective */
  Travel_Graph* prob;
  
  /// two tabu vectors
  Lisa_Vector<int>* tabu_para[2];

  /// and our tabulist that will store tabu vectors of course ;)
  Lisa_Tabu* tabulist;

protected:
  /// number of vertices in our problem
  /** And also the length of our solution vector.*/
  const int length;

  /// seed for our random number generator
  long seed;

  /// the vectors containing our solutions
  Lisa_Vector<int>* sol[5];

  /// create and save a tabu representation for a solution 
  /** As one can easily see the solutions A = { 0 1 2 3 4 } and 
      B = { 2 3 4 0 1 } are the same since a solution is always a complete 
      cycle. Our traveling salesman problem is symmetrical, so 
      C = { 3 2 1 0 4 } is also equal to A and B.

      Additionally all neighbours of A are equal to all neighbours of B and C.
      We now have to make sure that if A is on the tabu list also B and C are
      on the tabu list. Therefore we will create a tabu representation of a
      solution that is equal for A, B and C.

      To accomplish that we will start reading from the 0 in each solution and
      read into the direction where the smaller city is. So, A,B and C have the 
      (same) tabu representation D = { 0 1 2 3 }. */
  void inline put_to_tabu_vector(Lisa_Vector<int>* solution);

public:
  /// constructor
  /** Create a new RPI neighbourhood for a given traveling salesman
      problem.*/
  Travel_RPI_Neighbourhood(Travel_Graph* prob_in);
  
  /// destructor
  /** Delete this object.*/
  ~Travel_RPI_Neighbourhood();

  /// set objective type
  /** We dont need this since our problem only has 
      one objective type. */
  void inline set_objective_type(const int type){}
  
  /// set objective type for a given solution 
  /** Another one we dont need.*/
  void inline set_objective(const int type, const int solution){}
  
  /// compute the objective for a given solution
  TIMETYP inline get_objective_value(const int solution){ return prob->get_objective(sol[solution]);}
  
  /// write our current solution to our input object 
  /** After running an neighbourhood algorithm calling this method will put the
      best solution bach into the input object. */
  void inline write_best(){prob->set_solution(sol[BEST_SOLUTION]);}

  /// copy WORK_SOLUTION to ORIG_SOLUTION
  int inline accept_solution(){*sol[ORIG_SOLUTION]=*sol[WORK_SOLUTION]; return OK;}
  
  /// copy BEST_NGH_SOLUTION to ORIG_SOLUTION
  int inline accept_best_ngh(){*sol[ORIG_SOLUTION]=*sol[BEST_NGH_SOLUTION]; return OK;}
  
  /// copy ORIG_SOLUTION to BEST_SOLUTION
  int inline put_orig_to_best(){*sol[BEST_SOLUTION]=*sol[ORIG_SOLUTION]; return OK;}
  
  /// copy WORK_SOLUTION to BEST_NGH_SOLUTION
  int inline put_work_to_best_ngh(){*sol[BEST_NGH_SOLUTION]=*sol[WORK_SOLUTION]; return OK;} 
  
  /// propose a possible move 
  /** The Parameter can be ENUM or RAND, but this Neighbourhood does not support 
      enumerative generation of neighbours.*/
  int prepare_move(const int type);
  
  /// do the proposed move
  int do_move();
  
  /// create a anti-neighbour 
  /** This method basically does the same as prepare_move().*/
  int inline anti_neighbour(){return !OK;}
  
  /// create a tabu list with a specific length
  int inline init_tabulist(const unsigned int l_in){ return (tabulist = new Lisa_Tabu(l_in,length-1)) ? OK : !OK;}

  /// is this solution in our tabu list
  int inline use_tabulist(){ return tabulist->use_vector(tabu_para[0]);}

  /// put a solution to our tabu list
  int inline set_tabulist(){ tabulist->set_vector(tabu_para[1]); return OK;}

  /// copy tabu parameters
  void inline store_tabu_param(){*tabu_para[1]= *tabu_para[0];}
  
  /// clean tabu parameters
  void inline clean_tabu_param(){tabu_para[0]->fill(0);}

};

/// API neighbourhood  
/** The API neighbourhood swaps citys at two adjacent positions to generate a 
    neighbour. It supports both enumerative generation of neighbours and anti
    neighbours. 
    
    For example A = { 1 0 2 3 4 } is a neighbour of B = { 0 1 2 3 4 }.

    Therefore in enumeration mode a solution for a problem with n citys has n
    neighbours. An anti neighbour is generated by using the RPI neighbourhood.
    
    @version 2.3rc1
    @author Marc Moerig
    @see Travel_Graph
 */
class Travel_API_Neighbourhood : public Travel_RPI_Neighbourhood{
private:
  
  /// the position where we are if neighbours are enumerated
  int enum_pos;
public:
  
  /// constructor
  /** Create an API neighbourhood object. */
  // call constructor of base class and reset enumeration counter
  inline Travel_API_Neighbourhood(Travel_Graph* prob_in):Travel_RPI_Neighbourhood(prob_in){enum_pos = 0;}
 
  /// propose a possible move
  /** @param type ENUM or RAND.*/
  int prepare_move(const int type);

  /// propose a possible move to an anti neighbour
  // for this neighbourhood its just the neighbourhood from our base class
  int inline anti_neighbour(){return Travel_RPI_Neighbourhood::prepare_move(RAND);}

  /// copy WORK_SOLUTION to ORIG_SOLUTION
  // reset enumeration counter !
  int inline accept_solution(){*sol[ORIG_SOLUTION]=*sol[WORK_SOLUTION]; enum_pos=0; return OK;}

  /// copy BEST_NGH_SOLUTION to ORIG_SOLUTION
  // reset enumeration counter !
  int inline accept_best_ngh(){*sol[ORIG_SOLUTION]=*sol[BEST_NGH_SOLUTION]; enum_pos=0; return OK;}
};

#endif

