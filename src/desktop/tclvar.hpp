/*
 * ************** tclvar.h ******************************
 * 
 * description:    class Lisa_TCLVar
 * 
 * @author          Per Willenius
 * 
 * date:           16.9.1998
 *
 * @version 2.3pre3
*/


#ifndef _tclvar_h 
#define _tclvar_h 


// **************** Project Includes ********************
// TCLValues:
#include "win_obj.hpp"


//  **************  Class Definitions  ******************

/** In C++ used Tcl-Varables
 */
class Lisa_TCLVar
{
public:
  /// alive=1: Lisa is active, alive=0: exit by user
  int alive;
  /// for man_oper
  int JOsucc, MOsucc, JOpred, MOpred;
  Lisa_Matrix<int> *LR; 
  /// Tcl-need a copy of MO to display 
  Lisa_Matrix<int> * MO_as_Matrix;
  void init();
  void make_MO(int n,int m);
  void make_LR(int n,int m);
  Lisa_TCLVar();
};

#endif














