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

void pair::read(istream & strm) {
  //strm >> *actual_schedule;
}

void pair::write(ostream & strm) const {
 
}

bool pair::operator!=(const pair & other) const
     { 
       if (x!=other.x||y!=other.y)
         return true;
       else
         return false;
     }

bool pair::operator<(const pair & other) const
     { 
       if (x<other.x)
         return true;
       else if (x==other.x)
	 if (y<other.y)
	   return true;
       
       return false;
     }

bool pair::operator>(const pair & other) const
     { 
       if (x>other.x)
         return true;
       else if (x==other.x)
	 if (y>other.y)
	   return true;
       
       return false;
     }


