/*
 * ************** convertgraph.hpp *******************************
 *
 * this class will create the graph to a given plan and problem type and vice versa 
 *
 * @author Marc Moerig
 * @version 2.3pre3
 *
 * 15.07.01
 * last changed 15.07.01
 */

#ifndef convertgraph_h
#define convertgraph_h

#include "../../misc/except.hpp"
#include "../../basics/matrix.hpp"
#include "../../basics/graph.hpp"
#include "../../scheduling/mo_jo.hpp"
#include "../../lisa/ptype.hpp"
#include "graphalgo.hpp"

/** @name Create The Corresponding Graph's For A Problem
    
    The irreducibility test runs completly on graphs. Therefore 
    we need to create the disjunctive graph for a given problem
    and we need a tool to convert schedules to to graphs and vice versa.

*/

//@{
/** convert schedules to graphs ...\\

    This object can create the disjunctive graph describing a problem.
    It also serves as tool to convert schedules for that problem into 
    graphs and vice versa.

*/
class Lisa_ConvertGraph : public Lisa_GenericObject{
private:
  // possible problemtypes
  const static int O_CMAX = 1;
  const static int J_CMAX = 2;
  const static int O_RI_CMAX = 3;
  const static int J_RI_CMAX = 4;
  const static int O_LMAX = 5;
  const static int J_LMAX = 6;
  const static int O_RI_LMAX = 7;
  const static int J_RI_LMAX = 8;

  // which problemtype do we have ?
  const int curr_pt;
  // size of our plan, number of operations==vertice in disjkt/plangraph
  const int m;
  const int n;
  int vert,morevert;

  // which vertice belongs to plan(i,j) ?
  Lisa_Matrix<int>* lookup;
  // at which coordinate is vertice v ?
  Lisa_Vector<int>* I_lookup;
  Lisa_Vector<int>* J_lookup;
  // the disjkt graph
  Lisa_Graph* disjkt;
  
  // intern method's ... one for each different problemtype we can handle
  void initialize_J_CMAX(Lisa_Matrix<bool>* SIJ,Lisa_MO* MO);
  void initialize_O_CMAX(Lisa_Matrix<bool>* SIJ);
  void plan2graph_O_CMAX(Lisa_Matrix<int>* plan,Lisa_Graph* plangraph);

  void initialize_J_RI_CMAX(Lisa_Matrix<bool>* SIJ,Lisa_MO* MO);  //!!!
  void initialize_O_RI_CMAX(Lisa_Matrix<bool>* SIJ);
  void plan2graph_J_RI_CMAX(Lisa_Matrix<int>* plan,Lisa_Graph* plangraph); // !!!
  void plan2graph_O_RI_CMAX(Lisa_Matrix<int>* plan,Lisa_Graph* plangraph); 

  void initialize_J_LMAX(Lisa_Matrix<bool>* SIJ,Lisa_MO* MO); // !!!
  void initialize_O_LMAX(Lisa_Matrix<bool>* SIJ);
  void plan2graph_J_LMAX(Lisa_Matrix<int>* plan,Lisa_Graph* plangraph); //!!!
  void plan2graph_O_LMAX(Lisa_Matrix<int>* plan,Lisa_Graph* plangraph);

  void initialize_J_RI_LMAX(Lisa_Matrix<bool>* SIJ,Lisa_MO* MO); // !!!
  void initialize_O_RI_LMAX(Lisa_Matrix<bool>* SIJ);
  void plan2graph_J_RI_LMAX(Lisa_Matrix<int>* plan,Lisa_Graph* plangraph); // !!!
  void plan2graph_O_RI_LMAX(Lisa_Matrix<int>* plan,Lisa_Graph* plangraph); 

  /// constructor
  Lisa_ConvertGraph(const int new_pt,Lisa_Matrix<bool>* SIJ,Lisa_MO* MO=0);

public:

  /** create an object ...\\

      This method try's to create an converting Object for the given Problemype. It returns 0 
      if that is not possible cause either a MO is needed for the current problemtype
      or the problemtype is not supported. Additionally an exception is thrown */
  static Lisa_ConvertGraph* make_object(Lisa_ProblemType* pt,Lisa_Matrix<bool>* SIJ,Lisa_MO* MO=0);
  
  /// destructor
  ~Lisa_ConvertGraph();

  /// returns a pointer to the disjunctive graph describing the Problem
  Lisa_Graph*  get_disjkt();
  
  /// converts a plan of the current problem to the according plangraph
  void plan2graph(Lisa_Matrix<int>* plan,Lisa_Graph* plangraph);
  
  /// converts a plangraph back to the plan 
  void graph2plan(Lisa_Graph* plangraph,Lisa_Matrix<int>* plan);
  
  /**  sends the object data to a stream ...\\   
       This data is usually only interesting for debugging.*/
  void write(ostream& strm = cout) const;

};

//@}

#endif

