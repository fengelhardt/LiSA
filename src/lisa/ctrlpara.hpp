/*
 * ************************* ctrlpara.hpp ******************************
 * 
 * description:      contains class Lisa_ControlParameters 
 *                   This class can be used to hold three types of variables
 *                   (string, int, float)
 *                   This makes it easy to communicate with external programs.
 *
 *
 * owner:            Per Willenius
 * date:             25.2.1998
 * version:          V 1.0
*/



#ifndef _ctrlpara_h 
#define _ctrlpara_h 


// ********************* System Includes ********************
#include <string>
#include <iostream>

#include "../basics/list.hpp"
#include "lsaobjct.hpp"

using namespace std;

const int UNDEFINED=0;
const int INT=1;
const int DOUBLE=2;
const int STRING=3;


// ********************* Class Definitions *******************

/** Organizes communication between LiSA and its external modules.     
    You can define keywords and values of type string, double or int for each
    of these words. Lisa_ControlParameters holds these values and can write and read 
    them on files. For definition of file format see 
    LiSA/doc/use/fileform.doc.\\ 

    An Example of a Lisa_ControlParameters Entry in a file: 

    <CONTROLPARAMETERS>

    string    Method= SA

    int       Zahl1= 14

    double     Parameter1= 23.23

    </CONTROLPARAMETERS>\\
  
    Include file LiSA/src/lisa_dt/ctrlpara.h, it uses iostream, string, 
    glist and nodewith.

    @author Per Willenius and Thomas Tautenhahn 
    @version 2.3pre2
*/ 
  
class Lisa_ControlParameters: public  Lisa_Object
{
  private:
    // 6 separate lists to avoid type checking (searching is faster this way): 
    Lisa_List<string> StringKeyList, IntKeyList, DoubleKeyList;
    Lisa_List<string> StringList;
    Lisa_List<long int> IntList;
    Lisa_List<double> DoubleList;
  public:
    /**@name define keywords and values: */
    //@{
     /// add a new int-entry with name and value
     void add_key(const string name, long int value=0);
     /// add a new double-entry with name and value
     void add_key(const string name, double value=0.0);
     /// add a new string-entry with name and value
     void add_key(const string name, string value);
    //@}
    /**@name access values by keyword: */
    //@{ 
     /// test how keyword is defined (INT, DOUBLE, STRING or UNDEFINED)
     int defined(const string name);
     /// returns int-value which suits to the key
     long int get_int(const string name);
     /// returns double-value which suits to the key
     double get_double(const string name);
     /// returns string-value which suits to the key
     string get_string(const string name);
    //@}
    /**@name in- and output: */
    //@{
     /// write in stream
     void write(ostream & = cout) const;
     /// read from stream
     void read(istream& = cin);
    //@} 
 };

#endif









