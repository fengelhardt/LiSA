/**
 * @author : jan tusch, andre herms
 */ 
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <limits>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

#include "b_node.hpp"

using namespace std;

enum InsertionOrder{
  lpt,
  rndm,
  line_by_line,
  diagonal,
		spt,
		ect
};

enum InsertionMethod {
  insert1,
  insert2
};

//this is not working due to the wierd sorting of Lisa_Order (what is a bug in my eyes)
//static const double DOUBLE_INF = numeric_limits<double>::infinity();
static const double DOUBLE_INF = 10000000.;

CostFunc costFunc = CObjective;
int myproblemtype = O;
//Lisa_MO *myMO;
TIMETYP objective;

Lisa_Order* makeOrder(InsertionOrder, int& ops, Lisa_OsProblem*);

B_Node* beam_search(Lisa_Order *, int, Lisa_OsProblem *);

int beam_width = 5;
InsertionMethod insertionMethod = insert1; 

int main(int argc, char *argv[]){
  
  G_ExceptionList.set_output_to_cout();   
  
  // open files and assure existence:
  if (argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
  
  cout << "PID= " << getpid() << endl;
  
  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  if (!i_strm){
    cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
    exit(1);
  }
  if (!o_strm){
    cout << "ERROR: cannot write output file " << argv[1] << "." << endl;
    exit(1);
  }
  i_strm.close();
  o_strm.close();
		
  // read problem description and decide whether program is applicable:
  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;
  Lisa_Values Values;
  
	//initialize class
    LisaXmlFile::initialize();
    //create Input handler
    LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
    //communication objects
    
    //parse xml file
    xmlInput.read(argv[1]);
    //determine document type
    LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
    
    //check for successful parsing and valid document type
    if (!xmlInput || !(type == LisaXmlFile::INSTANCE || type == LisaXmlFile::SOLUTION))
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }
    //get Problem
    if( !(xmlInput >> Problem))
    {
      cout << "ERROR: cannot read ProblemType , aborting program." << endl;
      exit(1);
    }
    //get ControlParameters
    if( !(xmlInput >> Parameter))
    {
      cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
      exit(1);
    }
    //get Values
    if( !(xmlInput >> Values))
    {
      cout << "ERROR: cannot read Values , aborting program." << endl;
      exit(1);
    }
    // if something else went wrong
    if (!G_ExceptionList.empty())
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }
	
  myproblemtype = Problem.get_property(M_ENV);

  string cannot_handle="";
  if (Problem.get_property(PMTN)) cannot_handle="preemption";
  if (Problem.get_property(PRECEDENCE)!=EMPTY) 
    cannot_handle="precedence constraints"; 
  if (Problem.get_property(BATCH))  cannot_handle="batching"; 
  if (Problem.get_property(NO_WAIT))  cannot_handle="no-wait constraints";
  if (cannot_handle!=""){
    cout << "ERROR: beam cannot handle " << cannot_handle << 
    ". Aborting program."<< endl;
    exit(1);
  }  
  
  //check more ....
  B_Node::setObjective(Problem.get_property(OBJECTIVE));
  
  
  
  // solve the problem and store results in a schedule object
  // Insert your solution algorithm here:
  
  Lisa_OsProblem *os_problem = new Lisa_OsProblem(&Values);
  
  Lisa_Schedule * out_schedule = new Lisa_Schedule(Values.get_n(),
																																																			Values.get_m());
  out_schedule->make_LR();
   
  //which order was selected
  InsertionOrder iord = lpt;
  if (Parameter.defined("INS_ORDER")) {
    if (Parameter.get_string("INS_ORDER")=="LPT") iord = lpt;
    if (Parameter.get_string("INS_ORDER")=="RANDOM") iord = rndm;
    if (Parameter.get_string("INS_ORDER")=="MACHINEWISE") iord = line_by_line;
    if (Parameter.get_string("INS_ORDER")=="DIAGONAL") iord = diagonal;
				if (Parameter.get_string("INS_ORDER")=="SPT") iord = spt;
				if (Parameter.get_string("INS_ORDER")=="ECT") iord = ect;
  }


  //which choice method was selected
  if (Parameter.defined("INS_METHOD") && (Parameter.get_string("INS_METHOD")=="INSERT2") )
    insertionMethod = insert2;

  //which choice criterion was selected
  if (Parameter.defined("CRITERION") && (Parameter.get_string("CRITERION")=="CLAST") )
    costFunc = CLast;
  
  //which beamwidth was entered
  if (Parameter.defined("K_BRANCHES")) {
    beam_width = Parameter.get_long("K_BRANCHES");
  }

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
		
		int l = 0;
  
  Lisa_Order *order = makeOrder(iord,l,os_problem);
  
		/*
		//output order
		cout << "Order : " << Parameter.get_string("INS_ORDER") << endl;
		for(int i = 0; i < l; i++)
		cout << "[" << order->row(i) << "," << order->col(i) << "]" << endl;
		*/

		B_Node * res = beam_search(order, l, os_problem);
  
    delete order;
    
    res->write_LR(out_schedule->LR);
    delete res;
    
    LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
    xmlOutput << Problem << Values << Parameter << *out_schedule;
    xmlOutput.write(argv[2]);
    
    delete out_schedule;
    delete os_problem;
}



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
  for(int op = 1;op < length; op++){ //loop ops
    //make output to the progress window
    cout << "OBJECTIVE= "<< objective; 
    cout << " ready " << 100*op / length << "%" << endl;
    //the expanded schedule-list
    KList *children = new KList(beam_width); 
    
    //iterate over the at most k best from last iteration
    for(int i = 0; i < fathers->in_list; i++){ //loop fathers (i)
						
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
						B_Node * nb;// = new B_Node(b);
						//nb->insert(cur_opi,cur_opj,SOURCE,SOURCE);
						
						bool iIsSource = true;
						
						for (int	posi = SOURCE;
											iIsSource || (posi != SINK);
											posi = b->GetJOsucc(posi,cur_opj)) { //loop posi
								
								iIsSource = false;
								//this is for Open-Shop-Problems only
								if(myproblemtype == O)
										{
												bool jIsSource = true;
												for (int posj = SOURCE;
																	jIsSource || (posj != SINK);
																	posj = b->GetMOsucc(cur_opi,posj)) {
														
														jIsSource = false;
														
														nb = new B_Node(b);
														if ( nb->insert(cur_opi, cur_opj, posi, posj)!= CYCLE) {
																if ((insertionMethod == insert2) &&
																				(fathers->full()))
																		children->add(nb,i);
																else
																		children->add(nb);
														} 
														else	
																delete nb;
												}
										} //problem switch
								else //flow shop
										{
												posj = SOURCE;
												//find the predecessor in Mashine order
												for(int jj = cur_opj-1; jj > 0 ;jj--){
														if(b->exists(cur_opi,jj)){
																posj = jj;
																break;
														}
												}
												nb = new B_Node(b);
												if ( nb->insert(cur_opi, cur_opj, posi, posj)!= CYCLE) {
														
														if ((insertionMethod == insert2) &&
																		(fathers->full()))
																children->add(nb,i);
														else
																children->add(nb);
												} else {
														delete nb;
												}
										}//end flow shop
						}// end loop posi
						delete LR;
      // now we have the k best children of the current father-node
    } //end loop fathers (i)
    //now all fathers children are merged in children-list - k best are kept
    //fathers not needed anymore 
    delete fathers;
    fathers = children;
  } //end loop ops
  b = fathers->list[0];
  //find schedule with minimum C_MAX value
  for(int i = 1; i < fathers->in_list;i++)
    b= (fathers->list[i]->GetValue() < b->GetValue())?b = fathers->list[i]:b;
		B_Node* ret = new B_Node(b);
		delete fathers;
  return ret;
}

Lisa_Order* makeECT(int& ops, Lisa_OsProblem *os_problem);

Lisa_Order* makeOrder(InsertionOrder iord, int& ops, Lisa_OsProblem *os_problem){
		if(iord == ect)
				return makeECT(ops,os_problem);
		Lisa_Order *order = new Lisa_Order(os_problem->n,os_problem->m);
		int l = os_problem->n * os_problem->m;
		//use system time for independent random numbers
  //this is not reversable
  long seed = (long)time(NULL);
  
		//create the insertion order
  for (int i=0; i < os_problem->n; i++)
    for (int j=0; j < os_problem->m; j++) {
      //do nothing for non-existing operations
      if (! (*os_problem->sij)[i+1][j+1]) 
								{
										//order sorts non-decreasing
										//non-existing ops get maximum key : +infinity;
										l--;
										order->read_one_key( i, j, DOUBLE_INF);
								}
      if (iord == lpt) 
								order->read_one_key( i, j, -(*(os_problem->time))[i+1][j+1]);
						else if (iord == spt) 
								order->read_one_key( i, j, (*(os_problem->time))[i+1][j+1]);
      else if (iord == diagonal)
								order->read_one_key( i, j, (i - j + MAX(os_problem->n,os_problem->m)) % MIN(os_problem->n,os_problem->m));
      else if (iord == line_by_line)
								order->read_one_key( i, j, j * os_problem->n + i);
      else 
								order->read_one_key( i, j, lisa_random(1, 24213, &seed));
    }
		order->sort();
		ops = l;
		return order;
}

Lisa_Order* makeECT(int& ops, Lisa_OsProblem *os_problem){
		int n = os_problem->n, m = os_problem->m;
		int l = n*m;
		Lisa_Order *order = new Lisa_Order(n,m);
		Lisa_Order *ECT = new Lisa_Order(n,m);
		double *compl_times = new double[l];
		//initialize order and times
		for(int i = 0; i< n; i++)
				for(int j = 0; j < m; j++)
						{
								if (! (*os_problem->sij)[i+1][j+1])
										{
												l--;
												compl_times[i + j*m] = DOUBLE_INF;
												order->read_one_key(i,j,DOUBLE_INF);
										}
								else{
										if(os_problem->ri)
												compl_times[i + j*m] = (*os_problem->ri)[i+1] + (*os_problem->time)[i+1][j+1];
										else
												compl_times[i + j*m] = (*os_problem->time)[i+1][j+1];
								}
								ECT->read_one_key(i,j,compl_times[i + j*m]);
						}
		ECT->sort();
		for(int op = 0; op < l; op++)
				{
						//next operation found - insert in final order
						int i = ECT->row(0), j = ECT->col(0);
						order->read_one_key(i,j,op);
						//mark current op as processed
						compl_times[i + j*m] = -1.0;
						//update completion times on machine j
						for(int t = 0; t < n; t++)
								{
										//put processed to the end
										if(compl_times[t + j*m] < 0.0)
												ECT->read_one_key(i,j,DOUBLE_INF);
										//existing ops on machine j finish later now
										else if ((*os_problem->sij)[i+1][j+1]){
												compl_times[t + j*m] += (*os_problem->time)[i+1][j+1];
												ECT->read_one_key(t,j,compl_times[t + j*m]);
										}
								}
						ECT->sort();
				}
		delete[] compl_times;
		delete ECT;
		ops = l;
		order->sort();
		return order;
}
