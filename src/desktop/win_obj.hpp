
#ifndef _win_obj_h 
#define _win_obj_h
  
#include <string>
#include <tk.h>

#include "../basics/matrix.hpp"
#include "../scheduling/mo_jo.hpp"
#include "../scheduling/pgraph.hpp"
#include "../scheduling/os_sched.hpp"
#include "basicwin.hpp"

//**************************************************************************

//@{
/// define, needs documentation
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
//@}

//**************************************************************************

/// default value for drawing vertices
const float VERTICE_RADIUS = 20;

//**************************************************************************

/// graphical in and output of Lisa_Schedule
/** @author Per Willenius
    @version 2.3final
 */
class TCSchedule: public TCTable {
public:
  /// a label
  Lisa_Label *label;

  /// constructor          
  TCSchedule(Tcl_Interp *interp,std::string main,std::string horizontal_canvas, 
	           std::string vertical_canvas,std::string label_name);
  /// destructor
  ~TCSchedule();

  ///draw a table (to use with Lisa_Schedule)
  void draw(Lisa_MO *myMO,Lisa_Matrix<int> *myP,Lisa_JO *myJO,
	          Lisa_Matrix<TIMETYP> *myCIJ,Lisa_Matrix<bool> *SIJ);

  /// draw value on row xpos and column ypos
  void draw_entry(int xpos,int ypos,std::string value);
};

//**************************************************************************

/// graphical output of a list of Lisa_Schedule's lisa
/** @author Per Willenius
    @version 2.3final
 */
class TCScheduleList: public TCTable {
public:
  /// a label
  Lisa_Label *label;

  /// constructor
  TCScheduleList(Tcl_Interp *interp,std::string main,
                 std::string horizontal_canvas, std::string vertical_canvas,
                 std::string label_name);
  
  /// destructor
  ~TCScheduleList();
  
  ///draw a table (to use with Lisa_Schedule)
  void draw(Lisa_List<Lisa_ScheduleNode> *);
};

//**************************************************************************

/// graphical in and output of Lisa_Values
/** @author Per Willenius
    @version 2.3final
 */
class TCValues: public TCTable {
public:
  /// a label
  Lisa_Label *label;
  
  /// constructor
  TCValues(Tcl_Interp *interp,std::string main,std::string horizontal_canvas, 
	         std::string vertical_canvas,std::string label_name);
  
  ///destructor
  ~TCValues();
  
  /// draw a table (to use with Lisa_Values)
  void draw(Lisa_Matrix<int> *myMO,Lisa_Matrix<TIMETYP> *myPT, 
	          Lisa_Matrix<bool> *mySIJ, Lisa_Vector<TIMETYP> *myRD, 
            Lisa_Vector<TIMETYP> *myDD,  Lisa_Vector<double> * myWI);
  
  /// draw value on row xpos and column ypos
  void draw_entry(int xpos,int ypos,std::string value);
};

//**************************************************************************

/// graphical in and output of th CIJ matrix in Lisa_Schedule
/** @author Per Willenius
    @version 2.3final
 */
class TCCMatrix: public TCTable {
public:

  /// constructor
  TCCMatrix(Tcl_Interp *interp,std::string main,std::string horizontal_canvas, 
            std::string vertical_canvas);
  
  ///destructor
  ~TCCMatrix();
  
  /// draw the CIJ matrix in the main window
  void draw(Lisa_Matrix<TIMETYP> *myC, Lisa_Matrix<bool> *myCP);
};

//**************************************************************************
 
/// graphical output of graphs
/** @author Per Willenius
    @version 2.3final
 */
class TCGraph:  public TCGraphic {
public:
  /// size of a vertex
  float vertice_radius;
  
  /// constructor
  TCGraph();

  /// destructor
  ~TCGraph();
};

//**************************************************************************
 
/// graphical output of the sequence graph
/** @author Per Willenius
    @version 2.3final
 */
class TCSeqGraph: public TCGraph {
public:
  /// constructor
  TCSeqGraph(Tcl_Interp *interp,std::string main,std::string horizontal_canvas,
	           std::string vertical_canvas);

  ///destructor
  ~TCSeqGraph();
  
  /// draw the sequence graph
  void draw(Lisa_Matrix<bool> &CP, Lisa_SGraph &PG, Lisa_Matrix<bool> &SIJ);
  
  /// draw the sequence graph
  void draw(Lisa_Matrix<bool> &CP, Lisa_OsSchedule &myOsSchedule,
            Lisa_Matrix<bool> &SIJ);
};

//**************************************************************************

#endif

