/* ************************************************************************* */
/*                          Filename: Calc.h                                 */
/*                                                                           */
/*  Export: Procedure  Max     : Berechnet das Maximum von 2 Zahlen          */
/*                     FindOpNr: Berechnung der Operation-Nr. ausgehend vom  */
/*                               Matrix-Element 			     */	
/* ************************************************************************* */

#ifndef _CALC_H
#define _CALC_H

/* ************************************************************************* */
/*                         Procedure Max()                                   */
/*                                                                           */
/* INPUT: a,b  sind die zu vergleichenden Zahlen                             */
/*                                                                           */
/* FUNCTION: Berechnet das Maximum von 2 Zahlen                              */
/* ************************************************************************* */

int Max();


/* ************************************************************************* */
/*                           Procedure FindOpNr                              */
/*                                                                           */
/* INPUT: (i,j) Indices eines Matrixelementes                                */
/*                                                                           */
/* FUNCTION: Ermittelt zu einer Operation, die in Matrixschreibweise         */
/*           dargestellt wird, die einfache numerische Operationennummer     */
/*           (in dem Matchingproblem werden die Operationen notwendiger-     */
/*           weise als Matrixelemente dargestellt; die Umwandlung wird des-  */
/*           halb erforderlich, weil die uebrigen Funktionen die Op'en in    */
/*           der numerischen Darstellung benoetigen)                         */
/* ************************************************************************* */

int FindOpNr();

#endif
