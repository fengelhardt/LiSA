/* ************************************************************************* */
/*                           Filename: List.c                                */
/*                                                                           */
/*  EXPORT: Procedure Insert:          Append an element at the end of a list*/
/*          Procedure InsertBefore:    Append an element at the beginning of */
/*                                     a list                                */
/*          Procedure Delete:          Delete an element from a list         */
/*          Procedure Makeempty:       Delete all elements from a list       */
/*          Procedure Member:          Check whether an element is in a list */
/*          Procedure PrintList:       Print list on screen                  */
/*          Procedure Update_DisjArcs: Updating of fixed disjunctive arcs    */
/*                                     during the backtracking               */
//   @version 3.0
/* ************************************************************************* */

#include <malloc.h>

#include "jb_dtstr.hpp"
#include "jb_table.hpp"
#include "jb_list.hpp"


/* ************************************************************************* */
/*                          Procedure  Insert()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */ 
/*          Op         integer                                               */ 
/*                                                                           */
/*  FUNCTION: Op is appended at the end of l if member(l,Op) == false        */
/* ************************************************************************* */

struct List *Insert(struct List *l, int Op)
{
  register struct List *help;
  if ( l == NIL ) { 
     l = (struct List *) malloc(sizeof(struct List));
     l->number = Op;
     l->next = NIL;
  }
  else  {              
     help = l;
     while ( help->next != NIL ) help = help->next;
     help->next = (struct List *) malloc(sizeof(struct List));
     help = help->next;
     help->number = Op;
     help->next = NIL;
  }
  return(l);
}

/* ************************************************************************* */
/*                          Procedure  InsertBefore()                        */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Op is appended at the begining of l                            */
/* ************************************************************************* */

struct List *InsertBefore(struct List *l, int Op)
{

 struct List *help;

    help = (struct List *) malloc(sizeof(struct List));    
    help->number = Op;
    help->next = l;
    return(help);
}

/* ************************************************************************* */
/*                          Procedure  Delete()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Op is deleted from l                                           */
/* ************************************************************************* */

struct List *Delete(struct List *l, register int Op)

{

  register struct List *help, *cancel;

  if ( l == NIL ) return(NIL); 
  if ( l->number == Op ) {    
     cancel = l;
     l = l->next;
     free((void *) cancel);
     return(l);
  }
  help = l;                  
  while ( help->next != NIL && help->next->number != Op ) help = help->next;
  cancel = help->next;
  help->next = help->next->next;
  free((void *) cancel);
  return(l);
}

/* ************************************************************************* */
/*                          Procedure  Makeempty()                           */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*                                                                           */
/*  FUNCTION: l is deleted                                                   */
/* ************************************************************************* */

struct List *Makeempty(register struct List *l)
{

  struct List *cancel;

  while ( l != NIL ) {
     cancel = l;
     l = l->next;
     free((void *) cancel);
  }
  return(NIL);                            
}

/* ************************************************************************* */
/*                          Procedure  member()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Check whether Op is in l or not                                */
/* ************************************************************************* */

boolean Member(struct List *l, register int Op)
{

  register struct List *help;

  help = l;
  while ( help != NIL ) {
     if (help->number == Op) return (true); 
     help = help->next;
  }
  return(false);                           
}

/* ************************************************************************* */
/*                        Procedure Update_DisjArcs()                        */
/*                                                                           */
/*  INPUT : -/-                                                              */
/*                                                                           */
/*  FUNCTION: Deletes all invalid disjunctive arcs in ActualNode during      */
/*            the backtracking.                                              */
/* ************************************************************************* */



void Update_DisjArcs ()
{

  register struct List  *help;
  struct List  *cancel;
  int          num;
  register int i,j;

  for (i = 1; i <= NumOfOperations;++i) {
      if (ActualNode->num_of_succ[i] == 0)
         DisjArcs->succ[i] = Makeempty(DisjArcs->succ[i]);
      else {
         num = ActualNode->num_of_succ[i];
         help = DisjArcs->succ[i];
         for (j = 1; j < num; ++j) help = help->next;
         cancel = help->next;
         help->next = NIL;
         cancel = Makeempty(cancel);
      }
      if (ActualNode->num_of_pred[i] == 0)
         DisjArcs->pred[i] = Makeempty(DisjArcs->pred[i]);
      else {
         num = ActualNode->num_of_pred[i];
         help = DisjArcs->pred[i];
         for (j = 1; j < num; ++j) help = help->next;
         cancel = help->next;
         help->next = NIL;
         cancel = Makeempty(cancel);
      }
  }  /* end for ... */
}


