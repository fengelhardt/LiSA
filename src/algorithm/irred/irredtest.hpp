/*
 * ************** irredtest.hpp *******************************
 *
 * Owner: Marc Moerig
 *
 * 27.07.01
 * last changed 27.07.01
 */

#ifndef irredtest_h
#define irredtest_h

#include <time.h>
#include <math.h>
#include "../../basics/global.hpp"
#include "../../basics/matrix.hpp"
#include "../../basics/list.hpp"
#include "../../basics/order.hpp"
#include "../../basics/graph.hpp"
#include "graphalgo.hpp"
#include "lpartition.hpp"
#include "irrednode.hpp"
/** @name The Test Algorithm
 
  This object runs the irreducibility test.
  
  A plangraph is split up into implication classes. By turning them around and recombining them 
  new plans are generated. Those can be similar to or may reduce the original plan. Results can be send
  to a list for storage and later retrivial.
*/
//@{
/**
  This object just runs the irreducibility test.
*/
class Lisa_IrreducibilityTest : public Lisa_GenericObject{
private:
  // how many vertice ??
  int vert;
  // the disjuntive graph for the current problem
  Lisa_Graph* disjkt;
  // store results in here
  Lisa_IrredResult* result;
  
public:
 
  /** flag for the algorithm ... \\ Generate all plangraphs. */
  const static int GENERATE_ALL = 0;
  /** flag for the algorithm ... \\ Abort if one reducing plangraph was found. */
  const static int JUST_TEST = 1;
  /** flag for the algorithm ... \\ Abort if one reducing plangraph was found. Generate plangraphs in random order. */
  const static int JUST_TEST_RANDOM = 2;
  /** flag for the algorithm ... \\ 
      Generate all plangraphs similar to the input plangraph. No reducing plans will be generated. */
  const static int GENERATE_SIMILAR = 3;

  /// constructor
  Lisa_IrreducibilityTest(Lisa_Graph* disjkt_in);
  /// destructor
  ~Lisa_IrreducibilityTest();
  
  /** test this plan for irreducibility ...\\
      Run the algorithm for a given plangraph, the corresponding comparability graph
      and a parameter indicating what to do. */ 
  bool test(Lisa_Graph* plan,Lisa_Graph* comp,const int param);
  
  /** test this plan for irreducibility ...\\
      Run the algorithm for a given plangraph and a parameter indicating what to do. 
      The corresponding comparability graph will be generated.*/ 
  inline bool test(Lisa_Graph* plan,const int param){
    
    Lisa_Graph comp(vert);
    Lisa_GraphAlgorithms::build_compgraph(plan,&comp);
    
    return test(plan,&comp,param);
  }

  /// set an output list if results are wanted
  void set_output_to(Lisa_IrredResult* res=0);

  /** write the object data to a stream ...\\
      This is mostly used for debugging */
  void write(ostream& strm = cout) const;
};

//@}

#endif
