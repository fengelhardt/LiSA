/*
 * ************** lisapref.hpp ******************************
 * 
 * description: class Lisa_Preferences  
 * 
 * 
 * @author Per Willenius
 * date: 28.9.1999
 * 
*/


/** @name Class to manage preference information

  file: LiSA/src/main/lisapref.h

  uses: string

  @author Per Willenius
  @version 2.3pre3
  */ 

//@{
#ifndef _lisapref_h 
#define _lisapref_h 

#include <string>
#include <tk.h>

#include "../lisa/filentry.hpp"
#include "../lisa/ctrlpara.hpp"
#include "../basics/matrix.hpp"

using namespace std;


const int GANTT_JOB=0;
const int GANTT_MACHINE=1;
const int GANTT_NORMAL=0;
const int GANTT_CP=1;
const int GANTT_COLOR=2;
const int GANTT_RED=0;
const int GANTT_GREEN=1;
const int GANTT_BLUE=2;
const int GANTT_BROWN=3;
const int GANTT_YELLOW=4;

//  ************** Class Definitions ******************

/// Preference Class
class Lisa_Preferences : public Lisa_FileEntry
{
public:
  /// contains all settings in form of a Lisa_ControlParameters class
  Lisa_ControlParameters contents;
  /// the LISA_HOME path
  string LISA_HOME;
  /// the home-path of the user
  string CONFIG_HOME;
  /// orientation of the gantt chart
  bool gantt_orient;
  /// if gantt_col_type==GANTT_NORMAL the gantt chart is drawn 
  /// in 23 different colors
  /// if gantt_col_type==GANTT_CP  the critical path in the 
  /// gantt chart is red
  /// if gantt_col_type==GANTT_COLOR 5 selected machines (or jobs)
  /// are in the color which are defined in gannt_colors
  int gantt_col_type;
  /// color definitions for 5 selected machines (or jobs)
  Lisa_Vector<int> *gantt_colors;
  /// same as in Lisa_ControlParameters
  inline int get_int(string bezeichner) {return contents.get_int(bezeichner);}
  /// same as in Lisa_ControlParameters
  inline string get_string(string bezeichner) {return contents.get_string(bezeichner);}
  /// same as in Lisa_ControlParameters
  inline double get_double(string bezeichner) {return contents.get_double(bezeichner);}
  /// same as in Lisa_ControlParameters
  inline void write(ostream & strm= cout) const { strm << contents;}
  /// same as in Lisa_ControlParameters
  inline void read(istream & strm = cin) { strm >> contents;}
  /// to get the LISA_HOME name after starting tcl/tk
  void init(Tcl_Interp *interp);
  /// constructor
  Lisa_Preferences();
};

#endif

//@}













