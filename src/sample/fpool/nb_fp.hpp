/*
 * ******************** nb_fp.hpp ******************************
 * 
 * description:      neighbourhood for football pool problem
 * 
 * @author            Thomas Tautenhahn
 * @version 2.3pre3
 *
 * date:             20.10.1998
 *
 */

#ifndef _nb_fp_h
#define _nb_fp_h

#include "../../algorithm/nb_iter/neighbor.hpp"
#include "../../algorithm/nb_iter/tabu.hpp"
#include "fp_set.hpp"

class fp_nb: public Lisa_Neighborhood
 {
   public: 
           int n, sigma, fixed;
           long seed;
           Lisa_Tabu * tabu;
           FP_Set * solution;
           FP_Set * best_solution;
           FP_Vector * modification;
           FP_Vector * best_modification;
           int  work_value, best_step_value; 
           int repl_i, best_repl_i, chpos, best_chpos, elem, old_elem;
        /// construct only with given number and lenght of vectors:
        fp_nb( int , int);
        /// compute objective function of type param1 to solution param2
	virtual void    set_objective( int, int ){}
	/// returns objective value of solution nr.
	virtual TIMETYP get_objective_value( int );
	/// call constructor for solution nr. 
	virtual int     get_solution( int ){ return 0;}
	/// set the objective type
	virtual void  set_objective_type( int );
	/// some special copy functions:
	/// WORK_SOLUTION to ORIG_SOLUTION
	virtual int     accept_solution();
	/// BEST_NGH_SOLUTION to ORIG_SOLUTION
	virtual int     accept_best_ngh();
	/// ORIG_SOLUTION to BEST_SOLUTION
	virtual int     put_orig_to_best();
	/// WORK_SOLUTION to BEST_NGH_SOLUTION
	virtual int     put_work_to_best_ngh(); 
	/** propose a possible move
	    parameter is ENUM or RAND for enumerative or random
	    generation of neighbor                */
	virtual int     prepare_move( int );
	/// do the proposed move 
	virtual int     do_move();
	/// initialize the tabulist with specified length
     /// create a anti-neighbor
     virtual int     anti_neighbor();
	virtual int     init_tabulist( unsigned int );
	/// search in the tabulist
	virtual int     use_tabulist();
	/// set a tabulist-entry
	virtual int     set_tabulist();
	/// is called when last move could possibly be put into the tabulist
	virtual void    store_tabu_param();
	/// is calles at beginning of each step of tabu search
	virtual void    clean_tabu_param();
	virtual ~fp_nb();
    };
        
#endif

