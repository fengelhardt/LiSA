/*
 * ************** irrednode.hpp *******************************
 *
 * Owner: Marc Moerig
 *
 * 08.06.01
 * last changed 08.06.01
 */


#ifndef irrednode_h
#define irrednode_h

#include "../../main/global.hpp"
#include "../../basics/list.hpp"
#include "../../basics/graph.hpp"
#include "graphalgo.hpp"

/** @name Holding Results In A List
  The test algorithm generates plangraphs, which can be saved and returned later. 
  These object helps to hold these results. Additionaly they help to filter plangraphs
  which are known to be reducible so those can be deleted. This saves alot of time and memory.
*/
//@{
/**
  A node can hold the plangraph and the comparability graph for a given schedule so they can be put in a list.
  
  @see Lisa_IrredResult
 */
class Lisa_IrredNode : public Lisa_GenericObject{
public:
  /**status flag ...\\ It indicates that the status of the graph represented by this node is unknown. */
  const static int UNKNOWN = 1;
  /**status flag ...\\ It indicates that the graph represented by this node is irreducible. */
  const static int IRREDUCIBLE = 2;
  /**status flag ...\\ It indicates that the graph represented by this node is not irreducible. */
  const static int NOT_IRREDUCIBLE = 3;

  /// the status
  int status;

  /// graph representing our schedule
  Lisa_Graph* plangraph;
  /// the comparability graph for that schedule
  Lisa_Graph* compgraph;
  
  /** constructor ...\\ Create a new node. */
  Lisa_IrredNode(Lisa_Graph* plangraph_in, Lisa_Graph* compgraph_in, const int status_in);

  /** destructor ...\\  Delete a node. */
  ~Lisa_IrredNode();

  /** write node to a stream ...\\ This is mostly used for debugging. */
  void write(ostream& strm = cout) const;
};

/**
 This object holds result graphs in a list. 
 It can also filter and delete unwanted results by comparing them to each other.
 
 @see Lisa_IrredNode
*/
class Lisa_IrredResult : public Lisa_GenericObject{
private: 
  const int ins_method;
public:
 
  /** filter flag ...\\  Keep one plan for each similarity class. */
  static const int ALL = 0;
  /** filter flag ...\\ Keep only plans which have not been detected to 
      be reducible. This increases computation speed alot and 
      more important saves tones of ram.  */
  static const int POTENTIALLY_IRREDUCIBLE = 1;
  /** filter flag ...\\  Do not filter at all. */
  static const int NO_FILTER = 2;

  /// list that stores the created plans
  Lisa_List<Lisa_IrredNode*>* results;

  /** constructor ...\\ Create a new list with a given filter method. */
  Lisa_IrredResult(const int ins_method_in);
  
  /// destructor
  ~Lisa_IrredResult();

  /** inserts a new plangraph into the list ...\\
      Filter rules apply */
  void insert(Lisa_Graph* plangraph,Lisa_Graph* compgraph);

  /** compares all graphs in the list with each other ...\\
      This may classify some graphs to be reducible. */
  void compare_all();

  /** delete all plangraphs from the list which are known to be reducible */
  void delete_reducible();

  /** writes the object to a stream ...\\
      Mostly used for debugging */
  void write(ostream& strm = cout)const;
};

//@}

#endif
