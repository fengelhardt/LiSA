/* ************************************************************************* */
/*                            Filename: Branch.c                             */
/*                                                                           */
/*  Export: Procedure Compute_Branchlist;                                    */
/*                    Calculation of Before- and After-Candidates of         */
/*                    the search tree node SonNode.                          */ 
/*   @version 2.3pre3
/* ************************************************************************* */

#include <malloc.h>

#include "jb_dtstr.hpp"
#include "jb_table.hpp"
#include "jb_list.hpp"
#include "jb_brnch.hpp"


static boolean CheckBefore(int Op, struct List *block)
 /* Check whether Op can be moved before block or not */
{

 struct List *help;

 help = block;
 while (help->number != Op) {
    if (Member(DisjArcs->pred[Op],help->number)) return(false);
    help = help->next;
 }
 return(true);
}



static boolean CheckAfter(int Op, struct List *block)
  /* Check whether Op can be moved after block or not */
{

 struct List *help;

 help = block;
 while (help != NIL) {
    if (Member(DisjArcs->succ[Op],help->number)) return(false);
    help = help->next;
 }
 return(true);
}



static boolean SimpleLowerBound(int Op, struct List *block, int head[],
				int tail[])
 /* Calculation of a simple lower bound for the moving of Op before/after block */
{

  struct List  *help;
  int          MaxValue, SumProcess = 0, MinTail = MaxInt;

  MaxValue = tail[Op];
  help = block;
  while (help != NIL) {
     if (help->number != Op) {
        SumProcess += OpData[help->number].process_time;
        if (MinTail > tail[help->number]) MinTail = tail[help->number];
        if (OpData[help->number].process_time + tail[help->number] > MaxValue)
            MaxValue = OpData[help->number].process_time + tail[help->number];
     }
     help = help->next;
  }
  if (MaxValue < SumProcess + MinTail) MaxValue = SumProcess + MinTail;
  if (head[Op]+OpData[Op].process_time+MaxValue >= UpperBound) return(false);
  return(true);
}



static void InsertInOrder(int Op, char Direction)
                         /* Insertion of Op into Branchlist in the 
                            order of non-decreasing heads or tails */
{
  struct BranchList  *temp, *help;
  int                value;

  temp = (struct BranchList *) malloc(sizeof(struct BranchList));
  temp->branch_op = Op;
  temp->before_or_after = Direction;
  temp->next = NIL;
  if (Direction == 'b') value = head[Op];
  else                  value = tail[Op];
  if (SonNode->order == NIL) SonNode->order = temp;
  else {
     if (((SonNode->order->before_or_after == 'b') ?
           head[SonNode->order->branch_op] :
           tail[SonNode->order->branch_op]) > value) {
        temp->next = SonNode->order;
        SonNode->order = temp;
     }
     else {
        help = SonNode->order;
        while (help->next != NIL)
           if (((help->next->before_or_after == 'b') ?
               head[help->next->branch_op] :
               tail[help->next->branch_op]) > value)
              break;
           else help = help->next;
        temp->next = help->next;
        help->next = temp;
     }
  }
}



/* ************************************************************************* */
/*                      Procedure Compute_Branchlist()                       */
/*                                                                           */
/*  INPUT: -/-                                                               */
/*                                                                           */
/*                                                                           */
/*  FUNCTION: Calculation of the Before- and After-Candidates. They are      */
/*            in the order of non-decreasing heads/tails in SonNode->order.  */
/* ************************************************************************* */

void Compute_BranchList ()
{

 struct BlockList  *actual_block;    
 struct List       *help;
 int               Op;

 actual_block = SonNode->blocks;
 SonNode->order = NIL;
 while (actual_block != NIL) {
    switch(actual_block->fi_mi_la) {
       case 'f':   
                 help = actual_block->elements->next;
                 Op = actual_block->elements->number;
                 if ( CheckAfter(Op, actual_block->elements->next) &&
                      SimpleLowerBound(Op, actual_block->elements, tail, head))
                    InsertInOrder(Op, 'a');
                 while (help->next != NIL) {
                    if ( head[help->number] < head[Op] &&
                         CheckBefore(help->number, actual_block->elements)  &&
                         SimpleLowerBound(help->number,actual_block->elements,
                                                                   head,tail))
                       InsertInOrder(help->number, 'b');
                    if ( CheckAfter(help->number, help->next)  &&
                         SimpleLowerBound(help->number,actual_block->elements,
                                                                   tail,head))
                       InsertInOrder(help->number,'a');
                    help = help->next;
                }
                break;
       case 'm':   
                 help = actual_block->elements->next;
                 while (help->next != NIL) {
                    if ( CheckBefore(help->number, actual_block->elements) &&
                         SimpleLowerBound(help->number,actual_block->elements,
                                                                   head,tail))
                       InsertInOrder(help->number, 'b');
                    if ( CheckAfter(help->number, help->next) &&
                         SimpleLowerBound(help->number, actual_block->elements,
                                                                    tail,head))
                       InsertInOrder(help->number, 'a');
                    help = help->next;
                 }
                 if ( CheckBefore(help->number, actual_block->elements) &&
                      SimpleLowerBound(help->number, actual_block->elements,
                                                                    head,tail))
                    InsertInOrder(help->number, 'b');
                 break;
       case 'l':  
                 Op = actual_block->last_of_block;
                 help = actual_block->elements->next;
                 while (help->next != NIL) {
                    if ( CheckBefore(help->number, actual_block->elements) &&
                         SimpleLowerBound(help->number,actual_block->elements,
                                                                   head,tail))
                        InsertInOrder(help->number, 'b');
                    if ( tail[help->number] < tail[Op] &&
                         CheckAfter(help->number, help->next) &&
                         SimpleLowerBound(help->number,actual_block->elements,
                                                                   tail,head))
                       InsertInOrder(help->number, 'a');
                    help = help->next;
                 }
                 if ( CheckBefore(help->number, actual_block->elements) &&
                         SimpleLowerBound(help->number,actual_block->elements,
                                                                   head,tail))
                    InsertInOrder(help->number, 'b');
                 break;
    }
    actual_block = actual_block->next;
 }
}


