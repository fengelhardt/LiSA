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


/// TCLValues:
#include "win_obj.hpp"

/** In C++ used Tcl-Variables
 */
class Lisa_TCLVar
{
public:
  
  /// alive=1: Lisa is active, alive=0: exit by user
  int alive;
  
  /// for man_oper
  int JOsucc, MOsucc, JOpred, MOpred;
  
  /// the latin rectangle
  Lisa_Matrix<int> *LR;
  
  /// Tcl-need a copy of MO to display 
  Lisa_Matrix<int> * MO_as_Matrix;
  
  //delete machine order matrix
  void init();
  
  ///(re)initialize machine order matrix
  void make_MO(int n,int m);
  
  ///(re)initialize latin rectangle
  void make_LR(int n,int m);
  
  /// constructor
  Lisa_TCLVar();
};

#endif

