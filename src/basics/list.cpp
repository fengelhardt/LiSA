/*
 * ************** list.cpp *******************************
 * 
 * no classes here
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
 */
#include <fstream>
#include <string>

#include "list.hpp"

using namespace std;

extern string G_ExtraException;

void error(string message )
{ 
  cerr << "\nERROR: " << message << endl;
  G_ExtraException=message; 
}
