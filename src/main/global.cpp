/**
 * @author  Per Willenius
 * @version 2.3pre3
 */ 

#include "global.hpp"

//**************************************************************************

int lisa_random( int lb, int ub, long *seed ){
  long m=2147483647L,
    a=     16807L,
    b=    127773L,
    c=      2836L;
  if ((*seed=(a*(*seed % b)-(*seed/b)*c))<0) *seed+=m;
  return ( lb+ (int) ((1.0+ub-lb)*( (double) *seed )/m ) );
}

//**************************************************************************

