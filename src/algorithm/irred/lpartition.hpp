
#ifndef lpartition_h
#define lpartition_h

#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../../main/global.hpp"
#include "../../misc/except.hpp"
#include "../../basics/matrix.hpp"


/// Partitionize sets of entitys.
  
/** The arcs of a plangraph have to be partitionized into disjunktive sets, the 
    implication classes. This object helps doing exactly that.

    A partition object can be generated with a given size. Each number between 
    and including 0 and size-1 represents an entity. You can then tell the
    object which entitys belong together forming disjunkive sets of entitiys. 

    These sets can then be requested from the object.

    @author Marc Moerig
    @version 3.0
*/
class Lisa_Partition : public Lisa_GenericObject{
private:
  /// pointer behind part
  int* pstop;
  /// pointer to first element of part
  int* pstart;
  /// maximum number == number of partitions-1 in part vector 
  int max;
  /// holding our partitions
  /** i is in partition part[i] */
  Lisa_Vector<int>* part;
public:

  /// constructor
  /** Create this object for a given size. */
  Lisa_Partition(const int in);

  /// destructor
  ~Lisa_Partition();

  /// join the sets containing a and b
  void join(int a,int b);
 
  /// write all numbers which belong to partition num into the vector vec
  void get_partition(const int num,Lisa_Vector<int>* vec);
  
  /// returns the size of a partition
  int get_partition_size(const int num);
  
  /// returns the number of partitions
  int get_num_of_partitions();

  /// writes the object data to a stream
  /** Mostly used for debugging. */
  void write(std::ostream& strm = std::cout) const;
};

#endif

