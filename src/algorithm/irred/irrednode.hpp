
#ifndef irrednode_h
#define irrednode_h

#include "../../main/global.hpp"
#include "../../basics/list.hpp"
#include "../../basics/graph.hpp"
#include "graphalgo.hpp"

/// Hold results of an irreducibilitytest.
/** A node can hold the plangraph and the comparability graph for a given 
    schedule so they can be put in a list.
  
    @see Lisa_IrredResult
    @version 2.3final
    @author Marc Moerig

 */
class Lisa_IrredNode : public Lisa_GenericObject{
public:

  /// status flag
  enum{UNKNOWN /** Indicates that the graph of this node is unknown.*/ =1
      ,IRREDUCIBLE /// Indicates that the graph of this node is irreducible.
      ,NOT_IRREDUCIBLE /// Indicates that the graph of this node is not irreducible. 
      };
  
  /// the status
  int status;

  /// graph representing our schedule
  Lisa_MatrixListGraph* plangraph;
  /// the comparability graph for that schedule
  Lisa_MatrixListGraph* compgraph;
  
  /// Constructor ... Create a new node.
  Lisa_IrredNode(Lisa_MatrixListGraph* plangraph_in, Lisa_MatrixListGraph* compgraph_in, 
                 const int status_in);

  /// Destructor ...  Delete a node.
  ~Lisa_IrredNode();

  /// write node to a stream ... This is mostly used for debugging.
  void write(std::ostream& strm = std::cout) const;
};

/// Hold results of an irreducibility test
/** The test algorithm generates plangraphs, which can be saved and returned 
    later. These object helps to hold these results. Additionaly they help to 
    filter plangraphs which are known to be reducible so those can be deleted. 
    This saves alot of time and memory.
 
    This object holds result graphs in a list. It can also filter and delete 
    unwanted results by comparing them to each other.
 
    @see Lisa_IrredNode
    @version 2.3final
    @author Marc Moerig
*/
class Lisa_IrredResult : public Lisa_GenericObject{
private:
  /// applied filter flag 
  const int ins_method;
public:

  /// filter flag
  enum{ALL/** Keep one plan for each similarity class. */=0
      ,POTENTIALLY_IRREDUCIBLE /** Keep only plans which have not been detected 
             to be reducible. This increases computation speed alot 
             and more important saves tones of ram.*/
      ,NO_FILTER /** Do not filter at all. */
      };
  
  /// list that stores the created plans
  Lisa_List<Lisa_IrredNode*>* results;

  /// constructor ... Create a new list with a given filter method.
  Lisa_IrredResult(const int ins_method_in);
  
  /// destructor
  ~Lisa_IrredResult();

  /// inserts a new plangraph into the list ... Filter rules apply
  void insert(Lisa_MatrixListGraph* plangraph,Lisa_MatrixListGraph* compgraph);

  /// compares all graphs in the list with each other
  /** This may classify some graphs to be reducible. */
  void compare_all();

  /// delete all plangraphs from the list which are known to be reducible
  void delete_reducible();

  /// writes the object to a stream ...
  /** Mostly used for debugging */
  void write(std::ostream& strm = std::cout)const;
};

#endif

