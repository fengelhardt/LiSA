/*
 * ************** list.cpp *******************************
 * 
 * no classes here
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
 */
#include <ostream>
#include <string>

using namespace std;

#include"list.hpp"
extern string G_ExtraException;

void error(string message )
{ 
  cerr << "\nERROR: " << message << endl;
  G_ExtraException=message; 
}
