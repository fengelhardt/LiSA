/* ************************************************************************* */
/*                              Filename: Low.h                              */
/*                                                                           */
/* EXPORT: Procedure Compute_Lower_Bound: Computation of a lower bound for   */
/*                                        SonNode                            */
/*         Procedure Preemptive_Schedule: Calculation of the makespan of     */
/*                                        the optimal preemptive schedule    */
/*                                        for a set of operations on one     */
/*                                        machine                            */
/*   @version 2.3pre3
/* ************************************************************************* */


#ifndef _LOW_H
#define _LOW_H


/* ************************************************************************* */
/*                        Procedure   Preemptive_Schedule()                  */
/*                                                                           */
/*  INPUT:  op_array    Array of operations                                  */
/*          num         number of operations in op_array                     */
/*          head        array of heads                                       */
/*                                                                           */
/*  FUNCTION: Calculation of the makespan of the                             */
/*            (n|1|r_i >= 0, q_i >= 0|preempt,prec|C_max) problem based on   */
/*            the operations in op_array                                     */
/* ************************************************************************* */


int Preemptive_Schedule (int[] , int, int[]);

/* ************************************************************************* */
/*                      Procedure   Compute_LowerBound()                     */
/*                                                                           */
/*  INPUT :   ./.                                                            */
/*                                                                           */
/*  FUNCTION: Calculation of a lower bound for SonNode by solving the        */
/*            (n|1|r_i >= 0, q_i >= 0|preempt,prec|C_max) problem for all    */
/*            machines                                                       */
/* ************************************************************************* */

int Compute_LowerBound ();


#endif

