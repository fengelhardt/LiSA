
#ifndef _lsaobjct_h 
#define _lsaobjct_h 

#include <iostream>

/// interface having write functionality
/** Lisa_GenericObject has only a virtual write function and should 
    be the ''Grandfather'' of all LiSA objects, which can write
    itself on a stream. 
           
    @author Per Willenius 
    @version 2.3final
 */
class Lisa_GenericObject{
public:
  /// write object information on a stream
  virtual void write(std::ostream& = std::cout) const = 0;
  /// virtual destructor
  virtual ~Lisa_GenericObject() { }
};

//**************************************************************************

/// stream operator for writing Lisa_GenericObject objects
inline std::ostream& operator << (std::ostream& strm, 
                                  const Lisa_GenericObject& GenericObject){
  GenericObject.write(strm);
  return strm;
}

//**************************************************************************

/// interface having read functionality
/** Lisa_Object has only a virtual write and read function and should 
    be the ''Grandfather'' of all LiSA objects, which can write
    itself on a stream and can be read from a stream.
           
    @author Per Willenius 
    @version 2.3final
 */
class Lisa_Object : public Lisa_GenericObject {
public:
  /// read object information from a stream
  virtual void read(std::istream& = std::cin)= 0;
};

//**************************************************************************

/// stream operator for writing Lisa_Object objects
inline std::istream& operator >> (std::istream& strm, Lisa_Object& Object){
  Object.read(strm);
  return strm;
}

//**************************************************************************

#endif

