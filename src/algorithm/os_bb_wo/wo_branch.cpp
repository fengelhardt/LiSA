/* ************************************************************************* */
/*                           Filename: Branch.c                              */
/*                                                                           */
/*  Export: Procedure Compute_Branchlist;                                    */
/*                    Berechnung der Before- und After-Kandidaten des        */
/*                    Suchbaumknotens SonNode.                               */
/* ************************************************************************* */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_branch.hpp"


/* Uberpruefen, ob eine op vor einen Block bewegt werden kann, oder nicht:
   Wird eine der Op'en des Blockes bereits vor op bearbeitet, so darf 
   op nicht vor den Block bewegt werden, da sonst Zyklen entstehen
*/
static boolean CheckBefore(int op, struct List *block)
{
   struct List *help;

   help = block;
   while (help->number != op) 
   {
      if ( Member(DisjArcs->pred[op],help->number) ||
	   Member(ConjArcs->pred[op],help->number) ) 
	 return(false);
      help = help->next;
   }
   return(true);
}


/* Ueberpruefen, ob eine op hinter den Block bewegt werden kann, oder nicht
*/ 
static boolean CheckAfter(int op,struct List *block)              
{
   struct List *help;

   help = block;
   while (help != NIL) 
   {
      if ( Member(DisjArcs->succ[op],help->number) ||
	   Member(ConjArcs->succ[op],help->number) )
	 return(false);
      help = help->next;
   }
   return(true);
}


/* Berechnung einer einfachen unteren Schranke, wenn die Operation vor
   oder hinter den Block bewegt werden soll;
   ist der berechnete Wert >= UB, so ist die op kein Bewegungskandidat
*/
static boolean SimpleLowerBound(int op,struct List *block,int head[],int tail[])	
{
   struct List  *help;
   int          maxvalue, 
		sumprocess = 0, 
		mintail = MaxInt;

   maxvalue = tail[op];
   help = block;
   while (help != NIL) 
   {
      if (help->number != op) 
      {
	 /* Summe uber die Bearbeitungszeiten der Blockoperationen,ohne op
         */
	 sumprocess += OpData[help->number].process_time;
         
	 /* mintail ist der kleinste tail(head) bei CheckBefore(CheckAfter)
         */
	 if (mintail > tail[help->number]) 
	     mintail = tail[help->number];
	 if (OpData[help->number].process_time + tail[help->number] > maxvalue)
	     maxvalue = OpData[help->number].process_time + tail[help->number];
      }
      help = help->next;
   }
   if (maxvalue < sumprocess + mintail) 
       maxvalue = sumprocess + mintail;

   /* uebersteigt diese Grenze die bisher beste gefundene Loesung, so darf
      die Operation nicht bewegt werden
   */
   if ( head[op] + OpData[op].process_time + maxvalue >= UpperBound ) 
      return(false);
   
   return(true);
}



/* Fuer jede Op muss die Art des Blockes ('m' o. 'j') nachgehalten
   werden, da ansonsten in Fix_Arcs Kanten nicht richtig festgelegt werden 
   koennen;
   block_kind enthaelt die Info's aus 'kind' von struct BlockList, diese
   werden in 'kind_of_block' v. struct BranchList fuer jede Op geschrieben.
*/
/* Einfuegen der Operationen in der Reihenfolge nicht-fallender
   Heads oder Tails 
*/
static void InsertInOrder(int op,char direction,char block_kind)
{
   struct BranchList  *temp, 
		      *help;
   int                value;

   if ( (temp = (struct BranchList *) malloc(sizeof(struct BranchList)))
              == NIL )
   {
      fprintf(stderr, "branch,temp: malloc: kein Speicherplatz\n") ;
      exit(1);
   }

   temp->branch_op = op;
   temp->before_or_after = direction;
   temp->kind_of_block = block_kind;
   temp->next = NIL;
   if (direction == 'b') 
      value = Head[op];
   else                  
      value = Tail[op];
   if ( SonNode->order == NIL ) 
      SonNode->order = temp;
   else 
   {
      if ( ((SonNode->order->before_or_after == 'b') ?
	    Head[SonNode->order->branch_op] :
	    Tail[SonNode->order->branch_op]) > value ) 
      {
         temp->next = SonNode->order;
         SonNode->order = temp;
      }
      else 
      {
         help = SonNode->order;
         while (help->next != NIL)
	    if ( ((help->next->before_or_after == 'b') ?
		Head[help->next->branch_op] :
		Tail[help->next->branch_op]) > value )
	       break;
	    else 
	       help = help->next;
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
/*  FUNCTION: Berechnung der Before- und After-Kandidaten. Sie sind in der   */
/*            Reihenfolge nichtfallender Heads/Tails in SonNode->order       */
/*	      gespeichert.				                     */
/* ************************************************************************* */

 
void Compute_BranchList ()
{
   struct BlockList  *actual_block;    
   struct List       *help;
   int               op;
   char              block_kind;

   actual_block = SonNode->blocks;
   SonNode->order = NIL;
   while (actual_block != NIL) 
   {
      /* die 1. op wird gar nicht untersucht:
	 sie ist niemals B-kandidat und da zuerst alle B-Kandidaten
	 untersucht werden, braucht op auch nicht mehr als A-Kandidat
	 untersucht werden
      */
      block_kind = actual_block->kind;
      help = actual_block->elements->next;
      while (help->next != NIL) 
      {
	 /* Untersuchung der 2. bis vorletzten op nach Before-
	    und After-Kandidaten
	 */		
	 if ( CheckBefore(help->number, actual_block->elements) &&
	      SimpleLowerBound(help->number,actual_block->elements,
							   Head,Tail))
	    InsertInOrder(help->number, 'b', block_kind);
	 if ( CheckAfter(help->number, help->next) &&
	      SimpleLowerBound(help->number, actual_block->elements,
							    Tail,Head))
	    InsertInOrder(help->number, 'a', block_kind);
	 help = help->next;
      }
      /* Untersuchung der letzten op als Before-Kandidat
      */
      if ( CheckBefore(help->number, actual_block->elements) &&
	   SimpleLowerBound(help->number, actual_block->elements,
							    Head,Tail))
	 InsertInOrder(help->number, 'b', block_kind);

      actual_block = actual_block->next;
   }
}
