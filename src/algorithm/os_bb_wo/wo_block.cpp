/* ************************************************************************* */
/*                           Filename: Block.c                               */
/*                                                                           */
/*   Export: Procedure Compute_Blocks:                                       */
/*                     Berechnung der Bloecke auf dem kritischen Pfad        */
/* ************************************************************************* */

#include <malloc.h>
#include <stdio.h>
#include "data.h"
#include "table.h"
#include "list.h"
#include "block.h"


/* ************************************************************************* */
/*                      Procedure Compute_Blocks()                           */
/*                                                                           */
/*  INPUT: -/-                                                               */
/*                                                                           */
/*                                                                           */
/*  FUNCTION: Berechnung der Bloecke auf dem kritischen Pfad.                */
/*            Die Bloecke werden in der Reihenfolge nichtsteigender          */
/*            Kardinalitaet in SonNode gespeichert.                          */
/* ************************************************************************* */

void Compute_Blocks ()
{

 struct BlockList  *actual_block,  
		   *first_block,
		   *helpblock;
 struct List       *help,
                   *help1;
 int               num_of_elements,  
                   pos = 1,
		   pos_merk = 0,
		   job_nr,                 
		   mach_nr;



 if ( (first_block = (struct BlockList *) malloc(sizeof(struct BlockList)))
                   == NIL )
 {
    fprintf(stderr, "block,first_block: malloc: kein Speicherplatz\n") ;
    exit(1);
 }
 first_block->elements = NIL;

 if ( OpData[CriticalPath[pos]].machine_nr ==
			  OpData[CriticalPath[pos+1]].machine_nr )
 {
    /* Maschinenbloecke ?
    */
    first_block->kind = 'm'; 
    mach_nr = OpData[CriticalPath[pos]].machine_nr;
    first_block->elements = Insert(first_block->elements,
                                              CriticalPath[pos++]);
    num_of_elements = 1;
    while ( pos < PathPos &&
        	  OpData[CriticalPath[pos]].machine_nr == mach_nr )
    {
       first_block->elements = Insert(first_block->elements,
                                                 CriticalPath[pos++]);
       pos_merk = pos;
       num_of_elements ++;
    }
 }

 /* Jobbloecke ?
 */
 else                                                      
 {
    first_block->kind = 'j';
    job_nr = OpData[CriticalPath[pos]].job_nr;
    first_block->elements = Insert(first_block->elements,
 					 	CriticalPath[pos++]);
    num_of_elements = 1;
    while ( pos < PathPos &&
	          OpData[CriticalPath[pos]].job_nr == job_nr)
    {
       first_block->elements = Insert(first_block->elements,
                                            CriticalPath[pos++]);
       pos_merk = pos;
       num_of_elements++;
    }	       
 }

 /* Fuellen des ersten Blocks.
    Das erste Element ist die Groesse des Blocks, es wird spaeter zum 
    Sortieren der Bloecke der Groesse benutzt und dann geloescht.
 */
 first_block->last_of_block = CriticalPath[--pos];
 first_block->next = NIL;
 first_block->elements = InsertBefore(first_block->elements,
      						num_of_elements);

 SonNode->blocks = first_block;

 /* Diese Schleife wird fuer all weiteren Bloecke durchlaufen, d.h. fuer den
    Fall, dass CP aus mehr als einem Block besteht
 */
 while ( pos_merk < PathPos )           
 {
    if ( (actual_block = (struct BlockList *) malloc(sizeof(struct BlockList)))
                       == NIL )
    {
       fprintf(stderr, "block,actual_block: malloc: kein Speicherplatz\n") ;
       exit(1);
    }
    actual_block->elements = NIL;

    if ( OpData[CriticalPath[pos]].machine_nr ==
			  OpData[CriticalPath[pos+1]].machine_nr )
    {
       /* Maschinenbloecke ?
       */
       actual_block->kind = 'm';
       mach_nr = OpData[CriticalPath[pos]].machine_nr;
       actual_block->elements = Insert(actual_block->elements,
					           CriticalPath[pos++]);
       num_of_elements = 1;
       while ( pos < PathPos &&
		     OpData[CriticalPath[pos]].machine_nr == mach_nr ) 
       {
          actual_block->elements = Insert(actual_block->elements,
                                                 CriticalPath[pos++]);
          pos_merk = pos;
	  num_of_elements ++;
       }
    }

    /* Jobbloecke ?
    */
    else
    {
       actual_block->kind = 'j';
       job_nr = OpData[CriticalPath[pos]].job_nr;
       actual_block->elements = Insert(actual_block->elements,
                                                   CriticalPath[pos++]);
       num_of_elements = 1;
       while ( pos < PathPos &&
		     OpData[CriticalPath[pos]].job_nr == job_nr )
       {
          actual_block->elements = Insert(actual_block->elements,
                                                 CriticalPath[pos++]);
          pos_merk = pos;
	  num_of_elements ++;
       }	       
    }

    /* Fuellen der Bloecke , wie oben bei first_block
    */
    actual_block->last_of_block = CriticalPath[--pos];
    actual_block->next = NIL;
    actual_block->elements = InsertBefore(actual_block->elements,
						num_of_elements);

    /* Sortieren der Bloecke der Groesse nach, der groesste Block nach vorn.
       In SonNode->blocks->elements->number steht entweder die Groesse des
       ersten Blocks oder aber die Groesse eines weiteren Blocks
    */ 
    if (num_of_elements > SonNode->blocks->elements->number)
    {
       /* der neue Block ist groesser als der bisher groesste und wird daher
	  nach ganz vorne in der Blockliste gestellt
       */
       actual_block->next = SonNode->blocks;
       SonNode->blocks = actual_block;
    }

    else 
    {
       /* der gefundene Block ist kleiner als der bisher groesste und muss
	  daher vor einen gestellt werden, der kleiner ist.
       */
       helpblock = SonNode->blocks;
       while (helpblock->next != NIL) 
       {
	  /* es wird bewerkstelligt,dass Bloecke
	     mit gleicher Anzahl an Elementen in der Reihenfolge ihres Auf-
	     tretens gespeichert werden
	  */
	  if (helpblock->next->elements->number < num_of_elements) 
	      break;

	  helpblock = helpblock->next;
       }
       actual_block->next = helpblock->next;
       helpblock->next = actual_block;
    }
 }

  /* Loeschen des Elementes aus der Liste, das die Anzahl der Operationen 
     eines Blocks enthaelt
  */ 
  helpblock = SonNode->blocks;  
  while (helpblock != NIL) 
  {
     helpblock->elements = Delete(helpblock->elements,
					       helpblock->elements->number);
     helpblock = helpblock->next;
  }
}
