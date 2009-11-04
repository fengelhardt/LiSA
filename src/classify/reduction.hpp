/** @name Reduction Trees
    This section contains the reduction tree class. 
    It provides functionality to obtain a particular subgarph of the reduction graph
*/
//@{

#ifndef LISA_REDUCTION_HPP_
#define LISA_REDUCTION_HPP_

#include "../main/global.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"

#include "../basics/graph.hpp"

#include <vector>

//@{
/** All derived classes can be used as forward and/or backards reductions.
 *  A single object of a derived class can be used for multiple transforms.
 *  You therefore may want to declare it as a static variable and hand it over to
 *  the constructor of the Lisa_Transform object.
 *  
 *  @author Jan Tusch
 *  @version 3.0pre1
 *  @see Lisa_Transform
 *  @see Lisa_ReductionTree
 */
/// An abstract base class for all polynomial time transforms.
class Lisa_AtomicTransform {
		
 public:
  /// Direction of a transform.
  enum TransformDir {
    /// forward transform hint
    FORWARD, 
    /// backwards transform hint
    BACKWARDS
  };
  
  ///  Container for objects handed over to a transform operation.
  typedef struct {
    /// Pointer to problem type information (altered)
    Lisa_ProblemType* transf_problem;
    /// Pointer to desired problem type information (not altered)
    const Lisa_ProblemType* target_problem;
    /// Pointer to values to be transformed (altered)
    Lisa_Values* values;
    /// Pointer to a schedule beeing transformed (altered)
    Lisa_Schedule* schedule;
  } TransformObjects;
		

  /** Your derived class will implement this function such that 
   * the given instance is altered according to the given transform pattern.
   * @param g number of the graph, e.g. #M_ENV
   * @param s source of the reduction, e.g. #F
   * @param t target of the reduction, e.g. #J
   * @param to instance to be transformed
   * @param dir direction of desired transform
   * @return true on success, false otherwise
  */
  /// Implementation interface for a polynomial time transform.
  virtual bool transform(int g ,int s,int t,
			 TransformObjects& to,
			 TransformDir dir)  = 0;

  virtual ~Lisa_AtomicTransform(){}
		
};

//@}

//@{
/** It contains pointers to a forward and backwards transform.
 *  It is used to register a reduction implementation within the reduction tree class.
 *
 *  @author Jan Tusch
 *  @version 3.0pre1
 *  
 *  @see Lisa_ReductionTree
 */
/// A class representing the implementation of polynomial time reduction.
class Lisa_Transform {
		
 public:
  /// Measurement of the expenditure of operations neccessary to perform the transform
  int costs;
  /// Pointer to the object responsible for forward transform
  Lisa_AtomicTransform* forward;
  /// Pointer to the object responsible for backwards transform
  Lisa_AtomicTransform* backwards;
		

  /** You should provide pointers of "real" objects here, i.e. not NULL.
   *  Usually these are static objects of some transform implementation.
   *  You also should make the constructed object a static one.
   *  It is used to be registered within the Lisa_ReductionTree class.
   *  @param forwardTransform Pointer to Lisa_AtomicTransform performing forward transform
   *  @param backwardsTransform Pointer to Lisa_AtomicTransform performing backwards transform
   *  @param transformCosts Gives a hint how costly the transform is 
  */
  /// Contructor of the transform object.
  explicit Lisa_Transform(Lisa_AtomicTransform* forwardTransform, 
			  Lisa_AtomicTransform* backwardsTransform,
			  int transformCosts = 0)
  {
    forward = forwardTransform;
    backwards = backwardsTransform;
    costs = transformCosts;
  }
};

//@}



//@{

/** 
 *  This class represents a polynomial time reduction graph for a given property signature.\n
 *  It is similar to the Lisa_RedGraph class but provides more information.\n
 *  For a certain reduction graph (e.g. the precedence constraints) it stores the possible transitions
 *  between a pair of property types (e.g. [chain,tree]).\n
 *  The signature of the reduction constists of the identifiers of the graph and the transition property types.
 *  It is specified at construction time and fixed afterwards.
 *  You can obtain the graph storing the transition paths.\n
 *  However, the graph containes only edges for transitions for which an implementaion of the actual transform is provided.\n
 *  If there is at least one path from the source to the target property type the object is called sane.
 *  Consequently, it is possible to build an insane reduction graph object for a signature beeing polynomial reducible.
 *
 *
 *
 *  @author Jan Tusch
 *  @version 3.0pre1
 */
/// Reduction Graph for a particuler problem signature transition 
class Lisa_ReductionTree : public Lisa_GenericObject {
		
 private:
  /// sanity flag
  bool valid;
  /// the reduction graph
  Lisa_Graph* g;
  /// the property the tree corresponds to
  int graph_ID;
  /// the source of the reduction
  int root_ID;
  /// the target of the reduction
  int leaf_ID;

  /// recursive path traversal
  static bool visit(int,int,int);
		
  /// flag for the static initializer
  static bool initialized;
		
  /// map containing registerd transforms
  static Lisa_Transform** TransformGraph[TUPEL_INDEX];
		

 public:
		
  /// Inititializer of the transform map
  static void init();

  /** 
   *  @param graph ID of the reduction graph (e.g. #PRECEDENCE) 
   *  @param source ID of the reduction source within g (e.g. #IN_TREE)
   *  @param target ID of the reduction target within g (e.g. #TREE)
   *  @param T pointer to transform object (prefereably use static objects here)
   */
  /// Register a transform for a given scheduling property signature.
  static void registerTransform(int graph ,int source, int target, Lisa_Transform* T);
  
  /// default constructor - creates an insane tree
  Lisa_ReductionTree();
  
  /** 
   *  @param graph ID of the reduction graph (e.g. #PRECEDENCE) 
   *  @param source ID of the reduction source within g (e.g. #CHAIN)
   *  @param target ID of the reduction target within g (e.g. #TREE)
   */
  /// Attempt to build a reduction graph
  Lisa_ReductionTree(int graph, int source, int target);
		
  /// Copy constructor
  Lisa_ReductionTree(const Lisa_ReductionTree& other);
		
  /// Assignment operator
  const Lisa_ReductionTree& operator=(const Lisa_ReductionTree&);
		
  /** 
   * @return The graph ID in the range from 0 to #TUPEL_INDEX if sane, -1 otherwise
  */
  /// Get graph identifier passed at construction time if tree is sane.
  int getGraph_ID() const { return graph_ID;}
 
  /** 
   * @return The source ID in the range from 0 to #NUMBER[getGraph_ID()] if sane, -1 otherwise
   */
  /// Get source identifier passed at construction time if tree is sane.
  int getRoot_ID() const { return root_ID;}
		
  /** 
   * @return The target ID in the range from 0 to #NUMBER[getGraph_ID()] if sane, -1 otherwise
  */
  /// Get target identifier passed at construction time if tree is sane.
  int getLeaf_ID() const { return leaf_ID;}
		
  /** 
   * Note : Lisa_Graph uses vertex identifiers from {1,...,n}, i.e. 
   * you can access the root in the graph with the vertex number getRoot_ID() + 1
   * @return Pointer to internal reduction graph if sane, NULL otherwise
   */
  /// Get the actual reduction graph encoded in a Lisa_Graph.
  const Lisa_Graph* getGraph() const {return g;};
		

  /** 
   *  If present this object performs polynomial reduction transform.
   *  Note : Here the node numbers of the graph have to be given.
   *  These can be obtained from meber functions of the Lisa_Graph class.
   *  @param source_node vertex number of the source of the reduction within the current graph
   *  @param target_node vertex number of the target of the reduction within the current graph
   *  @returns Transform Object Pointer if present and tree is sane, NULL otherwise
   */
  /// Get a pointer to the transform object for the given directed edge.
  Lisa_Transform* getTransform(int source_node, int target_node) const;
		
  /** It launches a chain of transform operations for a given
   *  path within the reduction graph, where the node pairs correspont to edges in the reduction
   *  graph (vertex numbers).\n
   *  Note: The first and the last node of the sequence
   *  have to correspond to the root and leaf of the tree, respectively.
   *  @param nodes transform sequence given by node indices of the current graph.
   *  @param to instance to be transformed (Notice that it is altered in place!)
   *  @return true if all transforms complete successfully, false otherwise
   */
  /// Function to perform polynomial time forward transforms.
  bool forwardTransform(Lisa_List<Lisa_Pair> &nodes,
			Lisa_AtomicTransform::TransformObjects& to) const;
  
  /** It launches a chain of transform operations for a given
   *  path within the reduction graph, where the node pairs correspont to edges in the reduction
   *  graph (vertex numbers). \n 
   *  Note: The first and the last node of the sequence
   *  have to correspond to the root and leaf of the tree, respectively.
   *  @param nodes transform sequence given by node indices of the current graph.
   *  @param to instance to be transformed (Notice that it is altered in place!)
   *  @return true if all transforms complete successfully, false otherwise
   */
  /// Function to perform polynomial time backwards transforms.
  bool backwardsTransform(Lisa_List<Lisa_Pair> &nodes,
			  Lisa_AtomicTransform::TransformObjects& to) const;
		
  /// Destructor
  virtual ~Lisa_ReductionTree();
  
  /** Determine sanity of the reduction graph
   *  The tree is sane if reductions and transforms were found at construction time
   *  @return True if object sane, false otherwise
  */
  bool isSane() const {return valid;}
		
  ///output the tree to a stream
  virtual void write(std::ostream&) const;
};


//@}

#endif

