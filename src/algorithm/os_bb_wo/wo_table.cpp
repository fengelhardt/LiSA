/* ************************************************************************ */
/*                       Filename: table.c                                  */
/*                                                                          */
/*    Export: Globale Variablen                                             */
/* ************************************************************************ */

#include "wo_data.hpp"
#include "wo_table.hpp"

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

void (*Heuristic_Schedule) (void);

int minmax_match;
int head_match;

