/* ************************************************************************* */
/*                           Filename: Select.c                              */
/*                                                                           */
/*  EXPORT: Procedure Additional_Arcs:                                       */
/*                    Festlegen zusaetzlicher Maschinen- und  Jobkanten nach */
/*		      Carlier und Pinson  				     */
/* ************************************************************************* */


#include "data.h"
#include "table.h"
#include "list.h"
#include "head.h"
#include "sort.h"
#include "select.h"


static int Jackson_Immediate_Selection(number, Head, Tail, m_or_j)	

int  number,
     Head[],
     Tail[];
char m_or_j;

/* Loesung des Primal- oder Dualproblems fuer alle Operationen auf Maschine 
   'number' bzw. fuer alle Operationen von Job 'number';
*/

{
  int           set_O[MaxOpProM_And_J+1], 
		num_of_O, 
		set_S[MaxOpProM_And_J+1], 
		num_of_S, 
		tail_sort[MaxOpProM_And_J+1],  
		head_sort[MaxOpProM_And_J+1],  
		process[MaxNumOfOperations+1],
		tausch, 
		next_time, 
		time, 
		sum_processtime,
		rest_pos, 
		old_pos,   
		differenz, 
		max_low = 0, 
		new_low;

  register int i, 
	       op, 
	       num;
  boolean      computed[MaxNumOfOperations+1];
  struct List  *help;

  num = 0;

  if ( m_or_j == 'm' )
     help = OpsOnMachine[number];
  else
     help = OpsOfJob[number];

  while ( help != NIL ) 
  {
     ++num;
     process[help->number] = OpData[help->number].process_time;
     head_sort[num] = tail_sort[num] = help->number;
     help = help->next;
  }
  Sort( Head, head_sort, num );
  Sort( Tail, tail_sort, num );
  num_of_S = num_of_O = 0;
  time = Head[head_sort[1]];
  rest_pos = 1;
  while ( rest_pos <= num ) 
  {
     old_pos = rest_pos;
     while ( rest_pos <= num && Head[head_sort[rest_pos]] == time ) 
     {
	set_S[++num_of_S] = head_sort[rest_pos];
	rest_pos++;
     }
     while ( num_of_S > 0 )  
     {     
	op = set_S[num_of_S];
	sum_processtime = Head[op] + process[op];

	if ( m_or_j == 'm' )
	   help = OpsOnMachine[number];
	else
	   help = OpsOfJob[number];

	while ( help != NIL ) 
	{
	   if ( process[help->number] != 0 && Tail[help->number] > Tail[op] )
	   {
	      sum_processtime += process[help->number];
	      computed[help->number] = true;
	   }
	   else 
	      computed[help->number] = false;
	   help = help->next;
	}       
	i = 1;
	while ( i <= num ) 
	{  
	   if ( computed[tail_sort[i]] ) 
	   {
	      if ( sum_processtime + Tail[tail_sort[i]] >= UpperBound ) 
		 break;
	      else 
	      {
		 computed[tail_sort[i]] = false,
		 sum_processtime -= process[tail_sort[i]];
              }
           }
           i++;
        } 
	if ( i <= num ) 
	{
	   for ( i = 1; i <= num; ++i ) 
	   {
	      if ( computed[head_sort[i]] ) 
	      {
		 if ( Head[op] < Head[head_sort[i]] )
		    Head[op] = Head[head_sort[i]];
		 Head[op] += process[head_sort[i]];
	      }
	   }
	   rest_pos--;         
	   for ( i = old_pos; i < num; ++i ) 
	   {
	      if ( Head[head_sort[i]] > Head[head_sort[i+1]] ) 
	      {
		 tausch = head_sort[i];
		 head_sort[i] = head_sort[i+1];
		 head_sort[i+1] = tausch;
	      }
	   }
	}
	else 
	{
	   for ( i = 1; i <= num_of_O; ++i ) 
	   {
	      if ( Tail[op] < Tail[set_O[i]] ) 
	      {
		 tausch = set_O[i];
		 set_O[i] = op;
		 op = tausch;
	      }
	   }
	   set_O[++num_of_O] = op;
	}
	num_of_S--;
     }
     if ( rest_pos <= num ) 
     {
	next_time = Head[head_sort[rest_pos]];
	while ( num_of_O > 0 && time < next_time ) 
	{
	   differenz = next_time - time;
	   if ( process[ set_O[num_of_O] ] <= differenz ) 
	   {
	      time += process[ set_O[num_of_O] ];
	      new_low = time + Tail[set_O[num_of_O]];
	      max_low = (max_low < new_low)? new_low : max_low;
	      process[ set_O[num_of_O--] ] = 0;
	   }
	   else 
	   {
	      process[ set_O[num_of_O] ] -= differenz;
	      break;
	   }
	}
	time = next_time;
     }
     else  
	while ( num_of_O > 0 ) 
	{
	   time += process[ set_O[num_of_O] ];
	   new_low = time + Tail[set_O[num_of_O]];
	   max_low = (max_low < new_low)? new_low : max_low;
	   num_of_O--;
	}
  }
  return(max_low);
}




static boolean Select(number, m_or_j)	

/* number ist entweder Maschinen- oder Jobnummer
*/
int number;
char m_or_j;

/* Prozedur zum Festlegen aller direkten Kanten auf Nummer
   (entweder Maschinen- oder Jobnummer)
*/

{

  struct List  *help1, 
	       *help2;
  boolean      new_arcs = false;
  int          sum_process;
  register int Op1, 
	       Op2;

  if ( m_or_j == 'm' )
     help1 = OpsOnMachine[number];
  else
     help1 = OpsOfJob[number];

  while ( help1 != NIL && help1->next != NIL ) 
  {
     Op1 = help1->number;
     help2 = help1->next;
     while ( help2 != NIL ) 
     {
	Op2 = help2->number;
	sum_process = OpData[Op1].process_time + OpData[Op2].process_time;
	if ( m_or_j == 'm' )
	{
	   if ( Head[Op1] + sum_process + Tail[Op2] >= UpperBound &&
	      !Member(DisjArcs->succ[Op2], Op1)) 
	   {
	      DisjArcs->succ[Op2] = Insert(DisjArcs->succ[Op2], Op1);
	      DisjArcs->pred[Op1] = Insert(DisjArcs->pred[Op1], Op2);
	      SonNode->num_of_disj_pred[Op1]++;
	      SonNode->num_of_disj_succ[Op2]++;
	      new_arcs = true;
	   }
	   if ( Head[Op2] + sum_process + Tail[Op1] >= UpperBound &&
	      !Member(DisjArcs->succ[Op1], Op2)) 
	   {
	      DisjArcs->succ[Op1] = Insert(DisjArcs->succ[Op1], Op2);
	      DisjArcs->pred[Op2] = Insert(DisjArcs->pred[Op2], Op1);
	      SonNode->num_of_disj_pred[Op2]++;
	      SonNode->num_of_disj_succ[Op1]++;
	      new_arcs = true;
	   }
	}
	else
	{
	   if ( Head[Op1] + sum_process + Tail[Op2] >= UpperBound &&
	      !Member(ConjArcs->succ[Op2], Op1)) 
	   {
	      ConjArcs->succ[Op2] = Insert(ConjArcs->succ[Op2], Op1);
	      ConjArcs->pred[Op1] = Insert(ConjArcs->pred[Op1], Op2);
	      SonNode->num_of_conj_pred[Op1]++;
	      SonNode->num_of_conj_succ[Op2]++;
	      new_arcs = true;
	   }
	   if ( Head[Op2] + sum_process + Tail[Op1] >= UpperBound &&
	      !Member(ConjArcs->succ[Op1], Op2)) 
	   {
	      ConjArcs->succ[Op1] = Insert(ConjArcs->succ[Op1], Op2);
	      ConjArcs->pred[Op2] = Insert(ConjArcs->pred[Op2], Op1);
	      SonNode->num_of_conj_pred[Op2]++;
	      SonNode->num_of_conj_succ[Op1]++;
	      new_arcs = true;
	   }
	}
	help2 = help2->next;
     }
     help1 = help1->next;
  }
  return(new_arcs);
}


/* ************************************************************************* */
/*                       PROCEDURE   Additional_Arcs()                       */
/*                                                                           */
/*  INPUT: -/-                                                               */
/*                                                                           */
/*  FUNCTION: Festlegen weiterer Job- u. Maschinenkanten nach Carlier und    */
/*            Pinson. Die groesste berechnete lower bound wird an das Haupt- */
/*            programm (main.c) zurueckgegeben.                              */
/* ************************************************************************* */

int Additional_Arcs ()
{

  boolean  new_arcs = false;
  int      i, 
	   new_low,
	   max_low = 0,
	   max_low_j = 0;
  char     m_or_j;

  m_or_j = 'm';
  for ( i = 1; i <= NumOfMachines; ++i )
     if ( Select(i, m_or_j) ) 
	new_arcs = true;
  if ( new_arcs && !Compute_Head_Tail() ) 
     return(UpperBound);
  new_arcs = true;
  while ( new_arcs ) 
  {
     new_arcs = false;
     for ( i = 1; i <= NumOfMachines; ++i ) 
     {
	/* Primal-Kanten
	*/
	new_low = Jackson_Immediate_Selection(i, Head, Tail, m_or_j); 
	if ( new_low >= UpperBound )
	   return(new_low);
	else 
	   if ( new_low > max_low )
	      max_low = new_low;
	if ( Select(i, m_or_j) )  
	     new_arcs = true;
     }
     if ( new_arcs && !Compute_Head_Tail() ) 
	return(UpperBound);
     else 
	new_arcs = false;
     for ( i = 1; i <= NumOfMachines; ++i ) 
     {
	/* Dual-Kanten
	*/
	new_low = Jackson_Immediate_Selection(i, Tail, Head, m_or_j);
	if ( new_low >= UpperBound )
	   return(new_low);
	else 
	   if ( new_low > max_low )
	      max_low = new_low;
	if ( Select(i, m_or_j) ) 
	   new_arcs = true;
     }
     if ( new_arcs && !Compute_Head_Tail() ) 
	return(UpperBound);
  }


  new_arcs = false;


  m_or_j = 'j';
  for ( i = 1; i <= NumOfJobs; ++i )
     if ( Select(i, m_or_j) ) 
	new_arcs = true;
  if ( new_arcs && !Compute_Head_Tail() ) 
     return(UpperBound);

  new_arcs = true;
  while ( new_arcs ) 
  {
     new_arcs = false;
     for ( i = 1; i <= NumOfJobs; ++i ) 
     {
	/* Primal-Kanten
	*/
	new_low = Jackson_Immediate_Selection(i, Head, Tail, m_or_j);
	if ( new_low >= UpperBound )
	   return(new_low);
	else 
	   if ( new_low > max_low_j )
	      max_low_j = new_low;
	if ( Select(i, m_or_j) )  
	     new_arcs = true;
     }
     if ( new_arcs && !Compute_Head_Tail() ) 
	return(UpperBound);
     else 
	new_arcs = false;
     for ( i = 1; i <= NumOfJobs; ++i ) 
     {
	/* Dual-Kanten
	*/
	new_low = Jackson_Immediate_Selection(i, Tail, Head, m_or_j); 
	if ( new_low >= UpperBound )
	   return(new_low);
	else 
	   if ( new_low > max_low_j )
	      max_low_j = new_low;
	if ( Select(i, m_or_j) ) 
	   new_arcs = true;
     }
     if ( new_arcs && !Compute_Head_Tail() ) 
	return(UpperBound);
  } 
  return( (max_low > max_low_j)? max_low : max_low_j );
}
