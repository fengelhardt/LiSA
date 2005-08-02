/**
 * @author  Per Willenius
 * @version 2.3final
 */

#include <stdlib.h>
#include "lisapref.hpp"

using namespace std;

//**************************************************************************

Lisa_Preferences::Lisa_Preferences() {
}

//**************************************************************************

void Lisa_Preferences::init(){

  /*if(Tcl_GetVar2(interp,"env","LISAHOME",0) != 0){
    LISA_HOME = Tcl_GetVar2(interp,"env","LISAHOME",0);
  }else{
    G_ExceptionList.lthrow("Enviroment variable 'LISAHOME' not set.",
                           Lisa_ExceptionList::ANY_ERROR);
  }*/
  
  /*char* lh = getenv("LISAHOME");
  if(lh == 0){
    G_ExceptionList.lthrow("Enviroment variable 'LISAHOME' not set.",
                           Lisa_ExceptionList::ANY_ERROR);
  }else{
    LISA_HOME = lh;
  }*/
  
  /*if(Tcl_GetVar2(interp,"env","LISACONFIG",0) != 0){
    CONFIG_HOME = Tcl_GetVar2(interp,"env","LISACONFIG",0);
  }else{
    G_ExceptionList.lthrow("Enviroment variable 'LISACONFIG' not set.",
                           Lisa_ExceptionList::ANY_ERROR);
  }*/
  
  /*char* lc = getenv("LISACONFIG");
  if(lc == 0){
    G_ExceptionList.lthrow("Enviroment variable 'LISACONFIG' not set.",
                           Lisa_ExceptionList::ANY_ERROR); 
  }else{
    CONFIG_HOME = lc;
  }*/
  
  gantt_orient = GANTT_MACHINE;
  gantt_col_type= GANTT_NORMAL;
  gantt_colors = new Lisa_Vector<int>(5);
  gantt_colors->fill(0);
}

//**************************************************************************

