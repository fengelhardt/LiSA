/*
 * ******************** global.h ******************************
 * 
 * description:      convention for consistent usage with values
 *                   often used functions
 * 
 * owner:            Per Willenius
 *
 * date:             17.9.1998
 *
 * version:          V 1.0
 */ 

#ifndef _pair_h 
#define _pair_h 

#include "../lisa_dt/lsaobjct.hpp"
// ******************** class definitions ********************

/// Ordered Pairs are often used as Basistyp

class pair : public Lisa_Object{ 
public: int x,y;
   /// output to stream (stream operator itself is already defined)
   virtual void write(ostream & = cout) const;
  /// input from stream (stream operator itself is already defined)
   virtual void read(istream &   = cin);
  bool operator!=(const  pair&) const;
  /// compare lexicographically
 bool operator<(const  pair&) const;
 bool operator>(const  pair&) const;
};    



#endif

