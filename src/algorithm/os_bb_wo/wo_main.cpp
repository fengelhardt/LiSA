/* ************************************************************************* */
/*                             Filename: Main.c                              */
/*                                                                           */
/*                              Hauptprogramm                                */
/*                                                                           */
/* ************************************************************************* */

#include <unistd.h>
#include <signal.h>

#include <cstring>
#include <cstdlib>

#include <iostream>

#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_list.hpp"
#include "wo_init.hpp"
#include "wo_read.hpp"
#include "wo_head.hpp"
#include "wo_heur.hpp"
#include "wo_block.hpp"
#include "wo_branch.hpp"
#include "wo_stack.hpp"
#include "wo_fix.hpp"
#include "wo_select.hpp"
#include "wo_low.hpp"

#include "../../misc/except.hpp"

//**************************************************************************

bool abort_algorithm=false;

void set_abort(int i){  
   abort_algorithm=true;
   std::cout << std::endl << "Signal " << i << " received, aborting."
             << std::endl;
}

void run_start(){
   signal(SIGINT, &set_abort);
}

void run_stop(){
   signal(SIGINT, SIG_IGN);
}

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

int main(int argc,char *argv[]){
  
  if(argc != 3){
    std::cout << "Usage: " << argv[0] << " [input file] [output file]" << std::endl;
    exit(1);
  }
  
  std::cout << "This is the LiSA open shop branch and bound module." << std::endl;
  std::cout << "PID= " << getpid() << std::endl;
  
  G_ExceptionList.set_output_to_cout();
    
  struct BranchList  *DeleteBranch;
    
  Initialize();
    
  Read_Data(argv[1]);
  
  run_start();
  if ( Compute_Head_Tail() ){
    Heuristic_Schedule();
    if ((SonNode->lower_bound = Additional_Arcs()) < UpperBound){
      Compute_Blocks();
      Compute_BranchList();   
      ActualNode = SonNode;
      Push();
      if ( (SonNode = new struct NodeType) == NIL ) {
        G_ExceptionList.lthrow("main,SonNode: kein Speicherplatz",
                               Lisa_ExceptionList::NO_MORE_MEMORY);
        exit(1);
      } 
      SonNode->blocks = NIL; 
      SonNode->order = NIL;
    }
  }
  
  while (FirstOfStack != NIL && !abort_algorithm){
    
    Pop();
    Update_Arcs();
    while (ActualNode->order != NIL && ActualNode->lower_bound < UpperBound){
      
      Fix_Arcs(ActualNode->order->branch_op,
      ActualNode->order->before_or_after,
      ActualNode->order->kind_of_block);
      DeleteBranch = ActualNode->order;
      ActualNode->order = ActualNode->order->next;
      delete DeleteBranch;
      if ( Compute_Head_Tail() && 
        Compute_LowerBound() < UpperBound && 
        (SonNode->lower_bound = Additional_Arcs()) < UpperBound){
          
          Heuristic_Schedule();
          
          if ( SonNode->lower_bound < UpperBound ){
            Compute_Blocks();
            Compute_BranchList();
            SearchTreeNodes++;
            Push();                 
            ActualNode = SonNode;  
            if ( (SonNode = new struct NodeType)== NIL ){
              G_ExceptionList.lthrow("main,SonNode: kein Speicherplatz",
                                     Lisa_ExceptionList::NO_MORE_MEMORY);
              exit(1);
            }
            SonNode->blocks = NIL; 
            SonNode->order = NIL;
          }else{ 
            Update_Arcs();     
          }
        }else{ 
          Update_Arcs();                 
        }
    }
  }
  run_stop();
  
  Write_Solution(argv[2]);
    
  return(0); 
}
