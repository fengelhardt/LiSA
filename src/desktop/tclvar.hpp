
#ifndef _tclvar_h 
#define _tclvar_h 

#include "win_obj.hpp"

/// In c++ used Tcl variables
/**
  @author          Per Willenius
  @version 2.3pre3
 */
class Lisa_TCLVar{
public:
  
  /// LiSA active ?
  /** - alive = 1 LiSA is active
  
      - alive = 0 exit by user */
  int alive;
  
  /// for man_oper
  int JOsucc, MOsucc, JOpred, MOpred;
  
  /// the latin rectangle
  Lisa_Matrix<int> *LR;
  
  /// Tcl needs a copy of MO to display 
  Lisa_Matrix<int> * MO_as_Matrix;
  
  /// delete machine order matrix
  void init();
  
  /// (re)initialize machine order matrix
  void make_MO(int n,int m);
  
  /// (re)initialize latin rectangle
  void make_LR(int n,int m);
  
  /// constructor
  Lisa_TCLVar();
};

#endif

