/*
 * ************** lpartition.hpp *******************************
 *
 * Owner: Marc Moerig
 *
 * 25.02.01
 * last changed 26.02.01
 */

#ifndef lpartition_h
#define lpartition_h

#include<stdlib.h>
#include<iostream.h>
#include<fstream.h>
#include"../../main/global.hpp"
#include"../../misc/except.hpp"
#include"../../basics/matrix.hpp"
/** @name Partitionize Sets

    The arcs of a plangraph have to be partitionized into disjunktive
    sets, the implication classes. This object helps doing exactly that.

*/
//@{
/**

  Partitionize sets of entitys.   

  A partition object can be genereated with a given size. Each number between 
  and including 0 and size-1 represents an entity. You can then tell the object 
  which entitys belong together forming disjunkive sets of entitiys. 

  These sets can then be requested from the object.
*/
class Lisa_Partition : public Lisa_GenericObject{
private:
  int* pstop;
  int* pstart;
  int max;
  Lisa_Vector<int>* part;
public:
  /** constructor...\\
      Create this object for a given size. 
  */
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

  /** writes the object data to a stream ...\\
      Mostly used for debugging. */
  void write(ostream& strm = cout) const;
};

//@}

#endif
