/* ************************************************************************* */
/*                             Filename: Main.c                              */
/*                                                                           */
/*                              Hauptprogramm                                */
/*                                                                           */
/* ************************************************************************* */


#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_init.hpp"
#include "wo_read.hpp"
#include "wo_head.hpp"
#include "wo_heur.hpp"
#include "wo_block.hpp"
#include "wo_branch.hpp"
#include "wo_stack.hpp"
#include "wo_fix.hpp"
#include "wo_select.hpp"
#include "wo_low.hpp"


/* ************************************************************************* */
/*                                                                           */
/*                             Hauptprogramm                                 */
/*                                                                           */
/*  INPUT: Parameter der Kommandozeile                                       */
/*                                                                           */
/*  FUNCTION: Berechnung einer optimalen Loesung fuer das Problem in der     */
/*            Datei argv[1]                                                  */
/*									     */
/*            Alle Outputs des Programm stehen in der Datei output.dat       */
/*            Outputs: erste berechnete obere Schranke (Wurzel)              */
/*		       triviale untere Schranke				     */
/*		       Selektionen, deren Zf die aktuelle UB verbessern	     */
/*		       krtitischer Pfad der Selektion			     */
/* ************************************************************************* */

int main(int argc,char *argv[]){
  
  struct BranchList  *DeleteBranch;
  char *index;
    
  Initialize();
    
  if ( !Read_Data(argv[1]) ){
    printf("Not able to read data !\n");
    return(1);
  }
  
  if ( Compute_Head_Tail() ){
    printf("\n");
    Heuristic_Schedule();
    if ((SonNode->lower_bound = Additional_Arcs()) < UpperBound){
      Compute_Blocks();
      Compute_BranchList();   
      ActualNode = SonNode;
      Push();
      if ( (SonNode = (struct NodeType *) malloc(sizeof(struct NodeType))) == NIL ) {
        fprintf(stderr, "main,SonNode: malloc: kein Speicherplatz\n") ;
        exit(1);
      } 
      SonNode->blocks = NIL; 
      SonNode->order = NIL;
    }
  }
  
  while (FirstOfStack != NIL){
    
    Pop();
    Update_Arcs();
    while (ActualNode->order != NIL && ActualNode->lower_bound < UpperBound){
      
      Fix_Arcs(ActualNode->order->branch_op,
      ActualNode->order->before_or_after,
      ActualNode->order->kind_of_block);
      DeleteBranch = ActualNode->order;
      ActualNode->order = ActualNode->order->next;
      free((void *) DeleteBranch);
      if ( Compute_Head_Tail() && 
        Compute_LowerBound() < UpperBound && 
        (SonNode->lower_bound = Additional_Arcs()) < UpperBound){
          
          Heuristic_Schedule();
          
          if ( SonNode->lower_bound < UpperBound ){
            Compute_Blocks();
            Compute_BranchList();
            SearchTreeNodes++;
            Push();                 
            ActualNode = SonNode;  
            if ( (SonNode = (struct NodeType *) malloc(sizeof(struct NodeType)))== NIL ){
              fprintf(stderr, "main,SonNode: malloc: kein Speicherplatz\n");
              exit(1);
            }
            SonNode->blocks = NIL; 
            SonNode->order = NIL;
          }else{ 
            Update_Arcs();     
          }
        }else{ 
          Update_Arcs();                 
        }
    }
  } 
    
  return(0); 
}
