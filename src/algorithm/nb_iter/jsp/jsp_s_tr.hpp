/*
 * ******************** jsp_s_tr.hpp ******************************
 * 
 * description:      job shop semicritical-TRANSPOSE neighbourhood
 * 
 * @author            Andreas Winkler
 *
 * date:             16.10.2000
 *
 */

#ifndef _jsp_s_tr_h
#define _jsp_s_tr_h

#include "../../../scheduling/js_sched.hpp"
#include "../neighbour.hpp"
#include "../tabu.hpp"
#include "jsp_api.hpp"

/** Job-Shop semi-TRANSPOSE Neighbourhood.
    This is the problem dependent semicritical-TRANSPOSE neighbourhood class 
    for the job shop problem (i.e. these neighbourhood transpose a partial 
    sequence of operations on a machine but mostly such sequences with 
    critical end operations).
    It is inherited from the class JSHOP_API_Ngbh.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighbourhood
*/
class JSHOP_semi_trans_Ngbh: public JSHOP_API_Ngbh
     {
                int   pos1, pos2;
                Lisa_Vector<int> *JOrd;
                Lisa_Vector<int> *machines;
                Lisa_Vector<int> *poss;
                Lisa_Matrix<int> *LR;
                Lisa_Matrix<TIMETYP> *cij;
                Lisa_Matrix<int> *krv;
                Lisa_Matrix<int> *krr;
                Lisa_Matrix<int> *kr;
	public:	
                Lisa_Vector<int> *tabu_0;
                Lisa_Vector<int> *tabu_1;

		/** construct JSHOP_semi_trans_Ngbh with a start schedule
		    and specified problem datas   */
		JSHOP_semi_trans_Ngbh( Lisa_JsSchedule*, Lisa_JsProblem* );
                ~JSHOP_semi_trans_Ngbh(); 
		/** propose a possible move,
		    parameter is only RAND for random
		    generation of neighbour;
		    it propose a transpose of a partial sequence of operations
		    in the job-order of one machine;
		    it returns OK or !OK   */
		int   prepare_move(int);
		/// do the proposed move  it returns OK or !OK
		int   do_move();
                /// init the tabu list with a specified length
                int   init_tabulist( unsigned int ); 
		/// search in the tabulist  it returns OK or !OK
                int   use_tabulist();
		/// set a tabulist-entry
                int   set_tabulist();
		/// copy tabu-params for an entry
                void  store_tabu_param();
		/// set initial tabu_param datas
                void  clean_tabu_param();
     };

#endif




