/*
 * ************************* filentry.h ******************************
 * 
 * description:      contains class Lisa_FileEntry
 *                   
 *                   
 *                   
 *
 *
 * owner:            Per Willenius
 * date:             3.12.1998
*/



#ifndef _filentry_h 
#define _filentry_h 


// ********************* Lisa Includes ********************
#include "lsaobjct.hpp" 


// ********************* Class Definitions *******************

/** The Objects: Lisa_ControlParameters, Lisa_problemType, 
    Lisa_Values and Lisa_Schedule are children of Lisa_FileEntry
    because they are used for file commmunication
  
    Include file LiSA/src/lisa_dt/lsaobjkt.h, 

    @author Per Willenius 
    @version 2.3pre1
*/ 

/** Lisa_FileEntry has only a symbolic function.
    All Lisa-Objects, which are used for file-communication 
    via files of type *.lsa are children of  Lisa_FileEntry.
    */   
class Lisa_FileEntry : public Lisa_Object
{
public:
  // Object is empty

};

#endif









