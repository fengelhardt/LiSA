/* ************************************************************************ */
/*                       Filename: table.c                                  */
/*                                                                          */
/*    Export: Globale Variablen                                             */
/* ************************************************************************ */

#include "data.h"
#include "table.h"

struct Operation         OpData[MaxNumOfOperations+1];
struct List              *OpsOnMachine[MaxNumOfMachines+1],
			 *OpsOfJob[MaxNumOfJobs+1];
struct ArcList           *ConjArcs, *DisjArcs; 
int                      Head[MaxNumOfOperations+1],  
			 Tail[MaxNumOfOperations+1], 
			 CriticalPath[MaxNumOfOperations+1],
			 UpperBound,           
                         NumOfJobs, NumOfMachines, NumOfOperations,
                         SearchTreeNodes = 1,
                         PathPos = 1;
struct StackElement      *FirstOfStack;       
struct NodeType          *ActualNode, *SonNode; 
