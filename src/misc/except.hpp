/*
 * ************** except.h ******************************************
 * 
 * List of error messages for use with LiSA 
 *
 * Owner: Thomas Tautenhahn
 *
 * 15.12.1998
*/

#ifndef _except_h 
#define _except_h 

#include <string>
#include <ostream>

#include "../basics/list.hpp"

using namespace std;

/** @name Exception handling.
     Due to compatibility problems LiSA does not use C++ exception handling 
     but a global list of error messages instead.
    @Author Thomas Tautenhahn
    @version
    */

//@{
/// predefined error code
#define ANY_ERROR 0
/// predefined error code
const int OUT_OF_RANGE=1;
/// predefined error code
const int NO_MORE_MEMORY=2;
/// predefined error code
const int END_OF_FILE=3;
/// predefined error code
const int SYNTAX_ERROR=4;
/// predefined error code
const int UNDEFINED_OBJECT=5;
/// predefined error code
const int TCLTK_ERROR=6;
/// predefined error code
const int FILE_NOT_FOUND=7;
/// predefined error code
const int INCONSISTENT_INPUT=8;
/// predefined error code
const int WARNING=9;

// extra error message for objects which cannot use Lisa_ExceptionList 
extern string G_ExtraException;

/// class for storing error messages and error codes
class Lisa_ExceptionList 
{
public:
  Lisa_ExceptionList() 
    { 
      output_to_cerr=FALSE;
      output_to_cout=FALSE; 
    }
  /// register error message:
  void lthrow(const string msg, const int code=ANY_ERROR);
  /// register error message to be catched after earlier messages
  void lfthrow(const string msg, const int code=ANY_ERROR);
  /// let all further messages be printed immediately to cerr
  void set_output_to_cerr() { output_to_cerr=TRUE; }
  /// let all further messages be printed immediately to cout
  void set_output_to_cout() { output_to_cout=TRUE; }
  /// return last error message, delete it from the list
  string lcatch();
  /// return last error message with given error code, delete it
  string lcatch(int);
  /// test whether error is registered
  bool empty() { return (Codes.empty()&&(G_ExtraException=="")); }
  /// test whether error with given code is in list
  bool empty(int code) { return !(Codes.search_first(code)); }
private:
  bool output_to_cerr;
  bool output_to_cout;
  Lisa_List<string> Messages;
  Lisa_List<int> Codes;
};

/// global instance of exception list:
extern Lisa_ExceptionList G_ExceptionList;
//@}
#endif












