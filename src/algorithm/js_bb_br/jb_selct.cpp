/* ************************************************************************* */
/*                             Filename: Select.c                            */
/*                                                                           */
/*  EXPORT: Add_Disjunctive_Arcs: Fixation of additional disjunctive arcs    */
/*          due to J. Carlier and E.Pinson                                   */
//   @version 2.3pre3
/* ************************************************************************* */


#include "jb_dtstr.hpp"
#include "jb_table.hpp"
#include "jb_list.hpp"
#include "jb_hd_tl.hpp"
#include "jb_sort.hpp"
#include "jb_selct.hpp"


static int Jackson_Immediate_Selection(int mach_no, int head[], int tail[])
                                      /* solving the primal or the dual
                                         problem for all operations on
                                         machine mach_no */

{
  int           set_O[MaxOpProMachine], num_of_O, 
                set_S[MaxOpProMachine], num_of_S, 
                tail_sort[MaxOpProMachine+1],  
                head_sort[MaxOpProMachine+1],  
                process[MaxNumOfOperations+1],
                tausch, 
                next_time, time, sum_processtime,
                rest_pos, old_pos,   
                differenz, max_low = 0, new_low;

  register int i, Op, num;
  boolean       computed[MaxNumOfOperations+1];
  struct List   *help;

  num = 0;
  help = OpsOnMachine[mach_no];
  while (help != NIL) {
     ++num;
     process[help->number] = OpData[help->number].process_time;
     head_sort[num] = tail_sort[num] = help->number;
     help = help->next;
  }
  Sort(head, head_sort, num);
  Sort(tail, tail_sort, num);
  num_of_S = num_of_O = 0;
  time = head[head_sort[1]];
  rest_pos = 1;
  while (rest_pos <= num) {
     old_pos = rest_pos;
     while (rest_pos <= num && head[head_sort[rest_pos]] == time) {
        set_S[++num_of_S] = head_sort[rest_pos];
        rest_pos++;
     }
     while (num_of_S > 0)  {     
        Op = set_S[num_of_S];
        sum_processtime = head[Op] + process[Op];
        help = OpsOnMachine[mach_no];
        while (help != NIL) {
           if (process[help->number] != 0 && tail[help->number] > tail[Op]) {
              sum_processtime += process[help->number];
              computed[help->number] = true;
           }
           else computed[help->number] = false;
           help = help->next;
        }       
        i = 1;
        while (i <= num) {  
           if (computed[tail_sort[i]]) {
              if (sum_processtime + tail[tail_sort[i]] >= UpperBound) break;
              else {
                 computed[tail_sort[i]] = false,
                 sum_processtime -= process[tail_sort[i]];
              }
           }
           i++;
        } 
        if (i <= num) {        
           for (i = 1; i <= num; ++i) { 
              if (computed[head_sort[i]]) {
                 if (head[Op] < head[head_sort[i]])
                    head[Op] = head[head_sort[i]];
                 head[Op] += process[head_sort[i]];
              }
           }
           rest_pos--;         
           for (i = old_pos; i < num; ++i) {
              if (head[head_sort[i]] > head[head_sort[i+1]]) {
                 tausch = head_sort[i];
                 head_sort[i] = head_sort[i+1];
                 head_sort[i+1] = tausch;
              }
           }
        }
        else {  
           for (i = 1; i <= num_of_O; ++i) {
              if (tail[Op] < tail[set_O[i]]) {
                 tausch = set_O[i];
                 set_O[i] = Op;
                 Op = tausch;
              }
           }
           set_O[++num_of_O] = Op;
        }
        num_of_S--;
     }
     if (rest_pos <= num) {
        next_time = head[head_sort[rest_pos]];
        while (num_of_O > 0 && time < next_time) { 
           differenz = next_time - time;
           if (process[ set_O[num_of_O] ] <= differenz) {
              time += process[ set_O[num_of_O] ];
              new_low = time + tail[set_O[num_of_O]];
              max_low = (max_low < new_low)? new_low : max_low;
              process[ set_O[num_of_O--] ] = 0;
           }
           else {
              process[ set_O[num_of_O] ] -= differenz;
              break;
           }
        }
        time = next_time;
     }
     else  
       while (num_of_O > 0) {      
           time += process[ set_O[num_of_O] ];
           new_low = time + tail[set_O[num_of_O]];
           max_low = (max_low < new_low)? new_low : max_low;
           num_of_O--;
       }
  }
  return(max_low);
}


static boolean Select(int mach_no)
            /* Procedure for fixing all direct disjunctive arcs on mach_no */
{

  struct List  *help1, *help2;
  boolean      new_arcs = false;
  int          sum_process;
  register int Op1, Op2;

  help1 = OpsOnMachine[mach_no];
  while (help1 != NIL && help1->next != NIL) {
     Op1 = help1->number;
     help2 = help1->next;
     while (help2 != NIL) {
        Op2 = help2->number;
        sum_process = OpData[Op1].process_time + OpData[Op2].process_time;
        if (head[Op1] + sum_process + tail[Op2] >= UpperBound &&
             !Member(DisjArcs->succ[Op2], Op1)) {
           DisjArcs->succ[Op2] = Insert(DisjArcs->succ[Op2], Op1);
           DisjArcs->pred[Op1] = Insert(DisjArcs->pred[Op1], Op2);
           SonNode->num_of_pred[Op1]++;
           SonNode->num_of_succ[Op2]++;
           new_arcs = true;
        }
        if (head[Op2] + sum_process + tail[Op1] >= UpperBound &&
             !Member(DisjArcs->succ[Op1], Op2)) {     
           DisjArcs->succ[Op1] = Insert(DisjArcs->succ[Op1], Op2);
           DisjArcs->pred[Op2] = Insert(DisjArcs->pred[Op2], Op1);
           SonNode->num_of_pred[Op2]++;
           SonNode->num_of_succ[Op1]++;
           new_arcs = true;
        }
        help2 = help2->next;
     }
     help1 = help1->next;
  }
  return(new_arcs);
}


/* ************************************************************************* */
/*                     PROCEDURE     Additional_DisjArcs()                   */
/*                                                                           */
/*  INPUT:                                                                   */
/*                                                                           */
/*  FUNCTION: Fixation of additional disjunctive arcs due to Carlier and     */
/*            Pinson. The largest calculated lower bound is returned to      */
/*            the  main program.                                             */
/* ************************************************************************* */

int Additional_DisjArcs ()
{

  boolean  new_arcs = false;
  int      i, new_low,
           max_low = 0;

  for (i = 1; i <= NumOfMachines; ++i)
     if ( Select(i) ) new_arcs = true;
  if ( new_arcs && !Compute_Head_Tail() ) return(UpperBound);
  new_arcs = true;
  while (new_arcs) {
     new_arcs = false;
     for (i = 1; i <= NumOfMachines; ++i) {
        new_low = Jackson_Immediate_Selection(i, head, tail); /* primal arcs */
        if (new_low >= UpperBound) return(new_low);
        else if (new_low > max_low) max_low = new_low;
        if ( Select(i) )  new_arcs = true;
     }
     if ( new_arcs && !Compute_Head_Tail() ) return(UpperBound);
     for (i = 1; i <= NumOfMachines; ++i) {
        new_low = Jackson_Immediate_Selection(i, tail, head); /* dual arcs */
        if (new_low >= UpperBound) return(new_low);
        else if (new_low > max_low) max_low = new_low;
        if ( Select(i) ) new_arcs = true;
     }
     if ( new_arcs && !Compute_Head_Tail() ) return(UpperBound);
  }
  return(max_low);
}




