/* ************************************************************************* */
/*                           Filename: Fix.c                                 */
/*                                                                           */
/*   Export: Procedure Fix_Disjunctions:                                     */
/*                     Calculation of the fixed disjunctive arcs in SonNode  */ 
//   @version 2.3final
/* ************************************************************************* */

#include "jb_dtstr.hpp"
#include "jb_table.hpp"
#include "jb_list.hpp"
#include "jb_fix.hpp"


/* ************************************************************************* */
/*                      Procedure Fix_Disjunctions                           */
/*                                                                           */
/*  INPUT: MoveOp (operation to be moved),                                   */
/*         Direc  (before / after)                                           */ 
/*                                                                           */
/*                                                                           */
/*  FUNCTION: The disjunctive arcs of SonNode are stored in DisjArcs.        */
/*            The number of the disjunctive arcs ard stored in SonNode.      */
/* ************************************************************************* */
void Fix_Disjunctions(int MoveOp,char Direc)
{

  boolean           ready, found, just_found, look_at_first_block;
  struct List       *help, *elements_of_first_block;
  struct BlockList  *helpblock;
  int               last, first, i;
  char              type;

  for (i = 1; i <= NumOfOperations; ++i) { 
      SonNode->num_of_succ[i] = ActualNode->num_of_succ[i];
      SonNode->num_of_pred[i] = ActualNode->num_of_pred[i];
  }
  elements_of_first_block = NIL;
  helpblock = ActualNode->blocks;
  ready = found = just_found = look_at_first_block = false;

  while (helpblock != NIL && !ready) {
     type = helpblock->fi_mi_la;
     help = helpblock->elements;
     last = helpblock->last_of_block;
     while (!found && help != NIL) {
        if (help->number == MoveOp) {
           found = true;
           just_found = true;
        }
        help = help->next;
     }
     help = helpblock->elements;
     if (just_found) {   
        switch(type) {
           case 'f': if (Direc == 'b') {    
                       while (help != NIL) {
                          if (help->number != last &&
                               !Member(DisjArcs->succ[help->number],last)) {
                             DisjArcs->succ[help->number] =
                                Insert(DisjArcs->succ[help->number],last);
                             DisjArcs->pred[last] =
                                Insert(DisjArcs->pred[last],help->number);
                             SonNode->num_of_succ[help->number]++;
                             SonNode->num_of_pred[last]++;
                          }
			  if (help->number != MoveOp &&
                               !Member(DisjArcs->succ[MoveOp],help->number)) {
                             DisjArcs->succ[MoveOp] =
                                Insert(DisjArcs->succ[MoveOp],help->number);
                             DisjArcs->pred[help->number] =
                                Insert(DisjArcs->pred[help->number],MoveOp);
                             SonNode->num_of_pred[help->number]++;
                             SonNode->num_of_succ[MoveOp]++;
                          }
                          help = help->next;
                        }
                     }
                     else {                
                       ready = true;
                       while (help != NIL) {
                          if (help->number != MoveOp &&
                               !Member(DisjArcs->pred[MoveOp],help->number)) {
                             DisjArcs->pred[MoveOp] =
                                Insert(DisjArcs->pred[MoveOp],help->number);
                             DisjArcs->succ[help->number] =
                                Insert(DisjArcs->succ[help->number],MoveOp);
                             SonNode->num_of_succ[help->number]++;
                             SonNode->num_of_pred[MoveOp]++;
                          }
                          help = help->next;
                       }
                     }
                     break;

           case 'm': ready = true;        
                     if (Direc == 'b') {
                       while (help != NIL) {
                          if (help->number != MoveOp &&
                               !Member(DisjArcs->succ[MoveOp],help->number)) {
                             DisjArcs->succ[MoveOp] =
                                Insert(DisjArcs->succ[MoveOp],help->number);
                             DisjArcs->pred[help->number] =
                                Insert(DisjArcs->pred[help->number],MoveOp);
                             SonNode->num_of_pred[help->number]++;
                             SonNode->num_of_succ[MoveOp]++;
                          }
                          help = help->next;
                       }
                     }
                     else {              
                       first = help->number;
                       help = help->next;
                       while (help != NIL) {
                           if (!Member(DisjArcs->succ[first],help->number)) {
                             DisjArcs->succ[first] =
                                Insert(DisjArcs->succ[first],help->number);
                             DisjArcs->pred[help->number] =
                                Insert(DisjArcs->pred[help->number],first);
                             SonNode->num_of_pred[help->number]++;
                             SonNode->num_of_succ[first]++;
                           }
                           if (help->number != MoveOp &&
                               !Member(DisjArcs->succ[help->number],MoveOp)) {
                              DisjArcs->succ[help->number] =
                                 Insert(DisjArcs->succ[help->number],MoveOp);
                              DisjArcs->pred[MoveOp] =
                                 Insert(DisjArcs->pred[MoveOp],help->number);
                             SonNode->num_of_succ[help->number]++;
                             SonNode->num_of_pred[MoveOp]++;
                           }
                           help = help->next;
                       }
                     }
                     break;
           case 'l': if (Direc == 'b') {   
                       ready = true;
                       while (help != NIL) {
                          if (help->number != MoveOp &&
                               !Member(DisjArcs->succ[MoveOp],help->number)) {
                             DisjArcs->succ[MoveOp] =
                                Insert(DisjArcs->succ[MoveOp],help->number);
                             DisjArcs->pred[help->number] =
                                Insert(DisjArcs->pred[help->number],MoveOp);
                             SonNode->num_of_pred[help->number]++;
                             SonNode->num_of_succ[MoveOp]++;
                          }
                          help = help->next;
                       }
                     }
                     else {               
                       look_at_first_block = true;
                       first = help->number;
                       help = help ->next;
                       while (help != NIL) {
                          if (!Member(DisjArcs->succ[first],help->number)) {
                             DisjArcs->succ[first] =
                                Insert(DisjArcs->succ[first],help->number);
                             DisjArcs->pred[help->number] =
                                Insert(DisjArcs->pred[help->number],first);
                             SonNode->num_of_pred[help->number]++;
                             SonNode->num_of_succ[first]++;
                          }
                          if (help->number != MoveOp &&
                              !Member(DisjArcs->pred[MoveOp],help->number)) {
                             DisjArcs->pred[MoveOp] =
                                Insert(DisjArcs->pred[MoveOp],help->number);
                             DisjArcs->succ[help->number] =
                                Insert(DisjArcs->succ[help->number],MoveOp);
                             SonNode->num_of_succ[help->number]++;
                             SonNode->num_of_pred[MoveOp]++;
                          }
                          help = help->next;
                       }
                     }
                     break;

        }
        just_found = false;
     }
     else {
        switch(type) { 
           case 'f': elements_of_first_block = help;  
                     while (help->next != NIL) {
                        if (!Member(DisjArcs->pred[last],help->number)) {
                          DisjArcs->pred[last] =
                             Insert(DisjArcs->pred[last],help->number);
                          DisjArcs->succ[help->number] =
                             Insert(DisjArcs->succ[help->number],last);
                          SonNode->num_of_succ[help->number]++;
                          SonNode->num_of_pred[last]++;
                       }
                       help = help->next;
                     }
                     break;

           case 'm': first = help->number;        
                     help = help->next;
                     while (help != NIL) {
                        if (!Member(DisjArcs->succ[first],help->number)) {
                           DisjArcs->succ[first] =
                              Insert(DisjArcs->succ[first],help->number);
                           DisjArcs->pred[help->number] =
                              Insert(DisjArcs->pred[help->number],first);
                           SonNode->num_of_pred[help->number]++;
                           SonNode->num_of_succ[first]++;
                        }
                        if (help->number != last &&
                            !Member(DisjArcs->pred[last],help->number)) {
                           DisjArcs->pred[last] =
                              Insert(DisjArcs->pred[last],help->number);
                           DisjArcs->succ[help->number] =
                              Insert(DisjArcs->succ[help->number],last);
                          SonNode->num_of_succ[help->number]++;
                          SonNode->num_of_pred[last]++;
                        }
                        help = help->next;
                     }
                     break;

           case 'l': first = help->number;       
                     help = help->next;
                     while (help != NIL) {
                        if (!Member(DisjArcs->succ[first],help->number)) {
                           DisjArcs->succ[first] =
                              Insert(DisjArcs->succ[first],help->number);
                           DisjArcs->pred[help->number] =
                              Insert(DisjArcs->pred[help->number],first);
                           SonNode->num_of_pred[help->number]++;
                           SonNode->num_of_succ[first]++;
                        }
                        help = help->next;
                     }
                     break;
       }
     }
     helpblock = helpblock->next;
  }
  if (elements_of_first_block != NIL && look_at_first_block) {
     first = elements_of_first_block->number;
     help = elements_of_first_block->next;
     while (help != NIL) {    
        if (!Member(DisjArcs->succ[first],help->number)) {
           DisjArcs->succ[first] =
              Insert(DisjArcs->succ[first],help->number);
           DisjArcs->pred[help->number] =
      	      Insert(DisjArcs->pred[help->number],first);
           SonNode->num_of_pred[help->number]++;
           SonNode->num_of_succ[first]++;
        }
        help = help->next;
     }
  }
}


