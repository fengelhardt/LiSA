/*
 * ******************** jsp_api.hpp ******************************
 * 
 * description:      job shop API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _jsp_api_h
#define _jsp_api_h

#include "../neighbor.hpp"
#include "../../../scheduling/js_sched.hpp"
#include "../../../basics/order.hpp"
#include "../tabu.hpp"

/** Job-Shop API Neighborhood.
    This is the problem dependent API neighborhood class for the 
    job shop problem (i.e. these neighborhood swaps two arbitrary adjacent 
    operations on a machine).\\
    It is inherited from the class Lisa_Neighborhood.

    The numbers of the solutions are the follows:\\
        ORIG_SOLUTION     = 0;\\
	WORK_SOLUTION     = 1;\\
	BEST_SOLUTION     = 2;\\	
	BEST_NGH_SOLUTION = 3;\\

    @author Andreas Winkler
    @version 2.3pre1
    @see Lisa_Neighborhood
*/
class JSHOP_API_Ngbh: public Lisa_Neighborhood
     {
	public:	long seed;
		/// it contains the underlying problem datas
		Lisa_JsProblem *PP;
	        int   witch_swap;  
		int   machine1;
		int   job1, job2;
                bool   new_solution;
		int  tabu_param[2][4];
		/** the class contains 4 schedules:
	            original schedule, work-schedule, best neighbor schedule,
		    best schedule    */
		Lisa_JsSchedule  *P [4];
		/** construct OSHOP_API_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_API_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
		~JSHOP_API_Ngbh();
		/// get a new schedule, param. is number of schedule
		int   get_solution( int );
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
		/** propose a possible move,
		    parameter is ENUM or RAND for enumerative or random
		    generation of neighbor;\\
		    it propose an interchange of two arbitrary adjacent 
		    operations in the job-order of one machine;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
                /// create a anti-neighbor
                int   anti_neighbor();
		/// initialize the tabulist with specified length
		int   init_tabulist( unsigned int );
		/// search in the tabulist \\ it returns OK or !OK
		int   use_tabulist();
		/// set a tabulist-entry
		int   set_tabulist();
		/// copy tabu-params for an entry
		void  store_tabu_param();
		/// set initial tabu_param datas
	        void  clean_tabu_param();
		/// return the generated schedule in the specified schedule
		void  return_schedule( Lisa_JsSchedule* );
     protected: Lisa_Tabu *tabulist;
		int   copy_schedule( int , int );
     };

#endif




