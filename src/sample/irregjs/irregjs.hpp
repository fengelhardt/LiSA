/*
 * ************** irredjs.hpp*******************************
 * 
 * part of neumann-project
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
*/

#ifndef _irredjs_h 
#define _irredjs_h 

#include <iostream.h>
#include "../../basics/except.hpp"
#include "../../lisa_dt/schedule/js_sched.hpp"

class NM_JsProblem: public Lisa_JsProblem
   {
     public:
     NM_JsProblem( Lisa_Values *Pin );
     void read(istream& = cin);  
     ~NM_JsProblem(){};
   };

class NM_JsSchedule: public Lisa_JsSchedule
   {
     public:
     NM_JsSchedule( Lisa_JsProblem *Pin );
     void read(istream& = cin);  
     ~NM_JsSchedule(){};
   };



inline istream& operator >> (istream&strm, NM_JsSchedule& m)
   {
     m.read(strm);
     return strm;
   }

inline istream& operator >> (istream&strm, NM_JsProblem& m)
   {
     m.read(strm);
     return strm;
   }

#endif

















