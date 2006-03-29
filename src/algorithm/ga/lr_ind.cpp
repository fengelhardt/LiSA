
#include "./lr_ind.hpp"
#include <functional>

#include "../nb_iter/nb_iter.hpp"


#define MK_OP(i,j,n) ((i)*(n)+(j))
#define GET_I_OP(o,n) ((o)/(n))
#define GET_J_OP(o,n) ((o)%(n))

Lisa_OsProblem    *LR_Individuum::P          = NULL;
Lisa_OsSchedule   *LR_Individuum::S          = NULL;
Lisa_Order        *LR_Individuum::Order      = NULL;
Lisa_Matrix<int> *LR_Individuum::Cross_Mask = NULL;
int                LR_Individuum::Objective  = CMAX;

void LR_Individuum::init(const Lisa_OsProblem& Problem, int o){
  if(P) delete P;
  if(S) delete S;
  if(Order) delete Order;
  if(Cross_Mask) delete Cross_Mask;
  P = new Lisa_OsProblem(Problem);
  S = new Lisa_OsSchedule(P);
  Order = new Lisa_Order(P->n,P->m);
  Cross_Mask = new Lisa_Matrix<int>(P->n,P->m);
  Objective = o;
}

void LR_Individuum::setCrossMask(double prob){
  for(int i = 0; i < P->n; i++)
    for(int j = 0; j < P->m; j++)
      (*Cross_Mask)[i][j] = GA_Setup::random->yes_no(prob);
}



LR_Individuum::LR_Individuum(){
  c = new LR(P->n, P->m);
  fitness = -1;
  f_valid = false;
}

LR_Individuum::~LR_Individuum(){
  delete c;
  f_valid = false;
}

LR_Individuum::LR_Individuum(const LR_Individuum& i){
  c = new LR(*i.c);
  fitness = i.fitness;
  f_valid = i.f_valid;
}

LR_Individuum::LR_Individuum(LR_Individuum& i){
  c = new LR(*i.c);
  fitness = i.fitness;
  f_valid = i.f_valid;
}

typedef std::pair<int,int> SeqPos;
typedef std::pair< SeqPos ,int > SeqOp;

struct rank_sort : public std::less<SeqOp> {
  bool operator()(const SeqOp& a, const SeqOp& b){
    return a.second < b.second;
  }
};

//crossover_constructor
LR_Individuum::LR_Individuum(const LR_Individuum& i1, 
			     const LR_Individuum& i2){
  //std::cout << "Crossing over " << std::endl;
  //std::cout << *i1.c << " X " << *Cross_Mask << " X " << *i2.c << std::endl;
  c = new LR(*i1.c);
  fitness = -1;
  f_valid = false;
  std::vector<SeqOp> SeqOps;
  for(int i = 0; i < P->n; i++){
    for(int j = 0; j < P->m; j++){
      if((*Cross_Mask)[i][j]){
	(*c)[i][j] = (*i2.c)[i][j];
	SeqOps.push_back(SeqOp(SeqPos(i,j),(*c)[i][j]));
      }
    }
  }
  //sort by increasing ranks
  std::sort(SeqOps.begin(),SeqOps.end(), rank_sort());
  //we have to fix the ranks here
  //std::cout << "working on " << *c << std::endl;
  for(unsigned  p=0; p < SeqOps.size(); p++){
    bool conflict = false;
    for(int i = 0; !conflict && (i < P->n); i++){ //check column
      conflict |= (i!=SeqOps[p].first.first && ((*c)[i][SeqOps[p].first.second] == SeqOps[p].second));
    }
    for(int j = 0; !conflict && (j < P->m); j++){ //check column
      conflict |= (j!=SeqOps[p].first.second && ((*c)[SeqOps[p].first.first][j] == SeqOps[p].second));
    }
    if(conflict){//add one to all ranks from origanal schedule
      for(int i = 0; i < P->n; i++)
	for(int j = 0; j < P->m; j++)
	  if(!(*Cross_Mask)[i][j])
	    (*c)[i][j]++;
    }
  }
}


void LR_Individuum::mutate(){
  //std::cout << "mutating " << *c << std::endl;
  mutate_swap();
  //std::cout << "mutating result is " << *c << std::endl;
}


//mutation
void LR_Individuum::mutate_swap(){
  Order->read(c);
  int i = (*GA_Setup::random)(P->n);
  int j = (*GA_Setup::random)(P->m);
  int k = (*GA_Setup::random)(P->n);
  int l = (*GA_Setup::random)(P->m);
  while( (i==k) && (j==l) && ((P->n > 1) || (P->m > 1))){
    k = (*GA_Setup::random)(P->n);
    l = (*GA_Setup::random)(P->m);
  }
  Order->read_one_key(i,j,(*c)[k][l]);
  Order->read_one_key(k,l,(*c)[i][j]);
  Order->sort();
  S->clear();
  for(int h = 0; h < P->n*P->m; h++)
    {
      i = 1 + Order->row(h); j = 1+ Order->col(h);
      if((*P->sij)[i][j])
	S->insert(i,j,S->GetJOpred(SINK,j),S->GetMOpred(i,SINK));
    }
  S->write_LR(c);
  f_valid = false;
}

void LR_Individuum::mutate_rotate(){
  //not implemented
}


void LR_Individuum::improve(GA_Setup& s){
  //std::cout << "IMPROVING..." << *c << std::endl;
  Lisa_List<Lisa_ScheduleNode> Starters;
  Lisa_List<Lisa_ScheduleNode> Results;
  Lisa_Schedule sched(P->n,P->m);
  sched.make_LR();
  *(sched.LR) = *c;
  Lisa_ScheduleNode node(&sched);
  Starters.append(node);
  Starters.reset();
  s.improver.osp_iter(s.Values,Starters,Results);
  Results.reset();
  *c = *(Results.get().actual_schedule->LR);
  f_valid = false;
  //std::cout << "NOW IT IS ..." << *c << std::endl;
}

 
LR_Individuum& LR_Individuum::operator=(const LR_Individuum& i){
  delete c;
  c = new LR(*i.c);
  fitness = i.fitness;
  f_valid = i.f_valid;
  return *this;
}


LR_Individuum& LR_Individuum::operator=(LR_Individuum& i){
  delete c;
  c = new LR(*i.c);
  fitness = i.fitness;
  f_valid = i.f_valid;
  return *this;
}

void LR_Individuum::initialize(){
  //randomly initialize plan
  S->clear();
  const int M = std::max<int>(P->n,P->m);
  int i,j,h;
  for(i = 0; i < P->n; i++)
    for(j = 0; j < P->m; j++)
      Order->read_one_key(i,j,(*GA_Setup::random)(1,M*M));
  Order->sort();
  for(h = 0; h < P->n*P->m; h++)
    {
      i = 1+ Order->row(h); j = 1+ Order->col(h);
      if((*P->sij)[i][j])
	S->insert(i,j,S->GetJOpred(SINK,j),S->GetMOpred(i,SINK));
    }
  S->write_LR(c);
  f_valid = false;
}


std::ostream& operator<<(std::ostream& out, const LR_Individuum& i){
  return out << *i.c << "F = " << i.fitness << std::flush;
}

