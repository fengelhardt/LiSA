/**
 * @author  Thomas Tautenhahn
 * @version 2.3rc1
 */

#include "pair.hpp"

using namespace std;

//**************************************************************************

void Lisa_Pair::read(istream & strm) {
  
}

//**************************************************************************

void Lisa_Pair::write(ostream& strm) const {
  strm<<"["<<x<<","<<y<<"]";
}

//**************************************************************************

bool Lisa_Pair::operator!=(const Lisa_Pair & other) const
     { 
       if (x!=other.x||y!=other.y)
         return true;
       else
         return false;
     }
     
//**************************************************************************

bool Lisa_Pair::operator==(const Lisa_Pair & other) const
     { 
       if (x==other.x && y == other.y)
         return true;
       else
         return false;
     }

//**************************************************************************

bool Lisa_Pair::operator<(const Lisa_Pair & other) const
     { 
       if (x<other.x)
         return true;
       else if (x==other.x)
	 if (y<other.y)
	   return true;
       
       return false;
     }

//**************************************************************************

bool Lisa_Pair::operator>(const Lisa_Pair & other) const
     { 
       if (x>other.x)
         return true;
       else if (x==other.x)
	 if (y>other.y)
	   return true;
       
       return false;
     }

//**************************************************************************

