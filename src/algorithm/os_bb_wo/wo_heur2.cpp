/* ************************************************************************ */
/*                            Filename: heur2.c                             */
/*                                                                          */
/* Export: Procedure Heuristic_Schedule:                                    */
/*         Berechnung einer heuristischen Loesung fuer den Suchbaumknoten   */
/*         SonNode.                                                         */
/*									    */
/*         --> Heuristik auf der Basis eines Matchingproblems:              */
/*             HIER: SUMMENZUORDNUNGSPROBLEM/MAXIMIERUNG                    */
/* ************************************************************************ */

#include <stdio.h>
#include "wo_data.hpp"
#include "wo_calc.hpp"     
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_heur.hpp"


/* Bestimmung eines Matchings mit der Eigenschaft, dass die Summe ueber 
   die Kantengewichte im zugehoerigen bipartiten Graphen maximiert wird

   Algorithmus: siehe WALTHER/NAEGLER: GRAPHEN-ALGORITHMEN-PROGRAMME, S.131
*/
void WeightMatch(n, c, mate)    int n;
				int c[MaxNumOfJobs+1][MaxNumOfMachines+1];
				int mate[];
{
   int     i, j, k, min, t, z, aim, 
	   rowan[MaxNumOfJobs+1], 
	   colan[MaxNumOfMachines+1],
	   colmate[MaxNumOfMachines+1], 
	   number[2*MaxNumOfJobs+1];
   boolean broken, 
	   ready = false;


   for ( i = 1; i <= n; ++i ) 
      mate[i] = colmate[i] = 0;
   while ( !ready ) 
   {
      z = 0;
      for ( i = 1; i <= n; ++i ) 
      {
	 rowan[i] = colan[i] = number[2*i-1] = number[2*i] = 0;
	 if (mate[i] == 0) 
	 {
	    number[++z] = i;
	    rowan[i] = n+1;
	 }
      }
      if ( z > 0 ) 
      {
	 broken = false;
	 t = 1;
	 while ( (number[t] != 0) && !broken ) 
	 {
	    j = number[t++];
	    if ( j > 0 )
	       for ( k = 1; k <= n; ++k ) 
	       {
		  if ( (c[j][k] == 0) && (colan[k] == 0) )
		  {
		     colan[k] = j;
		     number[++z] = -k;
		     if ( colmate[k] == 0 ) 
		     {
			broken = true;
			aim = k;
		     }
		  }
	       }
	    else 
	    {
	       j = -j;
	       i = colmate[j];
	       if ( (c[i][j] == 0) && (rowan[i] == 0) ) 
	       {
		  rowan[i] = j;
		  number[++z] = i;
	       }
	    }
	 }
	 if ( broken ) 
	 {
	    j = aim;
	    i = colan[j];
	    colmate[j] = i;
	    mate[i] = j;
	    while (rowan[i] != n+1) 
	    {
	       j = rowan[i];
	       i = colan[j];
	       mate[i] = j;
	       colmate[j] = i;
	    }
	 }
	 else 
	 {
	    min = MaxInt;
	    for ( i = 1; i <= n; ++i )
	       for ( j = 1; j <= n; ++j )
		  if ( (rowan[i] > 0) && (colan[j] == 0) && (c[i][j] < min) )
		     min = c[i][j];
	    for ( i = 1; i <= n; ++i )
	       if ( rowan[i] != 0 )
		  for ( j = 1; j <= n; ++j ) 
		      c[i][j] -= min;
	    for ( j = 1; j <= n; ++j )
	       if ( colan[j] != 0 )
		  for ( i = 1; i <= n; ++i ) 
		     c[i][j] += min;
	 }
      }
      else ready = true;
   }
}


/* ************************************************************************* */
/*                        Procedure Heuristic_Schedule()                     */
/*                                                                           */
/* INPUT: -/-                                                                */
/*                                                                           */
/* FUNCTION: Berechnung einer heuristischen Loesung.  Der kritische Pfad ist */
/*           in CriticalPath gespeichert. Verbessert die berechnete Fertig-  */
/*           stellungszeit die aktuelle UpperBound UB, so wird UB auf-       */
/*           datiert. Die Loesung wird in der Datei output.dat gespeichert   */
/* ************************************************************************* */ 

static struct List *joblist[MaxNumOfJobs+1],          /* Ausgabelisten      */
		   *maschlist[MaxNumOfMachines+1];

static int local_head[MaxNumOfOperations+1],
	   num_of_pred[MaxNumOfOperations+1],
	   num_of_succ[MaxNumOfOperations+1],
	   obtained[MaxNumOfOperations+1];


void Heuristic_Schedule()

{
   struct List *cut,                                /*Op'en eines Matchings*/
	       *updatelist,	                    /*local_heads aufdatieren */
	       *help,
	       *help1;
   FILE        *fp;       
   int         i, 
	       n, 
	       j,
	       dif1,
	       dif2,
	       readytime,                           /* Fertigstellungszeit    */
	       c_max,
	       sum,
	       prior_op,                            /* Op'en fuer CriticalPath*/
	       op,
	       pos_path,                            /* lok. fuer PathPos */
	       iter,                                /*zaehlt betrachtete Op'en*/
	       mate[MaxNumOfJobs+1],                /* Zuordnung              */
	       c[MaxNumOfJobs+1][MaxNumOfMachines+1],
	       path_critical[MaxNumOfOperations+1]; /*darin steht krit.Pfad erst 
						      in umgek. Reihenfolge*/		


   cut = updatelist = NIL;

   for ( i = 0; i <= NumOfJobs; ++i )
      joblist[i] = NIL;
   for ( i = 0; i <= NumOfMachines; ++ i )
      maschlist[i] = NIL;

   n = Max(NumOfJobs,NumOfMachines);
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

   /* Bei einem nichtquadrat. Problem wird die Zeitmatrix durch (m-n) 
      zusaetzliche Zeilen bzw. (n-m) zusaetzliche Spalten, falls n<m bzw.
      m<n zu einem quadrat. Problem erweitert. Die neuen Matrixelemente
      bekommen den Wert Null.
   */
   dif1 = n - NumOfJobs;
   dif2 = n - NumOfMachines;

   for ( i = NumOfJobs + 1; i <= NumOfJobs + dif1; i++ )
      for ( j = 1 ; j <= NumOfMachines; j++ )
	 c[i][j] = 0;

   for ( i = 1; i <= NumOfJobs; i++ )
      for ( j = NumOfMachines + 1 ; j <= NumOfMachines + dif2; j++ )
	 c[i][j] = 0;


   /* Diese while-Schleife bricht ab, wenn alle Op'en betrachtet wurden
      ( iter = NumOfOperations ). Jedesmal, wenn eine Op in cut eingefuegt 
      wird, wird iter um eins erhoeht.
   */
   while ( iter < NumOfOperations )
   {
      /* Einlesen der Zeitmatrix :
	 Nichteinplanbare Operationen ausschliessen:
	   Op'en, deren Vorgaenger noch nicht alle eingeplant sind 
	   (num_of_pred > 0) muessen fuer das Matching ausgeschlos-
	   sen werden;
	   Op'en, die bereits im cut waren (num_of_pred = -1), die also ein 
	   Matching bildeten, ebenfalls ausschliessen;
	 Op'en, deren Vorgaenger alle eingeplant sind (num_of_pred = 0) 
	 werden fuer das Matching zugelassen;
      */
      sum = 1;

      for ( i = 1; i <= NumOfJobs; i++ )
	 for ( j = 1; j <= NumOfMachines; j++ )
	 {
	    if ( num_of_pred[sum] == 0 )
	       c[i][j] = -OpData[sum].process_time;  
	    else
	       c[i][j] = MaxInt;
	    sum++;
	 }

      /* Loesen des Matchingproblems
      */
      WeightMatch(n, c, mate);
      for ( i = 1; i <= n; i++ )
      {
	 j = mate[i];

	 /* n<m :  mate[i] = j wird fuer i > NumOfJobs unterdrueckt
	    m<n :  mate[i] = j wird fuer j > NumOfMachines unterdrueckt
	 */
	 if ( i <= NumOfJobs  &&  j <= NumOfMachines )
	 {
	    /* Matrixschreibweise fuer Op'en --> einfache Durchnumerierung 
	    */
	    op = FindOpNr(i,j);

	    /* die Op'en des optimalen Matchings stehen in cut
	    */
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

		  /* eine Op mit num_of_succ = 0, hat weder heurist.
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
      Gibt es mehrere, so wird die Op mit der groessten Fertigstellungszeit
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
