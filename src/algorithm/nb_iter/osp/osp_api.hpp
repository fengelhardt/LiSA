/*
 * ******************** osp_api.hpp ******************************
 * 
 * description:      open shop API neighborhood
 * 
 * @author            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

#ifndef _oshp_api_h
#define _oshp_api_h

#include "../../../scheduling/os_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"

/** Open-Shop API-Neighborhood.
    This is the problem dependent API neighborhood class for the 
    open shop problem (i.e. these neighborhood swaps two arbitrary 
    adjacent operations on a machine or for a job). It is inherited 
    from the class Lisa_Neighborhood.

    The numbers of the solutions are the follows:
        ORIG_SOLUTION     = 0;
	WORK_SOLUTION     = 1;
	BEST_SOLUTION     = 2;	
	BEST_NGH_SOLUTION = 3; 

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
*/
class OSHOP_API_Ngbh: public Lisa_Neighborhood
     {
	public:	long seed;
	        int  tabu_param[2][4];
		/// it contains the underlying problem datas
		Lisa_OsProblem *PP;
	        int   witch_swap;  
		int machine1, machine2;
		int job1, job2;
		/** the class contains 4 schedules:
	            original schedule, work-schedule, 
		    best neighbour schedule, best schedule   */
		Lisa_OsSchedule  *P [4];
		/** construct OSHOP_API_Ngbh with a start schedule
		    and specified problem datas   */
		OSHOP_API_Ngbh( Lisa_OsSchedule*, Lisa_OsProblem* );
		~OSHOP_API_Ngbh();

	        /// set the objective type
	        void  set_objective_type( int );
		/** specify the objective function;
		    first param is objective function
		    second param is number of schedule   */
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
		    generation of neighbor,
		    it propose an interchange of two arbitrary adjacent 
		    operations in the job-order of one machine or in the 
		    machine-order of one job,
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed moveit returns OK or !OK
		int   do_move();
                /// create a anti-neighbor
                int   anti_neighbor();
		/// initialize the tabulist with specified length
		int   init_tabulist( unsigned int );
		/// search in the tabulistit returns OK or !OK
		int   use_tabulist();
		/// set a tabulist-entry
		int   set_tabulist();
		/// copy tabu-params for an entry
		void  store_tabu_param();
		/// set initial tabu_param datas
		void  clean_tabu_param();
		/// return the generated schedule in the specified plan
		void  return_schedule( Lisa_OsSchedule* );
     protected: Lisa_Tabu *tabulist;
		int   copy_schedule( int , int );

     };

#endif





