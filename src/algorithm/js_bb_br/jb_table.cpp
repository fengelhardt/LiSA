/* ************************************************************************* */
/*                       Filename: table.c                                   */
/*                                                                           */
/*    Export: Global variables                                               */
//   @version 2.3rc1
/* ************************************************************************* */

#include "jb_dtstr.hpp"

struct Operation         OpData[MaxNumOfOperations+1];
struct List              *OpsOnMachine[MaxNumOfMachines+1],
                         *CriticalPath; 
struct ArcList           *ConjArcs, *DisjArcs; 
int                      head[MaxNumOfOperations+1],  
                         tail[MaxNumOfOperations+1], 
                         CurrentBest[MaxNumOfOperations+1],
                         UpperBound,           
                         NumOfJobs, NumOfMachines, NumOfOperations,
                         SearchTreeNodes = 1;
struct StackElement      *FirstOfStack;       
struct NodeType          *ActualNode, *SonNode; 

