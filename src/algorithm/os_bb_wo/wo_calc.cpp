/* ************************************************************************* */
/*                            Filename: Calc.c                               */
/*						      			     */
/*                                                                           */
/* EXPORT: Procedure  Max      : Berechnet das Maximum von 2 Zahlen          */  
/*                    FindOpNr : Berechnet fuer (ij) die zugehoerige Op'Nr   */
/* ************************************************************************* */

#include "wo_table.hpp"


/* ************************************************************************* */
/*                         Procedure FindOpNr()                              */
/*                                                                           */
/* INPUT: i,j  (Zeilen- und Spaltenindex eines Matrixelementes)              */
/*                                                                           */
/* FUNCTION: Jedem Matrixelement (ij) wird die zugehoerige Operationennummer */
/*           zugeordnet. Diese Prozedur wird nur fur Matchings benoetigt,    */
/* ************************************************************************* */

int FindOpNr(int i,int j){
   int Op;

   Op = ( i * NumOfMachines ) - ( NumOfMachines - j );
   return(Op);
}



/* ************************************************************************* */
/*                            Procedure Max ()                               */
/*                                                                           */
/* INPUT: a, b   zu vergleichende Zahlen                                     */
/*                                                                           */
/* FUNCTION: Berechnet das Maximum von 2 Zahlen                              */
/* ************************************************************************* */

int Max(int a,int b){
  return( (a > b) ? a : b );
}
