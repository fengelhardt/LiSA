#ifndef BEAM_ORDERS_HPP
#define BEAM_ORDERS_HPP

#include <limits>

#include "../../basics/order.hpp"
#include "../../scheduling/os_sched.hpp"


enum InsertionOrder{
  lpt,
  rndm,
  line_by_line,
  diagonal,
  queens,
  spt,
  ect
};


Lisa_Order* makeECT(int& ops, Lisa_OsProblem *os_problem);
Lisa_Order* makeQueenSweep(int& ops, Lisa_OsProblem *os_problem);
Lisa_Order* makeOrder(InsertionOrder iord, int& ops, Lisa_OsProblem *os_problem);



#endif
