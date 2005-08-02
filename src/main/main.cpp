/**
 * @author  Per Willenius
 * @version 2.3final
 */

#include <fstream>

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
class Lisa_Preferences G_Preferences;

///global data, various stuff
class Lisa_Status G_Status;

///global data, tcl variables needed in c++ code
class Lisa_TCLVar G_TclVar;

///global classification object
class Lisa_Classify *G_Classify=0;

/// pointer to the tcl interpreter running our main window
Tcl_Interp *interp;  

//**************************************************************************

bool init_G_Preferences(char * configfile){
  ifstream pref(configfile);
  if (! pref){
    cerr << "Could not open config file: '" << configfile << "'. Exiting." << endl;
    return false;
  }
  pref.close();
  
  LisaXmlFile conf;
  if(!conf.read(configfile) || !(conf >> G_Preferences)){
    cerr << "Could not read config file: '" << configfile << "'. Exiting." << endl;
    return false;
  }
  
  if(!(G_Preferences.contents.defined(Lisa_Pref::LISAHOME)==Lisa_ControlParameters::STRING)){
    char* lh = getenv(Lisa_Pref::LISAHOME);
    if(lh == 0){
      cerr << Lisa_Pref::LISAHOME << " neither defined in '" << configfile << "' nor as environment variable. Exiting." << endl;
      return false;
    }else{
      G_Preferences.contents.add_key(Lisa_Pref::LISAHOME,lh); 
    }
  }
  
  if(!(G_Preferences.contents.defined(Lisa_Pref::LISACONFIG)==Lisa_ControlParameters::STRING)){
    char* lc = getenv(Lisa_Pref::LISACONFIG);
    if(lc == 0){
      cerr << Lisa_Pref::LISACONFIG << " neither defined in '" << configfile << "' nor as environment variable. Exiting." << endl;
      return false; 
    }else{
      G_Preferences.contents.add_key(Lisa_Pref::LISACONFIG,lc);
    }
  }
  
  if(!(G_Preferences.contents.defined(Lisa_Pref::GANTT_ORIENT)==Lisa_ControlParameters::LONG)){
    G_Preferences.contents.add_key(Lisa_Pref::GANTT_ORIENT,Lisa_Pref::GANTT_MACHINE);
  }
  
  //cout << G_Preferences;
  
  return true;
}

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
  
  if(!init_G_Preferences(argv[1])) return 1;

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

