/* ************************************************************************ */
/*                           Filename: Fix.c                                */
/*                                                                          */
/*   Export: Procedure Fix_Arcs:                                            */
/*                     Berechnung der festgelegeten Kanten in SonNode 	    */ 
/* ************************************************************************ */

#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_fix.hpp"


/* ************************************************************************ */
/*                            Procedure Fix_Arcs                            */
/*                                                                          */
/*  INPUT: moveop (zu bewegende Operation),                                 */
/*         direc  (before / after)                                          */ 
/*         block_kind ('m' oder 'j')                                        */
/*                                                                          */
/*                                                                          */
/*  FUNCTION: Die disjunktiven (=Maschinen-)/konjunktiven (=Job-) Kanten in */
/*            SonNode werden in DisjArcs/ConjArcs gespeichert.              */
/*            Die Anzahl disjunktiver/konjunktiver Kanten wird in SonNode   */
/*            gespeichert.						    */
/* ************************************************************************ */

void Fix_Arcs(moveop,direc,block_kind)	

 int moveop;  
 char direc,
      block_kind;

{

  boolean           found,
		    just_found,
		    ready;
  struct List       *help;
  struct BlockList  *helpblock;
  int               last, 
		    first, 
		    i;
  char              kind;

  /* Es wird die Zahl der Vorgaenger/Nachfolger im aktuellen Suchbaumknoten 
     ermittelt;
     wenn Kanten gezogen werden, muessen diese Groessen aufdatiert werden;
  */
  for ( i = 1; i <= NumOfOperations; ++i )
  { 
      SonNode->num_of_disj_succ[i] = ActualNode->num_of_disj_succ[i];
      SonNode->num_of_disj_pred[i] = ActualNode->num_of_disj_pred[i];
      SonNode->num_of_conj_succ[i] = ActualNode->num_of_conj_succ[i];
      SonNode->num_of_conj_pred[i] = ActualNode->num_of_conj_pred[i];
  }
  helpblock = ActualNode->blocks;
  ready = found = just_found = false;


  while ( helpblock != NIL  &&  !ready )
  {
     help = helpblock->elements;
     last = helpblock->last_of_block;
     kind = helpblock->kind;

     /* Es wird ueberprueft, ob die zu bewegende Op (moveop) im gerade be-
	trachteten Block liegt;
	wenn ja  : found bekommt den Wert true
	wenn nein: found behaelt den initialen Wert;
     */
     while ( !found && help != NIL ) 
     {
	if ( help->number == moveop && kind == block_kind ) 
	{
	   found = true;
	   just_found = true;
	}
	help = help->next;
     }
     help = helpblock->elements;

     /* Wird found auf true gesetzt, 
	- so liegt moveop im gerade betrachteten Block, 
	- so werden die Kanten gezogen, die in Verbindung mit der moveop bzw 
	  ihrer direc liegen,
     */
     if ( just_found ) 
     {
	ready = true;
	if ( direc == 'b' ) 
	{
	   while ( help != NIL ) 
	   {
	      if ( help->number != moveop &&  
		   helpblock->kind == 'm' &&	
		   !Member(DisjArcs->succ[moveop],help->number) )
	      {
		 DisjArcs->succ[moveop] =
				Insert(DisjArcs->succ[moveop],help->number);
		 DisjArcs->pred[help->number] =
				Insert(DisjArcs->pred[help->number],moveop);
		 SonNode->num_of_disj_pred[help->number]++;
		 SonNode->num_of_disj_succ[moveop]++;
	      }
	      if ( help->number != moveop &&
		   helpblock->kind == 'j' &&	
		   !Member(ConjArcs->succ[moveop],help->number) )
	      {
		 ConjArcs->succ[moveop] =
				Insert(ConjArcs->succ[moveop],help->number);
		 ConjArcs->pred[help->number] =
				Insert(ConjArcs->pred[help->number],moveop);
		 SonNode->num_of_conj_pred[help->number]++;
		 SonNode->num_of_conj_succ[moveop]++;
	      }
	      help = help->next;
	   }
	}
	else 
	/* after-Kandidaten
	*/
	{
	   first = help->number;
	   help = help->next;
	   while ( help != NIL ) 
	   {
	      if ( helpblock->kind == 'm' &&
		   !Member(DisjArcs->succ[first],help->number) ) 
	      {
		 DisjArcs->succ[first] =
				Insert(DisjArcs->succ[first],help->number);
		 DisjArcs->pred[help->number] =
				Insert(DisjArcs->pred[help->number],first);
		 SonNode->num_of_disj_pred[help->number]++;
		 SonNode->num_of_disj_succ[first]++;
	      }    
	      if ( helpblock->kind == 'j' &&
		   !Member(ConjArcs->succ[first],help->number) ) 
	      {
		 ConjArcs->succ[first] =
				Insert(ConjArcs->succ[first],help->number);
		 ConjArcs->pred[help->number] =
				Insert(ConjArcs->pred[help->number],first);
		 SonNode->num_of_conj_pred[help->number]++;
		 SonNode->num_of_conj_succ[first]++;
	      }           
	      if ( help->number != moveop  &&  helpblock->kind == 'm' &&	
		   !Member(DisjArcs->succ[help->number],moveop) )
	      {
		 DisjArcs->succ[help->number] =
				 Insert(DisjArcs->succ[help->number],moveop);
		 DisjArcs->pred[moveop] =
				 Insert(DisjArcs->pred[moveop],help->number);
		 SonNode->num_of_disj_succ[help->number]++;
		 SonNode->num_of_disj_pred[moveop]++;
	      }
	      if ( help->number != moveop  &&  helpblock->kind == 'j' &&	
		   !Member(ConjArcs->succ[help->number],moveop) )
	      {
		 ConjArcs->succ[help->number] =
				 Insert(ConjArcs->succ[help->number],moveop);
		 ConjArcs->pred[moveop] =
				 Insert(ConjArcs->pred[moveop],help->number);
		 SonNode->num_of_conj_succ[help->number]++;
		 SonNode->num_of_conj_pred[moveop]++;
	      }
	      help = help->next;
	   }
	}
	just_found = false;
     }

     /* die moveop liegt nicht im gerade betrachteten Block:
	Kanten ziehen in den Bloecken, die vor dem gerade betrachteten
	Block in SonNode stehen: d.h. die erste Op eines Blockes vor 
	allen anderen Op'en des Blockes und die letzte Op eines Blockes 
	nach allen anderen Op'en des Blockes (Verzweigungsregel: Liste E)
     */
     else 
     {
	first = help->number;        
	help = help->next;
	while ( help != NIL ) 
	{
	   if ( helpblock->kind == 'm'  &&
		!Member(DisjArcs->succ[first],help->number) )
	   {
	      DisjArcs->succ[first] =
			      Insert(DisjArcs->succ[first],help->number);
	      DisjArcs->pred[help->number] =
			      Insert(DisjArcs->pred[help->number],first);
	      SonNode->num_of_disj_pred[help->number]++;
	      SonNode->num_of_disj_succ[first]++;
	   }
	   if ( helpblock->kind == 'j'  &&
		!Member(ConjArcs->succ[first],help->number) ) 
	   {
	      ConjArcs->succ[first] =
			      Insert(ConjArcs->succ[first],help->number);
	      ConjArcs->pred[help->number] =
			      Insert(ConjArcs->pred[help->number],first);
	      SonNode->num_of_conj_pred[help->number]++;
	      SonNode->num_of_conj_succ[first]++;
	   }   
	   if ( help->number != last  &&  helpblock->kind == 'm' &&
		!Member(DisjArcs->pred[last],help->number) ) 
	   {
	      DisjArcs->pred[last] =
			      Insert(DisjArcs->pred[last],help->number);
	      DisjArcs->succ[help->number] =
			      Insert(DisjArcs->succ[help->number],last);
	      SonNode->num_of_disj_succ[help->number]++;
	      SonNode->num_of_disj_pred[last]++;
	   }
	   if ( help->number != last  &&  helpblock->kind == 'j' &&
		!Member(ConjArcs->pred[last],help->number) ) 
	   {
	      ConjArcs->pred[last] =
			      Insert(ConjArcs->pred[last],help->number);
	      ConjArcs->succ[help->number] =
			      Insert(ConjArcs->succ[help->number],last);
	      SonNode->num_of_conj_succ[help->number]++;
	      SonNode->num_of_conj_pred[last]++;
	   }
	   help = help->next;
	}
     }
     helpblock = helpblock->next;
  }
}
