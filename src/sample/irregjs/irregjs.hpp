
#ifndef _irredjs_h 
#define _irredjs_h 

#include <iostream>

#include "../../misc/except.hpp"
#include "../../scheduling/js_sched.hpp"

//**************************************************************************

/// specialisation of Lisa_JsProblem
/** @version 2.3rc1
    @author Thomas Tautenhahn
 */
class NM_JsProblem: public Lisa_JsProblem{
public:
  /// constructor
  NM_JsProblem( Lisa_Values *Pin );
  
  /// read problem data from stream
  void read(std::istream& = std::cin);

  /// destructor  
  ~NM_JsProblem(){}
};

//**************************************************************************

/// specialisation of Lisa_JsSchedule
/** @version 2.3rc1
    @author Thomas Tautenhahn
 */   
class NM_JsSchedule: public Lisa_JsSchedule{
public:
  /// constructor
  NM_JsSchedule( Lisa_JsProblem *Pin );
  
  /// read schedule data from stream
  void read(std::istream& = std::cin);

  /// destructor  
  ~NM_JsSchedule(){}
};

//**************************************************************************

/// stream operator for reading NM_JsSchedule object
inline std::istream& operator >> (std::istream& strm, NM_JsSchedule& m){
  m.read(strm);
  return strm;
}

//**************************************************************************

/// stream operator for reading NM_JsProblem object
inline std::istream& operator >> (std::istream& strm, NM_JsProblem& m){
  m.read(strm);
  return strm;
}

//**************************************************************************

#endif

