/* ************************************************************************* */
/*                           Filename: List.h                                */
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
//   @version 2.3rc1
/* ************************************************************************* */

#ifndef _LIST_H
#define _LIST_H

#include "jb_dtstr.hpp"

/* ************************************************************************* */
/*                          Procedure  Insert()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */ 
/*          Op         integer                                               */ 
/*                                                                           */
/*  FUNCTION: Op is appended at the end of l if Member(l,Op) == false        */
/* ************************************************************************* */
/// Op is appended at the end of l if Member(l,Op) == false
/** @param l          linked list of integers 
    @param Op         integer */
struct List *Insert(struct List *l, int Op);


/* ************************************************************************* */
/*                          Procedure  InsertBefore()                        */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Op is appended at the begining of l                            */
/* ************************************************************************* */
/// Op is appended at the begining of l
/** @param l          linked list of integers 
    @param Op         integer */
struct List *InsertBefore(struct List *l, int Op);


/* ************************************************************************* */
/*                          Procedure  Delete()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Op is deleted from l                                           */
/* ************************************************************************* */
/// Op is deleted from l
/** @param l          linked list of integers 
    @param Op         integer */
struct List *Delete(struct List *l, register int Op);


/* ************************************************************************* */
/*                          Procedure  Makeempty()                           */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*                                                                           */
/*  FUNCTION: l is deleted                                                   */
/* ************************************************************************* */
/// l is deleted      
/** @param l          linked list of integers */
struct List *Makeempty(register struct List *l);


/* ************************************************************************* */
/*                          Procedure  member()                              */
/*                                                                           */
/*  INPUT : l          linked list of integers                               */
/*          Op         integer                                               */
/*                                                                           */
/*  FUNCTION: Check whether Op is in l or not                                */
/* ************************************************************************* */
/// Check whether Op is in l or not
/** @param l          linked list of integers 
    @param Op         integer */
boolean Member(struct List *l, register int Op);


/* ************************************************************************* */
/*                        Procedure Update_DisjArcs()                        */
/*                                                                           */
/*  INPUT : -/-                                                              */
/*                                                                           */
/*  FUNCTION: Deletes all invalid disjunctive arcs in ActualNode during      */
/*            the backtracking.                                              */
/* ************************************************************************* */
/// Deletes all invalid disjunctive arcs in ActualNode during the backtracking. 
void Update_DisjArcs();

#endif
