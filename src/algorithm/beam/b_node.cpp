#include "b_node.hpp"

B_Node::B_Node(B_Node *p):Lisa_OsSchedule(p->problem)
{
  ComputeHeadsTails( true, true);
		costValid = false;
  *this = *p;
  parent = p;
  this->lastAddedRow = p->lastAddedRow;
  this->lastAddedCol = p->lastAddedCol;
}
   
B_Node::B_Node(Lisa_OsProblem * p):Lisa_OsSchedule(p) {
  ComputeHeadsTails( true, true);
		costValid = false;
  parent = 0;
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
    else
      cost = GetHead(lastAddedRow, lastAddedCol)+
	GetTail(lastAddedRow, lastAddedCol) +
	(*(P->time))[lastAddedRow][lastAddedCol];
    costValid = true;
  }
  return cost;
}

ostream & operator << ( ostream & os , B_Node & b){
  Lisa_Matrix<int> *lr = new Lisa_Matrix<int>(b.problem->n,b.problem->m);
  b.write_LR(lr);
  return os << *lr << endl;
} 



KList::KList(int k, int obj, BeamSearch::CostFunc cf) {
  list = new B_Node*[k];
  in_list = 0;
  worst_in_list = -1;
  destObjective = obj;
  costFunc = cf;
  this->k = k;
}


void KList::add(B_Node *n) 
{
  if(in_list < k)
    {
      list[in_list]= n; // insert at the end
      if((worst_in_list < 0) || (n->getCosts(destObjective, costFunc) > list[worst_in_list]->getCosts(destObjective, costFunc)))
	worst_in_list = in_list;
      in_list++;
      return;
    }
  if(n->getCosts(destObjective, costFunc) >=list[worst_in_list]->getCosts(destObjective, costFunc)) {
    delete n;
    return;
  }
  delete list[worst_in_list];
  list[worst_in_list] = n;
  for(int i = 0; i<k;i++)
    if(list[worst_in_list]->getCosts(destObjective, costFunc) < list[i]->getCosts(destObjective, costFunc))
      worst_in_list = i;
}

void KList::add(B_Node *n,  int pos) 
{
  if (in_list > pos)
    {
      if (list[pos]->getCosts(destObjective, costFunc) > n->getCosts(destObjective, costFunc)) {
	delete list[pos];
	list[pos] = n;
      } else {
	delete n;
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
