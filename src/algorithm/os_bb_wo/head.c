/* ************************************************************************* */
/*                           Filename: Head.c                                */
/*                                                                           */
/*  Export: Procedure Compute_ Compute_Head_Tail:                            */
/*                    Berechnung der Heads und Tails basierend auf den fest- */
/*                    gelegten Kanten                                        */
/* ************************************************************************* */


#include "data.h"
#include "table.h"
#include "list.h"
#include "sort.h"
#include "head.h"




static void Compute_Head(actual_op)	
int actual_op;

{
  register struct List  *help;
  int op_array[MaxOpProM_And_J+1];
  register int i, 
	   num, 
	   new_c = 0,
           new_d = 0;


  help = ConjArcs->pred[actual_op]; 
  if (help != NIL)
  {
     num = 0;
     while (help != NIL)
     {
        ++num;
        op_array[num] = help->number;
        help = help->next;
     }
     Sort(Head, op_array, num);
     new_c = Head[op_array[1]] + OpData[op_array[1]].process_time;
     for (i = 2; i <= num; ++i)
     {
        new_c = (new_c < Head[op_array[i]])? Head[op_array[i]] : new_c;
        new_c += OpData[op_array[i]].process_time;
     }
  }
 

  help = DisjArcs->pred[actual_op]; 
  if (help != NIL) 
  {
     num = 0;
     while (help != NIL) 
     {
	++num;
	op_array[num] = help->number;
	help = help->next;
     }
     Sort(Head, op_array, num);
     new_d = Head[op_array[1]] + OpData[op_array[1]].process_time;
     for (i = 2; i <= num; ++i) 
     {
	new_d = (new_d < Head[op_array[i]])? Head[op_array[i]] : new_d;
	new_d += OpData[op_array[i]].process_time;
     }
  }
  Head[actual_op] = (new_c < new_d)? new_d : new_c;
}



static void Compute_Tail(actual_op)
int actual_op;

{
  register struct List  *help;
  int op_array[MaxOpProM_And_J+1];
  register int i, 
	   num, 
	   new_c = 0,
           new_d = 0;


  help = ConjArcs->succ[actual_op];
  if (help != NIL)
  {
     num = 0;
     while (help != NIL)
     {
        ++num;
        op_array[num] = help->number;
        help = help->next;
     }
     Sort(Tail, op_array, num);
     new_c =  OpData[op_array[1]].process_time + Tail[op_array[1]];
     for ( i = 2; i <= num; ++i )
     {
        new_c = (new_c < Tail[op_array[i]])? Tail[op_array[i]] : new_c;
        new_c += OpData[op_array[i]].process_time;
     }
  }
 

  help = DisjArcs->succ[actual_op];
  if (help != NIL) 
  {
     num = 0;
     while (help != NIL) 
     {
	++num;
	op_array[num] = help->number;
	help = help->next;
     }
     Sort(Tail, op_array, num);
     new_d =  OpData[op_array[1]].process_time + Tail[op_array[1]];
     for ( i = 2; i <= num; ++i ) 
     {
	new_d = (new_d < Tail[op_array[i]])? Tail[op_array[i]] : new_d;
        new_d += OpData[op_array[i]].process_time;
     }
  }
  Tail[actual_op] = (new_c < new_d)? new_d : new_c;
}



/* ************************************************************************* */
/*                      Procedure Compute_Head_Tail()                        */
/*                                                                           */
/*  INPUT: -/-                                                               */
/*                                                                           */
/*                                                                           */
/*  FUNCTION: Berechnung der Heads und Tails                                 */
/* ************************************************************************* */

boolean Compute_Head_Tail ()
{

  register struct List  *help;

  int          num_of_disj[MaxNumOfOperations+1],
	       num_of_pred[MaxNumOfOperations+1],
	       num_of_succ[MaxNumOfOperations+1],
	       pred_gesamt = 0,	
	       succ_gesamt = 0,
	       cut[MaxNumOfOperations+1],
	       max;

  /* cut[] enthaelt die Operationen bei deren Vorgaengern die Heads bereits
     berechnet sind , bzw.
     cut[] enthaelt die Operationen bei deren Nachfolgern die Tails bereits
     berechnet sind 
  */


  register int i, 
	   actual_op, 
	   pos;

  for ( i = 0; i <= NumOfOperations; ++i )
      num_of_disj[i] = 0;
  for ( i =  0; i <= NumOfOperations; ++i )
      cut[i] = 0;

  /* Die Anzahl der Vorgaenger einer Operation wird ermittelt;
  */
  for ( i = 1; i <= NumOfOperations; ++i )
  {
      num_of_pred[i] = num_of_disj[i] = SonNode->num_of_conj_pred[i] +
					SonNode->num_of_disj_pred[i];
      pred_gesamt += num_of_disj[i];
  }


  pos = 1;
  max = 0;

  /* Operationen 'ohne' Vorgaenger ermitteln:
     Die Op'en mit num_of_disj = 0, werden in den Array cut geschrieben.
     "cut" enthaelt die Op'en, deren Heads berechnet werden koennen, weil die
     Heads ihrer Vorgaenger bereits berechnet sind.
     In "init.c" werden Heads und Tails mit Null initialisiert, so dass der
     Head bzw. Tail erst dann von Null verschieden sein kann, wenn zu der Op
     eine Kante hin- bzw. wegfuehrt.
  */
  for ( i = 1; i <= NumOfOperations; ++i )
  {
     if ( num_of_disj[i] == 0 )
     {
	 cut[pos] = i;
	 pos++;
     }
  }

  /* Gibt es anfaenglich keine Op'en ohne Vorgaenger, so ist irgendwas falsch:
     Eine Selektion ohne Quelloperation enthaelt Zyklen !!
  */
  if ( pos == 1 )
     return(false);

  /* Wenn pos > 1, dann gibt es Op'en, deren Heads berechnet werden koennen
  */
  while (pos > 1) 
  {
     --pos;
     actual_op = cut[pos];

     /* der Head der aktuellen Op wird berechnet
     */
     Compute_Head(actual_op);

     /* max ist der groesste Fertigstellungszeitpunkt
     */
     if ( max < OpData[actual_op].process_time + Head[actual_op] )
	max = OpData[actual_op].process_time + Head[actual_op];


     /* Hat die akt.Op konj.Nachfolger, deren Heads berechnet werden 
	koennten ?
	Wenn ja: erniedrigen der Vorgaengerzahl bei diesen Nachfolgern;
		 gilt num_of_disj = 0, so wird die Op ins cut eingetragen;
		 erniedrigen der Gesamtvorgaengerzahl;
     */
     help = ConjArcs->succ[actual_op];
     while ( help != NIL )
     {
	num_of_disj[help->number]--;
	if ( num_of_disj[help->number] == 0 )
	{
	   cut[pos] = help->number;
	   pred_gesamt -= num_of_pred[help->number];
	   pos++;
	}
	help = help->next;
     }

     /* Hat die akt. Op disj.Nachfolger, deren Heads berechnet werden 
	koennten ? 
	Wenn ja: erniedrigen der Vorgaengerzahl bei diesen Nachfolgern;
		 gilt num_of_disj = 0, so wird die Op ins cut eingetragen;
		 erniedrigen der Gesamtvorgaengerzahl;
     */
     help = DisjArcs->succ[actual_op];              
     while (help != NIL) 
     {
	num_of_disj[help->number]--;
	if ( num_of_disj[help->number] == 0 )
	{
	   cut[pos] = help->number;
	   pred_gesamt -= num_of_pred[help->number];
	   pos++;
	}
	help = help->next;
     }
  }

  /* Gilt am Ende, dass die Gesamtvorgaengerzahl von Null verschieden ist,
     so enthaelt die Loesung Zyklen;
  */
  if ( pred_gesamt != 0  ||  max >= UpperBound )
     return(false);






  /* Die Anzahl der Nachfolger einer Operation wird ermittelt;
  */
  for ( i = 1; i <= NumOfOperations; ++i )
  {
     num_of_succ[i] =  num_of_disj[i] =  SonNode->num_of_conj_succ[i] +
					 SonNode->num_of_disj_succ[i];
     succ_gesamt += num_of_disj[i];
  }

  pos = 1;
  max = 0;

  /* Operationen 'ohne' Nachfolger ermitteln:
     Die Op'en mit num_of_disj = 0 in den Array cut schreiben.
     "cut" enthaelt die Op'en, deren Tails berechnet werden koennen, weil
     die Tails ihrer Nachfolger bereits berechnet sind.
     In "init.c" werden Heads und Tails mit Null initialisiert, so dass der
     Head bzw. Tail erst dann von Null verschieden sein kann, wenn zu der Op
     eine Kante hin- bzw. wegfuehrt.
  */
  for ( i = 1; i <= NumOfOperations; ++i )
  {
     if ( num_of_disj[i] == 0 )
     {
	cut[pos] = i;
	pos++;
     }
  }

  /* Gibt es anfaenglich keine Op'en ohne Nachfolger,so ist irgendwas falsch:
     Eine Selektion ohne Senkenoperation enthaelt Zyklen !!
  */
  if ( pos == 1 )
     return(false);

  /* Wenn pos > 1, dann gibt es Operationen, deren Tails berechnet werden
     koennen
  */
  while (pos > 1) 
  {
     --pos;
     actual_op = cut[pos];

     /* der Tail der aktuellen Op wird berechnet
     */
     Compute_Tail(actual_op);             
     
     if ( max < OpData[actual_op].process_time + Tail[actual_op] )
	max = OpData[actual_op].process_time + Tail[actual_op];
              

     /* Hat die akt.Op konj.Vorgaenger, deren Tails berechnet werden 
	koennten ?
	Wenn ja: erniedrigen der Nachfolgerzahl bei diesen Vorgaengern;
		 gilt num_of_disj = 0, so wird die Op ins cut eingetragen;
		 erniedrigen der Gesamtnachfolgerzahl;
     */
     help = ConjArcs->pred[actual_op];
     while ( help != NIL )
     {
	num_of_disj[help->number]--;
	if ( num_of_disj[help->number] == 0 )
	{
	   cut[pos] = help->number;
	   succ_gesamt -= num_of_succ[help->number];
	   pos++;
	}
	help = help->next;
     }

     /* Hat die akt. Op disj.Vorgaenger, deren Tails berechnet werden 
	koennten ? 
	Wenn ja: erniedrigen der Nachfolgerzahl bei diesen Vorgaengern;
		 gilt num_of_disj = 0, so wird die Op ins cut eingetragen;
		 erniedrigen der Gesamtnachfolgerzahl;
     */
     help = DisjArcs->pred[actual_op];
     while (help != NIL) 
     {
	num_of_disj[help->number]--;
	if ( num_of_disj[help->number] == 0 )
	{
	   cut[pos] = help->number;                                         
	   succ_gesamt -= num_of_succ[help->number];
	   pos++;
	}
	help = help->next;
     }
  }

  /* Gilt am Ende, dass die Gesamtnachfolgerzahl von Null verschieden ist,
     so enthaelt die Loesung Zyklen; (siehe oben).
  */
  if ( succ_gesamt != 0  ||  max >= UpperBound )
      return(false);

  return (true);
}
