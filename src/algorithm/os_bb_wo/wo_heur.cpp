/* ************************************************************************ */
/*                            Filename: heur1.c                             */
/*                                                                          */
/* Export: Procedure Heuristic_Schedule:                                    */
/*         Berechnung einer heuristischen Loesung fuer den Suchbaumknoten   */
/*         SonNode.                                                         */
/*									    */
/*         --> Heuristik auf der Basis eines Matchingproblems:              */
/*             HIER: SUMMENZUORDNUNGSPROBLEM/MINIMIERUNG                    */
/* ************************************************************************ */

#include <cstdio>
#include "wo_data.hpp"
#include "wo_calc.hpp"     
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_heur.hpp"
#include "wo_read.hpp"
#include "wo_low.hpp"

#include <iostream>

/* Bestimmung eines Matchings mit der Eigenschaft, dass die Summe ueber 
   die Kantengewichte im zugehoerigen bipartiten Graphen minimiert wird

   Algorithmus: siehe WALTHER/NAEGLER: GRAPHEN-ALGORITHMEN-PROGRAMME, S.131
*/
void WeightMatch(int n,int c[MaxNumOfJobs+1][MaxNumOfMachines+1],int mate[]){
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


void Heuristic_Schedule_MATCHING()

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
	       c[i][j] = minmax_match*OpData[sum].process_time;  
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
   if ( c_max < UpperBound ){   
      UpperBound = c_max;       
      std::cout << "OBJECTIVE= " << UpperBound << std::endl;
      Set_Solution(maschlist,joblist);
   } 

   
   /* Leeren der Listen: 'zu jedem "malloc" ein "free" !!!`
   */
   for ( i = 0; i <= NumOfJobs; ++i )
      joblist[i] = Makeempty(joblist[i]);
   for ( i = 0; i <= NumOfMachines; ++i )
      maschlist[i] = Makeempty(maschlist[i]);
}

static struct arcs {
		      int s, t;
		      int weight;
		   } A[MaxNumOfJobs*MaxNumOfMachines+1];
static int mate[MaxNumOfJobs*MaxNumOfMachines+1];

/* loest Bottleneck-Zuordnungsproblem:
   bestimmt ein maximales Matching im bipartiten Graphen derart, dass       
   das maximale Kantengewicht minimal wird;
*/
void BottleMatch_BOTTLE_MATCH(int num_S,int num_T,int num_A,struct arcs A[]){
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

void Heuristic_Schedule_BOTTLE_MATCHING()

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
	    Kantengewichte = pos. Bearbeitungszeiten, falls num_of_pred = 0,
	 */
	 if ( num_of_pred[sum] == 0 )
	 {
	    A[i].s = OpData[sum].job_nr;
	    A[i].t = OpData[sum].machine_nr + NumOfJobs;
	    A[i].weight = minmax_match*(OpData[sum].process_time+head_match*Head[sum]);
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
      BottleMatch_BOTTLE_MATCH(NumOfJobs, NumOfMachines, NumOfOperations, A);
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
      std::cout << "OBJECTIVE= " << UpperBound << std::endl;
      Set_Solution(maschlist,joblist);
   } 

   /* Leeren der Listen: 'zu jedem "malloc" ein "free" !!!`
   */
   for ( i = 0; i <= NumOfJobs; ++i )
      joblist[i] = Makeempty(joblist[i]);
   for ( i = 0; i <= NumOfMachines; ++i )
      maschlist[i] = Makeempty(maschlist[i]);
}

static int    op_array[MaxOpProM_And_J+1],
	      temp_head[MaxNumOfOperations+1];	

/* Berechnung einer lower bound, wenn Op auf der Maschine als naechstes 
   eingeplant wird, vor allen anderen Op'en, die noch nicht auf der 
   Maschine eingeplant wurden.
   Entspricht dem Wert des JPS angewendet auf die Menge der Op'en, die 
   noch auf der Maschine eingeplant werden muessen, unter der Annahme, 
   dass op als erstes eingeplant wird.
*/
static int LowerBoundCalc_LB_PREC_RULE(int machine_nr,int num_of_pred[],int local_head[],int op)
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

void Heuristic_Schedule_LB_PREC_RULE()
{

static int selection[MaxNumOfOperations+1], 
		     generate_set[MaxNumOfJobs+1],
	       genpos; 

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
        low_for_prior_op = LowerBoundCalc_LB_PREC_RULE(minreadytimemachine,
                                        num_of_pred, local_head, prior_op);
        for ( i = 2; i < genpos; ++i )
	{
           low_for_op = LowerBoundCalc_LB_PREC_RULE(minreadytimemachine, num_of_pred,
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
     std::cout << "OBJECTIVE= " << c_max << std::endl;                            

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

    Set_Solution(maschlist,joblist);

  }
}

/* Berechnung einer lower bound, wenn Op auf der Maschine als naechstes 
   eingeplant wird, vor allen anderen Op'en, die noch nicht auf der 
   Maschine eingeplant wurden.
   Entspricht dem Wert des JPS angewendet auf die Menge der Op'en, die 
   noch auf der Maschine eingeplant werden muessen, unter der Annahme, 
   dass op als erstes eingeplant wird. */
static int LowerBoundCalc_LB_PREC_RULE_VAR(int machine_nr,int  num_of_pred[],int local_head[],int op)
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

/* wie oben; anstatt auf einer Maschine werden die Operationen 
   eines Jobs betrachtet;*/
static int LowerBoundCalc_J_LB_PREC_RULE_VAR(int job_nr,int num_of_pred[],int local_head[],int op)
{

  struct List  *help;
  int          new_head, 
	       lower_bound;
  register int i = 0;

  new_head = local_head[op] + OpData[op].process_time;
  help = OpsOfJob[job_nr];
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

void Heuristic_Schedule_LB_PREC_RULE_VAR()
{

 /*static struct List  *maschlist[MaxNumOfMachines+1],
		     *joblist[MaxNumOfJobs+1];*/
 /*static int          num_of_pred[MaxNumOfOperations+1], */
	static int 	     selection[MaxNumOfOperations+1], 
		     generate_set[MaxNumOfJobs+1],
		     genpos; /*
		     local_head[MaxNumOfOperations+1],
		     obtained[MaxNumOfOperations+1];

*/

  struct List  *cut,
	       *help,
	       *updatelist;
  FILE         *fp;
  int          minreadytime,
	       minreadytimemachine,	
	       minreadytimejob,
	       newminreadytime,
	       selectpos = 1,
	       prior_op,
	       prior_op_j,
	       low_for_prior_op,
	       low_for_prior_op_j,
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

     minreadytimejob = OpData[op].job_nr;


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
        low_for_prior_op = LowerBoundCalc_LB_PREC_RULE_VAR(minreadytimemachine,
                                        num_of_pred, local_head, prior_op);
        for ( i = 2; i < genpos; ++i )
	{
           low_for_op = LowerBoundCalc_LB_PREC_RULE_VAR(minreadytimemachine, num_of_pred,
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

     /* generate_set ist die Menge der Operationen aus dem cut, die zum
	MRTJ gehoeren und deren local_head < MRT
     */
     genpos = 1; 
     help = cut;
     while (help != NIL) 
     {
	if (OpData[help->number].job_nr == minreadytimejob
			     && local_head[help->number] < minreadytime)
	   generate_set[genpos++] = help->number;
	help = help->next;
     }

     /* Besteht das generate_set aus (mind.) 2 Op'en, so wird fuer diese eine
	untere Schranke berechnet;
	die Op mit der kleinsten unteren Schranke wird zur prior_op_j
     */
     prior_op_j = generate_set[1];
     if ( genpos > 2 ) 
     {
	low_for_prior_op_j = LowerBoundCalc_J_LB_PREC_RULE_VAR(minreadytimejob, num_of_pred, 
						local_head, prior_op_j);
	for ( i = 2; i < genpos; ++i )
	{
	   low_for_op = LowerBoundCalc_J_LB_PREC_RULE_VAR(minreadytimejob, num_of_pred,
					       local_head, generate_set[i]);
	   if ( low_for_op < low_for_prior_op_j )
	   {
	      low_for_prior_op_j = low_for_op;
	      prior_op_j = generate_set[i];
	   }
	   else 
	   {
	      if ( low_for_op == low_for_prior_op_j &&
				 Tail[prior_op_j] < Tail[generate_set[i]] )
	      prior_op_j = generate_set[i];
	   }
        }
     }

     /* Zwischen den beiden ermittelten prior_op's wird entschieden:
        die Operation mite der groessten LB wird als naechstes eingeplant;
     */
     if ( low_for_prior_op_j > low_for_prior_op )
     {
	prior_op = prior_op_j;
     }
     else 
     {
	if ( low_for_prior_op == low_for_prior_op_j &&
				 Tail[prior_op] < Tail[prior_op_j] )
	   prior_op = prior_op_j;
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
     std::cout << "OBJECTIVE= " << c_max << std::endl;
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

    Set_Solution(maschlist,joblist);
  }
}
