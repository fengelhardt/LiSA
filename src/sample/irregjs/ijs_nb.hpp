/*
 * ******************** ijs_nb.hpp ******************************
 * 
 * description:      job shop API neighborhood with "rattling"
 * 
 * owner:            Tautenhahn
 *
 * date:             23.2.1999
 *
 */

//@{

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _ijs_nb_h
#define _ijs_nb_h

#include "../../algorithm/nb_iter/neighbor.hpp"
#include "../../algorithm/nb_iter/jsp/jsp_api.hpp"
#include "../../lisa_dt/schedule/js_sched.hpp"
#include "../../algorithm/nb_iter/tabu.hpp"


class Lisa_IjsApi: public JSHOP_API_Ngbh
     {
	public:	
		Lisa_IjsApi( Lisa_JsSchedule* x, Lisa_JsProblem* y)
                     : JSHOP_API_Ngbh(x,y)
                {
                };		
		int   do_move();
     };

#endif

//@}



