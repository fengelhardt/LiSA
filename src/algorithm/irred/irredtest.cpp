/**
 * @author  Marc Moerig
 * @version 3.0
 */

#include "irredtest.hpp"

using namespace std;

//**************************************************************************

Lisa_IrreducibilityTest::Lisa_IrreducibilityTest(Lisa_MatrixGraph* disjkt_in){
  vert = disjkt_in->get_vertices();
  
  disjkt = new Lisa_MatrixGraph(*disjkt_in);
  
  result = 0;
}

//**************************************************************************

Lisa_IrreducibilityTest::~Lisa_IrreducibilityTest(){
  delete disjkt;
}

//**************************************************************************

void Lisa_IrreducibilityTest::set_output_to(Lisa_IrredResult* res){
  result = res;
}

//**************************************************************************

bool Lisa_IrreducibilityTest::test(Lisa_MatrixGraph* plan, Lisa_MatrixGraph* comp,
const int param){
  
  int edges = 0;
  int a,b,c;
  
  // count arcs
  for (int i=1;i<=vert;i++)
    for (int j=i+1;j<=vert;j++)
      if(plan->get_connection(i,j)) edges++;
  
  //lookup[i][j] contains the number of the arc from vertice i to vertice j
  //start_v[a] and end_v[a] contain the the number of the start and end vertice
  //from arc a 
  Lisa_Vector<int> start_v(edges+1);
  Lisa_Vector<int> end_v(edges+1);
  Lisa_Matrix<int> lookup(vert+1,vert+1);
  
  a = 1;
  for (int i=1;i<=vert;i++){
    for (int j=i+1;j<=vert;j++){
      b = plan->get_connection(i,j);
      if(b==Lisa_MatrixGraph::ARC){  // there should be only ARC and CRA
        lookup[i][j] = a;
        start_v[a]=i;
        end_v[a++]=j;
      }else if(b==Lisa_MatrixGraph::CRA){ 
        lookup[j][i] = a;
        start_v[a]=j;
        end_v[a++]=i;
      }
    }
  }
  
  //cout << *start_v;
  //cout << *end_v;
  //cout << lookup;
  
  // create classes
  Lisa_Partition party(edges+1);
  for (int i=1;i<=edges;i++){
    a = start_v[i];
    b = end_v[i];
    
    plan->init_successor(a);
    c = plan->next_successor(a);
    while(c!=vert+1){
      if (comp->get_connection(c,b)==Lisa_MatrixGraph::NONE){
        party.join(i,lookup[a][c]);
      }
      c = plan->next_successor(a);
    }
    
    plan->init_predecessor(b);
    c = plan->next_predecessor(b);
    while(c!=vert+1){
      if (comp->get_connection(c,a)==Lisa_MatrixGraph::NONE){
        party.join(i,lookup[c][b]);
      }
      c = plan->next_predecessor(b);
    } 
  }
  
  int num = party.get_num_of_partitions()-1;
  
  // extract implication classes from the partition object
  // sort for size and check for turnability
  // turnable classes will be inserted in the implic list
  int currpos;
  Lisa_Vector<int>* currvec;
  Lisa_List<Lisa_Vector<int>*>* implic = new Lisa_List<Lisa_Vector<int>*>();
  Lisa_Vector<int> curr(edges+1);
  
  Lisa_Order order(num);
  bool turnable;
  bool all = 1;
  
  static long seed = time(0);
  
  for (int i=1;i<=num;i++) order.read_one_key(i-1,party.get_partition_size(i));
  order.sort();
  
  for (int i=0;i<num;i++){
    party.get_partition(order[i]+1,&curr);
    currpos = curr[0];
    turnable = 1;
    for (int j=1;j<=currpos;j++){
      if (disjkt->get_connection(start_v[curr[j]],end_v[curr[j]])!=Lisa_MatrixGraph::EDGE){
        turnable = 0;
        all = 0;
        break;
      }
    }
    if(turnable){
      currvec = new Lisa_Vector<int>(currpos);
      for (int j=0;j<currpos;j++){
        (*currvec)[j] = curr[j+1];
      }
      if(param == JUST_TEST_RANDOM){
        int length = implic->length();
        if (length==0) implic->append(currvec);
        else{
          int pos = lisa_random(0,length-1,&seed);
          implic->locate(pos);
          implic->insert(currvec);
        }
      }else implic->append(currvec);
    }
  }
  
  Lisa_MatrixGraph newplan(plan);
  num = implic->length();
  Lisa_Vector<bool> turned(num);
  Lisa_Vector<int> topsortvec(vert);
  turned.fill(0);
  
  double count = 0;
  double oldcount = count;
  double tocheck;
  if(all && param!=GENERATE_SIMILAR) tocheck = pow(2. , (double) num-1)-1;
  else tocheck = pow(2. ,(double) num)-1;
  double step = tocheck/PROGRESS_INDICATOR_STEPS;
  
  cout << "OBJECTIVE= 1 " << count << "/" << tocheck << endl;
  
  bool irre = 1;
  for(;;){ 
    
    if(oldcount+step<count){
      oldcount = count;
      cout << "OBJECTIVE= 1 " << count << "/" << tocheck << endl;
    }
    count++;
    
    // go through all combinations of implication classes
    // turn them  .. eg create a new combination by changing the newplan graph
    implic->reset();
    for (int i=0;;i++){
      
      if (all && param!=GENERATE_SIMILAR){
        if (i==num-1) goto ready_to_go; // yeah we've made it through all combinations
      }else{
        if (i==num) goto ready_to_go;
      }
      
      currvec = implic->get();
      
      if (turned[i]==1){
        turned[i]=0;
        for (int j=0;j<currvec->get_m();j++){
          currpos = (*currvec)[j];
          newplan.remove_arc(end_v[currpos],start_v[currpos]);
          newplan.insert_arc(start_v[currpos],end_v[currpos]);
        }
      }else{
        turned[i]=1;
        for (int j=0;j<currvec->get_m();j++){
          currpos = (*currvec)[j];
          newplan.remove_arc(start_v[currpos],end_v[currpos]);
          newplan.insert_arc(end_v[currpos],start_v[currpos]);
        }
        break;
      }
      implic->next();
    }
    
    if (Lisa_GraphAlg::topsort(&newplan,&topsortvec)){  // check whether the newplan graph contains a cycle
      
      Lisa_MatrixGraph newcomp(vert);
      Lisa_GraphAlg::build_compgraph(&newplan,&newcomp);
      
      if (Lisa_GraphAlg::smaller(&newcomp,comp)){ // does it reduce the original plan ?
        if (result){ // ok, output is wanted
          if (param!=GENERATE_SIMILAR) result->insert(&newplan,&newcomp);
          irre = 0;
          if (param==JUST_TEST||param==JUST_TEST_RANDOM) goto ready_to_go;
        }else{ // no output wanted ?? .. test done !!
          irre = 0;
          goto ready_to_go;
        }
      }else if(param==GENERATE_SIMILAR){
        if(result && Lisa_GraphAlg::equal(&newcomp,comp)){
          result->insert(&newplan,&newcomp);
        }
      }
    }
  }
  
  ready_to_go:
  
  
  // cleanup implication classes
  if (num>0){
    implic->reset();
    do{
      currvec = implic->exclude();
      delete currvec;
    }while(!implic->empty());
  }
  delete implic;
  
  return irre;
}

//**************************************************************************

void Lisa_IrreducibilityTest::write(ostream& strm) const{
  strm << "<IrreducibilityTest>" << endl;
  strm << *disjkt;
  if (result) strm << *result;
  strm << "</IrreducibilityTest>" << endl;
}

//**************************************************************************

