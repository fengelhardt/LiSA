/* ************************************************************************ */
/*                            Filename: Init.c                              */
/*                                                                          */
/*    EXPORT: Procedure Initialize:                                         */
/*                      Initialisierung globaler Variablen                  */ 
/* ************************************************************************ */

#include <stdio.h>
#include <malloc.h>
#include "data.h"
#include "table.h"
#include "init.h"

/* ************************************************************************ */
/*                           Procedure  Initialize()                        */
/*                                                                          */
/*  INPUT : -/-                                                             */
/*                                                                          */
/*  FUNCTION: Initialisierung globaler Variablen;                           */
/*            Initialisierung der Datei "output.dat"                        */
/* ************************************************************************ */

void Initialize (output_file)

char output_file[];
{

  int i;
  FILE *fp;

  fp = fopen(output_file,"w");
  fclose(fp);

  UpperBound = MaxInt;
  FirstOfStack = NIL;

  if ( (SonNode = (struct NodeType *) malloc(sizeof(struct NodeType)))
                == NIL )
  {
     fprintf(stderr, "init,SonNode: malloc: kein Speicherplatz\n") ;
     exit(1);
  }
  SonNode->blocks = NIL; 

  if ( (ConjArcs = (struct ArcList *) malloc(sizeof(struct ArcList)))
                 == NIL )
  {
     fprintf(stderr, "init,ConjArcs: malloc: kein Speicherplatz\n") ;
     exit(1);
  }
 
  if ( (DisjArcs = (struct ArcList *) malloc(sizeof(struct ArcList)))
                  == NIL )
  { 
     fprintf(stderr, "init,DisjArcs: malloc: kein Speicherplatz\n") ;
     exit(1);
  }

  for (i = 0; i <= MaxNumOfOperations; ++i) 
  {
     ConjArcs->pred[i] = ConjArcs->succ[i] = NIL;
     DisjArcs->pred[i] = DisjArcs->succ[i] = NIL;

     SonNode->num_of_disj_succ[i] = SonNode->num_of_disj_pred[i] = 0;
     SonNode->num_of_conj_succ[i] = SonNode->num_of_conj_pred[i] = 0;
     OpData[i].process_time = 0;
     OpData[i].machine_nr = 0;
     OpData[i].job_nr = 0;

     /* am Anfang gibt es keine Kanten;
        Head/Tail ist erst dann von Null verschieden, wenn eine Kante zur
        Op hin- bzw. wegfuehrt;
     */
     Head[i] = Tail[i] = 0;            
				
  }

  for ( i = 0; i <= MaxNumOfMachines; ++i )
      OpsOnMachine[i] = NIL;

  for ( i = 0; i <= MaxNumOfJobs; ++i )
      OpsOfJob[i] = NIL;
}
