
#include "beam_search.hpp"
#include "b_node.hpp"

using namespace std;

BeamSearch::BeamSearch(){
  problem = NULL;
  result = NULL;
  order = NULL;
  clear();
}

BeamSearch::~BeamSearch(){
  clear();
}

void BeamSearch::clear(){
  value = 0;
  step = 0;
  n_ops = 0;
  costFunc = CObjective;
  destObjective = CMAX;
  problemtype = O;
  beam_width = 5;
  insertionMethod = insert1;
  iord = lpt;
  attach = both;
  if(problem){ delete problem; problem = NULL; }
  if(result){ delete result; result = NULL; }
  if(order){ delete order; order = NULL; }
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
  if (!Parameter.defined("INS_ORDER")) {
    cout << "ERROR: \"INS_ORDER\" undefined. Aborting." << endl;
    return false;
  }
  if (Parameter.get_string("INS_ORDER")=="LPT") iord = lpt;
  else if (Parameter.get_string("INS_ORDER")=="RANDOM") iord = rndm;
  else if (Parameter.get_string("INS_ORDER")=="MACHINEWISE") iord = line_by_line;
  else if (Parameter.get_string("INS_ORDER")=="DIAGONAL") iord = diagonal;
  else if (Parameter.get_string("INS_ORDER")=="QUEEN_SWEEP") iord = queens;
  else if (Parameter.get_string("INS_ORDER")=="SPT") iord = spt;
  else if (Parameter.get_string("INS_ORDER")=="ECT") iord = ect;
  else if (Parameter.get_string("INS_ORDER")=="ATTACH") iord = att;
  else{
    cout << "ERROR: \"" << Parameter.get_string("INS_ORDER")
	 << "\" unknown Insertion Order. Aborting." << endl;
    return false;
  }
  
  if (iord == att && Parameter.defined("ATTACH_WHAT")) {
    if (Parameter.get_string("ATTACH_WHAT")=="Machines") attach=machines;
    else if (Parameter.get_string("ATTACH_WHAT")=="Jobs") attach=jobs;
    else if (Parameter.get_string("ATTACH_WHAT")=="Machines+Jobs") attach=both;
    else{
      cout << "ERROR: \"" << Parameter.get_string("ATTACH_WHAT")
	   << "\" unknown Attachment rule. Aborting." << endl;
      return false;
    }
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
  //should be sophisticated for objectoves other than C_MAX
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

bool BeamSearch::getNextOp(B_Node* parent, BeamSearch::Operation& next){
  if(iord != att){ //next op comes from some order
    if(order == NULL)
      order = makeOrder(iord, n_ops, problem);
    if(step >= n_ops){
      return false;
    }
    next.first = order->row(step)+1;
    next.second = order->col(step)+1;
    return true;
  }
  else { //dynamic next
    //attachment minimum head is first machine/job
    TIMETYP best = std::numeric_limits<TIMETYP>::infinity(), head;
    next.first = next.second = 1;
    for (int i=1; i<=parent->P->n; i++)
      for (int j=1; j<=parent->P->m; j++){
	if(! ((*problem->sij)[i][j]) || parent->exists(i,j))
	  continue;
	head = std::max<TIMETYP>(parent->GetHead(i,SINK),parent->GetHead(SINK,j));
	if(head < best){
	  next.first = i; next.second = j;
	  best = head;
	}
      }
    return true;
  }
  return false;
}

void BeamSearch::getDescendentAppendings(B_Node& parent, const Operation& op, InsertionList& ins){
  ins.clear();
  OpRankPos pos;
  Operation insOp = op;
  TIMETYP head = std::max<TIMETYP>(parent.GetHead(op.first,SINK),parent.GetHead(SINK,op.second));
  switch(attach){
  case jobs:
    //find the machine numbers of all non-delay insertables
    pos.second = parent.GetMOpred(insOp.first,SINK);
    for (int j=1; j<=parent.P->m; j++)
      if (((*problem->sij)[insOp.first][j]) && !parent.exists(insOp.first,j) && (head >=  parent.GetHead(SINK,j))){
	pos.first = parent.GetJOpred(SINK,j);
	insOp.second = j;
	ins.push_back(OpInsertion(insOp,pos));
      }
    break;
  case machines:
    //find the job numbers of all non-delay insertables
    pos.first = parent.GetJOpred(SINK,insOp.second);
    for (int i=1; i<=parent.P->n; i++)
      if (((*problem->sij)[i][insOp.second]) &&  !parent.exists(i,insOp.second) && (parent.GetHead(i,SINK) <=  head)){
	pos.second = parent.GetMOpred(i,SINK);
	insOp.first = i;
	ins.push_back(OpInsertion(insOp,pos));
      }
    break;
  default:
    return;
  }
  if(ins.empty()){ //wierd, this would tear down whole thing
    cerr << "No insertion found. Aborting." << endl;
    exit(1);
  }
}

void BeamSearch::getDescendentInsertions(B_Node& parent , const Operation& op, InsertionList& ins){
  if(iord == att){
    getDescendentAppendings(parent,op,ins);
    return;
  }
  ins.clear();
  OpRankPos pos(SOURCE,SOURCE);
  //produce all possible insertions for the particular father node
  OpInsertion insOp;
  insOp.first = op; //same opration with varying MO/JO
  bool iIsSource = true;
  int ret = 0;
  B_Node* b = new B_Node(parent);
  for (pos.first = SOURCE; iIsSource || (pos.first != SINK); pos.first = parent.GetJOsucc(pos.first,op.second)) { //loop posi
    iIsSource = false;
    //this is for Open-Shop-Problems only
    if(problemtype == O)
      {
	bool jIsSource = true;
	for (pos.second = SOURCE; jIsSource || (pos.second != SINK); pos.second = parent.GetMOsucc(op.first,pos.second)){
	  jIsSource = false;
	  ret = b->insert(op.first, op.second, pos.first, pos.second);
	  if ( ret == OK || ret == PERFEKT) {
	    ins.push_back(OpInsertion(op,pos));
	    b->exclude(op.first,op.second);
	  }
	  else{
	    delete b;
	    b = new B_Node(parent);
	  }
	}
      } //problem switch
    else //flow shop
      {
	pos.second = SOURCE;
	//find the predecessor in Mashine order
	for(int jj = op.second-1; jj > 0 ;jj--){
	  if(parent.exists(op.first,jj)){
	    pos.second = jj;
	    break;
	  }
	}
	ret = b->insert(op.first, op.second, pos.first, pos.second);
	if ( ret == OK || ret == PERFEKT) {
	  ins.push_back(OpInsertion(op,pos));
	  b->exclude(op.first,op.second);
	}
	else{
	  delete b;
	  b = new B_Node(parent);
	}
      }//end flow shop
  }// end loop posi
  delete b;
}

bool BeamSearch::run(){
  //the list with the schedules to be expanded
  KList *fathers = new KList(beam_width, destObjective, costFunc);
  n_ops = 0;
  //calc number of insertions
  for (int i=1; i<=problem->n; i++)
    for (int j=1; j<=problem->m; j++)
      if((*problem->sij)[i][j])
	n_ops++;
  B_Node *b = new B_Node(problem);   //the root of the tree = an empty schedule
  Operation next;  //next ob to be inserted
  OpInsertion opIns;   //next insertion to perform
  fathers->add(b);   //create the root of the search tree
  TIMETYP objective = 0; //progress display
  //insertion list
  InsertionList ins;
  B_Node* nb; //the one and only node
  for(step = 0; step < n_ops; step++){
    //make output to the progress window
    cout << "OBJECTIVE= "<< objective; 
    cout << " ready " << 100*step / n_ops << "%" << endl;
    //the expanded schedule-list
    KList *children = new KList(beam_width,destObjective,costFunc);
    //iterate over the at most k best from last iteration
    for(int i = 0; i < fathers->in_list; i++){ //loop fathers (i)
      b  = fathers->list[i]; //current node
      objective = (objective > b->getCosts(destObjective,costFunc))?objective:b->getCosts(destObjective,costFunc);
      
      getNextOp(b,next); //which is next operation
      getDescendentInsertions(*b,next,ins); //get insertions for the node and next operation
      while(!ins.empty()){ //process insertions
	OpInsertion opIns = ins.back();
	ins.pop_back();
	//create a new node and insert operation
	nb = new B_Node(*b);
	nb->insert(opIns.first.first,opIns.first.second,opIns.second.first,opIns.second.second);
	//add to the list
	if ((insertionMethod == insert2) && (fathers->full()))
	  children->add(nb,i);
	else
	  children->add(nb);
      }
    } //end loop fathers (i)
    //now all fathers children are merged in children-list - k best are kept
    //fathers not needed anymore 
    delete fathers;
    fathers = children;
  } //end loop ops
  
  b = fathers->list[0];
  //find best schedule
  for(int i = 1; i < fathers->in_list;i++){
    b= (fathers->list[i]->GetValue() < b->GetValue())?b = fathers->list[i]:b;
  }
  value = b->GetValue();
  b->write_LR(result->LR);
  delete fathers;
  return true;
}

