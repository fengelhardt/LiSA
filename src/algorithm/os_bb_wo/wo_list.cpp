/* ************************************************************************* */
/*                           Filename: List.c                                */
/*                                                                           */
/*  EXPORT: Procedure Insert:       Haengt ein Element ans Ende einer Liste  */
/*          Procedure InsertBefore: Stellt ein Element an Anfang einer Liste */
/*          Procedure Delete:       Loescht ein Element aus einer Liste      */
/*          Procedure Makeempty:    Loescht alle Elemente aus einer Liste    */
/*          Procedure Member:       Ueberprueft, ob Element in der Liste ist */
/*          Procedure Update_Arcs:  Aufdatieren festgelegter Kanten waehrend */
/*                                  des Backtrackings                        */
/* ************************************************************************* */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_list.hpp"


/* ************************************************************************* */
/*                          Procedure  Insert()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */ 
/*          op	       integer                                               */ 
/*                                                                           */
/*  FUNCTION: Op wird ans Ende von l gestellt, wenn Member(l,Op) == false    */
/* ************************************************************************* */

struct List *Insert(struct List *l,int op)
{

  register struct List *help;

  if ( l == NIL ) 
  {
     if ( (l = (struct List *) malloc(sizeof(struct List))) == NIL )
     {
        fprintf(stderr, "Insert,l: malloc: kein Speicherplatz\n");
	exit(1);
     }
     l->number = op;
     l->next = NIL;
  }
  else  
  {              
     help = l;
     while ( help->next != NIL ) 
             help = help->next;
     if ( (help->next = (struct List *) malloc(sizeof(struct List)))
                      == NIL )
     {
        fprintf(stderr, "Insert,help->next:malloc:kein Speicherplatz\n");
        exit(1);
     }
     help = help->next;
     help->number = op;
     help->next = NIL;
  }
  return(l);
}

/* ************************************************************************* */
/*                          Procedure  InsertBefore()                        */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          op         integer                                               */
/*                                                                           */
/*  FUNCTION: Op wird an den Anfang von l gestellt                           */
/* ************************************************************************* */

struct List *InsertBefore(struct List *l,int op)
{

 struct List *help;

    if ( (help = (struct List *) malloc(sizeof(struct List)))
               == NIL )
    {
       fprintf(stderr, "InsertBefore,help: malloc: kein Speicherplatz\n");
       exit(1);
    }
    help->number = op;
    help->next = l;
    return(help);
}

/* ************************************************************************* */
/*                          Procedure  Delete()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          op 	       integer                                               */
/*                                                                           */
/*  FUNCTION: Op wird aus l geloescht                                        */
/* ************************************************************************* */

struct List *Delete(struct List *l,register int op)
{

  register struct List *help, *cancel;

  if ( l == NIL ) 
     return(NIL); 
  if ( l->number == op ) 
  {    
     cancel = l;
     l = l->next;
     free((void *) cancel);
     return(l);
  }

  help = l;                  
  while ( help->next != NIL && help->next->number != op ) 
        help = help->next;
  /* die zu loeschende Op ist in der Liste nicht vorhanden
  */
  if ( help->next->number != op )
     return(l);
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
/*  FUNCTION: l wird geloescht                                               */
/* ************************************************************************* */

struct List *Makeempty(register struct List *l)
{

  struct List *cancel;

  while ( l != NIL ) 
  {
     cancel = l;
     l = l->next;
     free((void *) cancel);
  }
  return(NIL);                            
}

/* ************************************************************************* */
/*                          Procedure  Member()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          op	       integer                                               */
/*                                                                           */
/*  FUNCTION: Uberprueft, ob Op in l ist oder nicht                          */
/* ************************************************************************* */

boolean Member(struct List *l,register int op)
{

  register struct List *help;

  help = l;
  while ( help != NIL ) 
  {
     if (help->number == op) 
	return (true); 
     help = help->next;
  }
  return(false);                           
}



/* ************************************************************************* */
/*                        Procedure Update_Arcs()                            */
/*                                                                           */
/*  INPUT : -/-                                                              */
/*                                                                           */
/*  FUNCTION: Loescht alle ueberfluessigen Kanten in ActualNode waehrend des */
/*            Backtrackings.                                                 */
/* ************************************************************************* */

void Update_Arcs ()
{

  register struct List  *help;

  struct List  *cancel;

  int          num;

  register int i,
	       j;

  for ( i = 1; i <= NumOfOperations; ++i )
  {
     if ( ActualNode->num_of_conj_succ[i] == 0 )
	ConjArcs->succ[i] = Makeempty(ConjArcs->succ[i]);
     else 
     {
	help = ConjArcs->succ[i];
	num = ActualNode->num_of_conj_succ[i];
	for ( j = 1; j < num; ++j ) 
	   help = help->next;
	cancel = help->next;
	help->next = NIL;
	cancel = Makeempty(cancel);
     }

     if ( ActualNode->num_of_disj_succ[i] == 0 )
	DisjArcs->succ[i] = Makeempty(DisjArcs->succ[i]);
     else
     {
	help = DisjArcs->succ[i];
	num = ActualNode->num_of_disj_succ[i];
	for ( j = 1; j < num; ++j ) 
	   help = help->next;
	cancel = help->next;
	help->next = NIL;
	cancel = Makeempty(cancel);
     }

     if ( ActualNode->num_of_conj_pred[i] == 0 )
	ConjArcs->pred[i] = Makeempty(ConjArcs->pred[i]);        
     else 
     {
	help = ConjArcs->pred[i];
	num = ActualNode->num_of_conj_pred[i];
	for ( j = 1; j < num; ++j ) 
	   help = help->next;
	cancel = help->next;
	help->next = NIL;
	cancel = Makeempty(cancel);
     }

     if ( ActualNode->num_of_disj_pred[i] == 0 )
	DisjArcs->pred[i] = Makeempty(DisjArcs->pred[i]);
     else
     {
	help = DisjArcs->pred[i];
	num = ActualNode->num_of_disj_pred[i];
	for ( j = 1; j < num; ++j ) 
	   help = help->next;
	cancel = help->next;
	help->next = NIL;
	cancel = Makeempty(cancel);
     }
  }  /* end for ... */
}
