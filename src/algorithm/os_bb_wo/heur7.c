/* ************************************************************************ */
/*                           Filename: heur7.c                              */
/*                                                                          */
/*    Export: Procedure Heuristic_Schedule:                                 */
/*                      Berechnung einer heuristischen Loesung fuer den     */
/*                      Suchbaumknoten SonNode.                             */
/*									    */
/*         --> Heuristik auf der Basis einer Vorrangregel                   */
/*									    */
/* ************************************************************************ */

#include <stdio.h>
#include "data.h"
#include "table.h"
#include "list.h"
#include "low.h"
#include "heur.h"


static int    op_array[MaxOpProM_And_J+1],
	      temp_head[MaxNumOfOperations+1];	

static int LowerBoundCalc(machine_nr, num_of_pred, local_head, op)

int machine_nr;  
int num_of_pred[];  
int local_head[];  
int op;

/* Berechnung einer lower bound, wenn Op auf der Maschine als naechstes 
   eingeplant wird, vor allen anderen Op'en, die noch nicht auf der 
   Maschine eingeplant wurden.
   Entspricht dem Wert des JPS angewendet auf die Menge der Op'en, die 
   noch auf der Maschine eingeplant werden muessen, unter der Annahme, 
   dass op als erstes eingeplant wird.
*/
{

  struct List  *help;
  int          new_head, 
	       lower_bound;
  register int i = 0;

  new_head = local_head[op] + OpData[op].process_time;
  help = OpsOnMachine[machine_nr];
  while (help != NIL) 
  {
     if ( num_of_pred[help->number] >= 0 && help->number != op) 
     {
        if ( local_head[help->number] < new_head )
           temp_head[help->number] = new_head;
        else 
	   temp_head[help->number] = local_head[help->number];
        op_array[++i] = help->number;
     }
     help = help->next;
  }
  lower_bound = Preemptive_Schedule(op_array, i, temp_head);
  return( (lower_bound > Head[op] + OpData[op].process_time + Tail[op])?
          lower_bound : Head[op] + OpData[op].process_time + Tail[op]);
}


/* ************************************************************************* */
/*                         Procedure  Heuristic_Schedule()                   */
/*                                                                           */
/*  INPUT:  -/-                                                              */
/*                                                                           */
/*  FUNCTION: Berechnung einer heuristischen Loesung. Der kritische Pfad ist */
/*            in CriticalPath gespeichert. Verbessert die berechnete Fertig- */
/*            stellungszeit die aktuelle UpperBound UB, so wird UB auf-      */
/*            datiert. Die Loesung wird in der Datei output.dat gespeichert. */
/* ************************************************************************* */

void Heuristic_Schedule()
{

 static struct List  *maschlist[MaxNumOfMachines+1],
		     *joblist[MaxNumOfJobs+1];
 static int          num_of_pred[MaxNumOfOperations+1],
		     selection[MaxNumOfOperations+1], 
		     generate_set[MaxNumOfJobs+1],
		     genpos,
		     local_head[MaxNumOfOperations+1],
		     obtained[MaxNumOfOperations+1];



  struct List  *cut,
	       *help,
	       *updatelist;
  FILE         *fp;
  int          minreadytime,
  	       minreadytimemachine,	
	       newminreadytime,
	       selectpos = 1,
	       prior_op,
               low_for_prior_op,
               low_for_op,
	       c_max,
	       pos_path,
	       op, 
	       j,
	       sum,
	       path_critical[MaxNumOfOperations+1];

   register int i;


  /* cut ist die Liste der einplanbaren Operationen;
     updatelist ist die Menge der Operationen, deren Heads, aufgrund der
     Aenderungen der Heads ihrer Vorgaenger, aufdatiert werden muessen
  */
  cut = updatelist = NIL;


  /* Ermitteln der Vorgaengeranzahl und der Heads pro Operation,
     obtained[i] = j bedeutet: j wird vor i bearbeitet,
  */
  for ( i = 1; i <= NumOfOperations; ++i ) 
  {
     num_of_pred[i] = SonNode->num_of_disj_pred[i] +
                               SonNode->num_of_conj_pred[i];
     local_head[i] = Head[i];
     obtained[i] = 0;
  }


  /* Einmalig: cut initialisieren;
     Im cut werden alle Operationen aufgenommen, die keine echten 
     Vorgaenger besitzen;
     Spaeter wird cut (s. while-Schleife) mit geeigneten Nachfolgern seiner
     Op'en gefuellt.
  */
  for ( i = 1; i <= NumOfOperations; ++i )
  {
	if ( num_of_pred[i] == 0 )
	   cut = Insert(cut, i);
  }

/* ********************** Bestimmung der prior_op ************************** */

  /* Aus dem cut wird die Operation mit der kleinsten Fertigstellungszeit
     (MRT) ermittelt:
     Gibt es mehrere mit derselben Fertigst.zeit, so entscheidet der Tail;
  */
  while ( cut != NIL ) 
  {
     op = cut->number;
     minreadytime = local_head[op] + OpData[op].process_time;
     help = cut->next;
     while ( help != NIL )
     {
	newminreadytime = local_head[help->number] +
                                          OpData[help->number].process_time;
	if ( minreadytime >= newminreadytime ) 
        {
	   if ( minreadytime > newminreadytime ||
		   Tail[op] < Tail[help->number] ) 
           {
	      minreadytime = newminreadytime; 
	      op = help->number;
           }
        }
        help = help->next;
     }

     minreadytimemachine = OpData[op].machine_nr;


     /* generate_set ist die Menge der Operationen aus dem cut, die auf der
	MRTM bearbeitet werden und deren local_head < MRT
     */
     genpos = 1; 
     help = cut;
     while (help != NIL) 
     {
        if (OpData[help->number].machine_nr == minreadytimemachine
                             && local_head[help->number] < minreadytime)
           generate_set[genpos++] = help->number;
        help = help->next;
     }

     /* Besteht das generate_set aus (mind.) 2 Op'en, so wird fuer diese eine
	untere Schranke berechnet;
	die Op mit der kleinsten unteren Schranke wird zur prior_op
     */
     prior_op = generate_set[1];
     if ( genpos > 2 ) 
     {
        low_for_prior_op = LowerBoundCalc(minreadytimemachine,
                                        num_of_pred, local_head, prior_op);
        for ( i = 2; i < genpos; ++i )
	{
           low_for_op = LowerBoundCalc(minreadytimemachine, num_of_pred,
                                               local_head, generate_set[i]);
           if ( low_for_op < low_for_prior_op )
	   {
              low_for_prior_op = low_for_op;
              prior_op = generate_set[i];
           }
           else 
	   {
              if ( low_for_op == low_for_prior_op &&
                                 Tail[prior_op] < Tail[generate_set[i]] )
              prior_op = generate_set[i];
           }
        }
     }


     /* In selection werden die prior_op's eingetragen; jede Op wird ein-
	mal zur prior_op, so dass am Ende (cut=NIL) in selection eine 
	vollstaendige Selektion steht;

	prior_op wird aus dem cut geloescht;
     */	  
     selection[selectpos++] = prior_op;
     cut = Delete(cut, prior_op);
     num_of_pred[prior_op]--;           

     /* fuer die zuletzt eingefuegte Op kann auf die folgenden Abfragen ver-
	zichtet werden
     */
     if ( selectpos > NumOfJobs * NumOfMachines )
	break;


     /* Zunaechst muessen die local_heads der verbleibenden noch nicht 
	eingeplanten Operationen aufdatiert werden;
	dann:
	Bestimmung geeigneter Operationen, Nachfolger der prior_op, die in 
	den cut aufgenommen werden sollen;  
     */

     sum = local_head[prior_op] + OpData[prior_op].process_time;

     help = OpsOfJob[OpData[prior_op].job_nr];
     while ( help != NIL )
     {
	if (num_of_pred[help->number] >= 0 && local_head[help->number] <= sum) 
	{
	   if ( local_head[help->number] < sum ) 
	   {
	      local_head[help->number] = sum;
	      updatelist = Insert(updatelist, help->number); 
	      obtained[help->number] = prior_op;
	   }
	   else 
	   {
	      obtained[help->number] = prior_op;
	   }
	}
	help = help->next;
     }

     help = OpsOnMachine[OpData[prior_op].machine_nr];  
     while ( help != NIL ) 
     {
	if (num_of_pred[help->number] >= 0 && local_head[help->number] <= sum) 
	{
           if ( local_head[help->number] < sum ) 
           {
              local_head[help->number] = sum;
	      updatelist = Insert(updatelist, help->number);
	      obtained[help->number] = prior_op;
	   }
	   else 
	   {
	      obtained[help->number] = prior_op;
	   }
	}
	help = help->next;
     }


     
     help = ConjArcs->succ[prior_op];
     while ( help != NIL )
     {
	num_of_pred[help->number]--;
	if ( num_of_pred[help->number] == 0 )
	   cut = Insert(cut, help->number);
	help = help->next;
     }
	
     help = DisjArcs->succ[prior_op];  
     while ( help != NIL ) 
     {
        num_of_pred[help->number]--;
        if ( num_of_pred[help->number] == 0 ) 
	   cut = Insert(cut, help->number);
        help = help->next;
     }
     

     /* In updatelist sind die Operationen enthalten, deren local_head auf-
	datiert wurde. Das sind konj.Nachfolger der prior_op und Op'en, die
	auf derselben Maschine wie prior_op bearbeitete werden;
	Die konj. u. disj. Nachfolger dieser Op'en werden in updatelist auf-
	genommen, falls ihr local_head aufdatiert wird.
     */
     while ( updatelist != NIL ) 
     {   
	op = updatelist->number;
	updatelist = Delete(updatelist, op);
	sum = local_head[op] + OpData[op].process_time;
	help = ConjArcs->succ[op];
	while ( help != NIL ) 
	{
           if ( local_head[help->number] <= sum ) 
           {
              if ( local_head[help->number] < sum ) 
              {
                 local_head[help->number] = sum;
		 if ( !Member(updatelist, help->number) )
		    updatelist = Insert(updatelist, help->number);
		 obtained[help->number] = op;
	      }
	      else 
	      {
		 obtained[help->number] = op;
	      }
	   }
	   help = help->next;
	}

	help = DisjArcs->succ[op];
        while ( help != NIL ) 
        {
           if ( local_head[help->number] <= sum ) 
           {
	      if ( local_head[help->number] < sum ) 
              { 
                 local_head[help->number] = sum;
		 if ( !Member(updatelist, help->number) )
		    updatelist = Insert(updatelist, help->number);
		 obtained[help->number] = op;
	      }
	      else 
	      {
		 obtained[help->number] = op;
	      }
	   }
           help = help->next;
        }
     }                                  /* end  while (updatelist != NIL) .. */
  }                                            /* end  while (cut != NIL) .. */


/* ******************** Bestimmung des kritischen Pfades ******************* */

  /* Bestimmung des krit.Pfades - anhand der Selektion und obtained - und des
     Zielfunktionswertes c_max;
  */
  pos_path = 1;
  op = selection[--selectpos];
  path_critical[pos_path++] = op;
  c_max = OpData[op].process_time;
  while ( obtained[op] != 0 ) 
  {    
     op = obtained[op];
     path_critical[pos_path++] = op;
     c_max = c_max + OpData[op].process_time;
  }

  PathPos = pos_path;
  for ( i= 1; i < PathPos; ++i )
  {
     --pos_path;
     CriticalPath[i] = path_critical[pos_path];
  }  

/* **************************       Ausgabe       ************************** */

  /* Verbessert der gefundene Zielfunktionswert die bisher beste Loesung UB,
     -  so wird UB durch c_max aufdatiert,
     -  die Ausgabe aufbereitet: maschlist , joblist
     -  und die Job- u. Maschinenreihenfolgen in output.dat ausgegeben
  */
  if ( c_max < UpperBound ) 
  {
     UpperBound = c_max;       
     fp = fopen("output.dat","a");
     fprintf(fp,"Upper bound %d after %d search tree nodes \n",
			  UpperBound, SearchTreeNodes);

     fprintf(fp,"\n");                               

     /* Die Ausgabelisten maschlist, joblist werden initialisiert:
	  maschlist enthaelt die Job-Reihenfolgen auf den Maschinen,
	  joblist enthaelt die Maschinenreihenfolgen der Jobs
     */	
     for ( i = 0; i <= NumOfMachines; ++i ) 
	maschlist[i] = NIL; 
     for ( i = 0; i <= NumOfJobs; ++i )
	joblist[i] = NIL;

     /* Fuellen der Listen
     */
     for ( i = 1; i <= NumOfOperations; ++i ) 
	maschlist[ OpData[selection[i]].machine_nr ] =
	  Insert(maschlist[OpData[selection[i]].machine_nr], selection[i]);
     for ( i = 1; i <= NumOfOperations; ++i ) 
	joblist[ OpData[selection[i]].job_nr ] =
	  Insert(joblist[OpData[selection[i]].job_nr], selection[i]);

     /* Ausgabe der Daten in maschlist und joblist in die Datei output.dat
     */
     for ( i = 1; i <= NumOfMachines; ++i ) 
     {
        fprintf(fp,"MaschineNo %3d :  ",i);
	help = maschlist[i];
        while ( help != NIL ) 
        {
           fprintf(fp,"%3d ",help->number);
           help = help->next;
        }
        fprintf(fp,"\n");
     } 

     fprintf(fp," \n");                                

     for ( i = 1; i <= NumOfJobs; ++i ) 
     {
	fprintf(fp,"JobNo %3d :  ",i);
	help = joblist[i];
	while ( help != NIL ) 
	{
	   fprintf(fp,"%3d ",help->number);
	   help = help->next;
	}
	fprintf(fp,"\n");
     } 

     fprintf(fp,"\n");                                 

     /* Ausgabe des krit.Pfades
     */
     fprintf(fp,"Kritischer Pfad: \n");
     for ( i = 1; i < PathPos-1 ;++i )
     {
	 fprintf(fp,"%d  --> ", CriticalPath[i]);
     }
     fprintf(fp,"%d", CriticalPath[i]);

     fprintf(fp,"\n\n");                               

     /* Leeren der Listen: `zu jedem "malloc" ein "free" !!!'
     */
     for ( i = 0; i < NumOfMachines; ++i ) 
	maschlist[i] = Makeempty(maschlist[i]); 
     for ( i = 0; i < NumOfJobs; ++i ) 
	joblist[i] = Makeempty(joblist[i]); 

     /* Schliessen der Ausgabedatei output.dat, bzw. Freigabe des Zeigers;
     */
     fclose(fp); 
  }
}
