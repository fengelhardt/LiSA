/*
 * ************** lisa.C *******************************
 * 
 * description: Main Program
 *              declare global variables, read preferences file
 *              and start tcl-mainloop
 *
 * owner:       Per Willenius
 * date:        16.9.1998
*/


//  ********************** System Includes ***************************

// input of the preference file
#include <fstream.h>


// ********************** Project Includes **************************

// definition of the global variables
#include "../lisa_dt/xsched.hpp"
#include "../lisa_dt/lisapref.hpp"
#include "../lisa_dt/tclvar.hpp"
#include "../lisa_dt/ptype.hpp"
#include "../lisa_dt/lvalues.hpp"
#include "../lisa_dt/schedule.hpp"

#include "../basics/status.hpp"

// Tk_myMain()
#include "../desktop/c_tcl.hpp"

// read(G_Preferences.get_string("STARTFILE"),D_ALLES); 
#include "../desktop/file_io.hpp"

// new_mn()
#include "../desktop/callback.hpp"

// error handling
#include "../basics/except.hpp"

// ********************** Global Variables ********************
class Lisa_ProblemType G_ProblemType;
class Lisa_Values G_Values;
class Lisa_Schedule *G_Schedule;
class Lisa_XSchedule * G_XSchedule;
class Lisa_List<ScheduleNode> *G_ScheduleList;
class Lisa_Preferences G_Preferences;
class Lisa_Status G_Status;
class Lisa_TCLVar G_TclVar;
Tcl_Interp *interp;  


// *********************** functions *************************

int main(int argc, char *argv[]) {
  ifstream Pref(argv[1]);
  if (Pref==NULL) 
    {
      cerr << "sorry, no file:" << argv[1] << " in Lisa/data directory\n";
      return 0;
    }
  //  G_ExceptionList.set_output_to_cerr();
  Pref >> G_Preferences;
  G_Schedule= new Lisa_Schedule(1,1);
  G_XSchedule = new Lisa_XSchedule(G_Schedule);
  G_Schedule->make_LR();
  ScheduleNode *myScheduleNode;
  myScheduleNode = new ScheduleNode(G_Schedule);
  G_ScheduleList = new Lisa_List<ScheduleNode>;
  G_ScheduleList->append(*myScheduleNode);
  delete myScheduleNode;
  while(!(G_ScheduleList->empty())) 
	{
          ScheduleNode dummynode;
          dummynode=G_ScheduleList->pop();
          if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
	}
  
  G_Values.make_DD(); 
  G_Values.make_PT();
  G_Values.make_RD(); 
  G_Values.make_SIJ(); 
  G_Values.make_WI(); 
  if (G_Preferences.contents.defined("STARTFILE") ==STRING) {
    if (read(G_Preferences.get_string("STARTFILE"))==0) {
      new_mn();
      G_Values.PT_to_SIJ();
    }
    else cerr << "no Startfile:" << G_Preferences.get_string("STARTFILE") << "found\n";
  }
  Tk_myMain( argc, argv );
  delete G_Schedule;
  delete G_XSchedule;
  delete G_ScheduleList;
  return(0);
}














