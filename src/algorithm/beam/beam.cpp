/**
 * @author  Jan Tusch, Andre Herms
 * @version 2.3final
 */ 
 
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <iostream>
#include <fstream>

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp" 

#include "b_node.hpp"

using namespace std;

//**************************************************************************

enum InsertionOrder{lpt, rndm, line_by_line, diagonal};

enum InsertionMethod {insert1,insert2};

CostFunc costFunc = CLast;
int myproblemtype = O;
//Lisa_MO *myMO;
TIMETYP objective;

B_Node* beam_search(Lisa_Order *, int, Lisa_OsProblem *);

int beam_width = 5;
InsertionMethod insertionMethod = insert1; 

//**************************************************************************

int main(int argc, char *argv[]) 
{
  G_ExceptionList.set_output_to_cout();   
  
  // open files and assure existence:
  if (argc != 3) 
    {
      //  cout7 << "\nUsage: " << argv[0] << " [input file] [output file]\n";
      exit(1);
    }
  cout << "PID= " << getpid() << endl;
  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  if (!i_strm)
    {
      cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
      exit(1);
    }
  
  // read problem description and decide whether program is applicable:
  Lisa_ProblemType * lpr = new Lisa_ProblemType;
  i_strm >> (*lpr);
  if (!G_ExceptionList.empty())
    {
      cout << "ERROR: cannot read problem type, aborting program." << endl;
      exit(1);
    }  
  myproblemtype = lpr->get_property(M_ENV);

  string cannot_handle="";
  if (lpr->get_property(PMTN)) cannot_handle="preemption";
  if (lpr->get_property(PRECEDENCE)!=EMPTY) 
    cannot_handle="precedence constraints"; 
  if (lpr->get_property(BATCH))  cannot_handle="batching"; 
  if (lpr->get_property(NO_WAIT))  cannot_handle="no-wait constraints";
  if (cannot_handle!="")  
    {
      cout << "ERROR: beam cannot handle " << cannot_handle << 
	". Aborting program."<< endl;
      exit(1);
    }  
  delete lpr;   
  
  // read control parameters: 
  Lisa_ControlParameters * sp = new Lisa_ControlParameters;
  i_strm >> (*sp);
    
  // read problem instance:
  Lisa_Values * my_werte = new Lisa_Values;
  i_strm >> (*my_werte);
    
  // solve the problem and store results in a schedule object
  // Insert your solution algorithm here:
  
  Lisa_OsProblem *os_problem = new Lisa_OsProblem(my_werte);
  
  Lisa_Schedule * out_schedule = new Lisa_Schedule(my_werte->get_n(),
						   my_werte->get_m());
  out_schedule->make_LR();
   
  int l = my_werte->get_n() * my_werte->get_m();
  
  Lisa_Order *order = new Lisa_Order(os_problem->n,os_problem->m);
  
  //which order was selected
  InsertionOrder iord = lpt;
  if (sp->defined("INS_ORDER")) {
    if (sp->get_string("INS_ORDER")=="LPT") iord = lpt;
    if (sp->get_string("INS_ORDER")=="RANDOM") iord = rndm;
    if (sp->get_string("INS_ORDER")=="LINE_BY_LINE") iord = line_by_line;
    if (sp->get_string("INS_ORDER")=="DIAGONAL") iord = diagonal;
  }
  //which choice method was selected
  if (sp->defined("INS_METHOD") && (sp->get_string("INS_METHOD")=="INSERT2") )
    insertionMethod = insert2;

  //which choice criterion was selected
  if (sp->defined("CRITERION") && (sp->get_string("CRITERION")=="CMAX") )
    costFunc = CMax;
  
  //which beamwidth was entered
  if (sp->defined("K_BRANCHES")) {
    beam_width = sp->get_int("K_BRANCHES");
  }
  //use system time for independent random numbers
  //this is not reversable
  long seed = (long)time(NULL);
  
  TIMETYP x_bound = 0;
  TIMETYP y_bound = 0;
  TIMETYP r = 0;
  for (int i = 0; i< os_problem->n;i++){
    for (int j = 0; j < os_problem->m;j++){
      r += (*(os_problem->time))[i+1][j+1];
      
    }
    x_bound = MAX(r,x_bound);
    r=0;
  }

  for (int j = 0; j< os_problem->m;j++){
    for (int i = 0; i < os_problem->n;i++){
      r += (*(os_problem->time))[i+1][j+1];
    }
    y_bound = MAX(r,y_bound);
    r=0;
  }
  
  x_bound += y_bound;

  cout << "OBJECTIVE= "<< x_bound << endl; 

  //create the insertion order
  for (int i=0; i < os_problem->n; i++)
    for (int j=0; j < os_problem->m; j++) {
      //do nothing for non-existing operations
      if (! (*os_problem->sij)[i+1][j+1]) continue;
      if (iord == lpt) 
	order->read_one_key( i, j, (*(os_problem->time))[i+1][j+1]);
      else if (iord == diagonal)
	order->read_one_key( i, j, (i - j + MAX(os_problem->n,os_problem->m)) % MIN(os_problem->n,os_problem->m));
      else if (iord == line_by_line)
	order->read_one_key( i, j, j * os_problem->n + i);
      else 
	order->read_one_key( i, j, lisa_random(1, 24213, &seed));
    }
  order->sort();

  B_Node * res = beam_search(order, l, os_problem);

  res->write_LR(out_schedule->LR);
  // write results to output file:
  o_strm << *out_schedule;
  delete out_schedule;
  delete my_werte;
 
}

//**************************************************************************

B_Node* beam_search(Lisa_Order *lo, int length, Lisa_OsProblem * problem){
  //the list with the schedules to be expanded
  KList *fathers = new KList(beam_width);
  
  //the root of the tree = an empty schedule
  B_Node *b = new B_Node(problem);
  //insert first operation and add to the list
  b->insert(lo->row(0)+1,lo->col(0)+1,SOURCE,SOURCE);
  fathers->add(b);

  //inefficient - but have to use it
  Lisa_Matrix<int> *LR;

  //iterate over all operations
  for(int op = 1;op < length; op++){
    //make outpu to the progress window
    cout << "OBJECTIVE= "<< objective; 
    cout << " ready " << 100*op / length << "%" << endl;
    //the expanded schedule-list
    KList *children = new KList(beam_width); 
    
    //iterate over the at most k best from last iteration
    for(int i = 0; i< fathers->in_list; i++){

      b = fathers->list[i];
      LR = new Lisa_Matrix<int>(b->problem->n,b->problem->m);
      b->write_LR(LR);

      int posi,posj,cur_opi,cur_opj;
      //insert operation as SOURCE first = case 1
      posi = posj = SOURCE;
      //confusing index-stuff
      cur_opi = lo->row(op)+1;
      cur_opj = lo->col(op)+1;

      //produce all possible insertions for the particular father node
      
      //this is for Open-Shop-Problems only
      if(myproblemtype == O)
	{
	  B_Node *nb = new B_Node(b);
	  nb->insert(cur_opi,cur_opj,SOURCE,SOURCE);
	  if ((insertionMethod == insert2) && (fathers->full()))
	    children->add(nb,i);	  
	  else
	    children->add(nb);
	  
	  //insert after all existing operations in the row of the new operation
	  posj = b->GetMOsucc(cur_opi,posj);
	  while (posj != SINK){
	    nb = new B_Node(b);
	    //maximum rank of a possible predecessor in the column
	    int lr_posj_val = (*LR)[cur_opi - 1][posj - 1];
	    int runner = SOURCE;

	    //find the predecessor in the column, that has maximum rank
	    while( (b->GetJOsucc(runner,cur_opj) != SINK) && 
		   (lr_posj_val >= (*LR)[b->GetJOsucc(runner,cur_opj) -1 ][cur_opj - 1]))
	      runner = b->GetJOsucc(runner,cur_opj);
	    posi = runner;

	    //insert new operation and add to the children-list
	    nb->insert(cur_opi,cur_opj,posi,posj);
	    if ((insertionMethod == insert2) && (fathers->full()))
	      children->add(nb,i);	  
	    else
	      children->add(nb);
	    posj = b->GetMOsucc(cur_opi,posj);
	  }
	  
	  //insert after all existing operations in the column of the new operation
	  posi = b->GetJOsucc(SOURCE, cur_opj);
	  while( posi != SINK) {
	    nb = new B_Node(b);
	    //maximum rank of a possible predecessor in the row
	    int lr_posi_val = (*LR)[posi - 1][cur_opj -1];
	    int runner = SOURCE;

	    //find the predecessor in the row, that has maximum rank
	    //note that equatity is excluded, because it was considered above
	    while (( b->GetMOsucc(cur_opi, runner) != SINK) &&
		   (lr_posi_val > (*LR)[cur_opi -1][b->GetMOsucc(cur_opi, runner) - 1]))
	      runner = b->GetMOsucc(cur_opi, runner);
	    posj = runner;

	    //insert new operation and add to the children-list
	    nb->insert(cur_opi, cur_opj, posi, posj);
	    if ((insertionMethod == insert2) && (fathers->full()))
	      children->add(nb,i);	  
	    else
	      children->add(nb);
	    posi = b->GetJOsucc(posi, cur_opj);
	  }
	}// thats all for open shop

      //this is for Flow-Shop-Problems only
      else 
	{
	  posj = SOURCE;
	  //find the predecessor in Mashine order
	  for(int j = cur_opj-1; j > 0 ;j--){
	    if(b->exists(cur_opi,j)){
	      posj = j;
	      break;
	    }
	  }
	  posi = SOURCE;
	  B_Node *nb = new B_Node(b);
	  //insert it as SOURCE in the Job-Order and add to list
	  nb->insert(cur_opi,cur_opj,posi,posj);
	  if ((insertionMethod == insert2) && (fathers->full()))
	    children->add(nb,i);	  
	  else
	    children->add(nb);
	  //insert after all existing operations in the column - no cycles possible
	  while((posi = b->GetJOsucc(posi,cur_opj))!= SINK){
	    nb = new B_Node(b);
	    nb->insert(cur_opi,cur_opj,posi,posj);
	    if ((insertionMethod == insert2) && (fathers->full()))
	      children->add(nb,i);	  
	    else
	      children->add(nb);
	  }
	}// thats all for Flow-Shop
      delete LR;
      // now we have the k best children of the current father-node
    }
    //now all fathers children are merged in children-list - k best are kept
    //fathers not needed anymore 
    delete fathers;
    fathers = children;
  }
  b = fathers->list[0];
  //find schedule with minimum C_MAX value
  for(int i = 1; i < fathers->in_list;i++)
    b= (fathers->list[i]->GetValue() < b->GetValue())?b = fathers->list[i]:b;
  return b;
}

//**************************************************************************

