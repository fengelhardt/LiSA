/* ************************************************************************* */
/*                           Filename: Branch.h                              */
/*                                                                           */
/*  Export: Procedure Compute_Branchlist;                                    */
/*                    Berechnung der Before- und After-Kandidaten des        */
/*                    Suchbaumknotens SonNode                                */ 
/* ************************************************************************* */

#ifndef _BRANCH_H
#define _BRANCH_H

#include "data.h"


/* ************************************************************************* */
/*                      Procedure Compute_Branchlist()                       */
/*                                                                           */
/*  INPUT: -/-                                                               */
/*                                                                           */
/*                                                                           */
/*  FUNCTION: Berechnung der Before- und After-Kandidaten. Sie sind in der   */
/*            Reihenfolge nichtfallender Heads/Tails in SonNode->order       */
/*            gespeichert.  						     */
/* ************************************************************************* */

void Compute_BranchList ();

#endif
