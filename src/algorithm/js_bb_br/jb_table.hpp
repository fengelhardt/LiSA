/* ************************************************************************* */
/*                       Filename: table.h                                   */
/*                                                                           */
/*    Export: Global variables                                               */
//   @version 3.0
/* ************************************************************************* */

#include "jb_dtstr.hpp"
#include "../../basics/matrix.hpp"

#ifndef _TABLE_H
#define _TABLE_H
//@{
/// global variables
extern	struct Operation         OpData[MaxNumOfOperations+1];
extern	struct List              *OpsOnMachine[MaxNumOfMachines+1],
	                         *CriticalPath; 
extern	struct ArcList           *ConjArcs, *DisjArcs; 
extern	int                      head[MaxNumOfOperations+1],  
	                         tail[MaxNumOfOperations+1], 
	                         CurrentBest[MaxNumOfOperations+1],
	                         UpperBound,           
	                         NumOfJobs, NumOfMachines, NumOfOperations,
	                         SearchTreeNodes;
extern	struct StackElement      *FirstOfStack;       
extern	struct NodeType          *ActualNode, *SonNode;

extern Lisa_Matrix<bool> *SIJ;
extern Lisa_Matrix<int> *JO;
//@}
#endif

