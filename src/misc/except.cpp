/**
 * @author  Thomas Tautenhahn
 * @version 2.3pre3
 */

#include "except.hpp"

using namespace std;

//**************************************************************************

Lisa_ExceptionList G_ExceptionList;
string G_ExtraException="";

//**************************************************************************

void Lisa_ExceptionList::lthrow(const string msg, const int code){
  
  string type_word="ERROR";
  Messages.push(msg);
  Codes.push(code);
    
  if (code==WARNING) type_word="WARNING";
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
  Messages.append(msg);
  Codes.append(code);
  
  if (code==WARNING) type_word="WARNING";
  if (output_to_cerr) cerr << endl << type_word << ": " << msg << endl;
  if (output_to_cout) cout << endl << type_word << ": " << msg << endl;

#ifdef LISA_DEBUG
  if(!output_to_cerr && !output_to_cout){
    cerr << endl << type_word << ": " << msg << endl;
  }
#endif  
}
  
//**************************************************************************

string Lisa_ExceptionList::lcatch(){
    
  if(G_ExtraException!=""){
    string tempstring=G_ExtraException;
    G_ExtraException="";
    return tempstring;
  }
  
  if (Codes.empty()) return "No error messages in list.";
    
  Codes.pop();
  return Messages.pop();
}
  
//**************************************************************************
   
string Lisa_ExceptionList::lcatch(int code){
  if (!Codes.search_first(code)) return "No error of this kind in list.";
     
  Messages.locate(Codes.get_index());
  Codes.exclude();
     
  return Messages.exclude();
}   

//**************************************************************************
  
