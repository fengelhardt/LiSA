/*
 * ******************** fp_set.hpp ******************************
 * 
 * description:  vector set for football pool problem
 * 
 * @author        Thomas Tautenhahn
 * @version 2.3pre3
 *
 * date:         17. 11. 1998
 *
 */

#ifndef _fp_set_h
#define _fp_set_h

#include "../../basics/matrix.hpp"
#include "fp_vect.hpp"

class fp_set
  {
    private:
       int slack, sigma,n;
       Lisa_Vector<int> * covered;
       fp_vector ** vectors;
    public:
       // construct with given vector length and number of vectors
       fp_set(int, int);
       ~fp_set();
       const fp_set& operator=(fp_set& other);
       fp_vector * replace_vector(int , fp_vector *);
       int get_slack() { return slack; }
       int get_cover( int i) { return (*covered)[i]; }
       fp_vector& get_vector(int i) { return (*vectors[i]); }
       void spread();
       void random(long *);
       void random2(long *);
       void write(ostream& strm = cout) const;
  };    

inline ostream& operator << (ostream& strm, const fp_set& l)
   {
     l.write(strm);
     return strm;
   }

#endif



