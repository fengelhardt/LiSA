/**
 * @author  Per Willenius
 * @version 2.3final
 */ 

#include <fstream>

#include "../main/global.hpp"


#include "../lisa/ptype.hpp"
#include "../lisa/ctrlpara.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"

#include "../main/file_io.hpp"

#include "../misc/except.hpp"


#include "alg_call.hpp"

using namespace std;

//**************************************************************************

int start_ext_algo(Tcl_Interp *interp,
         string name_of_algo,
         string algo_call,
         string output_file,
         string result_file,
	     Lisa_ControlParameters& G_Preferences,
	     Lisa_ProblemType & G_ProblemType,
	     Lisa_ControlParameters & parameter,
	     Lisa_Schedule & G_Schedule,
         Lisa_Values & G_Values) {

  string str="",str2="";
  
  // write algorithm input
  write_alg_call_xml(output_file,G_ProblemType,parameter, G_Schedule, G_Values);
	
  
  //clean algorithm output file
  ofstream fin(result_file.c_str());
  if(!fin) G_ExceptionList.lthrow("Could not open '"+(string) result_file.c_str()+"' for writing",
                                  Lisa_ExceptionList::FILE_NOT_FOUND);
  fin.close();
  
  // call the external program
  // TCL/TK does this for us
  
  str="set lsa_status(fid) [open \"| "+G_Preferences.get_string(Lisa_Pref::LISAHOME)+"/bin/" + 
    algo_call + " " + output_file + " " + result_file+ "\" \"r\" ]";

  Tcl_Eval(interp,(char*) str.c_str());
  
  str="set lsa_status(algo_call) "+  algo_call;
  Tcl_Eval(interp,(char*) str.c_str());
  // get pid number of process and
  // test, if algorithm exists
  str="set lsa_status(pid) -1";
  Tcl_Eval(interp,(char*) str.c_str());
  str="set lsa_status(pid) [pid $lsa_status(fid)]";
  Tcl_Eval(interp,(char*) str.c_str());
  str2=Tcl_GetVar2(interp,"lsa_status","pid",TCL_GLOBAL_ONLY);
  if (str2=="-1") {
    G_ExceptionList.lthrow("no file: "+G_Preferences.get_string(Lisa_Pref::LISAHOME)+"/bin/"+algo_call+ " in LiSA path",
                           Lisa_ExceptionList::END_OF_FILE);
    return !OK;
  }
  
  // external Module has started
  // open process window and give the process handling to Tcl
  str="fileevent $lsa_status(fid) readable \"algo_event $lsa_status(fid) " + result_file +" "+ name_of_algo + "\"";
  Tcl_Eval(interp,(char*) str.c_str());
  Tcl_Eval(interp,"Window show .ext_alg_graph");
  return OK;
}

//**************************************************************************

