
#ifndef _basicwin_h 
#define _basicwin_h

#include <string>
#include <tk.h>

//**************************************************************************

//@{
///constants
const int MAX_CANV_COLORS=23;
const int ENTRYHEIGHT =25;
const int ENTRYWIDTH =60;
//@}

/// output to the main window
/** Lisa_Canvas manages the basic drawing operations like line, or circle.
    If the name of such function contains the string _rel (like line_rel),
    it has to be called with relativ coordinates in the range of 0 to 100.
    Otherwise a function uses absolute coordinates (pixel).
    
    Lisa_Canvas contains a list of 23 (value of MAX_CANV_COLORS) colors. Such 
    it is possible to call a drawing function with the name of the color as
    string (like "Black" or "\#4992a6"), or as number.
  
    @author Per Willenius
    @version 2.3final
 */
class Lisa_Canvas{
  
  /// connection to the Tcl interpreter
  Tcl_Interp *cv_interp;
  
  /// command
  char *command;
  
  /// 23 nice colours:
  char color[MAX_CANV_COLORS][8]; 
  
  /// width and height of canvas
  float width,height;

  public:
  /// name of canvas in Tcl/Tk
  char *cv_name;
  
  /// constructor
  Lisa_Canvas(const char * name,Tcl_Interp * tclinterp);
  
  /// clear whole canvas
  void clear();
  
  /// delete all ements with tag or id
  void clear(std::string tag);

  /// delete element on position xpos ypos
  void clear( double xpos, double ypos );
  
  /// draw line
  void line(float x1,float y1,float x2,float y2,char * col);
  
  /// draw line with tag
  void line(float x1,float y1,float x2,float y2,std::string tag, char * col);
  
  /// draw line
  void line(float x1,float y1,float x2,float y2,int col);

  /// draw linie with relativ koordinates (0-100)
  void line_rel(float x1,float y1,float x2,float y2,char * col);

  /// draw linie with relativ koordinates (0-100)
  void line_rel(float x1,float y1,float x2,float y2,int col);

  /// draw horizontal dashed line with relativ koordinates (0-100)
  void dashed_hline_rel(double x1, double x2, double y);

  /// draw arrow (used as directed edge)
  void arrow(float x1,float y1,float x2,float y2,char* col);
  
  /// draw arrow (used as directed edge)
  void arrow(float x1,float y1,float x2,float y2,int col);
  
  /// draw arrow with relativ koordinates (0-100)
  void arrow_rel(float x1,float y1,float x2,float y2,char* col);
  
  /// draw arrow with relativ koordinates (0-100)
  void arrow_rel(float x1,float y1,float x2,float y2,int col);
  
  /// draw arrow (used as directed edge)
  /** additional to arrow() the arrow is bent corresponding
      to the value of rad */ 
  void arc_arrow(float x1,float y1,float x2,float y2,float rad,char *col);

  /// draw rectangle with  koordinates x1,y1,x2,y2
  void rect(float x1,float y1,float x2,float y2,char * col);
  
  /// draw rectangle with  koordinates x1,y1,x2,y2
  void rect(float x1,float y1,float x2,float y2,int col);

  /// draw rectangle with relativ koordinates (0-100)
  void rect_rel(float x1,float y1,float x2,float y2,char * col);

  /// draw rectangle with relativ koordinates (0-100)
  void rect_rel(float x1,float y1,float x2,float y2,int col);

  /// write text
  void text(float x,float y,char *text, char * col);

  /// write text
  void text(float x,float y,char *text, int col);

  /// write text with relativ koordinates (0-100)
  void text_rel(float x,float y,char *text, char * col);
  
  /// write text with relativ koordinates (0-100)
  void text_rel(float x,float y,char *text, int col);

  /// write text
  void text(float x,float y, std::string text, char * col);
  
  /// write text
  void text(float x,float y, std::string text, int col);
  
  /// write text  with relativ koordinates (0-100)
  void text_rel(float x,float y, std::string text, char * col);
  
  /// write text  with relativ koordinates (0-100)
  void text_rel(float x,float y, std::string text, int col);

  /// draw point
  void point(float x,float y,char col);

  /// draw circle
  void circle(float x,float y,float rad,char* col);

  /// draw circle
  void circle(float x,float y,float rad,int col);

  /// draw filled circle
  void fil_circle(float x,float y,float rad,char* col);

  /// draw filled circle
  void fil_circle(float x,float y,float rad,int col);

  /// set the canvas geometry
  void set_wh(int with, int height);
  
  /// return the width of the canvas
  int get_width(); 
  
  /// return the height of the canvas 
  int get_height(); 
  
   /// destructor
  ~Lisa_Canvas();
};

//**************************************************************************

/// output of simple text
class Lisa_TextObject{
  
  /// connection to the Tcl interpreter
  Tcl_Interp *cv_interp;
  
  ///command
  std::string command;
  
  /// width and height of textwindow
  float width,height;
  
public:

  /// name of textwindow in Tcl/Tk
  char *cv_name;
  
  ///default constructor
  Lisa_TextObject();
  
  /// constructor
  Lisa_TextObject(char * name,Tcl_Interp * tclinterp);
  
  ///constructor
  Lisa_TextObject(Tcl_Interp * tclinterp);
  
  //@{
  /// write text to the window
  void text(const char * text);
   void text(const std::string text);
  //@}
  
  /// clear all
  void clear();
  
  ///destructor
  ~Lisa_TextObject();
};

//**************************************************************************

/// manage the label widget
class Lisa_Label{
  
  /// connection to the Tcl interpreter
  Tcl_Interp *label_interp;
  
public:
  /// name of the label in Tcl/Tk
  std::string label_name;
  
  ///constructor
  Lisa_Label(std::string name,Tcl_Interp * tclinterp);

  /// print text on label position
  void set_text(std::string text);

  ///destructor
  ~Lisa_Label();
};

//**************************************************************************

/// graphical in-and output 
/** used in the main window and in the schedule window and values window */
class TCGraphic{
 public:
  /// constructor
  TCGraphic();
  
  /// zoomed ?
  /** if the zoom flag is true each output function cannot set the canvas 
      geometry */
  bool zoom;
  
  /// canvas to draw the graphic model
  Lisa_Canvas* main_canvas;
  
  /// canvas to draw the names of the columns
  Lisa_Canvas* canv_horizontal;
  
  /// canvas to draw the names of the rows
  Lisa_Canvas* canv_vertikal;
  
  /// clear all 3 canvas:
  void clear();
  
  /// set geometry of the 3 canvas
  void set_wh(int width, int height);
  
  ///destructor
  ~TCGraphic();
};

//**************************************************************************

/// graphical input and output in form of a table
class TCTable :public TCGraphic{
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

//**************************************************************************

#endif

