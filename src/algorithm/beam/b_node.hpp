#ifndef _B_NODE_H_
#define _B_NODE_H_

#include <fstream>

#include "../../main/global.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../basics/order.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

/// algorithm parameter flags
enum CostFunc {
  CMax,
  CLast
};


/// algorithm parameter
extern CostFunc costFunc;

/// current objective while algorithm runs
extern TIMETYP objective;

///  B_Node a modified Lisa_OsSchedule
/**  @author Jan Tusch Andre Herms
     @version 2.3rc1
     @see Lisa_OsSchedule
     @see KList
*/ 
class B_Node : public Lisa_OsSchedule {
protected:
  /// parent
  B_Node *parent; 
  /// costs of (partial) schedule
  TIMETYP cost; 
  /// remember where the last operation was inserted
  int lastAddedRow, lastAddedCol;
  
  /// allow stream operator acess to private data
  friend std::ostream& operator<<(std::ostream&, B_Node&); 

public:
  ///the problem
  Lisa_OsProblem * problem;
  
  ///construct a schedule from another
  B_Node(B_Node *parent);
  ///construct a schedule that hands over the problem to its children
  B_Node(Lisa_OsProblem *);
  ///get costs of the (partial) schedule
  TIMETYP getCosts();
  ///for comparision
  operator TIMETYP (void){return getCosts();}
  ///insert an operation in the partial schedule
  /** and remember this operation until insert is invoked again
      @see Lisa_OsSchedule */
  int insert(int i,int j,int woi,int woj);
};


/// represents a container with capacity k and special functionality regarding insertion
/** @author Jan Tusch & Andre Herms
    @version 2.3rc1
    @see B_Node
*/
class KList {
public:
  /// array holding pointers to B_Node elements
  B_Node **list;
  ///index of next free insertion position
  int in_list;
  ///position of the element with the maximum costValue over all elements
  int worst_in_list;
  ///list's capacity
  int k;
  ///constructs a KList with capacity k
  KList(int k);
  /// destructor
  ~KList();
  ///add a B_Node to the list
  /** if either the list is not full or there is a B_Node with larger costs 
      in the list */
  void add(B_Node *);
  ///add a B_Node to the list at position p
  /** if there is no B_Node or a B_Node with larger costs at position p */
  void add(B_Node *, int p);
  ///returns true if the capacity is exhausted
  bool inline full() { return in_list >= k;} 
};

#endif

