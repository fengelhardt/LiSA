/*
 * ******************** jsp_crtm.hpp ******************************
 * 
 * description:      job shop critical-TRANSPOSE-mixed-API neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             16.10.2000
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _jsp_crtr_mix_h
#define _jsp_crtr_mix_h

#include "../neighbor.hpp"
#include "../../../lisa_dt/schedule/js_sched.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop critical-TRANSPOSE-mixed-API Neighborhood.
    This is the problem dependent critical-TRANSPOSE neighborhood class for 
    the job shop problem mixed with 25% API (i.e. these neighborhood transpose 
    a partial sequence of operations with critical end operations 
    on a machine).\\
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre1
    @see Lisa_Neighborhood
*/
class JSHOP_cr_trans_mix_Ngbh: public JSHOP_API_Ngbh
     {
                int   pos1, pos2;
                bool  api;
                Lisa_Vector<int> *JOrd;
                Lisa_Vector<int> *machines;
                Lisa_Matrix<int> *LR;
                Lisa_Matrix<TIMETYP> *cij;
                Lisa_Matrix<int> *krv;
                Lisa_Matrix<int> *krr;
                Lisa_Matrix<int> *kr;
	public:	
                Lisa_Vector<int> *tabu_0;
                Lisa_Vector<int> *tabu_1;

		/** construct JSHOP_cr_trans_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_cr_trans_mix_Ngbh(Lisa_JsSchedule*, Lisa_JsProblem*);
                ~JSHOP_cr_trans_mix_Ngbh(); 
		/** propose a possible move,
		    parameter is only RAND for random
		    generation of neighbor;\\
		    it propose a transpose of a partial sequence of operations
		    in the job-order of one machine;\\
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move \\ it returns OK or !OK
		int   do_move();
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
     };

#endif




