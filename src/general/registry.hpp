/*
 * ************** registry.h *******************************
 * 
 * object which choses a set of suitable algorithms for a given problem
 *
 * Owner:
 *
 * 19.9.1998
*/

#ifndef _registry_h 
#define _registry_h 

#include "../lisa_dt/problem.hpp"
#include "../classify/classify.hpp"
#include <iostream.h>

/** @name Register for Choosing Algorithms
    Due to the huge amount of different problem settings in LiSA, it is 
    difficult to provide the correct sub-menues in Algorithms - Heuristics, 
    Algorithms - Exact and so on for each case. \\

    The class Lisa_SuitableAlgorithms provides two list of algorithms which 
    can be used to solve a given problem heuristicly or exactly, respectively.
    To accomplish this, for each algorithm there is an object Lisa_Scope, 
    which contains together with an id of the algorithm a set of maximal 
    problems solved optimally / heuristicly by this algorithm. \\
    
    Given an instance of Lisa_Problem and a set of instances of Lisa_Scope, 
    Lisa_SuitableAlgorithms uses the compare-function of module classify 
    to test for each registered algorithm wether it is suitable to solve 
    this problem.

    @author Thomas Tautenhahn
    @version 2.3pre1
    @see Lisa_GenericList
    @see Lisa_Problem
    @see Lisa_compare
*/ 

class Lisa_SuitableAlgorithms
 {
  private:
    Lisa_GenericList* algorithms;        
  public: 
    /// constructs only with given set of algorithms 
    Lisa_SuitableAlgorithms(string filename ?);
    /// lists of suitable algorithms
    Lisa_GenericList* exact;
    Lisa_GenericList* heuristics:
    /// generate these two lists according to given problem:
    void generate_lists(Lisa_Problem*);
    ~Lisa_SuitableAlgorithms();
 }
    









