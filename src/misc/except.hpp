
#ifndef _except_h 
#define _except_h 

#include <string>
#include <fstream>
#include <list>
#include <iterator>

/// handle error code and message together
struct Lisa_ExceptionNode{
  /// error message string
  std::string message;
  /// exception code
  int code;  
};
  
  
/// class for storing error messages and error codes
/** Due to compatibility problems LiSA does not use C++ exception handling 
    but uses a global list of error messages instead.
    
    @author Thomas Tautenhahn
    @version 2.3final
 */
class Lisa_ExceptionList{
public:
  /// error codes
  enum {ANY_ERROR=0,OUT_OF_RANGE,NO_MORE_MEMORY,END_OF_FILE,
        SYNTAX_ERROR,UNDEFINED_OBJECT,TCLTK_ERROR,FILE_NOT_FOUND,
        INCONSISTENT_INPUT,WARNING};
private:
  /// output flag
  bool output_to_cerr;
  /// output flag
  bool output_to_cout;
    
  /// list with error messages
  std::list<Lisa_ExceptionNode> Messages;

  /// look for first occurance of exception with code
  std::list<Lisa_ExceptionNode>::iterator search_first(const int code);
  
public:

  /// constructor 
  inline Lisa_ExceptionList(){ 
      output_to_cerr=false;
      output_to_cout=false; 
  }
  
  /// register error message:
  void lthrow(const std::string msg, const int code=ANY_ERROR);
  
  /// register error message to be catched after earlier messages
  void lfthrow(const std::string msg, const int code=ANY_ERROR);
  
  /// let all further messages be printed immediately to cerr
  inline void set_output_to_cerr() { output_to_cerr=true; }
  
  /// let all further messages be printed immediately to cout
  inline void set_output_to_cout() { output_to_cout=true; }
  
  /// return last error message, delete it from the list
  std::string lcatch();
  
  /// return last error message with given error code, delete it
  std::string lcatch(const int code);
  
  /// test whether error is registered
  inline bool empty() { return Messages.empty(); }
  
  /// test whether error with given code is in list
  inline bool empty(const int code) { return search_first(code) == Messages.end(); }
};

/// global instance of exception list:
extern Lisa_ExceptionList G_ExceptionList;

#endif

