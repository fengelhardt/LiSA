/* ************************************************************************* */
/*                           Filename: Fix.h                                 */
/*                                                                           */
/*   Export: Procedure Fix_Arcs:                                             */
/*                     Berechnung der festgelegten Job- und Maschinenkanten  */
/*                     in SonNode                                            */ 
/* ************************************************************************* */

#ifndef _FIX_H
#define _FIX_H

/* ************************************************************************* */
/*                            Procedure Fix_Arcs                             */
/*                                                                           */
/*  INPUT: moveOp (zu bewegende Operation),                                  */
/*         direc  (before / after)                                           */ 
/*         block_kind (`m' or 'j')                                           */
/*                                                                           */
/*                                                                           */
/*  FUNCTION: Die disjunktiven (=Maschinen-)/konjunktiven (=Job-) Kanten in  */
/*            SonNode werden in DisjArcs/ConjArcs gespeichert.               */
/*            Die Anzahl disjunktiver/konjunktiver Kanten wird in SonNode    */
/*            gespeichert.                                                   */
/* ************************************************************************* */

void Fix_Arcs(int moveop,char direc,char block_kind);

#endif
