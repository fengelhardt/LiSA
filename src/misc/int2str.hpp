
#ifndef _int2str_h 
#define _int2str_h 

#include <string>

/// returns true if value is an integer
bool inttst(const double value);

/// returns string
/** @author Per Willenius
    @version 2.3rc1
 */
std::string ztos(const double value);

/// returns formated string
std::string ztofs(const double value);

/// returns numeric value
int stoz(const std::string);

#endif

