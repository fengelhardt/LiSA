/*
 * ******************** m1_nb.hpp ******************************
 * 
 * description:      single machine API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef _m1_nb_h
#define _m1_nb_h

#include "../../../scheduling/m1_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"

/** API-Neighborhood.
    This is the problem dependent API neighborhood class for the 
    single machine problem (i.e. this neighborhood swaps two arbitrary 
    adjacent jobs).
   
    The numbers of the solutions are the follows:\\
        ORIG_SOLUTION     = 0;\\
	WORK_SOLUTION     = 1;\\
	BEST_SOLUTION     = 2;\\	
	BEST_NGH_SOLUTION = 3;\\ 

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood  */
class API_Neighborhood: public Lisa_Neighborhood
     {
	public:	long seed;
		/// it contains the underlying problem datas
		Lisa_1Problem *PP;
		int num;
		int pos;
		int  tabu_param[2][4];
		/** the class contains 4 schedules:
	            original schedule, work-schedule, 
		    best neighbor schedule, best schedule    */
		Lisa_1Schedule *P [4];
		/** construct API_Neighborhood with a start schedule
		    and specified problem datas    */
		API_Neighborhood(Lisa_1Schedule*,Lisa_1Problem*);
		~API_Neighborhood();

	        /// set the objective type
	        void  set_objective_type( int );
		/** specify the objective function;
		    first param is objective function,
		    second param is number of schedule    */
		void  set_objective( int, int );
		/** determs the objective value,
		    parameter is number of schedule   */
		TIMETYP get_objective_value( int );
		//  some special copy functions:
	        /// copy WORK_SOLUTION to ORIG_SOLUTION
	        virtual int   accept_solution();
		/// copy BEST_NGH_SOLUTION to ORIG_SOLUTION
		virtual int   accept_best_ngh();
		/// copy ORIG_SOLUTION to BEST_SOLUTION
		virtual int   put_orig_to_best();
		/// copy WORK_SOLUTION to BEST_NGH_SOLUTION
		virtual int   put_work_to_best_ngh();
		/** propose a possible move;
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbour,\\
		    it propose an interchange of two adjacent jobs on the 
		    machine,\\
		    it returns OK or !OK   */	
		int   prepare_move(int);
		/// do the proposed move \\it returns OK or !OK
		int   do_move();
                /// create a anti-neighbor
                int     anti_neighbor();
		/// initialize the tabulist with specified length
		int   init_tabulist( unsigned int );
		/// search in the tabulist \\it returns OK or !OK
		int   use_tabulist();
		/// set a tabulist-entry
		int   set_tabulist();
		/// copy tabu-params for an entry
		void  store_tabu_param();
		/// set initial tabu_param datas
		void  clean_tabu_param();
		/// return the generated schedule in the specified schedule
		void  return_schedule( Lisa_1Schedule* );
     protected: Lisa_Tabu *tabulist;
		int   copy_schedule( int , int );

     };

#endif




