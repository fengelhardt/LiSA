/*
 * ************** irredjs.hpp*******************************
 * 
 * part of neumann-project
 * @version 2.3pre3
 *
 * @author Thomas Tautenhahn
 *
 * 19.9.1998
*/

#ifndef _irredjs_h 
#define _irredjs_h 

#include <iostream>

#include "../../misc/except.hpp"
#include "../../scheduling/js_sched.hpp"

class NM_JsProblem: public Lisa_JsProblem
   {
     public:
     NM_JsProblem( Lisa_Values *Pin );
     void read(std::istream& = std::cin);  
     ~NM_JsProblem(){}
   };

class NM_JsSchedule: public Lisa_JsSchedule
   {
     public:
     NM_JsSchedule( Lisa_JsProblem *Pin );
     void read(std::istream& = std::cin);  
     ~NM_JsSchedule(){}
   };



inline std::istream& operator >> (std::istream& strm, NM_JsSchedule& m)
   {
     m.read(strm);
     return strm;
   }

inline std::istream& operator >> (std::istream& strm, NM_JsProblem& m)
   {
     m.read(strm);
     return strm;
   }

#endif

