/* ************************************************************************* */
/*                            Filename: Stack.c                              */
/*                                                                           */
/*   EXPORT: Procedure Push: 						     */
/*		       Legt einen Suchbaumknoten auf dem Stack ab            */
/*           Procedure Pop:  						     */
/*		       Loescht einen Suchbaumknoten vom Stack                */
/* ************************************************************************* */

#include <cstdio>
#include <cstdlib>
#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_stack.hpp"

#include "../../misc/except.hpp"

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

  if ((newelement = new struct StackElement)
                  == NIL )
  {
     G_ExceptionList.lthrow("stack,newelement: kein Speicherplatz",
                            Lisa_ExceptionList::NO_MORE_MEMORY) ;
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
        delete saveblock;
     }
     branchhelp = ActualNode->order;
     while (branchhelp != NIL) 
     {
        savebranch = branchhelp;
        branchhelp = branchhelp->next;
        delete savebranch;
     }
     delete ActualNode;
  }
  ActualNode = FirstOfStack->node;     
  savestack = FirstOfStack;
  FirstOfStack = FirstOfStack->next;
  delete savestack;
}
