 /*
 * ****************** basicwin.hpp ******************************
 * 
 * description:      objects to draw graphical objects and text
 *
 * @author            Per Willenius
 *
 * date:             28.9.1999
 *
 */ 

/** @name Basic Window Classes
  
  file: LiSA/src/desktop/basicwin.hpp
  
  @author Per Willenius
  @version 2.3pre3
*/ 

//@{

#ifndef _basicwin_h 
#define _basicwin_h

#include <string>
#include <tk.h>

const int MAX_CANV_COLORS=23;
const int ENTRYHEIGHT =25;
const int ENTRYWIDTH =60;

/** output in the main window.
  Lisa_Canvas manage the basic drawing operations like line, or circle.
  If the name of such function contain the string _rel (like line_rel),
  it has to been called with relativ coordinates in the range of 0 to 100.
  Otherwise a function use absolute coordinates (pixel).
  Lisa_Canvas contain a list of 23 (value of MAX_CANV_COLORS) colors.
  Such it is possible to call a drawing function with the name of the color as
  string (like "Black" or "\#4992a6"), or as number.
  */
class Lisa_Canvas{
  /// connection to the Tcl-Interpreter
  Tcl_Interp *cv_interp;
  char *command;
  /// 22 nice colours:
  char color[MAX_CANV_COLORS][8]; 
  /// width and height of Canvas
  float width,height;
public:
  /// Name of Canvas in Tcl/Tk
  char *cv_name;
  Lisa_Canvas(const char * name,Tcl_Interp * tclinterp);
  /// Clear whole Canvas
  void clear();
  /// Delete all ements with tag or id
  void clear(std::string tag);
  /// Delete element on position xpos ypos
  void clear( double xpos, double ypos );
  /**@name line: */
  //@{
  /// Line
  void line(float x1,float y1,float x2,float y2,char * col);
  /// line with tag
  void line(float x1,float y1,float x2,float y2,std::string tag, char * col);
  /// Line
  void line(float x1,float y1,float x2,float y2,int col);
  /// Linie with relativ koordinates (0-100)
  void line_rel(float x1,float y1,float x2,float y2,char * col);
 /// Linie with relativ koordinates (0-100)
  void line_rel(float x1,float y1,float x2,float y2,int col);
  /// horizontal dashed line with relativ koordinates (0-100)
  void dashed_hline_rel(double x1, double x2, double y);
  //@}
  /**@name arrow: */
  //@{
  /// arrow (used as directed edge)
  void arrow(float x1,float y1,float x2,float y2,char* col);
 /// arrow (used as directed edge)
  void arrow(float x1,float y1,float x2,float y2,int col);
  /// arrow with relativ koordinates (0-100)
  void arrow_rel(float x1,float y1,float x2,float y2,char* col);
 /// arrow with relativ koordinates (0-100)
  void arrow_rel(float x1,float y1,float x2,float y2,int col);
  /** arrow (used as directed edge)
    additional to arrow() the arrow is bent corresponding
    to the value of rad */ 
  void arc_arrow(float x1,float y1,float x2,float y2,float rad,char *col);
   //@}
  /**@name rectangle: */
  //@{
  /// draw rectangle with  koordinates x1,y1,x2,y2
  void rect(float x1,float y1,float x2,float y2,char * col);
  /// draw rectangle with  koordinates x1,y1,x2,y2
  void rect(float x1,float y1,float x2,float y2,int col);
  /// draw rectangle with relativ koordinates (0-100)
  void rect_rel(float x1,float y1,float x2,float y2,char * col);
  /// draw rectangle with relativ koordinates (0-100)
  void rect_rel(float x1,float y1,float x2,float y2,int col);
  //@}
  /**@name text: */
  //@{
  /// write text called with char * parameter
  void text(float x,float y,char *text, char * col);
  /// write text called with char * parameter
  void text(float x,float y,char *text, int col);
  /// char * text with relativ koordinates (0-100)
  void text_rel(float x,float y,char *text, char * col);
  /// char * text with relativ koordinates (0-100)
  void text_rel(float x,float y,char *text, int col);
  /// text called with string parameter
  void text(float x,float y, std::string text, char * col);
  /// text called with string parameter
  void text(float x,float y, std::string text, int col);
  /// string text  with relativ koordinates (0-100)
  void text_rel(float x,float y, std::string text, char * col);
  /// string text  with relativ koordinates (0-100)
  void text_rel(float x,float y, std::string text, int col);
  //@}
  /**@name other draw routines: */
  //@{
  /// point
  void point(float x,float y,char col);
  /// circle
  void circle(float x,float y,float rad,char* col);
  /// circle
  void circle(float x,float y,float rad,int col);
  /// filled circle
  void fil_circle(float x,float y,float rad,char* col);
  /// filled circle
  void fil_circle(float x,float y,float rad,int col);
  //@}
  /**@name with and height: */
  //@{
  /// set the canvas geometry
  void set_wh(int with, int height);
  /// return the width of the canvas
  int get_width(); 
  /// return the height of the canvas 
  int get_height(); 
  //@}
  ~Lisa_Canvas();
};

/// output of simple text
class textobj{
  /// connection to the Tcl-Interpreter
  Tcl_Interp *cv_interp;
  std::string command;
  /// width and height of textwindow
  float width,height;
public:
  /// Name of Textwindow in Tcl/Tk
  char *cv_name;
  textobj();
  textobj(char * name,Tcl_Interp * tclinterp);
  textobj(Tcl_Interp * tclinterp);
  /// write text in the window
  void text(const char * text);
   void text(const std::string text);
  /// clear all
  void clear();
  ~textobj();
};

/// Manage the label widget
class Lisa_Label{
  /// connection to the Tcl-Interpreter
  Tcl_Interp *label_interp;
public:
  /// Name of the label in Tcl/Tk
  std::string label_name;
  Lisa_Label(std::string name,Tcl_Interp * tclinterp);
  /// print text on label position
  void set_text(std::string text);
  ~Lisa_Label();
};

/** graphical in-and output 
  (used in the main window and in the schedule window and values window)
  */
class TCGraphic{
private:
 public:
  TCGraphic();
  /// if the zoom flag is TRUE 
  /// each output function cannot set the canvas geometry
  bool zoom;
  /// Canvas to draw the graphic model
  Lisa_Canvas* main_canvas;
  /// Canvas to draw the names of the columns
  Lisa_Canvas* canv_horizontal;
  /// Canvas to draw the names of the rows
  Lisa_Canvas* canv_vertikal;
  /// clear all 3 canvas:
  void clear();
  /// set geometry of the 3 canvas
  void set_wh(int width, int height);
  ~TCGraphic();
};

/// graphical in-and output in form of a table
class TCTable :public TCGraphic{
private:
public:
  /// width of an entry
  int entrywidth;
  /// height of an entry
  int entryheight;
  /// draw the horizontal and vertical lines of the table 
  void draw_table(int m, int n);
  /// mark an entry as selected
  void mark(int row, int column);
  /// constructor
  TCTable();
};

//@}

#endif

