/*                             Filename: Main.c                              */
/*                                                                           */
/*                               main program                                */
/*                                                                           */
/* ************************************************************************* */


#include <stdlib.h>

#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include <iostream>
#include <fstream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../basics/matrix.hpp"
#include "../../lisa/ptype.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../misc/except.hpp"

#include "jb_block.hpp"
#include "jb_brnch.hpp"
#include "jb_dtstr.hpp"
#include "jb_fix.hpp"
#include "jb_hd_tl.hpp"
#include "jb_heur.hpp"
#include "jb_init.hpp"
#include "jb_list.hpp"
#include "jb_low.hpp"
#include "jb_rd_dt.hpp"
#include "jb_selct.hpp"
#include "jb_sort.hpp"
#include "jb_stack.hpp"
#include "jb_table.hpp"

using namespace std;

/* ************************************************************************* */
/*                                                                           */
/*                              main program                                 */
/*                                                                           */
/*  INPUT: command line parameters                                           */
/*                                                                           */
/*  FUNCTION: Calculation of the optimal solution for the problem given in   */
/*            file argv[1]                                                   */
/* ************************************************************************* */

bool abort_algorithm;

void set_abort(int i) 
{  
  abort_algorithm=true;
  cout << "\nSignal " << i << " received, writing results \n";
}

// ##############################################################
// WARNING! The following two functions have to be replaced for Windows!

void run_start()
{
  signal(SIGINT, &set_abort);
}

void run_stop()
{
  signal(SIGINT, SIG_DFL);
}
// ##############################################################



int main(int argc, char *argv[]){
  G_ExceptionList.set_output_to_cout();
  abort_algorithm = false;
  // the follow code is addited by A.Winkler
  
  // print a message that the programm started:
  cout << "This is the Branch & Bound Module for Job Shop by P.Brucker." << endl;
  if (argc != 3) {
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(7);
  }
  
  cout << "PID= " << getpid() << endl;  
  
  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  if (!i_strm)
  {
    cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
    exit(1);
  }
  if (!o_strm)
  {
    cout << "ERROR: cannot find output file " << argv[1] << "." << endl;
    exit(1);
  }
  i_strm.close();
  o_strm.close();
  
  LisaXmlFile::initialize();
  LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
  
  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;   
  Lisa_Values Values;
  
  xmlInput.read(argv[1]);
  LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
  
  if (!xmlInput || !(type == LisaXmlFile::INSTANCE || type == LisaXmlFile::SOLUTION))
  {
    cout << "ERROR: cannot read input , aborting program." << endl;
    exit(1);
  }
  if( !(xmlInput >> Problem))
  {
    cout << "ERROR: cannot read ProblemType , aborting program." << endl;
    exit(1);
  }
  if( !(xmlInput >> Parameter))
  {
    cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
    exit(1);
  }
  if( !(xmlInput >> Values))
  {
    cout << "ERROR: cannot read Values , aborting program." << endl;
    exit(1);
  }
  
  if (!G_ExceptionList.empty())
  {
    cout << "ERROR: cannot read input , aborting program." << endl;
    exit(1);
  }
  
  ofstream js_in("js_in.dat"); // the Brucker input file we have to write
  if (!js_in){
    G_ExceptionList.lthrow("ERROR: Could not open file for writing: js_in.dat");
    exit(1);
  }
  
  // do you really have a job shop problem ?
  const int OBJ_TYPE = Problem.get_property(OBJECTIVE);
  const int PROB_TYPE = Problem.get_property(M_ENV);
  
  if ( (OBJ_TYPE != CMAX) || ((PROB_TYPE != J) && (PROB_TYPE != F)) ){
    G_ExceptionList.lthrow("Wrong Problemtype or Objective function.",Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(7);
  }
  
  // now write an input file for Bruckers procedure Read_Data():
  // first read the problem data from a LiSA file
  
  if ( !(Values.MO)){
    G_ExceptionList.lthrow("You must define a MO for a job-shop-problem !");
    exit( 7 );
  }
  
    // the new Bruckers data file:
  int i, j, counter, mo;
  const int m = Values.get_m();
  const int n = Values.get_n();
  
  if(m > MaxNumOfMachines || n > MaxNumOfJobs ){
    G_ExceptionList.lthrow("This Algorithm does not support Problems with more than "
    +ztos(MaxNumOfMachines)+" Machines or " +ztos(MaxNumOfJobs)+ " Jobs. Recompile it to change this.");
    exit( 7 );
  }
 
  SIJ = Values.SIJ;
  JO = new Lisa_Matrix<int>(n,m);
  if(!JO){  
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
  
  js_in << m << endl;
  js_in << n << endl;
  
  int *m_nr = new int[m];
  TIMETYP *ptime = new TIMETYP[m];
  
  for (i=0; i<n; i++){
    counter = 0;
    mo = -1;
    if (Values.MO->succ(i,mo) != -1){
      do{
        mo = Values.MO->succ(i,mo);
        //cout << "counter = " << counter << "; mo = " << mo << "\n";
        if ( mo != -1 ){
          ptime[counter] = (*Values.PT)[i][mo];
          m_nr [counter] = mo+1;
          counter += 1;
        }
      }while ( mo != -1 );
    }
    js_in << counter << " ";
    //cout << "output " << counter << "\n";
    if (counter > 0){
      for (j=0; j<counter; j++){
        js_in.width(4);
        js_in << ptime[j];
        js_in.width(4);
        js_in << m_nr[j];
      }
      js_in << endl;
    }
  }
  js_in.close();
  
  
  // original program code by P.Brucker:
  struct BranchList  *DeleteBranch;
  
  Initialize();
  if ( !Read_Data("js_in.dat") ) {
    printf("Not able to read data !\n");
    return(1);
  }
  
  
  run_start();
  if ( Compute_Head_Tail() ) {
    printf("\n");
    Heuristic_Schedule(n, m);
    if ((SonNode->lower_bound = Additional_DisjArcs()) < UpperBound) {
      printf("\nLower Bound = %4d\n\n",SonNode->lower_bound);
      Compute_Blocks();
      Compute_BranchList();
      ActualNode = SonNode;
      Push();
      SonNode = (struct NodeType *) malloc(sizeof(struct NodeType));
      SonNode->blocks = NIL; SonNode->order = NIL;
    }
  }
  while ( (FirstOfStack != NIL) && (!(abort_algorithm)) ) {
    Pop();          
    Update_DisjArcs();
    while (ActualNode->order != NIL && ActualNode->lower_bound < UpperBound) {
      Fix_Disjunctions(ActualNode->order->branch_op,
      ActualNode->order->before_or_after);
      DeleteBranch = ActualNode->order;
      ActualNode->order = ActualNode->order->next;
      free((void *) DeleteBranch);     
      if ( Compute_Head_Tail() &&     
        Compute_LowerBound() < UpperBound && 
        (SonNode->lower_bound = Additional_DisjArcs()) < UpperBound) {
          Heuristic_Schedule(n, m);
          if ( SonNode->lower_bound < UpperBound ) {
            Compute_Blocks();         
            Compute_BranchList();    
            SearchTreeNodes++;
            Push();                 
            ActualNode = SonNode;  
            SonNode = (struct NodeType *) malloc(sizeof(struct NodeType));
            SonNode->blocks = NIL; SonNode->order = NIL;
          } else { 
            CriticalPath = Makeempty(CriticalPath); 
            Update_DisjArcs();     
          }
        } else {
          Update_DisjArcs();
        }                 
    }
  }
  run_stop();
  
 
  Lisa_Matrix<int> *MO;
  if ( !( MO = new Lisa_Matrix<int>(n,m) ) ){  
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
  Values.MO->write_rank(MO);
    
  Lisa_Schedule plan_in(n,m);
  plan_in.make_LR();
  bool feasible = plan_in.MO_JO_to_LR(plan_in.LR,Values.SIJ,MO,JO);
  
  type = LisaXmlFile::INSTANCE;
  if(feasible) type = LisaXmlFile::SOLUTION;
  
  LisaXmlFile xmlOutput(type);
  xmlOutput << Problem << Values << Parameter;
  if(feasible) xmlOutput << plan_in;
  xmlOutput.write(argv[2]);
  
  delete MO;
  delete JO;
  
  delete[] m_nr;
  delete[] ptime;
  
  return 0;
}

