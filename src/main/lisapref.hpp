
#ifndef _lisapref_h 
#define _lisapref_h 

#include <string>
#include <tk.h>

#include "../lisa/filentry.hpp"
#include "../lisa/ctrlpara.hpp"
#include "../basics/matrix.hpp"

/// gantt chart job or machine oriented ?
/** @see Lisa_Preferences::gantt_orient */
enum{GANTT_JOB=0,GANTT_MACHINE};

/// three ways to color gantt chart
/** @see Lisa_Preferences::gantt_col_type */
enum{GANTT_NORMAL=0,GANTT_CP,GANTT_COLOR};

/// indexes into Lisa_Preferences::gantt_colors
enum{GANTT_RED=0,GANTT_GREEN,GANTT_BLUE,GANTT_BROWN,GANTT_YELLOW};

/// class to manage preference information 
/** @author Per Willenius
    @version 2.3rc1
 */
class Lisa_Preferences : public Lisa_FileEntry{
public:
  /// contains all settings in form of a Lisa_ControlParameters class
  Lisa_ControlParameters contents;
  
  /// the LISA_HOME path
  std::string LISA_HOME;
  
  /// the home-path of the user
  std::string CONFIG_HOME;
  
  /// orientation of the gantt chart
  bool gantt_orient;
  
  /// how to draw gantt chart ?
  /** 
    - if gantt_col_type is GANTT_NORMAL the gantt chart is drawn in 23 
    different colors
    
    - if gantt_col_type is GANTT_CP the critical path in the gantt chart is 
    highlighted red
    
    - if gantt_col_type is GANTT_COLOR 5 selected machines (or jobs) are in the 
    color which are defined in gannt_colors 
  */
  int gantt_col_type;
  
  /// color definitions for 5 selected machines (or jobs)
  /** contains at position i the machine (or job) to be colored
      with special color i */
  Lisa_Vector<int> *gantt_colors;
  
  //@{
  /// access internal Lisa_ControlParameters
  /** these are just wrappers for the methods with the same name
      in Lisa_ControlParameters */
  inline int get_int(std::string bezeichner) {return contents.get_int(bezeichner);}
  inline std::string get_string(std::string bezeichner) {return contents.get_string(bezeichner);}
  inline double get_double(std::string bezeichner) {return contents.get_double(bezeichner);}
  
  ///write objext data to a stream
  inline void write(std::ostream& strm= std::cout) const { strm << contents;}
  
  ///read object data from a stream
  inline void read(std::istream& strm = std::cin) { strm >> contents;}
  
  /// get the LISA_HOME name after starting tcl/tk
  void init(Tcl_Interp *interp);
  
  /// constructor
  Lisa_Preferences();
};

#endif

