
#ifndef _m1_nb_sh_h
#define _m1_nb_sh_h

#include "../../../scheduling/m1_sched.hpp"
#include "../neighbor.hpp"
#include "../tabu.hpp"
#include "m1_api.hpp"

/// single machine SHIFT neighbourhood
/** This is the problem dependent SHIFT neighbourhood class for the single 
    machine problem, it swaps two arbitrary jobs on the machine.

    @author Andreas Winkler
    @version 2.3pre3
    @see Lisa_Neighborhood
    @see API_Neighborhood
*/
class shift_Neighborhood: public API_Neighborhood{
private: 
  /// private data needs documentation
  int pos1, pos2, inc_dec;

public:  
  /// construct shift_Neighborhood
  /** with a start schedule and specified problem data  */
  shift_Neighborhood(Lisa_1Schedule*,Lisa_1Problem*);

  int prepare_move(int );

  int do_move();
};

#endif




