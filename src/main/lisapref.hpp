#ifndef _lisapref_h 
#define _lisapref_h 

#include "../lisa/ctrlpara.hpp"

class Lisa_Pref{
public:
  /// keys for the object holding global preferences, G_Preferences
  const static char *WIDTH,*HEIGHT,*LISAHOME,*LISACONFIG,*LANGUAGE,*HTML_VIEWER,
                    *GANTT_ORIENT,*GANTT_COL_TYPE,
                    *GANTT_RED,*GANTT_GREEN,*GANTT_BLUE,*GANTT_BROWN,*GANTT_YELLOW;
  
  /// possible values for the key GANTT_ORIENT
  const static long int GANTT_JOB,GANTT_MACHINE;
  
  /// possible values for the key GANTT_COL_TYPE
  const static long int GANTT_NORMAL,GANTT_CP,GANTT_COLOR;

};

//***************************************************************************

///initialize global preferences into G_Preferences
/**
  This function trys to retrieve some preferences from the given configfile.
  Preferences for the Gantt diagram will be reset to default values.
  Pathes LISAHOME and LISACONFIG will be fetched from environment variables
  if not present in configfile.
 */
bool init_G_Preferences(char * configfile,Lisa_ControlParameters& G_Preferences);
  
#endif

