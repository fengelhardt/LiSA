#ifndef _LISAXMLDEFS_HPP_
#define _LISAXMLDEFS_HPP_

/// file used internally for xml implementation.
/// containes definitions for algorithm description objects and helper functions

#include <string>
#include <vector>

class Lisa_ProblemType;
//@{
/**
 * 
 * @author Jan Tusch
 * @version 3.0pre1
 */

/// Data structure for the definition of an GUI element
typedef struct {
  /// Name of the variable (used within Tcl scripts)
  std::string name;
  /// Default initial value 
  long default_value;
  //evtl. add range information here
  /// Caption to be displayed in GUI
  std::string caption;
} Integer_Control;

/// Data structure for the definition of an GUI element
typedef struct {
/// Name of the variable (used within Tcl scripts)
  std::string name;
  /// Default initial value 
  double default_value;
  //evtl. add range information here
  /// Caption to be displayed in GUI
  std::string caption;
} Real_Control;

/// Data structure for the definition of an GUI element
typedef struct {
  /// Name of the variable (used within Tcl scripts)
  std::string name;
  /// Caption to be displayed in GUI
  std::string caption;
  /// List of user's choice items
  std::vector < std::string > items;
} Choice_Control;

/// Data structure to store information obtained from an algorithm description
struct Lisa_ExtAlg {
  //GENRAL
  /// Name of the algorithm
  std::string name;
  /// Type of the algorithm (iterative, constructive, etc.)
  std::string type;
  /// Name of the binary (what to execute to run the algorithm)
  std::string call;
  /// Coding type (external)
  std::string code;
  /// Name of the Helpfile
  std::string helpFile;
  
  /// List of Problems the algorithm solves exactly
  std::vector <Lisa_ProblemType> handle_exact;
				
  /// List of Problems for which the algorithm is a heuristic
  std::vector <Lisa_ProblemType> handle_heuristic;
  
  /// List of GUI elements for adjusting integer value parameters
  std::vector <Integer_Control> Integer_Controls;
  /// List of GUI elements for adjusting real value parameters
  std::vector <Real_Control> Real_Controls;
  /// List of GUI elements for choosing options
  std::vector <Choice_Control> Choice_Controls;
  
  /// A fixed controls, just to display (for parameter test)
  typedef std::pair< std::string,std::string > Fixed_Control;

  /// List of fixed controls
  std::vector< Fixed_Control  > Fixed_Controls;
	
} ;

/// Conversion from ISO to UTF-8
xmlChar* string2xmlChar(const std::string& input);
/// COnversion from UTF-8 to ISO
bool xmlChar2string(const xmlChar* in , std::string& result);

//@}
#endif
