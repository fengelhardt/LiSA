/*
 * ************** insert.hpp *******************************
 * 
 * part of neumann project
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
*/


#ifndef _insert_h
#define _insert_h

#include "../../lisa_dt/schedule/js_sched.hpp"
#include "../../basics/order.hpp"

void rattle_js( Lisa_JsSchedule * sched);

class js_inserter
  {
    public:
      void init (Lisa_JsSchedule * in_sched, int in_zfn);
      void rattle( int job );
      void insert();
      void simple_order();
      ~js_inserter();
    private:   
      Lisa_JsSchedule * sched;
      Lisa_JsSchedule * sched2;
      int zfn;
      Lisa_JsProblem * P;
      Lisa_Order * order;
  };

#endif











