/*
 * ************************* lsaobjct.hpp ******************************
 * 
 * description:      contains class Lisa_GenericObject and Lisa_Object 
 *                   
 *                   
 *                   
 *
 *
 * owner:            Per Willenius
 * date:             3.12.1998
 * patch 2.10.99 Thomas Tautenhahn
*/



#ifndef _lsaobjct_h 
#define _lsaobjct_h 


// ********************* System Includes ********************
#include <iostream>

using namespace std;


// ********************* Class Definitions *******************

/** All LiSA-Objects which name starts with Lisa_ must either have
    Lisa_GenericObject or Lisa_Object as parents. 
  
    Include file LiSA/src/lisa_dt/lsaobjkt.h, it uses iostream, 

    @author Per Willenius 
    @version 2.3pre2
*/ 

/** Lisa_GenericObject has only a virtual write-function and schould 
    be the ''Grandfather'' of all Lisa-Objects, which can write
    itself on a stream. 
    */   
class Lisa_GenericObject 
{
public:
  /// write Object-Information on a stream
  virtual void write(ostream& = cout) const = 0;
  virtual ~Lisa_GenericObject() { }
};


// operators for IO via streams:
inline ostream& operator << (ostream&strm, const Lisa_GenericObject& GenericObject)
{
  GenericObject.write(strm);
  return strm;
}

/** Lisa_Object has only a virtual write and read function and schould 
    be the ''Grandfather'' of all Lisa-Objects, which can write
    itself on a stream and can be written from a stream. 
    */   
class Lisa_Object : public Lisa_GenericObject 
{
public:
  /// read Object-Information on a stream
  virtual void read(istream& = cin)= 0;
};

// operators for IO via streams:
inline istream& operator >> (istream&strm, Lisa_Object& Object)
{
  Object.read(strm);
  return strm;
}

#endif









