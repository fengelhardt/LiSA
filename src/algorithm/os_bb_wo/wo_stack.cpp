/* ************************************************************************* */
/*                            Filename: Stack.c                              */
/*                                                                           */
/*   EXPORT: Procedure Push: 						     */
/*		       Legt einen Suchbaumknoten auf dem Stack ab            */
/*           Procedure Pop:  						     */
/*		       Loescht einen Suchbaumknoten vom Stack                */
/* ************************************************************************* */

#include <malloc.h>
#include <stdio.h>
#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_stack.hpp"


/* ************************************************************************* */
/*                           Procedure  Push()                               */
/*                                                                           */
/*  INPUT : -/-                                                              */
/*                                                                           */
/*  FUNCTION: Legt ActualNode auf dem Stack ab                               */
/* ************************************************************************* */

void Push ()
{
  struct StackElement *newelement;

  if ((newelement = (struct StackElement *) malloc(sizeof(struct StackElement)))
                  == NIL )
  {
     fprintf(stderr, "stack,newelement: malloc: kein Speicherplatz\n") ;
     exit(1);
  }

  newelement->next = FirstOfStack;
  newelement->node = ActualNode;
  FirstOfStack = newelement;       
  ActualNode = NIL;
}

/* ************************************************************************* */
/*                           Procedure  Pop()                                */
/*                                                                           */
/*  INPUT : -/-                                                              */
/*                                                                           */
/*  FUNCTION: Loescht ActualNode und weist den Top des Stacks ActualNode zu  */
/* ************************************************************************* */

void Pop ()
{

  struct StackElement  *savestack;
  struct BlockList     *blockhelp, 
                       *saveblock;
  struct BranchList    *branchhelp, 
                       *savebranch;

  if ( ActualNode != NIL )
  {        
     blockhelp = ActualNode->blocks;
     while ( blockhelp != NIL ) 
     {
        saveblock = blockhelp;
        blockhelp->elements = Makeempty(blockhelp->elements);
        blockhelp = blockhelp->next;
        free((void *) saveblock);
     }
     branchhelp = ActualNode->order;
     while (branchhelp != NIL) 
     {
        savebranch = branchhelp;
        branchhelp = branchhelp->next;
        free((void *) savebranch);
     }
     free((void *) ActualNode);
  }
  ActualNode = FirstOfStack->node;     
  savestack = FirstOfStack;
  FirstOfStack = FirstOfStack->next;
  free((void *) savestack);
}
