
#ifndef irredtest_h
#define irredtest_h

#include <time.h>
#include <math.h>

#include "../../main/global.hpp"
#include "../../basics/matrix.hpp"
#include "../../basics/list.hpp"
#include "../../basics/order.hpp"
#include "../../basics/graph.hpp"

#include "graphalgo.hpp"
#include "lpartition.hpp"
#include "irrednode.hpp"

///Just runs the irreducibility test.
/**
  This algorithm checks if a given schedule for a problem is irreducible. A 
  schedule S* reduces another schedule S if its objective is better than that
  for S for any given set of processing types. S is therefore irreducible if no
  such schedule S* exists.
  
  It works for open, job and flow shop problems, in combination with release 
  dates and for the Cmax, Lmax, SumCi, SumWiCi, SumUi, SumWiUi, SumTi and 
  SumWiTi objectives.

  A plangraph is split up into implication classes. By turning them around and 
  recombining them new plans are generated. Those can be similar to or may 
  reduce the original plan. Results can be send to a list for storage and later
  retrivial.

  @author Marc Moerig
  @version 2.3final
*/
class Lisa_IrreducibilityTest : public Lisa_GenericObject{
private:
  // how many vertice ??
  int vert;
  // the disjuntive graph for the current problem
  Lisa_MatrixListGraph* disjkt;
  // store results in here
  Lisa_IrredResult* result;
  
public:


  /// Flags for the algorithm.
  enum{GENERATE_ALL /** Generate all plangraphs.*/ =0
      ,JUST_TEST /// Abort if one reducing plangraph was found.
      ,JUST_TEST_RANDOM /** Abort if one reducing plangraph was found. 
                         Generate plangraphs in random order. */
      ,GENERATE_SIMILAR /** Generate all plangraphs similar to the input 
                         plangraph. No reducing plans will be generated. */     
      };
    
  /// constructor
  Lisa_IrreducibilityTest(Lisa_MatrixListGraph* disjkt_in);
  /// destructor
  ~Lisa_IrreducibilityTest();
  
  /// Test this plan for irreducibility.
  /** Run the algorithm for a given plangraph, the corresponding comparability 
      graph and a parameter indicating what to do. */ 
  bool test(Lisa_MatrixListGraph* plan,Lisa_MatrixListGraph* comp,const int param);
  
  /// Test this plan for irreducibility.
  /** Run the algorithm for a given plangraph and a parameter indicating what 
      to do. The corresponding comparability graph will be generated.*/ 
  inline bool test(Lisa_MatrixListGraph* plan,const int param){
    
    Lisa_MatrixListGraph comp(vert);
    Lisa_MatrixListGraphAlgorithms::build_compgraph(plan,&comp);
    
    return test(plan,&comp,param);
  }

  /// set an output list if results are wanted
  void set_output_to(Lisa_IrredResult* res=0);

  /// Write the object data to a stream.
  /** This is mostly used for debugging. */
  void write(std::ostream& strm = std::cout) const;
};

#endif

