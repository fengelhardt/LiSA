/*
 * ************** file_io_.hpp ******************************
 * 
 * description: file read and file save
 * 
 * @author Per Willenius
 *
 * date: 28.9.1999
 * version: V 1.0
*/ 

/** @name File in and output

    @author Per Willenius
    @version 2.3pre3
*/ 

//@{

#ifndef _file_io_h 
#define _file_io_h 


//  ************** System Includes  ******************
#include <string>

using namespace std;


//  **************** Functions  ******************

/// read the Lisa Kernel Variables from file
int read(string filename);

/// write Lisa Kernel Variables in file
int save(string filename);

#endif

//@}







