/* ************************************************************************* */
/*                       Filename: Datastruct.h                              */
/*                                                                           */
/*    Export: Global constants and datastructures                            */
//   @version 3.0pre1
/* ************************************************************************* */

#ifndef _DATASTRUCT_H
#define _DATASTRUCT_H

/// maximum number of machines to handle
#define MaxNumOfMachines     30
/// maximum number of jaobs to handle
#define MaxNumOfJobs         30
/// maximum number of operations per machine
#define MaxOpProMachine      MaxNumOfJobs
/// maximum number of operations total 
#define MaxNumOfOperations   MaxNumOfJobs * MaxNumOfMachines

/// logic
#define true                  1
/// logic
#define false                 0
/// logic
#define NIL                   0
/// maximum int size
#define MaxInt            32000

#include "../../scheduling/js_sched.hpp"

/// need bool (this ws once pure c code)
typedef int boolean;        

/// a list element
struct List{
  /// sture ints in list
  int          number;
  /// pointer to next list lement
  struct List  *next;
};

/// an operation
struct Operation{
  /// processing time of this operation
  int process_time;
  /// to which machine belongs this operation 
  int machine_nr;
};

/// hold predecessors and successors for all operations
struct ArcList{
  /// hold predecessors and successors for all operations
  struct List  *pred[MaxNumOfOperations+1],
               *succ[MaxNumOfOperations+1];
};

/// a list of lists
struct BlockList{
  /// list in list
  struct List       *elements;
  /// next element of list
  struct BlockList  *next;
  /// needs documentation
  int               last_of_block;
  /// first - middle - last
  char              fi_mi_la;    
};

/// a branch list
struct BranchList{
  /// brach operation
  int                branch_op;
  /// before - after
  char               before_or_after;
  /// next element in list
  struct BranchList  *next;
};

/// node to work on
struct NodeType {
  /// successors and predecessors for all operations
  int num_of_succ[MaxNumOfOperations+1],
      num_of_pred[MaxNumOfOperations+1];
  /// known lower bound
  int lower_bound;
  /// blocklist, needs documentation
  struct BlockList   *blocks;
  /// brachlist, needs documentation
  struct BranchList  *order;
};

/// a stack element
struct StackElement{
  /// holding a node
  struct NodeType       *node;
  /// next element on stack
  struct StackElement   *next;
};



#endif

