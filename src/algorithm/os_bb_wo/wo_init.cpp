/* ************************************************************************ */
/*                            Filename: Init.c                              */
/*                                                                          */
/*    EXPORT: Procedure Initialize:                                         */
/*                      Initialisierung globaler Variablen                  */ 
/* ************************************************************************ */

#include <cstdlib>
#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_init.hpp"

#include "../../misc/except.hpp"

/* ************************************************************************ */
/*                           Procedure  Initialize()                        */
/*                                                                          */
/*  INPUT : -/-                                                             */
/*                                                                          */
/*  FUNCTION: Initialisierung globaler Variablen;                           */
/*                                                                          */
/* ************************************************************************ */

void Initialize(){

  int i;

  UpperBound = MaxInt;
  FirstOfStack = NIL;

  if ( (SonNode = new struct NodeType)
                == NIL )
  {
     G_ExceptionList.lthrow("init,SonNode: kein Speicherplatz",
                            Lisa_ExceptionList::NO_MORE_MEMORY) ;
     exit(1);
  }
  SonNode->blocks = NIL; 

  if ( (ConjArcs = new struct ArcList)
                 == NIL )
  {
     G_ExceptionList.lthrow("init,ConjArcs: kein Speicherplatz",
                            Lisa_ExceptionList::NO_MORE_MEMORY) ;
     exit(1);
  }
 
  if ( (DisjArcs = new struct ArcList)
                  == NIL )
  { 
     G_ExceptionList.lthrow("init,DisjArcs: kein Speicherplatz",
                            Lisa_ExceptionList::NO_MORE_MEMORY) ;
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
