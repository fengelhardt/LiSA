/* ************************************************************************* */
/*                            Filename: Sort.h                               */
/*                                                                           */
/*  EXPORT: Procedure Sort: 						     */
/*                    Sortieren eines Arrays in nicht-fallender Reihenfolge  */
/* ************************************************************************* */


#ifndef _SORT_H
#define _SORT_H


/* ************************************************************************* */
/*                             Procedure Sort()                              */
/*                                                                           */
/*  INPUT:  key_array:  Array mit 'keys'                                     */
/*          sort_array: Array, der gemaess der keys sortiert wird            */
/*          last:       letzte Position des Arrays                           */
/*                                                                           */
/*  FUNCTION: Beim Aufruf dieser Funktion, werden die sortierten Integer in  */
/*            sort_array[1] ... sort_array[last] gespeichert.                */
/*            Der Index mit kleinstem key_array-entry is sort_array[1] zuge- */
/*	      ordnet, usw.						     */
/* ************************************************************************* */

void Sort ();


#endif
