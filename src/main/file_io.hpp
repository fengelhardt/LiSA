/*
 * ************** file_io_.hpp ******************************
 * 
 * description: file read and file save
 * 
 * owner: Per Willenius
 *
 * date: 28.9.1999
 * version: V 1.0
*/ 

/** @name File in and output

    @author Per Willenius
    @version 2.3pre2
*/ 

//@{

#ifndef _file_io_h 
#define _file_io_h 


//  ************** System Includes  ******************
#include "../misc/lisa_str.hpp"


//  **************** Functions  ******************

/// read the Lisa Kernel Variables from file
int read(string filename);

/// write Lisa Kernel Variables in file
int save(string filename);

#endif

//@}







