/*
 * ******************** fp_vector.hpp ******************************
 * 
 * description:  vector for football pool problem
 * 
 * owner:        Thomas Tautenhahn
 *
 * date:         17. 11. 1998
 *
 */

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _fp_vector_h
#define _fp_vector_h

#include "../../basics/matrix.hpp"
#include <ostream.h>

class fp_vector
  {
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
       fp_vector(int);
       /// copy:
       const fp_vector& operator=(fp_vector&);
       /// modify vector:
       void set_element(int, int);
       int get_element(int pos) {return (*v)[pos];}
       int get_neighbour(int pos) {return (*neighbour)[pos];}
       void write(ostream& strm = cout) const
          {
	    strm << *v;
          }
       ~fp_vector(){ delete v; delete neighbour;}
  };    
        
inline ostream& operator << (ostream& strm, const fp_vector& l)
   {
     l.write(strm);
     return strm;
   }

bool smalldistance(int, int);

#endif








