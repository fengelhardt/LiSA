/**
 * @author  Thomas Tautenhahn
 * @version 2.3final
 */

#include <iostream>
#include "except.hpp"

using namespace std;

//**************************************************************************

Lisa_ExceptionList G_ExceptionList;

//**************************************************************************

void Lisa_ExceptionList::lthrow(const string msg, const int code){
  
  string type_word="ERROR";
  if (code==WARNING) type_word="WARNING";
  
  Lisa_ExceptionNode en;
  en.message = msg;
  en.code = code;
  
  Messages.push_front(en);

  if (output_to_cerr) cerr << endl << type_word << ": " << msg << endl;
  if (output_to_cout) cout << endl << type_word << ": " << msg << endl;

#ifdef LISA_DEBUG
  if(!output_to_cerr && !output_to_cout){
    cerr << endl << type_word << ": " << msg << endl;
  }
#endif  
}
  
//**************************************************************************

void Lisa_ExceptionList::lfthrow(const string msg, const int code){
  string type_word="ERROR";
  if (code==WARNING) type_word="WARNING";
  
  Lisa_ExceptionNode en;
  en.message = msg;
  en.code = code;
  
  Messages.push_back(en);
  
  if (output_to_cerr) cerr << endl << type_word << ": " << msg << endl;
  if (output_to_cout) cout << endl << type_word << ": " << msg << endl;

#ifdef LISA_DEBUG
  if(!output_to_cerr && !output_to_cout){
    cerr << endl << type_word << ": " << msg << endl;
  }
#endif  
}
  
//**************************************************************************

std::list<Lisa_ExceptionNode>::iterator Lisa_ExceptionList::search_first(const int code){
  std::list<Lisa_ExceptionNode>::iterator it = Messages.begin();
  
  while(it != Messages.end()){
    if(it->code == code) return it;
    it++;
  }
  
  return Messages.end();
}
  
//**************************************************************************

string Lisa_ExceptionList::lcatch(){

  if (Messages.empty()) return "No error messages in list.";
    
  Lisa_ExceptionNode en = Messages.front();
  Messages.pop_front();
  return en.message;
}
  
//**************************************************************************
   
string Lisa_ExceptionList::lcatch(const int code){
  list<Lisa_ExceptionNode>::iterator it = search_first(code);
  
  if(it == Messages.end()) return "No error of this kind in list.";
     
  string retval = it->message;
  Messages.erase(it);
     
  return retval;
}   

//**************************************************************************
  
