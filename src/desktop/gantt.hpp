
#ifndef _gantt_h 
#define _gantt_h 

#include "win_obj.hpp"

/// holding operation data
struct O_Data{
  //@{
  /// oparation data
  int machine;
  int job;
  TIMETYP time;
  //@}
};

/// just a typedef
typedef struct O_Data operation_data;

/// Gantt Chart.
/** graphical output of schedules
    @author Ines Wasmund
    @version 2.3final
 */
class TCGantt: public TCGraphic{
private:
  /// number of x-values
  int number_x_values;
  /// number of y-values
  int number_y_values;
  /// the last values of an x-row
  Lisa_Vector<TIMETYP> *last_values;
  /// the largest value of the gantt-chart
  TIMETYP x_max_value;
  /// size of the interval for the x-scaling
  double x_interval;
  /// size of the interval for the y-scaling
  int y_interval;
  /// geometry of the x-scale
  double x_geometry;
  /// geometry of the y-scale
  Lisa_Matrix<float> *y_geometry;
  /// extension of the x-scale
  double x_extent;
  /// extension of the y-scale
  double y_extent;
  /// extension of a box in y-direction
  double box_height;
  /// thickness of the frame for marking an operation in x-direction
  double x_frame_size;
  /// thickness of the frame for marking an operation in y-direction
  double y_frame_size;
  /// datas for an operation
  operation_data my_data;
  
  /// initialise
  void init(Lisa_Values *G_Value, Lisa_Schedule *G_Schedule, bool orientation);
  
  /// calculates the geometry of the y-scaling and saves it to y_geometry
  void set_y_geometry();
  
  /// get scale value
  /** gives the smallest interval for a scale for an interval of final_value
      with at least step_number steps */
  double scaling(TIMETYP final_value, int step_number);
  
  /// finds the color for a box of the ganttchart out
  int box_color(int i, int j, Lisa_Matrix<bool> *CP, bool orientation, 
                int color_type, Lisa_Vector<int> *active_path);
  
  /// draws the x-scaling
  void x_scaling();
  
  /// draws the y-scaling
  void y_scaling(bool orientation);
  
  /// draws the lines betwen the rows 
  void draw_x_lines();
  
  /// draws a colored box
  void draw_box(TIMETYP end, TIMETYP time, int x_value, int y_value, 
                int color, char *text_color);
  
  /// draws the duedates
  void draw_DD(Lisa_Vector<TIMETYP> *DD);
  
  /// draws the releasedates
  void draw_RD(Lisa_Vector<TIMETYP> *RD);
  
  /// checks for the right x-value for get_data(...);
  bool check_x_value(Lisa_Values *G_Values, Lisa_Schedule *G_Schedule,
                     int x, int y, TIMETYP time);
  
  
public:
  /// constructor
  TCGantt(Tcl_Interp *interp,std::string canv,std::string horizontal_canvas, 
          std::string vertical_canvas);
  
  /// draws a Gantt-Chart   
  void draw(Lisa_Matrix<bool> *CP, Lisa_Values *G_Values,
            Lisa_Schedule *G_Schedule, bool orientation, 
            int color_type, Lisa_Vector<int> *active_path);
  
  /// gives information about an operation
  operation_data* get_data(double x_coordinate, double y_coordinate, 
                           Lisa_Values *G_Values, 
                           Lisa_Schedule *G_Schedule, bool orientation);
  
  /// marks a special operation
  void mark(int machine, int job, Lisa_Values *G_Values, 
            Lisa_Schedule *G_Schedule, bool orientation);
  
  /// destructor
  ~TCGantt();
};

#endif

