/*
 * ************** bb_main.h *******************************
 * 
 * Doc++ start file for external module branch and bound
 *
 * @author Thomas Tautenhahn
 *
 * 19.9.1998
*/

#ifndef _lisa_bb_main_h_ 
#define _lisa_bb_main_h_ 

/** @name Branch and Bound 
    Branch and bound algorithms for several problems are collected in the 
    external module bb. Because the differences in the algorithm for different
    problems are bigger than the similarities, each branch and bound algorithm 
    is implemented separately (using LiSA schedule classes).\\
 
    Up to now only a simple branch and bound for O|r_i|* is implemented,
    where * stands for one of the regular objective functions defined in 
    global.h.

    An analogous algorithm for job shop is under way. 

    @author Thomas Tautenhahn
    @version 2.3pre3
*/ 
//@{
  //@Include: os_bb.h
//@}

#endif



