
#ifndef _ctrlpara_h 
#define _ctrlpara_h 


#include <string>
#include <iostream>

#include "../basics/list.hpp"
#include "lsaobjct.hpp"

/// Organizes communication between LiSA and its external modules.     
/** You can define keywords and values of type string, double or long for each
    of these words. Lisa_ControlParameters holds these values and can write and 
    read them on files.

    An example of a Lisa_ControlParameters entry in a file: 

    <CONTROLPARAMETERS>
     string method SA
     long   number 14
     double parameter 23.23
    </CONTROLPARAMETERS>
  
    @author Per Willenius, Thomas Tautenhahn 
    @version 2.3final
*/   
class Lisa_ControlParameters: public  Lisa_Object{
private:

  //@{
  /// separate list to avoid type checking (searching is faster this way) 
  Lisa_List<std::string> StringKeyList, IntKeyList, DoubleKeyList;
  Lisa_List<std::string> StringList;
  Lisa_List<long int> IntList;
  Lisa_List<double> DoubleList;
  //@}
  
public:

/// flags for type of value
enum{UNDEFINED /** unknown type */ =0
    ,INT /// integer type
    ,DOUBLE /// floating point type
    ,STRING /// string type
    };
    
    /// add a new int-entry with name and value
    void add_key(const std::string name, long int value=0);
    
    /// add a new double-entry with name and value
    void add_key(const std::string name, double value=0.0);
    
    /// add a new string-entry with name and value
    void add_key(const std::string name, std::string value);

    /// test how keyword is defined (INT, DOUBLE, STRING or UNDEFINED)
    int defined(const std::string name);
    
    /// returns int-value which suits to the key
    long int get_int(const std::string name);
    
    /// returns double-value which suits to the key
    double get_double(const std::string name);
    
    /// returns string-value which suits to the key
    std::string get_string(const std::string name);

    /// write in stream
    void write(std::ostream & = std::cout) const;
    
    /// read from stream
    void read(std::istream& = std::cin);

};

#endif

