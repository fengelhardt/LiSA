#ifndef _B_NODE_H_
#define _B_NODE_H_

#include <fstream>

#include "../../main/global.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

#include "beam_search.hpp"

using namespace std;

/**  B_Node a modified Lisa_OsSchedule
     @author Jan Tusch & Andre Herms
     @see Lisa_OsSchedule
     @see KList
*/ 
//@{

class B_Node : public Lisa_OsSchedule {
  
  friend ostream & operator<<(ostream&, B_Node&); 
		
protected:
  B_Node *parent;
  TIMETYP cost;
  
  bool costValid;

  int lastAddedRow, lastAddedCol;
		
public:
  ///construct a schedule from another
  B_Node(B_Node *parent);
  ///construct a schedule that hands over the problem to its children
  B_Node(Lisa_OsProblem *);
  ///get costs of the (partial) schedule
  virtual TIMETYP getCosts(int obj, BeamSearch::CostFunc cf);
  ///for comparision
  //operator TIMETYP (void){return getCosts();}
  ///insert an operation in the partial schedule and remember this operation until insert is invoked again
  ///@see Lisa_OsSchedule
  int insert(int,int,int,int);

  ///set objective value
  //static void setObjective(int obj){destObjective = obj;}

  ///the problem
  Lisa_OsProblem * problem;
};


/** represents a container with capacity k and special functionality regarding insertion
    
@author Jan Tusch & Andre Herms
@see B_Node
*/

class KList {

public:
  B_Node **list;
  ///index of next free insertion position
  int in_list;
  ///position of the element with the maximum costValue overall elements
  int worst_in_list;
  int destObjective;
  BeamSearch::CostFunc costFunc;
  int k;
  ///constructs a KList with capacity k
  KList(int k, int obj, BeamSearch::CostFunc cf);
  ~KList();
  ///add a B_Node to the list if either the list is not full or there is a B_Node with larger costs in the list
  void add(B_Node *);
  ///add a B_Node to the list at position p, if there is no B_Node or a B_Node with larger costs at position p
  void add(B_Node *, int p);
  ///returns true if the capacity is exhausted
  bool full() { return in_list >= k;} 
};

//@}
#endif

