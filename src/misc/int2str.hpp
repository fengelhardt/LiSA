/*
 * ************** int2str_.h ******************************
 * 
 * description: functions to convert from int to string 
 *
 * owner: Per Willenius
 * date: 15.9.1998
 * version: V.1.0
*/ 

/** @name Converting numbers in strings

    @author Per Willenius
    @version 2.3pre2
*/ 

//@{
#ifndef _int2str_h 
#define _int2str_h 


//  ************** System Includes  ******************
#include <string>

using namespace std;


//  ************** Functions  ******************

/// returns true if value integer
bool inttst(const double value);

/// returns string
string ztos(const double value);

/// returns formated string
string ztofs(const double value);

/// returns numeric value
int stoz(const string S);

#endif

//@}



