/* ************************************************************************* */
/*                            Filename: Block.c                              */
/*                                                                           */
/*   Export: Procedure Compute_Blocks:                                       */
/*                     Calculation of the blocks on the critical path        */
/* ************************************************************************* */

#include <malloc.h>
#include "jb_dtstr.hpp"
#include "jb_table.hpp"
#include "jb_list.hpp"
#include "jb_block.hpp"



/* ************************************************************************* */
/*                      Procedure Compute_Blocks()                           */
/*                                                                           */
/*  INPUT: -/-                                                               */
/*                                                                           */
/*                                                                           */
/*  FUNCTION: Calculation of the blocks on the critical path                 */
/*            The blocks are stored in the order of non-decreasing           */
/*            in SonNode.                                                    */
/* ************************************************************************* */

void Compute_Blocks ()
{

 struct BlockList  *actual_block,  
                   *first_block=0,  
                   *helpblock;
 struct List       *help;
 int               num_of_elements,  
                                    
                   mach_nr;

 actual_block = NIL;
 while (CriticalPath != NIL) {       /* Calculation of the first block */
    help = CriticalPath;
    mach_nr = OpData[CriticalPath->number].machine_nr;
    num_of_elements = 1;
    while (help->next != NIL &&
            OpData[help->next->number].machine_nr == mach_nr) {
       help = help->next;
       num_of_elements ++;
    }
    if (num_of_elements == 1)                                  /* no block */
       CriticalPath = Delete(CriticalPath,help->number);
    else {                                                     /* first block */
       first_block = (struct BlockList *) malloc(sizeof(struct BlockList));
       first_block->elements = CriticalPath;
       first_block->last_of_block = help->number;
       first_block->fi_mi_la = 'f';                
       first_block->next = NIL;
       first_block->elements = InsertBefore(first_block->elements,
                                                 num_of_elements);
                          /* The first element is used for sorting the blocks. 
                             It will be deleted at the end of the procedure */

       CriticalPath = help->next;
       help->next = NIL;
       SonNode->blocks = first_block;
       break;                              /* first block found */
    }
 }
 while (CriticalPath != NIL) {           /* Calculation of the other blocks */ 
    help = CriticalPath;
    mach_nr = OpData[CriticalPath->number].machine_nr;
    num_of_elements = 1;
    while (help->next != NIL &&
            OpData[help->next->number].machine_nr == mach_nr) {
       help = help->next;
       num_of_elements ++;
    }
    if (num_of_elements == 1)                                  /* no block  */
       CriticalPath = Delete(CriticalPath,help->number);
    else {
       actual_block = (struct BlockList *) malloc(sizeof(struct BlockList));
       actual_block->elements = CriticalPath;
       actual_block->last_of_block = help->number;
       actual_block->next = NIL;
       actual_block->elements = InsertBefore(actual_block->elements,
                                                   num_of_elements);
       CriticalPath = help->next;
       help->next = NIL;
       if (CriticalPath == NIL) actual_block->fi_mi_la = 'l';
       else actual_block->fi_mi_la = 'm';
       if (num_of_elements > SonNode->blocks->elements->number) {
          actual_block->next = SonNode->blocks;
          SonNode->blocks = actual_block;
       }
       else {
          helpblock = SonNode->blocks;
          while (helpblock->next != NIL) {
             if (helpblock->next->elements->number <= num_of_elements) break;
             helpblock = helpblock->next;
          }
          actual_block->next = helpblock->next;
          helpblock->next = actual_block;
       }
    }
  }
  if (actual_block != NIL) {
     if (actual_block->fi_mi_la != 'l') first_block->fi_mi_la = 'm';
  }
  else first_block->fi_mi_la = 'l';  
  helpblock = SonNode->blocks;  
  while (helpblock != NIL) {
     helpblock->elements = Delete(helpblock->elements,
                                               helpblock->elements->number);
     helpblock = helpblock->next;
  }
}


