
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
  /** Comparison of two problems concerning the complexity status.  This 
      function takes two problems as input and returns the relation between 
      these problems with respect to polynomial reducibility. 
      @param Lisa_Problem first input problem object
      @param Lisa_Problem second input problem object
      @return IDENT, FIRST_TO_SECOND, SECOND_TO_FIRST, NOT_CMP
  */
  static int compare(const Lisa_ProblemType& ,const Lisa_ProblemType&);
		
		/** Obtain the reduction trees of a given pair of problems for each property graph.
						If a graph does not provide reduction for the given problems its entry is set to NULL
						Note : The deletion of the trees is on behalf of the caller. You should check for
						redicbility of the problems with the reducible(...) function in advance.
						@param Lisa_Problem source problem 
      @param Lisa_Problem target problem
						@param Lisa_Problem array of pojnters to reduction trees
  */
		static void getReductions(const Lisa_ProblemType&,
																					const Lisa_ProblemType&,
																					const Lisa_ReductionTree* [TUPEL_INDEX]);
		
		friend class Lisa_ReductionTree;
		
};

#endif

