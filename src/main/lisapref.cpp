/**
 * @author Mathias Plauschin
 * @version 3.0pre1
 */

#include "lisapref.hpp"

#include "../xml/LisaXmlFile.hpp"

#include <fstream>
#include <iostream>
#include <string>

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

bool init_G_Preferences(char * configfile,Lisa_ControlParameters& G_Preferences){

  std::ifstream pref(configfile);
  if (! pref){
    std::cerr << "Could not open config file: '" << configfile << "'. Exiting." << std::endl;
    return false;
  }
  pref.close();

  LisaXmlFile conf;
  if(!conf.read(configfile) ){
    std::cerr << "Could not read config file: '" << configfile << "'. Exiting." << std::endl;
    return false;
  }
  if(!(conf >> G_Preferences)){
      std::cerr << "Could not read config file1: '" << configfile << "'. Exiting." << std::endl;
    return false;
  }

  if(!(G_Preferences.defined(Lisa_Pref::LISAHOME)==Lisa_ControlParameters::STRING)){
    char* lh = getenv(Lisa_Pref::LISAHOME);
    if(lh == 0){
      std::cerr << Lisa_Pref::LISAHOME << " neither defined in '" << configfile << "' nor as environment variable. Exiting." << std::endl;
      return false;
    }else{
	  // convert '\' to '/' on windows systems
	  std::string lisahome = lh;
	  for(int i=0; i<(int)lisahome.length(); i++) if(lisahome[i]=='\\') lisahome[i]='/';
      G_Preferences.add_key(Lisa_Pref::LISAHOME,lisahome.c_str());
    }
  }

  if(!(G_Preferences.defined(Lisa_Pref::LISACONFIG)==Lisa_ControlParameters::STRING)){
    char* lc = getenv(Lisa_Pref::LISACONFIG);
    if(lc == 0){
      std::cerr << Lisa_Pref::LISACONFIG << " neither defined in '" << configfile << "' nor as environment variable. Exiting." << std::endl;
      return false;
    }else{
	  // convert '\' to '/' on windows systems
	  std::string lisaconfig = lc;
	  for(int i=0; i<(int)lisaconfig.length(); i++) if(lisaconfig[i]=='\\') lisaconfig[i]='/';
      G_Preferences.add_key(Lisa_Pref::LISACONFIG,lisaconfig.c_str());
    }
  }

  G_Preferences.add_key(Lisa_Pref::GANTT_ORIENT,Lisa_Pref::GANTT_MACHINE);
  G_Preferences.add_key(Lisa_Pref::GANTT_COL_TYPE,Lisa_Pref::GANTT_NORMAL);
  G_Preferences.add_key(Lisa_Pref::GANTT_RED,(long)0);
  G_Preferences.add_key(Lisa_Pref::GANTT_GREEN,(long)0);
  G_Preferences.add_key(Lisa_Pref::GANTT_BLUE,(long)0);
  G_Preferences.add_key(Lisa_Pref::GANTT_BROWN,(long)0);
  G_Preferences.add_key(Lisa_Pref::GANTT_YELLOW,(long)0);
  //cout << G_Preferences;

  return true;
}

//**************************************************************************

