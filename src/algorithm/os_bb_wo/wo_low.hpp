/* ************************************************************************* */
/*                              Filename: Low.h                              */
/*                                                                           */
/* EXPORT: Procedure Compute_Lower_Bound:                                    */
/*                   Berechnung einer lower bound fuer SonNode               */
/*         Procedure Preemptive_Schedule:                                    */
/*                   Berechnung des Zielfunktionswertes eines optimalen      */
/*                   Planes mit Unterbrechung fuer eine Menge von Op'en auf  */
/*                   einer Maschine                                          */
/* ************************************************************************* */


#ifndef _LOW_H
#define _LOW_H


/* ************************************************************************* */
/*                        Procedure   Preemptive_Schedule()                  */
/*                                                                           */
/*  INPUT:  op_array    Array von Operationen                                */
/*          num         Anzahl der Operationen in op_array                   */
/*          head        Array der Heads                                      */
/*                                                                           */
/*  FUNCTION: Berechnung des Zielfunktionswertes des Problems                */
/*            (n|1|r_i >= 0, q_i >= 0|preempt,prec|C_max) basierend auf den  */
/*            Operationen in op_array                                        */
/* ************************************************************************* */


int Preemptive_Schedule (int op_array[],int num,int head[]);

/* ************************************************************************* */
/*                      Procedure   Compute_LowerBound()                     */
/*                                                                           */
/*  INPUT :   ./.                                                            */
/*                                                                           */
/*  FUNCTION: Berechnung einer lower bound fur SonNode durch die Loesung des */
/*            (n|1|r_i >= 0, q_i >= 0|preempt,prec|C_max) Problems fur alle  */
/*            Maschinen und alle Jobs                                        */
/* ************************************************************************* */

int Compute_LowerBound ();


#endif
