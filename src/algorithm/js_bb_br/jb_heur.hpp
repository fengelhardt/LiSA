/* *********************************************************************** */
/*                               Filename: Heur.h                          */
/*                                                                         */
/*    Export: Procedure Heuristic_Schedule:                                */
/*                      Calculation of a heuristic solution for the        */
/*                      search tree node SonNode.                          */
//   @version 2.3pre3
/* *********************************************************************** */

#ifndef _HEUR_H
#define _HEUR_H



/* ************************************************************************* */
/*                         Procedure  Heuristic_Schedule()                   */
/*                                                                           */
/*  INPUT:  -/-                                                              */
/*                                                                           */
/*  FUNCTION: Calculation of a heuristic schedule. The critical path is      */
/*            stored in CriticalPath. If the makespan improves the actual    */
/*            upper bound, UB and CurrentBest are reset. Also the solution   */
/*            is stored in the file output.dat.                              */
/* ************************************************************************* */

void Heuristic_Schedule (int, int );

#endif





