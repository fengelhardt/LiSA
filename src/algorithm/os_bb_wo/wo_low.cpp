/* ************************************************************************ */
/*                             Filename: Low.c                              */
/*                                                                          */
/* EXPORT: Procedure Compute_Lower_Bound:                                   */
/*                   Berechnung einer lower bound fuer SonNode              */
/*         Procedure Preemptive_Schedule: 				    */
/*                   Berechnung des Zielfunktionswertes eines optimalen     */
/*                   Planes mit Unterbrechung fuer eine Menge von Op'en auf */
/*                   einer Maschine bzw. von Op'en eines Jobs               */
/* ************************************************************************ */


#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_sort.hpp"
#include "wo_low.hpp"


/* ************************************************************************* */
/*                        Procedure   Preemptive_Schedule()                  */
/*                                                                           */
/*  INPUT:  op_array    Array von Operationen                                */
/*          num         Anzahl der Operationen in op_array                   */
/*          head        Array von Heads                                      */
/*                                                                           */
/*  FUNCTION: Berechnung des Zielfunktionswertes des 'Einmaschinen'-Problems */
/*            (n|1|r_i >= 0, q_i >= 0|preempt,prec|C_max) basierend auf den  */
/*            Op'en in op_array 					     */
/*            bzw.                                                           */
/*            Berechnung des Zielfunktionswertes des 'Einjob'-Problems       */
/*            (1|m|r_i >= 0, q_i >= 0|preempt,prec|C_max) basierend auf den  */
/*            Op'en in op_array 					     */
/* ************************************************************************* */

/* die Prozedur laeuft fuer die Op'en einer Maschine und fuer die Op'en
   eines Jobs: OpsOfJob bzw. OpsOnMachine werden in 'op_array' eingelesen;
*/  
int Preemptive_Schedule(int op_array[],int num,int head[]){

  int set_O[MaxOpProM_And_J+1],
      time, 
      next_time,
      process[MaxNumOfOperations+1],
      gap,
      max_low = 0, 
      new_low,  
      op, 
      dummy;

  register int i, 
	   rest_pos, 
	   num_of_O;

  Sort(head, op_array, num);
  num_of_O = 0;
  for ( rest_pos = 1; rest_pos <= num; ++rest_pos )
       process[op_array[rest_pos]] = OpData[op_array[rest_pos]].process_time;
  rest_pos = 1;
  time = head[ op_array[1] ];
  while ( rest_pos <= num ) 
  {   
     while ( rest_pos <= num && head[ op_array[rest_pos] ] == time ) 
     {
	 op = op_array[rest_pos];
	 for ( i = 1; i <= num_of_O; ++i ) 
	 {   
	    if ( Tail[op] < Tail[set_O[i]] ) 
	    {
		 dummy = set_O[i];
		 set_O[i] = op;
		 op = dummy;
            }
         }
	 set_O[++num_of_O] = op;
         rest_pos++;
     }
     if ( rest_pos <= num )
     {    
	next_time = head[ op_array[rest_pos] ];
	while ( num_of_O > 0 && time < next_time ) 
	{
	   gap = next_time - time;
	   if ( process[ set_O[num_of_O] ] <= gap ) 
	   {
	      time += process[ set_O[num_of_O] ];
	      new_low = time + Tail[ set_O[num_of_O] ];
              max_low = (max_low < new_low)? new_low : max_low;
              process[ set_O[num_of_O--] ] = 0;
           }
	   else 
	   {
	      process[ set_O[num_of_O] ] -= gap;
	      break;  
	   }
	}
	time = next_time;
     }
     else 
     {          
	while ( num_of_O > 0 ) 
	{
	   time += process[ set_O[num_of_O] ];
	   new_low = time + Tail[ set_O[num_of_O] ];
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
/*  FUNCTION: Berechnung einer lower bound fur SonNode durch die Loesung des */
/*            (n|1|r_i >= 0, q_i >= 0|preempt,prec|C_max) Problems fur alle  */
/*            Maschinen (= 'Einmaschinenproblem') und durch die Loesung des  */
/*            (1|m|r_i >= 0, q_i >= 0|preempt,prec|C_max) Problems fuer alle */
/*            Jobs (= 'Einjobproblem').                                      */
/*	      Der groesste Zf-wert wird an main zurueckgegeben		     */
/* ************************************************************************* */

int Compute_LowerBound ()
{

  int          i, 
	       j,
	       lb,
	       max_lower_bound = 0,   
	       max_lower_bound_j = 0,   
	       new_lower_bound,      
	       op_array[MaxOpProM_And_J+1]; 

  struct List  *help;

  /* Berechnung einer unteren Schranke durch (n|1|...)
     (Berechnung des JPS fuer alle Maschinen)
  */
  for ( i = 1; i <= NumOfMachines; ++i ) 
  {
     j = 0;
     help = OpsOnMachine[i];
     while ( help != NIL ) 
     {
	op_array[++j] = help->number;
	help = help->next;
     }
     if ( (new_lower_bound = Preemptive_Schedule(op_array, j, Head) )
            > max_lower_bound)  
	 max_lower_bound = new_lower_bound;

     /* ergibt sich bei der Berechnung der Schranken ein Wert, der die 
	UpperBound uebersteigt, so wird diese Prozedur abgebrochen, und der
	gerade berechnete Wert wird an das Hauptprogramm uebergeben.
     */
     if ( max_lower_bound >= UpperBound ) 
	return(max_lower_bound);
  }

  /* Berechnung einer unteren Schranke nach (1|m|...)
     (Berechnung des JPS fuer alle Jobs)
  */
  for ( i = 1; i <= NumOfJobs; ++i ) 
  {
     j = 0;
     help = OpsOfJob[i];
     while ( help != NIL ) 
     {
	op_array[++j] = help->number;
	help = help->next;
     }
     if ( (new_lower_bound = Preemptive_Schedule(op_array, j, Head) )
	    > max_lower_bound_j)  
	 max_lower_bound_j = new_lower_bound;

     /* ergibt sich bei der Berechnung der Schranken ein Wert, der die 
	UpperBound uebersteigt, so wird diese Prozedur abgebrochen, und der
	gerade berechnete Wert wird an das Hauptprogramm uebergeben.
     */
     if ( max_lower_bound_j >= UpperBound ) 
	return(max_lower_bound_j);
  }

  /* die groessere der beiden berechneten Schranken wird an das Haupt-
     programm zurueckgegeben
  */
  lb = ( (max_lower_bound > max_lower_bound_j)? 
			     max_lower_bound : max_lower_bound_j );           
  return(lb);
}
