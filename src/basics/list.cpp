/**
 * @author  Thomas Tautenhahn
 * @version 2.3final
 */
 
#include <fstream>
#include <string>

#include "list.hpp"

using namespace std;

//**************************************************************************

extern string G_ExtraException;

//**************************************************************************

void error(string message )
{ 
  cerr << "\nERROR: " << message << endl;
  G_ExtraException=message; 
}

//**************************************************************************

