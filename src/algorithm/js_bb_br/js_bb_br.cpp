/*                             Filename: Main.c                              */
/*                                                                           */
/*                               main program                                */
/*                                                                           */
/*   @version 2.3pre3
/* ************************************************************************* */


#include <stdlib.h>

#include "js_bb_br.hpp"


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
   abort_algorithm=TRUE;
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



int main(int argc, char *argv[])
{
  G_ExceptionList.set_output_to_cout();
  abort_algorithm = FALSE;
  // the follow code is addited by A.Winkler

  // print a message that the programm started:
  cout << "This is the Branch & Bound Module for Job Shop by P.Brucker." << endl;
  if (argc != 3) {
      cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
      exit(7);
  }
  
  cout << "PID= " << getpid() << endl;  
  
  ifstream strm(argv[1]);      // the input file contains the problem, schedule and some parameters
  if (!strm){
      G_ExceptionList.lthrow("ERROR: Could not open file for reading: "+string(argv[1]));
      exit(1);
  }
  
  ofstream fplan_o(argv[2]);   // this file returns the best computed schedule
  if (!fplan_o){
      G_ExceptionList.lthrow("ERROR: Could not open file for writing: "+string(argv[2]));
      exit(1);
  }

  
  ofstream js_in("js_in.dat"); // the Brucker input file we have to write
  if (!js_in){
      G_ExceptionList.lthrow("ERROR: Could not open file for writing: js_in.dat");
      exit(1);
  }
  
  Lisa_ProblemType *prob_type;
  if ( !( prob_type = new Lisa_ProblemType ) ){
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  strm.seekg(0);
  strm >> (*prob_type);
  fplan_o << (*prob_type);

  // do you really have a job shop problem ?
  const int OBJ_TYPE = prob_type->get_property(OBJECTIVE);
  const int PROB_TYPE = prob_type->get_property(M_ENV);

  if ( (OBJ_TYPE != CMAX) || ((PROB_TYPE != J) && (PROB_TYPE != F)) ){
      G_ExceptionList.lthrow("Wrong Problemtype or Objective function.",2);
      exit(7);
  }

  // now write an input file for Bruckers procedure Read_Data():
  // first read the problem data from a LiSA file
  Lisa_Values *problem_in;
  if ( !( problem_in = new Lisa_Values() ) ){  
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  strm >> (*problem_in);
  strm.close();
   
  if ( !(problem_in->MO)){
      G_ExceptionList.lthrow("You must define a MO for a job-shop-problem !");
      exit( 7 );
  }
  
  fplan_o << (*problem_in);

  // the new Bruckers data file:
  Lisa_JsSchedule *best_schedule;
  int i, j, k, counter, mo, maschine;
  const int m = problem_in->get_m();
  const int n = problem_in->get_n();
 
  if(m > MaxNumOfMachines || n > MaxNumOfJobs ){
      G_ExceptionList.lthrow("This Algorithm does not support Problems with more than "
                             +ztos(MaxNumOfMachines)+" Machines or " +ztos(MaxNumOfJobs)+ " Jobs. Recompile it to change this.");
      exit( 7 );
  }
  
  js_in << m << " ";
  js_in << n << " ";
  
  int *m_nr = new int[m];
  TIMETYP *ptime = new TIMETYP[m];
  
  for (i=0; i<n; i++){
      counter = 0;
      mo = -1;
      if (problem_in->MO->succ(i,mo) != -1){
          do{
              mo = problem_in->MO->succ(i,mo);
              //cout << "counter = " << counter << "; mo = " << mo << "\n";
              if ( mo != -1 ){
                  ptime[counter] = (*problem_in->PT)[i][mo];
                  m_nr [counter] = mo+1;
                  counter += 1;
              }
          }while ( mo != -1 );
      }
      js_in << counter << " ";
      //cout << "output " << counter << "\n";
      if (counter > 0)
          for (j=0; j<counter; j++)
              js_in << ptime[j] << " " << m_nr[j] << " ";
  }
  js_in.close();

  // the best schedule:
  Lisa_JsProblem *js_Prob;
  if ( !( js_Prob = new Lisa_JsProblem( problem_in ) ) ){  
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  if ( !( best_schedule = new Lisa_JsSchedule( js_Prob ) ) ){  
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  delete problem_in;
  
  // original program code by P.Brucker:
  struct BranchList  *DeleteBranch;
 
  Initialize();
  if ( !Read_Data("js_in.dat") ) {
    printf("Not able to read data !\n");
    return(1);
  }

  // starting time:
  // clock_t time1, time2;
  time_t time1, time2;
  double elapsed;
  time1 = time(&time1);
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
   
  // end time:
  time2 = time(&time2);
  // run time:
  //time2 = (time2 - time1)/CLK_TCK/10000;
  //elapsed = ( double( time2 - time1 ) ) / CLOCKS_PER_SEC;
  elapsed = difftime( time2, time1 );
  cout << "TIME = " << elapsed << "\n";

  printf("Upper Bound = %4d, SearchTreeNodes = %d\n",UpperBound,SearchTreeNodes);
  
  Lisa_Matrix<int> *LR;
  if ( !( LR = new Lisa_Matrix<int>(n,m) ) ){  
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  Lisa_Matrix<int> *JO;
  if( !( JO = new Lisa_Matrix<int>(n,m) ) ){  
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  Lisa_Matrix<int> *MO;
  if ( !( MO = new Lisa_Matrix<int>(n,m) ) ){  
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  Lisa_Matrix<int> *MJ;
  if ( !( MJ = new Lisa_Matrix<int>(n,m) ) ){  
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  for (i=0; i<n; i++){
      maschine = SOURCE;
      for (j=1; j<=m; j++){
          maschine = (*js_Prob->MOsucc)[i+1][maschine];
          (*MO)[i][maschine-1] = j;
      }
  }
 
  // read machine order from algortihm output file !!!
  // this data is written in jb_heur.cpp
  ifstream JO_in("jo_out.dat");
  if (!JO_in){
      G_ExceptionList.lthrow("ERROR: Could not open file for reading: jo_out.dat");
      exit(1);
  }
  JO->read( JO_in );
  JO_in.close();
  
  // make LR from MO and JO:
  for (i=0; i<n; i++)
    for (j=0; j<m; j++)
      (*MJ)[i][j] = (*MO)[i][j] + (*JO)[i][j];
      
  int* row = new int[n];
  int* col = new int[m];
  
  int count;
  count = 0; // counts the entrys
  int count_2 ;
  k = 1;
  do
    {
      count_2 = 0;
      for (i=0; i<n; i++) 
	row[i] = 0;
      for (i=0; i<m; i++) 
	col[i] = 0;
      for (i=0; i<n; i++)
	for (j=0; j<m; j++)
	  if ( (*MJ)[i][j] == 2 )
	    {
	      count_2++; count++;
	      (*LR)[i][j] = k;
	      row[i] = 1; col[j] = 1;
	    }
      if ( count_2 == 0 )
	{  
	  G_ExceptionList.lthrow("ERROR: Cycle in LR !",2);
	  exit( 7 );
	}
      for (i=0; i<n; i++)
	for (j=0; j<m; j++)
	  (*MJ)[i][j] = (*MJ)[i][j] - row[i] - col[j];
      k++;
    }
  while( count < n*m );


  //cout << "LR=" << *LR;
  best_schedule->read_LR( LR );
  //best_schedule->write( cout );
  Lisa_Schedule * plan_in;
  
  if ( !( plan_in = new Lisa_Schedule( n, m ) ) ){  
      G_ExceptionList.lthrow("out of memory",2);
      exit( 7 );
  }
  
  plan_in->make_LR();
  best_schedule->write_LR( plan_in->LR );
  fplan_o << (*plan_in);
  //cout << (*plan_in);

  fplan_o << "<TIME>\n";
  fplan_o << "TIME= " << time2 << "\n";
  fplan_o << "</TIME>\n";

  //fplan_o << (*best_schedule);
 
  fplan_o.close();
 
  delete LR;
  delete MO;
  delete JO;
  delete MJ;
  delete best_schedule;
  delete prob_type;
  delete js_Prob;
  
  delete[] m_nr;
  delete[] ptime;
  
  delete[] row;
  delete[] col;
  
  return 0;
}


