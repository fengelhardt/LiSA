#include "b_node.hpp"

B_Node::B_Node(B_Node& p):Lisa_OsSchedule(p.problem)
{
  problem = p.problem;
  ComputeHeadsTails( true, true);
  *this = p;
  costValid = false;
  //parent = p;
  this->lastAddedRow = p.lastAddedRow;
  this->lastAddedCol = p.lastAddedCol;
}
   
B_Node::B_Node(Lisa_OsProblem * p):Lisa_OsSchedule(p) {
  ComputeHeadsTails( true, true);
  costValid = false;
  //parent = 0;
  problem = p;
}

int B_Node::insert(int i, int j, int k, int l) {
  this->lastAddedRow = i;
  this->lastAddedCol = j;
  costValid = false;
  return Lisa_OsSchedule::insert(i,j,k,l);
}

TIMETYP B_Node::getCosts(int destObjective,  BeamSearch::CostFunc costFunc){
  if ( ! costValid ) {
    SetValue(destObjective);
    if (costFunc == BeamSearch::CObjective)
      cost = GetValue();
    else if (costFunc == BeamSearch::CLast)
      cost = GetHead(lastAddedRow, lastAddedCol)+GetTail(lastAddedRow, lastAddedCol) +(*(P->time))[lastAddedRow][lastAddedCol];
    else if (costFunc == BeamSearch::CLb_Sum_Ci){
      cost = 0.0;
      for (int i=1; i<=P->n; i++){
	cost += GetHead(i,SINK);
	for (int j=1; j<=P->m; j++)
	  if(((*P->sij)[i][j]) &&  !exists(i,j))
	    cost += (*(P->time))[i][j]; //sum up remaing op times
      }
    }
    costValid = true;
  }
  return cost;
}

B_Node::~B_Node(){

}

ostream & operator << ( ostream & os , B_Node & b){
  Lisa_Matrix<int> *lr = new Lisa_Matrix<int>(b.P->n,b.P->m);
  b.write_LR(lr);
  os << *lr << endl;
  delete lr;
  return os;

} 



KList::KList(int k, int obj, BeamSearch::CostFunc cf) {
  list = new B_Node*[k];
  in_list = 0;
  worst_in_list = -1;
  destObjective = obj;
  costFunc = cf;
  this->k = k;
}


void KList::add(B_Node*& n) 
{
  if(in_list < k)
    {
      list[in_list]= n; // insert at the end
      if((worst_in_list < 0) || (n->getCosts(destObjective, costFunc) > list[worst_in_list]->getCosts(destObjective, costFunc)))
	worst_in_list = in_list;
      in_list++;
      return;
    }
  if(n->getCosts(destObjective, costFunc) >= list[worst_in_list]->getCosts(destObjective, costFunc)) {
    delete n;
    n = NULL;
    return;
  }
  delete list[worst_in_list];
  list[worst_in_list] = n;
  for(int i = 0; i<k;i++)
    if(list[worst_in_list]->getCosts(destObjective, costFunc) < list[i]->getCosts(destObjective, costFunc))
      worst_in_list = i;
}

void KList::add(B_Node*& n,  int pos) 
{
  if (in_list > pos)
    {
      if (list[pos]->getCosts(destObjective, costFunc) > n->getCosts(destObjective, costFunc)) {
	delete list[pos];
	list[pos] = n;
      } 
      else {
	delete n;
	n = NULL;
      }
    } 
  else {
    list[in_list] = n;
    in_list++;
  }
}



KList::~KList() {
  for (int i= 0; i < in_list; i++)
    delete list[i]; 
  delete[] list;
}
