/* ************************************************************************* */
/*                             Filename: Main.c                              */
/*                                                                           */
/*                              Hauptprogramm                                */
/*                                                                           */
/* ************************************************************************* */


#include <malloc.h>
#include <stdio.h>
#include "data.h"
#include "table.h"
#include "list.h"
#include "init.h"
#include "read.h"
#include "head.h"
#include "heur.h"
#include "block.h"
#include "branch.h"
#include "stack.h"
#include "fix.h"
#include "select.h"
#include "low.h"


/* ************************************************************************* */
/*                                                                           */
/*                             Hauptprogramm                                 */
/*                                                                           */
/*  INPUT: Parameter der Kommandozeile                                       */
/*                                                                           */
/*  FUNCTION: Berechnung einer optimalen Loesung fuer das Problem in der     */
/*            Datei argv[1]                                                  */
/*									     */
/*            Alle Outputs des Programm stehen in der Datei output.dat       */
/*            Outputs: erste berechnete obere Schranke (Wurzel)              */
/*		       triviale untere Schranke				     */
/*		       Selektionen, deren Zf die aktuelle UB verbessern	     */
/*		       krtitischer Pfad der Selektion			     */
/* ************************************************************************* */

int main(argc, argv)

int argc;
char *argv[];

{
  struct BranchList  *DeleteBranch;
  char output_file[30];
  char *index;

  FILE  *fp;

  
  strcpy(output_file,argv[1]);
  strcat(output_file,".erg");

  Initialize(output_file);


  fp = fopen(output_file,"a");
  fprintf(fp,"%s %s: \n-------------\n",argv[0], argv[1]);
  fclose(fp);

  if ( !Read_Data(argv[1]) )
  {
     printf("Not able to read data !\n");
     return(1);
  }
  if ( Compute_Head_Tail() )
  {
     printf("\n");
     Heuristic_Schedule();
     if ((SonNode->lower_bound = Additional_Arcs()) < UpperBound) 
     {
	fp = fopen(output_file,"a");
	fprintf(fp,"Lower Bound = %4d\n\n",SonNode->lower_bound);   
        fclose(fp);
	Compute_Blocks();
	Compute_BranchList();   
	ActualNode = SonNode;
	Push();
	if ( (SonNode = (struct NodeType *) malloc(sizeof(struct NodeType)))
                      == NIL )
        {
	   fprintf(stderr, "main,SonNode: malloc: kein Speicherplatz\n") ;
           exit(1);
        } 
	SonNode->blocks = NIL; 
	SonNode->order = NIL;
     }
  }
  while (FirstOfStack != NIL) 
  {
     Pop();
     Update_Arcs();
     while (ActualNode->order != NIL && ActualNode->lower_bound < UpperBound)
     {
	Fix_Arcs(ActualNode->order->branch_op,
		 ActualNode->order->before_or_after,
		 ActualNode->order->kind_of_block);
	DeleteBranch = ActualNode->order;
	ActualNode->order = ActualNode->order->next;
	free((void *) DeleteBranch);
	if ( Compute_Head_Tail() && 
	     Compute_LowerBound() < UpperBound && 
	     (SonNode->lower_bound = Additional_Arcs()) < UpperBound)
	{
	   Heuristic_Schedule();
	   if ( SonNode->lower_bound < UpperBound )
	   {
	      Compute_Blocks();
	      Compute_BranchList();
	      SearchTreeNodes++;
	      Push();                 
	      ActualNode = SonNode;  
              if ( (SonNode = (struct NodeType *) malloc(sizeof(struct NodeType)))
                      == NIL )
              {
		 fprintf(stderr, "main,SonNode: malloc: kein Speicherplatz\n");
                 exit(1);
              }
	      SonNode->blocks = NIL; 
	      SonNode->order = NIL;
	   }
	   else 
	   { 
	      Update_Arcs();     
	   }
	}
	else 
	{ 
	   Update_Arcs();                 
	}
     }
  } 

  fp = fopen(output_file,"a");
  fprintf(fp,"SearchTreeNodes  = %d\n",SearchTreeNodes);
  fprintf(fp,"Optimal Solution = %d\n",UpperBound);
  fclose(fp);

  return(0); 
}
