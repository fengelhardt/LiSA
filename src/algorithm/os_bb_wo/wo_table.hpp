/* ************************************************************************* */
/*                       Filename: Table.h                                   */
/*                                                                           */
/*    Export: Globale Variablen                                              */
/* ************************************************************************* */

#include "wo_data.hpp"

#ifndef _TABLE_H
#define _TABLE_H

extern	struct Operation         OpData[MaxNumOfOperations+1];
extern	struct List              *OpsOnMachine[MaxNumOfMachines+1],
				 *OpsOfJob[MaxNumOfJobs+1];
extern	struct ArcList           *ConjArcs, *DisjArcs; 
extern	int                      Head[MaxNumOfOperations+1],  
				 Tail[MaxNumOfOperations+1], 
				 CriticalPath[MaxNumOfOperations+1],
				 UpperBound,           
				 NumOfJobs, NumOfMachines, NumOfOperations,
				 SearchTreeNodes,
                                 PathPos;
extern	struct StackElement      *FirstOfStack;       
extern	struct NodeType          *ActualNode, *SonNode; 

#endif

