/* ************************************************************************* */
/*                            Filename: ReadData.h                           */
/*                                                                           */
/*     EXPORT: Procedure Read_Data: Read data from file 'FileName'           */
//   @version 2.3rc1
/* ************************************************************************* */


#ifndef _READDATA_H
#define _READDATA_H

#include "jb_dtstr.hpp"

/* ************************************************************************* */
/*                           Procedure      Read_Data()                      */
/*                                                                           */
/*   INPUT:  FileName:    file which contains the data of the problem        */
/*                                                                           */
/*   FUNCTION: Reads data from file and stores it in global variables.       */
/*             If the file-read is correct Read_Data returns TRUE.           */
/* ************************************************************************* */
/// Reads data from file and stores it in global variables.
/** If the file-read is correct Read_Data returns TRUE. */
boolean Read_Data(char *);


#endif

