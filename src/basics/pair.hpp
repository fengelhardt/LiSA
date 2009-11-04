
#ifndef _pair_h 
#define _pair_h 

#include "../lisa/lsaobjct.hpp"

/// ordered pair
/** ordered pairs are often used as basictype
    
    @author            Per Willenius
    @version 3.0pre1
  */
class Lisa_Pair : public Lisa_Object{ 
public:
  
  /// constructor
  Lisa_Pair();
  
  ///constructor
  Lisa_Pair(const int x_in,const int y_in);

  /// pair values 
  int x,y;
  
  /// output to stream
  void write(std::ostream& strm = std::cout) const;
  
  /// input from stream (not implementad)
  void read(std::istream& strm = std::cin);
  
  /// compare for equality
  bool operator!=(const Lisa_Pair& other) const;
  
  /// compare for equality
  bool operator==(const Lisa_Pair& other) const;
  
  /// compare lexicographically
  bool operator<(const Lisa_Pair& other) const;
  
  /// compare lexicographically  
  bool operator>(const Lisa_Pair& other) const;
};

#endif

