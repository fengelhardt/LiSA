/* ************************************************************************* */
/*                              Filename: Sort.h                             */
/*                                                                           */
/*  EXPORT: Procedure Sort: Sorting of an array into a non-decreasing order  */
//   @version 2.3pre3
/* ************************************************************************* */


#ifndef _SORT_H
#define _SORT_H


/* ************************************************************************* */
/*                             Procedure Sort()                              */
/*                                                                           */
/*  INPUT:  key_array:  Array with keys                                      */
/*          sort_array: Array to be sorted according to the keys             */
/*          last:       last position of the array                           */
/*                                                                           */
/*  FUNCTION: When this function is called, the integers to be sorted        */
/*            are stored in sort_array[1] ... sort_array[last].              */
/*            The index with smallest key_array-entry is assigned to         */
/*            sort_array[1], and so on.                                      */ 
/* ************************************************************************* */

void Sort (int[], int[], int);


#endif

