/*
 * ************** except.hpp ******************************************
 * 
 * special exception classes for use with LiSA 
 *
 * @author Thomas Tautenhahn
 * @version 2.3pre3
 *
 * 15.12.1998
*/

#include "except.hpp"

Lisa_ExceptionList G_ExceptionList;
string G_ExtraException="";

void Lisa_ExceptionList::lthrow(const string msg, const int code)
  {
    string type_word="ERROR";
    Messages.push(msg);
    Codes.push(code);
    if (code==WARNING) type_word="WARNING";
    if (output_to_cerr)
      cerr << "\n" << type_word << ": " << msg << endl;
    if (output_to_cout)
      cout << "\n" << type_word << ": " << msg << endl;
  }

void Lisa_ExceptionList::lfthrow(const string msg, const int code)
  {
    string type_word="ERROR";
    Messages.append(msg);
    Codes.append(code);
    if (code==WARNING) type_word="WARNING";
    if (output_to_cerr)
      cerr << "\n" << type_word << ": " << msg << endl;
    if (output_to_cout)
      cout << "\n" << type_word << ": " << msg << endl;
  }

string Lisa_ExceptionList::lcatch()
  {
    if (G_ExtraException!="") 
      {
        string tempstring=G_ExtraException;
        G_ExtraException="";
        return tempstring;
        
      }
    if (Codes.empty())
      return "No error messages in list.";
    Codes.pop();
    return Messages.pop();
  }
   
string Lisa_ExceptionList::lcatch(int code)
  {
    if (!Codes.search_first(code))
      return "No error of this kind in list.";
    Messages.locate(Codes.get_index());
    Codes.exclude();
    return Messages.exclude();
  }   















