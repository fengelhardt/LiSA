/* ************************************************************************* */
/*                             Filename: Init.c                              */
/*                                                                           */
/*    EXPORT: Procedure Initialize:                                          */
/*                      Initialization of global variables                   */ 
//   @version 3.0pre1
/* ************************************************************************* */

#include <stdio.h>
#include <malloc.h>

#include "jb_dtstr.hpp"
#include "jb_table.hpp"
#include "jb_init.hpp"

/* ************************************************************************* */
/*                           Procedure  Initialize()                         */
/*                                                                           */
/*  INPUT : -/-                                                              */
/*                                                                           */
/*  FUNCTION: Initialization of global variables;                            */
/*            Initialization of the file "output.dat"                        */
/* ************************************************************************* */

void Initialize ()
{

  int i;
  FILE *fp;

  fp = fopen("output.dat","w");
  fclose(fp);
  UpperBound = MaxInt;
  FirstOfStack = NIL;
  SonNode = (struct NodeType *) malloc(sizeof(struct NodeType));
                               /* Speicherplatz fuer SonNode; SonNode stellt
                                     zunaechst die Wurzel des Suchbaumes dar */
  ConjArcs = (struct ArcList *) malloc(sizeof(struct ArcList));
  DisjArcs = (struct ArcList *) malloc(sizeof(struct ArcList));
  for (i = 0; i <= MaxNumOfOperations; ++i) {
     ConjArcs->pred[i] = ConjArcs->succ[i] = NIL;
     DisjArcs->pred[i] = DisjArcs->succ[i] = NIL;
     SonNode->num_of_succ[i] = SonNode->num_of_pred[i] = 0;
     OpData[i].process_time = 0;
     OpData[i].machine_nr = 0;
  }
  for (i = 0; i <= MaxNumOfMachines; ++i)
      OpsOnMachine[i] = NIL;
}


