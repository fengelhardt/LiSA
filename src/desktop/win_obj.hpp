 /*
 * ****************** win_obj.hpp ******************************
 * 
 * description:      objects to draw graphical objects and text
 *
 * owner:            Per Willenius
 *
 * date:             28.9.1999
 *
 * version:          V 1.0
 *
*/ 

/** @name Extended Window Classes

    file: LiSA/src/win_obj.hpp

    include: tk.h and string

    @author Per Willenius
    @version 2.3pre3
*/ 

//@{

#ifndef _win_obj_h 
#define _win_obj_h
  
// ************************ defines **********************************
const float VERTICE_RADIUS =20;

#define VW_MAINCANV ".datawin.fra22.fra32.cpd34.03"
#define VW_HORICANV ".datawin.fra22.fra28.fra36.can37"
#define VW_VERTCANV ".datawin.fra22.fra32.fra33.can38"
#define VW_LABEL    ".datawin.fra23.lab24"

#define SW_MAINCANV ".schedulewin.fra22.fra32.cpd34.03" 
#define SW_HORICANV ".schedulewin.fra22.fra28.fra36.can37"
#define SW_VERTCANV ".schedulewin.fra22.fra32.fra33.can38"
#define SW_LABEL    ".schedulewin.fra23.lab24"

#define SLW_MAINCANV ".seqlist.fra22.fra32.cpd34.03" 
#define SLW_HORICANV ".seqlist.fra22.fra28.fra36.can37"
#define SLW_VERTCANV ".seqlist.fra22.fra32.fra33.can38"
#define SLW_LABEL    ".seqlist.fra23.lab24"

#define MW_MAINCANV ".lisa.frame_for_all_canvas.fra32.cpd34.03"
#define MW_HORICANV ".lisa.frame_for_all_canvas.fra28.fra36.can37"
#define MW_VERTCANV ".lisa.frame_for_all_canvas.fra32.fra33.can38"
#define MW_LABEL    ".lisa.fra38.lab39"


//  ********************** System Includes ***************************
#include <string>
#include <tk.h>

#include "../basics/matrix.hpp"
#include "../scheduling/mo_jo.hpp"
#include "../scheduling/pgraph.hpp"
#include "../scheduling/os_sched.hpp"
#include "basicwin.hpp"

using namespace std;

/// graphical in-and output of Lisa_Schedule
class TCSchedule: public TCTable {
public:
  Lisa_Label *label;
  ///draw a table (to use with Lisa_Schedule)
  void draw(Lisa_MO *myMO,Lisa_Matrix<int> *myP,Lisa_JO *myJO,
	    Lisa_Matrix<TIMETYP> *myCIJ,Lisa_Matrix<bool> *SIJ);
  TCSchedule(Tcl_Interp *interp,string main,string horizontal_canvas, 
	     string vertical_canvas,string label_name);
 /// draw value on row xpos and column ypos
  void draw_entry(int xpos,int ypos,string value);
  ~TCSchedule();
};

/// graphical output of a list of Lisa_Schedules
class TCScheduleList: public TCTable {
public:
  Lisa_Label *label;
  ///draw a table (to use with Lisa_Schedule)
  void draw(Lisa_List<ScheduleNode> *);
  TCScheduleList(Tcl_Interp *interp,string main,string horizontal_canvas, 
	     string vertical_canvas,string label_name);
  ~TCScheduleList();
};


/// graphical in-and output of Lisa_Values
class TCValues: public TCTable {
public:
  Lisa_Label *label;
  TCValues(Tcl_Interp *interp,string main,string horizontal_canvas, 
	   string vertical_canvas,string label_name);
  /// draw a table (to use with Lisa_Values)
  void draw(Lisa_Matrix<int> *myMO,Lisa_Matrix<TIMETYP> *myPT, 
	    Lisa_Matrix<bool> *mySIJ, Lisa_Vector<TIMETYP> *myRD, 
	    Lisa_Vector<TIMETYP> *myDD,  Lisa_Vector<double> * myWI);
  /// draw value on row xpos and column ypos
  void draw_entry(int xpos,int ypos,string value);
  ~TCValues();
};

/// graphical in-and output of th C-matrix in Lisa_Schedule
class TCCMatrix: public TCTable {
public: 
  TCCMatrix(Tcl_Interp *interp,string main,string horizontal_canvas, 
	    string vertical_canvas);
  /// draw the C-Matrix in the main window
  void draw(Lisa_Matrix<TIMETYP> *myC, 
	    Lisa_Matrix<bool> *myCP);
~TCCMatrix();
};
 
/// graphical output of graphs
class TCGraph:  public TCGraphic {
public:
  float vertice_radius;
  TCGraph();
  ~TCGraph();
};
 
/// graphical output of the sequence graph
class TCSeqGraph: public TCGraph {
public:
  TCSeqGraph(Tcl_Interp *interp,string main,string horizontal_canvas,
	     string vertical_canvas);
  /// draw the sequence graph
  void draw(Lisa_Matrix<bool> &CP,
	    Lisa_SGraph &PG,
	    Lisa_Matrix<bool> &SIJ); 
  void draw(Lisa_Matrix<bool> &CP,
	    Lisa_OsSchedule &myOsSchedule,
	    Lisa_Matrix<bool> &SIJ);
~TCSeqGraph();
};


#endif

//@}



