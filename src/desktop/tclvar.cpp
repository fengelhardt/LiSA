/**
 * @author  Per Willenius
 * @version 2.3pre3
 */

#include "tclvar.hpp"

//**************************************************************************

// in C++ used tcl-variables
Lisa_TCLVar::Lisa_TCLVar() {
  alive=1;
  MO_as_Matrix=NULL;
  LR=NULL;
  JOsucc=MOsucc=JOpred=MOpred=0;
}

//**************************************************************************

void Lisa_TCLVar::init() {
  delete  MO_as_Matrix;
  MO_as_Matrix=NULL;
}

//**************************************************************************

void Lisa_TCLVar::make_MO(int n, int m) {
  if (MO_as_Matrix)
    delete  MO_as_Matrix;
  MO_as_Matrix= new Lisa_Matrix<int>(n,m); 
  MO_as_Matrix->fill(0);
}

//**************************************************************************

void Lisa_TCLVar::make_LR(int n, int m) {
  if (LR)
    delete  LR;
  LR= new Lisa_Matrix<int>(n,m); 
  LR->fill(0);
}

//**************************************************************************

