/*
 * ******************** global.h ******************************
 * 
 * description:      convention for consistent usage with values
 *                   often used functions
 * 
 * @author            Per Willenius
 *
 * date:             17.9.1998
 *
 * @version 2.3pre3
 */ 

#ifndef _pair_h 
#define _pair_h 

#include "../lisa/lsaobjct.hpp"

/// Ordered Pairs are often used as basistype
class Lisa_Pair : public Lisa_Object{ 
public: 
  int x,y;
  
  /// output to stream (stream operator itself is already defined)
  virtual void write(std::ostream & = std::cout) const;
  
  /// input from stream (stream operator itself is already defined)
  virtual void read(std::istream &   = std::cin);
  
  /// compare for equality
  bool operator!=(const  Lisa_Pair&) const;
  
  /// compare lexicographically
  bool operator<(const  Lisa_Pair&) const;
  
  /// compare lexicographically  
  bool operator>(const  Lisa_Pair&) const;
};    

#endif

