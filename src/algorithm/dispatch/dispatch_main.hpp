/*
 * ************** bb_main.h *******************************
 * 
 * Doc++ start file for external module branch and bound
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
*/

#ifndef _bb_main_h 
#define _bb_main_h 

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
    @version 2.3pre2
*/ 
\\@{
  \\@Include: os_bb.h
\\@}

#endif



