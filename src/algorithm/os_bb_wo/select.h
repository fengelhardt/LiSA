/* ************************************************************************* */
/*                             Filename: Select.h                            */
/*                                                                           */
/*  EXPORT: Procedure Additional_Arcs:                                       */
/*                    Festlegen zusaetzlicher disjunktiver (= Maschinen-)    */
/*                    und konjunktiver (= Job-) Kanten nach Carlier, Pinson  */
/* ************************************************************************* */


#ifndef _SELECT_H
#define _SELECT_H


/* ************************************************************************* */
/*                     PROCEDURE     Additional_Arcs()                       */
/*                                                                           */
/*  INPUT:    -/-                                                            */
/*                                                                           */
/*  FUNCTION: Festlegen weiterer Job- u. Maschinenkanten nach Carlier und    */
/*            Pinson. Die groesste berechnete lower bound wird an das Haupt- */
/*            programm (main.c) zurueckgegeben.                              */
/* ************************************************************************* */

int Additional_Arcs ();


#endif
