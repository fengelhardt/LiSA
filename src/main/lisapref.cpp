/**
 * @author  Per Willenius
 * @version 2.3final
 */

#include "lisapref.hpp"

using namespace std;

//**************************************************************************

Lisa_Preferences::Lisa_Preferences() {
}

//**************************************************************************

void Lisa_Preferences::init(Tcl_Interp *interp){

  if(Tcl_GetVar2(interp,"env","LISAHOME",0) != 0){
    LISA_HOME = Tcl_GetVar2(interp,"env","LISAHOME",0);
  }else{
    G_ExceptionList.lthrow("Enviroment variable 'LISAHOME' not set.",ANY_ERROR);
  }
  
  if(Tcl_GetVar2(interp,"env","LISACONFIG",0) != 0){
    CONFIG_HOME = Tcl_GetVar2(interp,"env","LISACONFIG",0);
  }else{
    G_ExceptionList.lthrow("Enviroment variable 'LISACONFIG' not set.",ANY_ERROR);
  }
  
  gantt_orient = GANTT_MACHINE;
  gantt_col_type= GANTT_NORMAL;
  gantt_colors = new Lisa_Vector<int>(5);
  gantt_colors->fill(0);
}

//**************************************************************************

