/** @name Reduction Trees
				This section contains the reduction tree class. 
				It provides functionality to obtain a particular subgarph of the reduction graph
*/
//@{

#ifndef LISA_REDUCTION_HPP_
#define LISA_REDUCTION_HPP_

#include "../main/global.hpp"
#include "../lisa/ptype.hpp"

#include "../basics/graph.hpp"

#include <vector>

/** An abstract base class for all polynomial time transforms.
				All derived classes can be used as either forward or backards reductions.
				A single object of a derived class can be used for multiple transforms.
				You therefore may want to declare it as a static variable and hand it over to
				the constructor of the Lisa_Transform object.
    @author Jan Tusch
    @version 2.3pre3
				@see Lisa_Transform
				@see Lisa_ReductionTree
*/
class Lisa_AtomicTransform {
		
 public:
		/**	Direction of a transform.
			*/
		enum TransformDir {
				FORWARD, /// forward transform hint
				BACKWARDS /// backwards transform hint
		};
		
		/** Container for objects handed over to a transform operation, 
						which are altered by the particular transform.
			*/
		typedef struct {
				Lisa_ProblemType* transf_problem;
				const Lisa_ProblemType* target_problem;
				Lisa_Values* values;
				Lisa_Schedule* schedule;
		} TransformObjects;
		

		/**Implementation interface for a polynomial time transform.
					Your derived class will implement this function such that 
					the given instance is altered according to the given transform pattern.
					@param g number of the graph, e.g. M_ENV
					@param s source of the reduction, e.g. F
					@param t target of the reduction, e.g. J
					@param to instance to be transformed
					@param dir direction of desired transform
					@return true on success, false otherwise
			*/
		virtual bool transform(int g ,int s,int t,
																									TransformObjects& to,
																									TransformDir dir)  = 0;
		
};

/** A class representing the implementation of polynomial time reduction.
				It contains pointers to a forward and backwards transform.
				It is used to register a reduction implementation within the reduction tree class.
    @author Jan Tusch
    @version 2.3pre3
				@see Lisa_ReductionTree
*/
class Lisa_Transform {
		
 public:
		/// measurement of the expenditure of operations neccessary to perform the transform
		int costs;
		/// pointer to the object responsible for forward transform
		Lisa_AtomicTransform* forward;
		/// pointer to the object responsible for backwards transform
		Lisa_AtomicTransform* backwards;
		

		/** Contructor of the transform object.
						You should provide pointers of "real" objects here, i.e. not NULL.
						Usually these are static objects of some transform implementation.
						You also should make the constructed object a static one.
						It is used to be registered within the Lisa_ReductionTree class.
			*/
		explicit Lisa_Transform(Lisa_AtomicTransform* forwardTransform, 
																										Lisa_AtomicTransform* backwardsTransform,
																										int transformCosts = 0)
		{
				forward = forwardTransform;
				backwards = backwardsTransform;
				costs = transformCosts;
		}
};

/** Reduction Tree class.  This class represents a polynomial time reduction tree
				for a given property signature.
    @author Jan Tusch
    @version 2.3pre3
*/

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

		/** Register a transform for a given scheduling property signature.
						@param g ID of the reduction graph 
						@param s ID of the reduction source within g
						@param t ID of the reduction target within g
						@param tf pointer to transform object (prefereably use static objects here)
			*/
		static void registerTransform(int g,int s,int t,Lisa_Transform* tf);

		/// default constructor - creates an insane tree
		Lisa_ReductionTree();
		
		/// attempt to build a reduction tree
		Lisa_ReductionTree(int Graph, int source, int target);
		
		/// copy contructor
		Lisa_ReductionTree(const Lisa_ReductionTree& other);
		
		/// assignment operator
		const Lisa_ReductionTree& operator=(const Lisa_ReductionTree&);
		
		/** Get graph identifier passed at construction time if tree is sane
      @return 0 <= graph < TUPEL_INDEX if sane, -1 otherwise
						\Label{get_graph}
  */
		int getGraph_ID() const { return graph_ID;}
		/** Get source identifier passed at construction time if tree is sane
      @return 0 <= source < NUMBER[getGraph_ID()]  if sane, -1 otherwise
  */
		int getRoot_ID() const { return root_ID;}
		
		/** Get target identifier passed at construction time if tree is sane
      @return 0 <= target < NUMBER[getGraph_ID()]  if sane, -1 otherwise
  */
		int getLeaf_ID() const { return leaf_ID;}
		
		/** Get the actual reduction graph encoded in a Lisa_Graph
						Note : Lisa_Graph uses vertex identifiers from {1,...,n}, i.e. 
						the vertex number of the root is : getRoot_ID() + 1
      @return reduction graph  if sane, NULL otherwise
  */
		const Lisa_Graph* getGraph() const {return g;};
		

		/** Get a pointer to the transform object for the given nodes 
						responsible for polynomial reduction.
						Note : Here the node numbers of the graph have to be given.
						These can be obtained from meber functions of the Lisa_Graph class.
						@param source_node node index of the source of the reduction within the current graph
						@param target_node node index of the target of the reduction within the current graph
						@returns Transform Object Pointer if present and tree is sane, NULL otherwise
  */
		Lisa_Transform* getTransform(int source_node, int target_node) const;
		
		/** Function to perform polynomial time transforms according to a given
						path within the reduction graph, where the pairs correspont to edges in the reduction
						graph (node indices). Note: The first and the last node of the sequence
						have to correspond to the root and leaf of the tree, respectively.
						@param nodes transform sequence given by node indices of the current graph.
						@param to instance to be transformed (Notice that it is altered in place!)
						@return true if all transforms complete successfully, false otherwise
		*/
		bool forwardTransform(Lisa_List<Lisa_Pair> &nodes,
																								Lisa_AtomicTransform::TransformObjects& to) const;
		

		
		/** Function to perform polynomial time transforms according to a given
						path within the reduction graph, where the pairs correspont to edges in the reduction
						graph (node indices). Note: The first and the last node of the sequence
						have to correspond to the root and leaf of the tree, respectively.
						@param nodes transform sequence given by node indices of the current graph.
						@param to instance to be transformed (Notice that it is altered in place!)
						@return true if all transforms complete successfully, false otherwise
		*/
		bool backwardsTransform(Lisa_List<Lisa_Pair> &nodes,
																										Lisa_AtomicTransform::TransformObjects& to) const;
		
		/// destructor
		~Lisa_ReductionTree();
		
		/** Determine sanity of the reduction graph
						The tree is sane if reductions and transforms were found at construction time
						Note : If source == target the tree is sane but the graph is NULL
						@return true if sane, false otherwise
  */
		bool isSane() const {return valid;}
		
		///output the tree
		virtual void write(ostream&) const;
};

#endif
//@}
