
#ifndef _nb_fp_h
#define _nb_fp_h

#include "../../algorithm/nb_iter/neighbour.hpp"
#include "../../algorithm/nb_iter/tabu.hpp"
#include "fp_set.hpp"

/// neighbourhood for football pool problem
/** @author  Thomas Tautenhahn
    @version 2.3rc1
 */
class FP_Neighbourhood: public Lisa_Neighbourhood{
public:
//@{ 
  /// needs documentation
  int n, sigma, fixed;
  long seed;
  Lisa_Tabu * tabu;
  FP_Set * solution;
  FP_Set * best_solution;
  FP_Vector * modification;
  FP_Vector * best_modification;
  int  work_value, best_step_value; 
  int repl_i, best_repl_i, chpos, best_chpos, elem, old_elem;
//@}

  /// construct only with given number and lenght of vectors:
  FP_Neighbourhood( int , int);

  /// compute objective function of type param1 to solution param2
  inline void  set_objective( int, int ){}

  /// returns objective value of solution nr.
  TIMETYP get_objective_value( int );
  
  /// call constructor for solution nr. 
  inline int get_solution( int ){ return 0;}
  
  /// set the objective type
  void  set_objective_type( int );
  
  /// WORK_SOLUTION to ORIG_SOLUTION
  int     accept_solution();
  
  /// BEST_NGH_SOLUTION to ORIG_SOLUTION
  int     accept_best_ngh();
  
  /// ORIG_SOLUTION to BEST_SOLUTION
  int     put_orig_to_best();
  
  /// WORK_SOLUTION to BEST_NGH_SOLUTION
  int     put_work_to_best_ngh(); 
  
  /// propose a possible move
  /** parameter is ENUM or RAND for enumerative or random generation of 
      neighbour */
  int     prepare_move( int );
  
  /// do the proposed move 
  int     do_move();
    
  /// create a anti-neighbour
  int     anti_neighbour();

  /// initialize the tabulist with specified length  
  int     init_tabulist( unsigned int );
  
  /// search in the tabulist
  int     use_tabulist();
  
  /// set a tabulist-entry
  int     set_tabulist();
  
  /// is called when last move could possibly be put into the tabulist
  void    store_tabu_param();
  
  /// is calles at beginning of each step of tabu search
  void    clean_tabu_param();
  
  /// destructor
  virtual ~FP_Neighbourhood();
};
        
#endif

