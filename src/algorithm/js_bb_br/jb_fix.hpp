/* ************************************************************************* */
/*                           Filename: Fix.h                                 */
/*                                                                           */
/*   Export: Procedure Fix_Disjunctions:                                     */
/*                     Calculation of the fixed disjunctive arcs in SonNode  */ 
//   @version 2.3rc1
/* ************************************************************************* */

#ifndef _FIX_H
#define _FIX_H



/* ************************************************************************* */
/*                      Procedure Fix_Disjunctions                           */
/*                                                                           */
/*  INPUT: MoveOp (operation to be moved),                                   */
/*         Direc  (before / after)                                           */ 
/*                                                                           */
/*                                                                           */
/*  FUNCTION: The disjunctive arcs of SonNode are stored in DisjArcs.        */
/*            The number of the disjunctive arcs ard stored in SonNode.      */
/* ************************************************************************* */
///The disjunctive arcs of SonNode are stored in DisjArcs.  
/** The number of the disjunctive arcs ard stored in SonNode.
    @param MoveOp (operation to be moved)
    @param Direc  (before / after) */        
void Fix_Disjunctions (int MoveOp, char Direc);

#endif

