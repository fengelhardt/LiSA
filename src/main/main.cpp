/**
 * @author  Per Willenius
 * @version 2.3final
 */

#include "../xml/LisaXmlFile.hpp"

#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"
#include "../scheduling/xsched.hpp"
#include "../desktop/tclvar.hpp"
#include "../desktop/c_tcl.hpp"
#include "../desktop/callback.hpp"
#include "../misc/except.hpp"
#include "../classify/classify.hpp"

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
class Lisa_ControlParameters G_Preferences;

///global data, various stuff
class Lisa_Status G_Status;

///global data, tcl variables needed in c++ code
class Lisa_TCLVar G_TclVar;

///global classification object
class Lisa_Classify *G_Classify=0;

/// pointer to the tcl interpreter running our main window
Tcl_Interp *interp;  

//**************************************************************************

/// LiSA's main routine, the fun begins here ;)
int main(int argc, char *argv[]) {

#ifdef LISA_DEBUG
  G_ExceptionList.set_output_to_cout();
#endif  
  
  if(argc < 2){
      cerr << "Usage: " << argv[0] << " [config file] " << endl;
      return -1;
  }
  
  //  G_ExceptionList.set_output_to_cerr();

  char *dtd_env = getenv("LISA_DTD_PATH");
  std::string dtd_path = (dtd_env) ? dtd_env : "";
  LisaXmlFile::initialize(dtd_path);
  
  if(!init_G_Preferences(argv[1],G_Preferences)) return 1;

  G_Classify = Lisa_Classify::make_instance(G_Preferences.get_string(Lisa_Pref::LISAHOME)+"/data/classify/classify.bib");
  if(G_Classify == 0){
    cerr << "Could not create classify object. Exiting" << endl;
    return 1;
  }
  
  G_Schedule= new Lisa_Schedule(1,1);
  G_XSchedule = new Lisa_XSchedule(G_Schedule);
  G_Schedule->make_LR();
  Lisa_ScheduleNode *myLisa_ScheduleNode;
  myLisa_ScheduleNode = new Lisa_ScheduleNode(G_Schedule);
  G_ScheduleList = new Lisa_List<Lisa_ScheduleNode>;
  G_ScheduleList->append(*myLisa_ScheduleNode);
  delete myLisa_ScheduleNode;
  while(!(G_ScheduleList->empty())){
    Lisa_ScheduleNode dummynode;
    dummynode=G_ScheduleList->pop();
    if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
  }
  
  G_Values.make_DD(); 
  G_Values.make_PT();
  G_Values.make_RD(); 
  G_Values.make_SIJ(); 
  G_Values.make_WI(); 
  
  Tk_myMain( argc, argv );
  
  if(G_Classify) delete G_Classify;
  delete G_Schedule;
  delete G_XSchedule;
  delete G_ScheduleList;
  return(0);
}

//**************************************************************************

