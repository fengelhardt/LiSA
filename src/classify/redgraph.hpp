
#ifndef _redgraph_h
#define _redgraph_h

#include "../main/global.hpp"
#include "../lisa/ptype.hpp"

/// used for indication of terminal nodes graphs
const int MY_NULL=100;
/// maximal number of successors for each node of reduction graph
const int SUCC_MAX=4;
/// maximal number of nodes of reduction graph
const int V_MAX=25;

/// flags: how are problems related ?
enum{SECOND_TO_FIRST /** the second problem polynomially reduces to the first problem */ =-1
    ,NOT_CMP /// the problems are not comparable
    ,FIRST_TO_SECOND /// the first problem polynomially reduces to the second problem
    ,IDENT /// the problems are identical
    };

/// reduction graph class
/** This class contains the reduction graphs
    as well as methods for evaluating the complexity status of input
    problem objects with the aid of the reduction graphs.
    
    @author Martin Harborth 
    @version 2.3final
*/
class Lisa_RedGraph{ 
private:
  /// contains the reduction graphs
  int Graph[TUPEL_INDEX][V_MAX+1][SUCC_MAX]; 
  /// for recursive visits of nodes
  int visit(int,int,int);    
  /// returns true if first tupel is polynomial reducible to second tupel
  int reducible(Lisa_ProblemType*,Lisa_ProblemType*);

public: 
  /// reduction graph initialization
  Lisa_RedGraph(void);              
  /// output of reduction graphs 
  int output(void);             
  /// Comparison of two problems concerning the complexity status.
  /** This function takes two problems as input and returns the relation
      between these problems with respect to polynomial reducibility. 
      @param t1 first input problem object
      @param t2 second input problem object
      @return IDENT, FIRST_TO_SECOND, SECOND_TO_FIRST, NOT_CMP */
  int compare(Lisa_ProblemType* t1,Lisa_ProblemType* t2);  
};

#endif

