/* ************************************************************************* */
/*                           Filename: List.h                                */
/*                                                                           */
/*  EXPORT: Procedure Insert:       Haengt ein Element ans Ende einer Liste  */
/*          Procedure InsertBefore: Stellt ein Element an Anfang einer Liste */
/*          Procedure Delete:       Loescht ein Element aus einer Liste      */
/*          Procedure Makeempty:    Loescht alle Elemente aus einer Liste    */
/*          Procedure Member:       Ueberprueft, ob Element in der Liste ist */
/*          Procedure Update_Arcs:  Aufdatieren festgelegter Kanten waehrend */
/*                                  des Backtrackings                        */
/* ************************************************************************* */

#ifndef _WO_LIST_H
#define _WO_LIST_H

#include "wo_data.hpp"

/* ************************************************************************* */
/*                          Procedure  Insert()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Op wird ans Ende von l gestellt, wenn Member(l,Op) == false    */
/* ************************************************************************* */

struct List *Insert (struct List* l, int op);


/* ************************************************************************* */
/*                          Procedure  InsertBefore()                        */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Op wird an den Anfang von l gestellt                           */
/* ************************************************************************* */

struct List *InsertBefore (struct List *l,  int op);


/* ************************************************************************* */
/*                          Procedure  Delete()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Op wird aus l geloescht                                        */
/* ************************************************************************* */

struct List *Delete (struct List *l, int op);


/* ************************************************************************* */
/*                          Procedure  Makeempty()                           */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*                                                                           */
/*  FUNCTION: l wird geloescht                                               */
/* ************************************************************************* */

struct List *Makeempty (struct List *l);


/* ************************************************************************* */
/*                          Procedure  Member()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Uberprueft, ob Op in l ist oder nicht                          */
/* ************************************************************************* */

boolean Member (struct List *l,  int op);


/* ************************************************************************* */
/*                        Procedure Update_Arcs()                            */
/*                                                                           */
/*  INPUT : -/-                                                              */
/*                                                                           */
/*  FUNCTION: Loescht alle ueberfluessigen Kanten in ActualNode waehrend des */
/*            Backtrackings.                                                 */
/* ************************************************************************* */

void Update_Arcs ();

#endif
