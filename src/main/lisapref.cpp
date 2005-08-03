/**
 * @author  Per Willenius
 * @version 2.3final
 */

#include <stdlib.h>
#include "lisapref.hpp"

using namespace std;
  
//**************************************************************************
  
const char* Lisa_Pref::WIDTH = "WIDTH";
const char* Lisa_Pref::HEIGHT ="HEIGHT";  
const char* Lisa_Pref::LISAHOME = "LISAHOME";
const char* Lisa_Pref::LISACONFIG = "LISACONFIG";
const char* Lisa_Pref::LANGUAGE = "LANGUAGE";
const char* Lisa_Pref::HTML_VIEWER = "HTML_VIEWER";
const char* Lisa_Pref::GANTT_ORIENT = "GANTT_ORIENT";
const char* Lisa_Pref::GANTT_COL_TYPE = "GANTT_COL_TYPE";
const char* Lisa_Pref::GANTT_RED = "GANTT_RED";
const char* Lisa_Pref::GANTT_GREEN = "GANTT_GREEN";
const char* Lisa_Pref::GANTT_BLUE = "GANTT_BLUE";
const char* Lisa_Pref::GANTT_BROWN = "GANTT_BROWN";
const char* Lisa_Pref::GANTT_YELLOW = "GANTT_YELLOW";

const long Lisa_Pref::GANTT_JOB=0;
const long Lisa_Pref::GANTT_MACHINE=1;

const long Lisa_Pref::GANTT_NORMAL=0;
const long Lisa_Pref::GANTT_CP=1;
const long Lisa_Pref::GANTT_COLOR=2;
  
//**************************************************************************

Lisa_Preferences::Lisa_Preferences() {
}

//**************************************************************************

void Lisa_Preferences::init(){


  
  //gantt_orient = GANTT_MACHINE;
  //gantt_col_type= GANTT_NORMAL;
  //gantt_colors = new Lisa_Vector<int>(5);
  //gantt_colors->fill(0);
}

//**************************************************************************

