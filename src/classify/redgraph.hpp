
#ifndef _redgraph_h
#define _redgraph_h

#include "../main/global.hpp"
#include "../lisa/ptype.hpp"

#include "../basics/graph.hpp"
#include "reduction.hpp"
#include <vector>


/// used for indication of terminal nodes graphs
//const int MY_NULL=100;
/// maximal number of successors for each node of reduction graph
//const int SUCC_MAX=4;
/// maximal number of nodes of reduction graph
//const int V_MAX=25;

/// flags: how are problems related
enum{
  /// the second problem polynomially reduces to the first problem 
  SECOND_TO_FIRST = -1,
  /// the problems are not comparable  
  NOT_CMP,
  /// the first problem polynomially reduces to the second problem
  FIRST_TO_SECOND,
  /// the problems are identical
  IDENT
};

/// reduction graph class
/** This class contains the reduction graphs
 *  as well as methods for evaluating the complexity status of input
 *  problem objects with the aid of the reduction graphs.
 *  
 *  @author Martin Harborth , Jan Tusch
 *  @version 2.3final
 * 
 */

/*
    changes by Jan Tusch - 10-08-2003
    - made Graph static to avoid its repetive construction
    and static initializers
    - made Graph a lookup table instead of listed trees
    - added consts for whole class
    - made all inputs const references
    - added friend class Lisa_ReductionTree
    - added methods to retrieve reduction trees for given problem pair
*/ 


class Lisa_RedGraph
{ 
 private:
  /// contains the reduction graphs
  static unsigned char* Graph[TUPEL_INDEX];
  /// initiaializes reduction graphs
  static void initializeGraph();
  /// flag to avoid repetitive graph construction
  static bool isInitialized;
  /// for recursive visits of nodes
  static int visit(int,int,int);    
  /// returns TRUE if first tupel is polynomial reducible to second tupel
  static int reducible(const Lisa_ProblemType&, const Lisa_ProblemType&);
		
 public: 
  /// output of reduction graphs 
  static int output(void);             
  /** 
   *  This function takes two problems as input and returns the relation between 
   *  these problems with respect to polynomial reducibility. 
   *  @param first input problem object
   *  @param second input problem object
   *  @return One of #IDENT, #FIRST_TO_SECOND, #SECOND_TO_FIRST, #NOT_CMP
   */
  /// Comparison of two problems concerning the complexity status.  
  static int compare(const Lisa_ProblemType& first ,const Lisa_ProblemType& second);
		
  /** 
   *  If a graph does not provide reduction for the given problems its entry is set to NULL.
			*  In this case there either is no polynomial time reduction or the requiered 
			*  transform is not implemented.
   *  Note : The deletion of the trees is on behalf of the caller. You should check for
   *  redicbility of the problems with the compare() function in advance.
   *  @param source problem 
   *  @param target problem
   *  @param rts array of pojnters to reduction trees
			*  @return index of the first NULL entry or -1 if all trees are sane
   */
  /// Obtain the reduction trees for a given pair of problems for each property graph.
  static int getReductions(const Lisa_ProblemType& source,
			    const Lisa_ProblemType& target,
			    const Lisa_ReductionTree* rts[TUPEL_INDEX]);
		
  friend class Lisa_ReductionTree;
		
};

#endif

