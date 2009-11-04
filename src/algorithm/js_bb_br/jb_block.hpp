/* ************************************************************************* */
/*                            Filename: Block.h                              */
/*                                                                           */
/*   Export: Procedure Compute_Blocks:                                       */
/*                     Calculation of the blocks on the critical path        */
//   @version 3.0pre1
/* ************************************************************************* */


#ifndef _BLOCK_H
#define _BLOCK_H


/* ************************************************************************* */
/*                      Procedure Compute_Blocks()                           */
/*                                                                           */
/*  INPUT: -/-                                                               */
/*                                                                           */
/*                                                                           */
/*  FUNCTION: Calculation of the blocks on the critical path                 */
/*            The blocks are stored in the order of non-decreasing           */
/*            in SonNode.                                                    */
/* ************************************************************************* */

/// Calculation of the blocks on the critical path      
/** The blocks are stored in the order of non-decreasing in SonNode. */                                         
void Compute_Blocks ();

#endif

