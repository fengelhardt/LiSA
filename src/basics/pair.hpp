
#ifndef _pair_h 
#define _pair_h 

#include "../lisa/lsaobjct.hpp"

/// ordered pair
/** ordered pairs are often used as basictype
    
    @author            Per Willenius
    @version 2.3final
  */
class Lisa_Pair : public Lisa_Object{ 
public:

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

