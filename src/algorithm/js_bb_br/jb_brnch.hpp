/* ************************************************************************* */
/*                            Filename: Branch.h                             */
/*                                                                           */
/*  Export: Procedure Compute_Branchlist;                                    */
/*                    Calculation of Before- and After-Candidates of         */
/*                    the search tree node SonNode.                          */ 
//   @version 2.3rc1
/* ************************************************************************* */

#ifndef _BRANCH_H
#define _BRANCH_H

#include "jb_dtstr.hpp"


/* ************************************************************************* */
/*                      Procedure Compute_Branchlist()                       */
/*                                                                           */
/*  INPUT: -/-                                                               */
/*                                                                           */
/*                                                                           */
/*  FUNCTION: Calculation of the Before- and After-Candidates. They are      */
/*            in the order of non-decreasing heads/tails in SonNode->order.  */
/* ************************************************************************* */
/// Calculation of the Before- and After-Candidates.
/** They are in the order of non-decreasing heads/tails in SonNode->order. */
void Compute_BranchList ();

#endif

