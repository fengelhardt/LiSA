/*
 * ************** lisapref.cpp ******************************
 * 
 * description: class Lisa_Preferences  
 * 
 * 
 * owner: Per Willenius
 * date: 16.9.1998
 * version: V 1.0
*/

// ************************ Local Includes ********************
#include "lisapref.hpp"


// *********************** functions *************************

Lisa_Preferences::Lisa_Preferences() {
}

void Lisa_Preferences::init(Tcl_Interp *interp) {
  LISA_HOME = Tcl_GetVar2(interp,"env","LISAHOME",0);
  CONFIG_HOME = Tcl_GetVar2(interp,"env","LISACONFIG",0);
  gantt_orient = GANTT_MACHINE;
  gantt_col_type= GANTT_NORMAL;
  gantt_colors = new Lisa_Vector<int>(5);
  gantt_colors->fill(0);
}






