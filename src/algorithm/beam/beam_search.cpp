
#include "beam_search.hpp"
#include "b_node.hpp"

using namespace std;

BeamSearch::BeamSearch(){
  problem = NULL;
  result = NULL;
  clear();
}

BeamSearch::~BeamSearch(){
  clear();
}

void BeamSearch::clear(){
  costFunc = CObjective;
  destObjective = CMAX;
  problemtype = O;
  beam_width = 5;
  insertionMethod = insert1;
  iord = lpt;
  if(problem){ delete problem; problem = NULL; }
  if(result){ delete result; result = NULL; }
}

bool BeamSearch::init(LisaXmlFile& xmlInput){
  //get Problem
  clear();
  if( !(xmlInput >> Problem))
    {
      cout << "ERROR: cannot read ProblemType , aborting program." << endl;
      return false;
    }
  //get ControlParameters
  if( !(xmlInput >> Parameter))
    {
      cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
      return false;
    }
  //get Values
  if( !(xmlInput >> Values))
    {
      cout << "ERROR: cannot read Values , aborting program." << endl;
      return false;
    }
  // if something else went wrong
  if (!G_ExceptionList.empty())
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      return false;
    }
  
  problemtype = Problem.get_property(M_ENV);
  
  string cannot_handle="";
  if (Problem.get_property(PMTN)) cannot_handle="preemption";
  if (Problem.get_property(PRECEDENCE)!=EMPTY) 
    cannot_handle="precedence constraints"; 
  if (Problem.get_property(BATCH))  cannot_handle="batching"; 
  if (Problem.get_property(NO_WAIT))  cannot_handle="no-wait constraints";
  if (cannot_handle!=""){
    cout << "ERROR: beam cannot handle " << cannot_handle << 
      ". Aborting program."<< endl;
    return false;
  }  
  //B_Node::setObjective(Problem.get_property(OBJECTIVE)); 
  destObjective = Problem.get_property(OBJECTIVE);
  

  problem = new Lisa_OsProblem(&Values);
  result = new Lisa_Schedule(Values.get_n(), Values.get_m());
  result->make_LR();
  
  //which order was selected
  if (Parameter.defined("INS_ORDER")) {
    if (Parameter.get_string("INS_ORDER")=="LPT") iord = lpt;
    else if (Parameter.get_string("INS_ORDER")=="RANDOM") iord = rndm;
    else if (Parameter.get_string("INS_ORDER")=="MACHINEWISE") iord = line_by_line;
    else if (Parameter.get_string("INS_ORDER")=="DIAGONAL") iord = diagonal;
    else if (Parameter.get_string("INS_ORDER")=="QUEEN_SWEEP") iord = queens;
    else if (Parameter.get_string("INS_ORDER")=="SPT") iord = spt;
    else if (Parameter.get_string("INS_ORDER")=="ECT") iord = ect;
    else{
      cout << "ERROR: \"" << Parameter.get_string("INS_ORDER")
	   << "\" unknown Insertion Order. Aborting." << endl;
      return false;
    }
  }
  else
    {
      cout << "ERROR: \"INS_ORDER\" undefined. Aborting." << endl;
      return false;
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
  return true;
}

TIMETYP BeamSearch::guessObjective(){
  if(problem == NULL) return 0.0;
  TIMETYP x_bound = 0;
  TIMETYP y_bound = 0;
  TIMETYP r = 0;
  for (int i = 0; i< problem->n;i++){
    for (int j = 0; j < problem->m;j++){
      r += (*(problem->time))[i+1][j+1];
      
    }
    x_bound = MAX(r,x_bound);
    r=0;
  }
  
  for (int j = 0; j< problem->m;j++){
    for (int i = 0; i < problem->n;i++){
      r += (*(problem->time))[i+1][j+1];
    }
    y_bound = MAX(r,y_bound);
    r=0;
  }
  return x_bound + y_bound;
}


bool BeamSearch::run(){
  int length = 0;
  Lisa_Order *lo = makeOrder(iord,length,problem);
  //the list with the schedules to be expanded
  KList *fathers = new KList(beam_width, destObjective, costFunc);
  
  //the root of the tree = an empty schedule
  B_Node *b = new B_Node(problem);
  //insert first operation and add to the list
  b->insert(lo->row(0)+1,lo->col(0)+1,SOURCE,SOURCE);
  fathers->add(b);
		
  //inefficient - but have to use it
  Lisa_Matrix<int> *LR;
  TIMETYP objective = 0;
  //iterate over all operations
  for(int op = 1;op < length; op++){ //loop ops
    //make output to the progress window
    cout << "OBJECTIVE= "<< objective; 
    cout << " ready " << 100*op / length << "%" << endl;
    //the expanded schedule-list
    KList *children = new KList(beam_width,destObjective,costFunc); 
    
    //iterate over the at most k best from last iteration
    for(int i = 0; i < fathers->in_list; i++){ //loop fathers (i)
						
      b = fathers->list[i];
      objective = (objective > b->getCosts(destObjective,costFunc))?objective:b->getCosts(destObjective,costFunc);
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
	if(problemtype == O)
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
  //B_Node* ret = new B_Node(b);
  b->write_LR(result->LR);
  delete fathers;
  delete lo;
  return true;
}
