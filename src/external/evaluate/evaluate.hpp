/*
 * ************** evaluate.hpp ********************************************
 * 
 * description:   realizes the evaluation of tables and the graphical 
 *		  representation of it 
 * 
 * owner:         Ines Wasmund
 *
 * date:          12.10.1999
*/
 

/**  @name Graphical Output
  
  file: LiSA/tcl/external/eval/evaluate.hpp
  
  include:  <iostream.h>, <tk.h>, lisa_str.hpp, matrix.hpp, basicwin.hpp, 
	    <fstream.h>, <math.h>, ctrlpara.hpp, inout.hpp and int2str.hpp
  @author Ines Wasmund
  @version 2.2a0
 */

//@{


#ifndef _evaluate_h 
#define _evaluate_h 



#define MAINCANV ".menu.frame_for_all_canvas.fra32.cpd34.03"
#define VERTCANV ".menu.frame_for_all_canvas.fra32.fra33.can38"
#define HORICANV ".menu.frame_for_all_canvas.fra28.fra36.can37"



// **************** Project Includes ********************
#include <iostream.h>
#include <tk.h>
#include "../../basics/lisa_str.hpp"
#include "../../basics/matrix.hpp"
#include "../../desktop/basicwin.hpp"


/** @name Evaluation 
    @author Ines Wasmund
    @version 2.2a0

*/

//@{

/** Realizes the evaluation of values in a table.
    For using information please read LiSA/doc/language/evaluate.doc.*/


//  **************  Function Definitions ******************

/// searches in file until string str is found or returns 0
int Data_find(string str, istream & fein);


/// calls an object for drawing a table
int TC_Table(ClientData clientData, 
	     Tcl_Interp *interp,
	     int argc, char *argv[]);

/// calls an object for drawing a diagram
int TC_PointXY(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 

/// calls an object for export of data 
int TC_export(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 

/// calls an object for calculating characteristica 
int TC_characte(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 

/// calls an object for calculating the confidence estimation
int TC_confid(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 

/// calls an object for calculating the correlation
int TC_corr(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 

/// calls an object for drawing a 2-dimensional bar diagram
int TC_Bar2(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 

/// calls an object for drawing a 3-dimensional bar diagram
int TC_Bar3(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 

/// calls an object for drawing a pie diagram
int TC_pie(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 

/// calls an object for drawing a trendline
int TC_trend(ClientData clientData,
	 Tcl_Interp *interp,
	 int argc, char *argv[]); 


//  **************  Object Definitions ******************


/** Class for administration. It holds the original values of the table, 
the values of the main menu, which are validated by it, and decides what 
is to do for this values of menu, in particular decides which object is 
to call*/

class preference
{
private:
/// the number of the original columns of the table
  int number_original_columns;
/// the number of the original rows of the table
  int number_original_rows;

public:
/// holds which type of values, which are to consider, 
/// are chosen in the menu
/// possible values are: "characteristica", "confidence" and "correlation" 
  string values_type; 

/// holds which type of drawing was chosen in the menu
///possible values are: "table", "pointxy", "bar2", "bar3" and "pie"
  string draw_type;
// holds whether a trend is to draw
  bool trend;

/// holds the original names of columns 
  Lisa_Vector<string> *original_columns;
/// holds the original values of the table
  Lisa_Matrix<TIMETYP> *original_values;

/// constructor
  preference(int argc, char *argv[]);
/// gives the number of original columns
  int get_number_original_columns();
/// gives the number of original rows
  int get_number_original_rows();
/// draws whatever was chosen in the menu
  void draw(Tcl_Interp *interp, string MC ,string HC, string VC);
/// exports a table or the contents of the mainwindow to a document
  void export_to(Tcl_Interp *interp, string MC ,string HC, string VC);
/// destructor
  ~preference();
};


//----------------------------------------------------------------------------

/// Class of general functions used for getting information out of the menu
class menudata
{
protected:
/// converts a string ("1" or "0") in a boolean value
  bool str2bool(string convert);
/// converts a string got from Tcl into a boolean vector of columns 
/// which are used for calculating
  Lisa_Vector<bool>* get_columns(Tcl_Interp *interp, 
				 Lisa_Vector<bool> *columns, 
				 char *structure, char *type);
/// counts in the vector all values which are true
  int count_bool_values(Lisa_Vector<bool> *columns);
/// converts a string got from Tcl into a number of column
/// which is used for calculating
  int get_column(Tcl_Interp *interp, char *structure, char *type);
};

//----------------------------------------------------------------------------

/// Class for holding information about the in the menu chosen characteristics
/// of the characteristica
class character: public menudata
{
protected:
/// stores which charateristeristica (exp_value, minimum, maximum, 
/// variance, st_deviation) is wanted
  Lisa_Vector<bool> *characteristica;
/// string which contains whether all or only some columns are used
  string radio_columns;
/// vector which contains the columns which are used
  Lisa_Vector<bool> *columns;

/// gets the information from the menu
  void get_values(Tcl_Interp *interp);

public:
/// constructor
  character(Tcl_Interp *interp);
/// destructor
  ~character();
};

//----------------------------------------------------------------------------

/// Class for holding information about the in the menu chosen characteristics
/// of the correlation
class correlation: public menudata
{
protected:
/// the original number of columns
  int number_original_columns;
/// vector to indicate which columns are used in the rows
  Lisa_Vector<bool> *columns1;    
/// vector to indicate which columns are used in the columns
  Lisa_Vector<bool> *columns2;    
/// string which contains wheter the covariances (="cov") 
/// or the correlation-coefficients (="corrc") are wished
  string radio_type;       
/// string which contains whether all or some columns are used for the rows
  string radio_columns1;
/// string which contains whether all or some columns are used for the columns
  string radio_columns2;

/// gets the information from the menu
  void get_values(Tcl_Interp *interp);

public:
/// constructor
  correlation(Tcl_Interp *interp);
/// destructor
  ~correlation();
};

//----------------------------------------------------------------------------

/// Class of a general table
class table
{
public: //protected:
/// converts a string to a boolean value
  bool str2bool(string convert);
/// number of rows of the table
  int number_rows;
/// number of columns of the table
  int number_columns;
/// number of values of the table
  int number_values;
/// names of the rows of the table
  Lisa_Vector<string> *row_names;
/// names of the columns of the table
  Lisa_Vector<string> *column_names;
/// values of the table
  Lisa_Matrix<TIMETYP> *table_values;

/// calculates the maximal length of an entry of the table
  int entry_length(string dec_point);

public:
/// exports the table to a tex-file
  void export_latex(string file);
/// exports the table to a by excel readable file
  void export_excel(string file);
};


//----------------------------------------------------------------------------

/// Class of the graphical output of a table
class graphic:public TCGraphic, public table
{
public: //protected:
/// width of an entry in the table
  int entrywidth;
/// height of an entry in the table
  int entryheight;
/// width of the mainwindow
  int width;
/// height of the mainwindow
  int height;
/// minimum of the values below 0
  TIMETYP minimum;
/// maximum of the values above 0
  TIMETYP maximum;
/// y-value for drawing a graphic
  TIMETYP y_value;
/// stepsize in y-direction for the coordinatesystem
  double y_step;
/// distance from the outside margin to the graphic
  double frame;
/// distance in the graphic between the original values 
/// and the characteristica
  double dist_between;
/// number of the values in x-direction
  int x_number;
/// x-scale of the graphic
  double x_scale;
/// y-scale of the graphic
  double y_scale;
/// 0 at the y-scale
  double y0;
/// x-value for the first row_name;
  double first_row_name;
/// distance of row_names in diagram;
  double step_row_name;

/// finds the minimum of the table
  TIMETYP get_minimum();
/// finds the maximum of the table
  TIMETYP get_maximum();

public:
/// constructor
  graphic(Tcl_Interp *interp, string MC, string HC, string VC);
/// initialisation of the canvas
  void init_canvas();
/// initialisation of the scale
/// Set x_number before using!
  bool init_scale(TIMETYP mini,TIMETYP maxi);
/// setting the variables in case of an appearance of an error
  void error_set();
/// gives the smallest interval for a scale for an interval of final_value
/// with at least step_number steps 
  double scaling(double final_value, int step_number);
/// draws the coordinate system
  void coordinate_system();
/// draws the table
  void draw_table();
/// draws the point(x,y) representation 
  void draw_pointxy();
/// draws the 2-dimensional bar chart
  void draw_bar2();
/// destructor
  ~graphic();
};


//----------------------------------------------------------------------------

/// Class for the valuation of the values of the table, which is needed 
/// in different evaluations
class valuation: public graphic
{
protected:
/// store of the expected values
  Lisa_Matrix<TIMETYP> *store_exp_values;
/// store of the variances
  Lisa_Matrix<TIMETYP> *store_variances;
/// store of the standard deviations
  Lisa_Matrix<TIMETYP> *store_st_deviations;


/// calculates the expected values of the given table *values_from and
/// writes it to *values_to
  void exp_values(int k, Lisa_Matrix <TIMETYP> *values_from, 
		  Lisa_Matrix <TIMETYP> *values_to, int n_columns, int n_rows);

/// calculates the variances of the given table *values_from and
/// writes it to *values_to based on the expected values, which are stored
/// in row row_exp_value of *exp_values_from
  void variances(int k, int row_exp_values,
		 Lisa_Matrix <TIMETYP> *values_from,
		 Lisa_Matrix <TIMETYP> *exp_values_from, 
		 Lisa_Matrix <TIMETYP> *values_to, int n_columns, int n_rows);
/// calculates the standard deviations of the given table *values_from and
/// writes it to *values_to based on the variances, which are stored
/// in row row_variances of *variances_from
  void st_deviations(int k, int row_variances,
		     Lisa_Matrix <TIMETYP> *variances_from,
		     Lisa_Matrix <TIMETYP> *values_to, int n_columns);

public:
/// constructor
  valuation(Tcl_Interp *interp, string MC, string HC, string VC);
};

//----------------------------------------------------------------------------

/// Class of the table with added characteristica
class charactertable: public character, public valuation
{
protected:
/// number of row in the table, in which are the expected values to put
  int row_exp_values;
/// number of row in the table, in which are the variances to put
  int row_variances;

/// initialisation
  void init(Tcl_Interp *interp);
/// finds out the minimum of the k-th column 
  void minimums(int k);
/// finds out the maximum of the k-th column 
  void maximums(int k);

public:
/// constructor
  charactertable(Tcl_Interp *interp, string MC, string HC, 
	         string VC);
/// destructor
  ~charactertable(); 
};

//----------------------------------------------------------------------------

/// Class of the table of correlation
class correlationtable: public correlation, public valuation
{
protected:
/// store of the covariances
  Lisa_Matrix<TIMETYP> *store_covariance;

/// initialisation
  void init();
/// calculates the covariances and writes it to *where_to 
  void covariance(Lisa_Matrix<TIMETYP> *where_to, int divisor);
/// calculates the covariances based on the covariances stored 
/// in *store_covariance and writes it to *table_values
  void corr_coeffi();

public:
/// constructor
  correlationtable(Tcl_Interp *interp, string MC, string HC, string VC);
/// destructor
  ~correlationtable();
};


//@}

#endif






















