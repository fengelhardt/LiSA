
#ifndef _except_h 
#define _except_h 

#include <string>
#include <fstream>

#include "../basics/list.hpp"

/// error codes
enum{ANY_ERROR=0,OUT_OF_RANGE,NO_MORE_MEMORY,END_OF_FILE,SYNTAX_ERROR,
     UNDEFINED_OBJECT,TCLTK_ERROR,FILE_NOT_FOUND,INCONSISTENT_INPUT,WARNING};

// extra error message for objects which cannot use Lisa_ExceptionList 
extern std::string G_ExtraException;

/// class for storing error messages and error codes
/** Due to compatibility problems LiSA does not use C++ exception handling 
    but uses a global list of error messages instead.
    
    @author Thomas Tautenhahn
    @version 2.3rc1
 */
class Lisa_ExceptionList{
public:
  
  /// constructor 
  Lisa_ExceptionList(){ 
      output_to_cerr=FALSE;
      output_to_cout=FALSE; 
  }
  
  /// register error message:
  void lthrow(const std::string msg, const int code=ANY_ERROR);
  
  /// register error message to be catched after earlier messages
  void lfthrow(const std::string msg, const int code=ANY_ERROR);
  
  /// let all further messages be printed immediately to cerr
  inline void set_output_to_cerr() { output_to_cerr=TRUE; }
  
  /// let all further messages be printed immediately to cout
  inline void set_output_to_cout() { output_to_cout=TRUE; }
  
  /// return last error message, delete it from the list
  std::string lcatch();
  
  /// return last error message with given error code, delete it
  std::string lcatch(int);
  
  /// test whether error is registered
  inline bool empty() { return (Codes.empty()&&(G_ExtraException=="")); }
  
  /// test whether error with given code is in list
  inline bool empty(int code) { return !(Codes.search_first(code)); }

private:

  /// output flag
  bool output_to_cerr;
  /// output flag
  bool output_to_cout;
 
  /// list with error messages
  Lisa_List<std::string> Messages;
  /// list with error codes
  Lisa_List<int> Codes;
};

/// global instance of exception list:
extern Lisa_ExceptionList G_ExceptionList;

#endif

