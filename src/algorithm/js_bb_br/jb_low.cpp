/* ************************************************************************* */
/*                              Filename: Low.c                              */
/*                                                                           */
/* EXPORT: Procedure Compute_Lower_Bound: Computation of a lower bound for   */
/*                                        SonNode                            */
/*         Procedure Preemptive_Schedule: Calculation of the makespan of     */
/*                                        the optimal preemptive schedule    */
/*                                        for a set of operations on one     */
/*                                        machine                            */
//   @version 2.3final
/* ************************************************************************* */


#include "jb_dtstr.hpp"
#include "jb_table.hpp"
#include "jb_sort.hpp"
#include "jb_low.hpp"


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


int Preemptive_Schedule(int op_array[], int num, int head[])
{

  int set_O[MaxOpProMachine+1],
      time, next_time,
      process[MaxNumOfOperations+1],
      gap,
      max_low = 0, new_low,  
      Op, dummy;

  register int i, rest_pos, num_of_O;

  Sort(head, op_array, num);
  num_of_O = 0;
  for (rest_pos = 1; rest_pos <= num; ++rest_pos)
       process[op_array[rest_pos]] = OpData[op_array[rest_pos]].process_time;
  rest_pos = 1;
  time = head[ op_array[1] ];
  while (rest_pos <= num) {   
     while (rest_pos <= num && head[ op_array[rest_pos] ] == time) {
         Op = op_array[rest_pos];
         for (i = 1; i <= num_of_O; ++i) {   
            if ( tail[Op] < tail[set_O[i]] ) {
                 dummy = set_O[i];
                 set_O[i] = Op;
                 Op = dummy;
            }
         }
         set_O[++num_of_O] = Op;
         rest_pos++;
     }
     if (rest_pos <= num) {    
        next_time = head[ op_array[rest_pos] ];
        while (num_of_O > 0 && time < next_time) {
           gap = next_time - time;
           if ( process[ set_O[num_of_O] ] <= gap ) {
              time += process[ set_O[num_of_O] ];
              new_low = time + tail[ set_O[num_of_O] ];
              max_low = (max_low < new_low)? new_low : max_low;
              process[ set_O[num_of_O--] ] = 0;
           }
           else {
              process[ set_O[num_of_O] ] -= gap;
              break;  
           }
        }
        time = next_time;
     }
     else {          
        while (num_of_O > 0) {
           time += process[ set_O[num_of_O] ];
           new_low = time + tail[ set_O[num_of_O] ];
           max_low = (max_low < new_low)? new_low : max_low;
           num_of_O--;
        }
     }
  }
  return(max_low);
}

/* ************************************************************************* */
/*                      Procedure   Compute_LowerBound()                     */
/*                                                                           */
/*  INPUT :   ./.                                                            */
/*                                                                           */
/*  FUNCTION: Calculation of a lower bound for SonNode by solving the        */
/*            (n|1|r_i >= 0, q_i >= 0|preempt,prec|C_max) problem for all    */
/*            machines                                                       */
/* ************************************************************************* */

int Compute_LowerBound ()
{

  int          i, j,
               max_lower_bound = 0,   
               new_lower_bound,      
               op_array[MaxOpProMachine]; 
  struct List  *help;

  for (i = 1; i <= NumOfMachines; ++i) {
     j = 0;
     help = OpsOnMachine[i];
     while (help != NIL) {
        op_array[++j] = help->number;
        help = help->next;
     }
     if ( (new_lower_bound = Preemptive_Schedule(op_array, j, head) )
            > max_lower_bound)  
         max_lower_bound = new_lower_bound;
     if ( max_lower_bound >= UpperBound ) return(max_lower_bound);
  }
  return(max_lower_bound);           
}


