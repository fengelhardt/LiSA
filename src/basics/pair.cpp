/*
 * ************** matrix.cpp *******************************
 * 
 * vectors and matrices with dynamic size and fast access
 *
 * Owner: Thomas Tautenhahn
 *
 * 21.10.99
*/



#include"pair.hpp"

void Lisa_Pair::read(istream & strm) {
  //strm >> *actual_schedule;
}

void Lisa_Pair::write(ostream & strm) const {
 
}

bool Lisa_Pair::operator!=(const Lisa_Pair & other) const
     { 
       if (x!=other.x||y!=other.y)
         return true;
       else
         return false;
     }

bool Lisa_Pair::operator<(const Lisa_Pair & other) const
     { 
       if (x<other.x)
         return true;
       else if (x==other.x)
	 if (y<other.y)
	   return true;
       
       return false;
     }

bool Lisa_Pair::operator>(const Lisa_Pair & other) const
     { 
       if (x>other.x)
         return true;
       else if (x==other.x)
	 if (y>other.y)
	   return true;
       
       return false;
     }


