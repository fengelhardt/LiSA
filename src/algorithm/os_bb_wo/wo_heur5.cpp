/* ************************************************************************* */
/*                           Filename: heur5.c                               */
/*                                                                           */
/* Export: Procedure Heuristic_Schedule:                                     */
/*         Berechnung einer heuristischen Loesung fuer den Suchbaumknoten    */
/*         SonNode.                                                          */
/*                                                                           */
/*         --> Heuristik auf der Basis eines Matchingproblems                */
/*             HIER: MODIFIZIERTES BOTTLENECK-MATCHINGPROBLEM / MINIMIERUNG  */ 
/* ************************************************************************* */

#include <stdio.h>
#include "wo_data.hpp"
#include "wo_calc.hpp"     
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_heur.hpp"

static struct arcs {
		      int s, t;
		      int weight;
		   } A[MaxNumOfJobs*MaxNumOfMachines+1];
static int mate[MaxNumOfJobs*MaxNumOfMachines+1];


/* loest modifiziertes Bottleneck-Zuordnungsproblem:
   bestimmt ein maximales Matching im bipartiten Graphen derart, dass       
   das maximale Kantengewicht minimal wird;
*/
void BottleMatch(num_S, num_T, num_A, A)

int  num_S; 
int  num_T;
int  num_A;
struct arcs A[];
{
   int     i, 
	   j, 
	   s, 
	   num_Q, 
	   Q[MaxNumOfJobs*MaxNumOfMachines+1], /*Q: Labels to be scanned*/
	   pi[MaxNumOfJobs*MaxNumOfMachines+1], 
	   W, 
	   maxpi, 
	   v;

   int label[MaxNumOfJobs*MaxNumOfMachines+1];

   boolean exposed[MaxNumOfJobs*MaxNumOfMachines+1];

   boolean augmented = true;

   W = MaxInt;
   for ( s = 1; s <= num_S + num_T; ++s ) 
   {
      mate[s] = 0;
      exposed[s] = true;
   }
   while (augmented) 
   {
      num_Q = 0;
      for ( s = 1; s <= num_S; ++s )
      {
	 if ( exposed[s] ) 
	 {
	    label[s] = 0;
	    Q[num_Q++] = s;
	 }
      }
      for ( s = num_S+1; s <= num_S + num_T; ++s ) 
	 pi[s] = -MaxInt;
      augmented = false;
      while ( num_Q && !augmented )
      {
	 for ( i = 0; i < num_Q; ++i )
	    if ( (Q[i] <= num_S) || (pi[Q[i]] >= W) ) 
	       break;
	 if ( i == num_Q )
	 {
	    maxpi = -MaxInt;
	    for ( i = num_S+1; i <= num_S + num_T; ++i )
	       if ( (pi[i] < W) && (pi[i] > maxpi) )
		  maxpi = pi[i];
	    W = maxpi;
	    for ( i = 0; i < num_Q; ++i )
	       if ( (Q[i] <= num_S) || (pi[Q[i]] >= W) ) 
		  break;
	 }
	 v = Q[i];
	 Q[i] = Q[--num_Q];
	 /* v in S */
	 if ( v <= num_S )
	 {
	    for ( i = 0; i < num_A; ++i ) 
	    {
	       if ( (A[i].s != v) || (mate[A[i].s] == A[i].t) )
		  continue;
	       if ( (pi[A[i].t] >= A[i].weight) || (pi[A[i].t] >= W) ) 
		  continue;
	       label[A[i].t] = v;
	       pi[A[i].t] = A[i].weight;
	       for ( j = 0; j < num_Q; ++j )
		  if ( Q[j] == A[i].t )
		     break;
	       if ( j == num_Q )
		  Q[num_Q++] = A[i].t;
	    }
	 }
	 else 
	 { 
	    /* v in T */
	    if ( exposed[v] )
	    {
	       exposed[v] = false;
	       while ( label[v] ) 
	       {
		  mate[v] = label[v];
		  mate[label[v]] = v;
		  v = label[v];
		  if ( !label[v] )
		     break;
		  v = label[v];
	       }
	       exposed[v] = false;

	       augmented = true;
	    }
	    else 
	    {
	       label[mate[v]] = v;
	       for ( j = 0; j < num_Q; ++j )
		  if ( Q[j] == mate[v] )
		     break;
	       if ( j == num_Q )
		  Q[num_Q++] = mate[v];
	    }
	 }
      }
   }
}



/* ************************************************************************* */
/*                        Procedure Heuristic_Schedule()                     */
/*                                                                           */
/* INPUT: -/-                                                                */
/*                                                                           */
/* FUNCTION: Berechnung eines heuristischen Loesung. Der kritische Pfad ist  */
/*           in CriticalPath gespeichert. Verbessert die berechnete Fertig-  */
/*           stellungszeit die aktuelle UpperBound UB, so wird UB auf-       */
/*           datiert. Die Loesung wird in der Datei output.dat gespeichert   */
/* ************************************************************************* */ 

static struct List *joblist[MaxNumOfJobs+1],          /* Ausgabelisten    */
		   *maschlist[MaxNumOfMachines+1];

static int local_head[MaxNumOfOperations+1],
	   num_of_pred[MaxNumOfOperations+1],
	   num_of_succ[MaxNumOfOperations+1],
	   obtained[MaxNumOfOperations+1];



void Heuristic_Schedule()

{
   struct List *cut,                             /* Op'en e-s Matchings */
	       *updatelist,	                 /* local_heads aufdatieren*/
	       *help,
	       *help1;
   FILE        *fp;       
   int         i, 
	       j,
	       k,
	       readytime,                         /* Fertigstellungszeit    */
	       c_max,
	       sum,
	       prior_op,                          /* Op'en fuer CriticalPath*/
	       op,
	       iter,                              /* zaehlt betrachtete Open*/
               pos_path,
               path_critical[MaxNumOfOperations+1];


   cut = updatelist = NIL;

   for ( i = 0; i <= NumOfJobs; ++i )
      joblist[i] = NIL;
   for ( i = 0; i <= NumOfMachines; ++ i )
      maschlist[i] = NIL;

   iter = 0;

   for ( i = 1; i <= NumOfOperations; ++i )
   {
      num_of_pred[i] = SonNode->num_of_conj_pred[i] +
		       SonNode->num_of_disj_pred[i];
      num_of_succ[i] = SonNode->num_of_conj_succ[i] +
		       SonNode->num_of_disj_succ[i];
      local_head[i] = Head[i];
      obtained[i] = 0;
   }

   /* Besonderheit: BottleMatch ist so implementiert, dass die erste Maschine
      bei NumOfJobs+1 beginnt (z.B.:2 Jobs, 3 Maschinen --> 1,2 fuer Jobs,
      3,4,5 fuer Maschinen);
      wird spaeter wieder rueckgaengig gemacht, wenn die Zuordnung berechnet
      ist (s.u. j = mate[i])
   */

   /* Diese while-Schleife bricht ab, wenn alle Op'en betrachtet wurden
      ( iter = NumOfOperations ). Jedesmal, wenn eine Op in cut eingefuegt 
      wird, wird iter um eins erhoeht.
   */
   while ( iter < NumOfOperations )
   {
      /* Einlesen der Zeitmatrix :
	 Op'en, mit num_of_pred <> 0 muessen fuers Matching ausgeschlossen
	 werden;
      */
      sum = 1;
      for ( i = 0; i < NumOfOperations; ++i )
      {
	 /* hier:
	    Kantengewichte = pos.(Bearbeitungszeiten + Head), 
	    falls num_of_pred = 0,
	 */
	 if ( num_of_pred[sum] == 0 )
	 {
	    A[i].s = OpData[sum].job_nr;
	    A[i].t = OpData[sum].machine_nr + NumOfJobs;
	    A[i].weight = OpData[sum].process_time + Head[sum];
	    sum++;
	 }
	 else
	 {
	    sum++;
	    --i;
	 }
      }



      /* Loesen des Matchingproblems
      */
      BottleMatch(NumOfJobs, NumOfMachines, NumOfOperations, A);
      for ( i = 1; i <= NumOfJobs; i++ )
      {
	 if ( mate[i] != 0 )
	 {
	    j = mate[i];
	    /* (s.o "Rueckgaengigmachen") Maschinennummer fuer Maschine n 
	       ist wieder n
	    */
	    k = j - NumOfJobs;
	    op = FindOpNr(i,k);

	    if ( num_of_pred[op] == 0 )
	    {
	       cut = Insert(cut,op);

	       /* Op'en des Matchings werden fuer's folgende ausgeschlossen
	       */
	       num_of_pred[op]--;
	       iter++;

	       /* konj./disj. Nachfolger der Op'en des Matchings werden fuer 
		  folgendes zugelassen, wenn die Anzahl ihrer Vorgaenger auf
		  Null reduziert wird.
	       */
	       help = ConjArcs->succ[op];
	       while ( help != NIL )
	       {
		  num_of_pred[help->number]--;
		  help = help->next;
	       }

	       help = DisjArcs->succ[op];
	       while ( help != NIL )
	       {
		  num_of_pred[help->number]--;
		  help = help->next;
	       }
	    }
	 }
      }

      help = cut;
      while ( help != NIL )
      /* 1. Die Op'en des cut werden in die Job- u. Maschinenreihenfolgen
	    eingereiht.
	 2. Aufdatieren:
	    bei Op'en, die nach den Op'en des Matchings bearbeitet werden,
	    muessen die Heads aufdatiert werden ( --> updatelist ):
      */
      {
	 op = help->number;
	 joblist[OpData[op].job_nr] = 
			      Insert(joblist[OpData[op].job_nr],op); 
	 maschlist[OpData[op].machine_nr] =
			      Insert(maschlist[OpData[op].machine_nr],op);


	 readytime = local_head[op] + OpData[op].process_time;

	 help1 = OpsOfJob[OpData[op].job_nr];
	 while ( help1 != NIL )
	 {
	    if ( num_of_pred[help1->number] >= 0 )
	       if ( local_head[help1->number] <= readytime )
	       {
		  if ( local_head[help1->number] < readytime )
		  {
		     obtained[help1->number] = op;   
		     local_head[help1->number] = readytime;
		     if ( !Member(updatelist, help1->number) )
			updatelist = Insert(updatelist, help1->number);
		  }
		  else
		     obtained[help1->number] = op;
		  
		  /* eine Op mit num_of_succ = 0, hat weder heuristische
		     noch echte Nachfolger; bei einer dieser Op'en endet
		     der krit.Pfad.
		  */
		  num_of_succ[op]++;         
	       }
	       else
		  num_of_succ[op]++;
	    help1 = help1->next;
	 }

	 help1 = OpsOnMachine[OpData[op].machine_nr];
	 while ( help1 != NIL )
	 {
	    if ( num_of_pred[help1->number] >= 0 )
	       if ( local_head[help1->number] <= readytime )
	       {
		  if ( local_head[help1->number] < readytime )
		  {
		     obtained[help1->number] = op;
		     local_head[help1->number] = readytime;
		     if ( !Member(updatelist, help1->number) )
			updatelist = Insert(updatelist, help1->number);
		  }
		  else
		     obtained[help1->number] = op;   

		  /* eine Op mit num_of_succ = 0, hat weder heuristische
		     noch echte Nachfolger; bei einer dieser Op'en endet
		     der krit.Pfad.
		  */
		  num_of_succ[op]++;
	       }
	       else
		  num_of_succ[op]++;
	    help1 = help1->next;
	 }
	 help = help->next;
      }

      cut = Makeempty(cut);


      while ( updatelist != NIL )
      {
	 op = updatelist->number;
	 updatelist = Delete(updatelist, op);
	 readytime = local_head[op] + OpData[op].process_time;
	 help = ConjArcs->succ[op];
	 while ( help != NIL ) 
	 {
	    if ( local_head[help->number] <= readytime ) 
	    {
	       if ( local_head[help->number] < readytime )
	       {
		  obtained[help->number] = op;
		  local_head[help->number] = readytime;
		  if ( !Member( updatelist, help->number ) )
		     updatelist = Insert(updatelist, help->number);
	       }
	       else
		  obtained[help->number] = op;   
	    }
	    help = help->next;
	 }

	 help = DisjArcs->succ[op];
	 while ( help != NIL ) 
	 {
	    if ( local_head[help->number] <= readytime )
	    {
	       if ( local_head[help->number] < readytime )
	       {
		  obtained[help->number] = op;
		  local_head[help->number] = readytime;
		  if ( !Member( updatelist, help->number ) )
		     updatelist = Insert(updatelist, help->number);
	       }
	       else
		  obtained[help->number] = op;   
	    }
	    help = help->next;
	 }
      }
   }

   /* Ermitteln des krit.Pfades:
   */

   /* Op'en, die keine echten bzw. heurist. Nachfolger haben, werden als 
      letzte in die Job- und Maschinenlisten eingereiht;
      Gibt es mehrere, so wird die Op mit dem groessten Fertigstellungszeit
      End-Op des krit.Pfades.
   */
   for ( i = 1; i <= NumOfOperations; ++i )
      if ( num_of_succ[i] == 0 )
	cut = Insert(cut, i);


   while ( cut != NIL )
   { 
      op = cut->number;
      help = cut->next;
      while ( help != NIL )
      {
	 if ( local_head[help->number] + OpData[help->number].process_time 
	      >= local_head[op] + OpData[op].process_time )
	    op = help->number;

	 help = help->next;
      }	
      cut = Makeempty(cut);
   }

   pos_path = 1;
   path_critical[pos_path++] = op;

   c_max = OpData[op].process_time;
   while ( obtained[op] != 0 )
   {
      op = obtained[op];
      path_critical[pos_path++] = op;
      c_max = c_max + OpData[op].process_time;
   }

   /* path_critical() enthaelt die Op'en noch in umgekehrter Reihenfolge
      (denn CP wurde vorher mit 'InsertBefore' gefuellt, aber an der restl.
      Prozedur wurde nichts geaendert). Es wird ja bei der letzten Op be-
      gonnen und ueber obtained die erste Op erreicht. Darum muss path_c
      noch einmal andersherum in CriticalPath gelesen werden.
   */

   PathPos = pos_path;

   for ( i = 1; i < PathPos; ++i )
   {
      --pos_path;  
      CriticalPath[i] = path_critical[pos_path];
   }

   /* falls der gefundene Zielfunktionswert die vorlaeufig beste Loesung,
      UpperBound, verbessert, wird diese aufdatiert und die zugehoerige
      Selektion mit Angabe des aktuellen Suchbaumknotens in "output.dat" 
      zur Verfuegung gestellt.
   */
      
   if ( c_max < UpperBound )
   {   
      UpperBound = c_max;       

      fp = fopen("output.dat","a");
      fprintf(fp,"Upper Bound %d after %d search tree nodes\n",
					    UpperBound, SearchTreeNodes);

      fprintf(fp,"\n");                              

      /* Ausgabe der Jobreihenfolgen 
      */
      for ( i = 1; i <= NumOfJobs; ++i )
      {
	 fprintf(fp,"JobNo %3d :  ",i);
	 help = joblist[i];
	 while ( help != NIL )
	 {
	    fprintf(fp,"%3d ", help->number);
	    help = help->next;
	 }
	 fprintf(fp,"\n");
      }
      fprintf(fp,"\n");                                
 

      /* Ausgabe der Maschinenreihenfolgen 
      */
      for ( i = 1; i <= NumOfMachines; ++i )
      {
	 fprintf(fp,"MaschineNo %3d :  ",i);
	 help = maschlist[i];
	 while ( help != NIL)
	 {
	    fprintf(fp,"%3d ", help->number);
	    help = help->next;
	 }
	 fprintf(fp,"\n");
      }
      fprintf(fp,"\n");                                

      /* Ausgabe des krit.Pfades
      */
      fprintf(fp,"Kritischer Pfad: \n");
      for ( i = 1; i < PathPos - 1; ++i )
      {
	 fprintf(fp,"%d  --> ", CriticalPath[i]);
      }
      fprintf(fp,"%d", CriticalPath[i]);

      fprintf(fp,"\n\n");                              


      fclose(fp);
   } 

   /* Leeren der Listen: 'zu jedem "malloc" ein "free" !!!`
   */
   for ( i = 0; i <= NumOfJobs; ++i )
      joblist[i] = Makeempty(joblist[i]);
   for ( i = 0; i <= NumOfMachines; ++i )
      maschlist[i] = Makeempty(maschlist[i]);
}
