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


// ******************** Local Includes ********************
#include "global.hpp"


// *********************** functions *************************

int lisa_random( int lb, int ub, long *germe )
{
  long m=2147483647L,
    a=     16807L,
    b=    127773L,
    c=      2836L;
  if ((*germe=(a*(*germe % b)-(*germe/b)*c))<0) *germe+=m;
  return ( lb+ (int) ((1.0+ub-lb)*( (double) *germe )/m ) );
};

 



