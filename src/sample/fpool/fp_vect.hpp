
#ifndef _FP_Vector_h
#define _FP_Vector_h

#include <fstream>

#include "../../basics/matrix.hpp"

/// vector for football pool problem
/**
 * @author        Thomas Tautenhahn
 * @version 2.3rc1
 */
class FP_Vector{
private:
  /// size:
  int n;
  /// the vector itself
  Lisa_Vector<int> * v;      
  /// all vectors are lexicographically numbered
  int number;
  /// numbers of covered vectors
  Lisa_Vector<int> * neighbour;
public:  
  /// construct only with given size
  FP_Vector(int);
  /// assignment operator
  const FP_Vector& operator=(FP_Vector&);
  
  /// modify vector:
  void set_element(int, int);
  /// member function, needs documentation
  int get_element(int pos) {return (*v)[pos];}
  ///member function, needs documentation
  int get_neighbour(int pos) {return (*neighbour)[pos];}
   
  /// write object values to a stream
  void write(std::ostream& strm = std::cout) const{strm << *v;}
  /// destructor
  ~FP_Vector(){ delete v; delete neighbour;}
};    

//**************************************************************************

/// stream operator for writing FP_Vector object      
inline std::ostream& operator << (std::ostream& strm, const FP_Vector& l){
  l.write(strm);
  return strm;
}

//**************************************************************************

/// function, needs documentation
bool smalldistance(int, int);

#endif

