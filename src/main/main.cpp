/**
 * @author  Per Willenius
 * @version 2.3rc1
 */

#include <fstream>

#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"
#include "../scheduling/xsched.hpp"
#include "../desktop/tclvar.hpp"
#include "../desktop/c_tcl.hpp"
#include "../desktop/callback.hpp"
#include "../misc/except.hpp"

#include "status.hpp"
#include "file_io.hpp"
#include "lisapref.hpp"

using namespace std;

//**************************************************************************

///global data, our current problem type
class Lisa_ProblemType G_ProblemType;

///global data, our current values
class Lisa_Values G_Values;

///global data, our current schedule
class Lisa_Schedule *G_Schedule;

///global data, our schedule for drawing to the main window
class Lisa_XSchedule * G_XSchedule;

///global data, list of schedules if algorithm returns more than one solution
class Lisa_List<Lisa_ScheduleNode> *G_ScheduleList;

///global data, preferences, read from default.lsa file
class Lisa_Preferences G_Preferences;

///global data, various stuff
class Lisa_Status G_Status;

///global data, tcl variables needed in c++ code
class Lisa_TCLVar G_TclVar;

/// pointer to the tcl interpreter running our main window
Tcl_Interp *interp;  

//**************************************************************************

/// LiSA's main routine, the fun begins here ;)
int main(int argc, char *argv[]) {
  if(argc < 2){
      cerr << "Usage: " << argv[0] << " [config file] " << endl;
      return -1;
  }
  
  ifstream Pref(argv[1]);
  if (! Pref){
    cerr << "Could not open config file: '" << argv[1] << "'. Exiting." << endl;
    return -1;
  }
    
  //  G_ExceptionList.set_output_to_cerr();
  Pref >> G_Preferences;
  Pref.close();
  
  G_Schedule= new Lisa_Schedule(1,1);
  G_XSchedule = new Lisa_XSchedule(G_Schedule);
  G_Schedule->make_LR();
  Lisa_ScheduleNode *myLisa_ScheduleNode;
  myLisa_ScheduleNode = new Lisa_ScheduleNode(G_Schedule);
  G_ScheduleList = new Lisa_List<Lisa_ScheduleNode>;
  G_ScheduleList->append(*myLisa_ScheduleNode);
  delete myLisa_ScheduleNode;
  while(!(G_ScheduleList->empty())) 
	{
          Lisa_ScheduleNode dummynode;
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
    else cerr << "No Startfile: '" << G_Preferences.get_string("STARTFILE") << "' found." << cerr;
  }
  
  Tk_myMain( argc, argv );
  
  delete G_Schedule;
  delete G_XSchedule;
  delete G_ScheduleList;
  return(0);
}

//**************************************************************************

