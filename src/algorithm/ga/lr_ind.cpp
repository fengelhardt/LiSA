
#include "./lr_ind.hpp"
#include <functional>

#include "../nb_iter/nb_iter.hpp"
#include <cstdio>

#define MK_OP(i,j,n) ((i)*(n)+(j))
#define GET_I_OP(o,n) ((o)/(n))
#define GET_J_OP(o,n) ((o)%(n))

Lisa_OsProblem    *LR_Individuum::P          = NULL;
Lisa_OsSchedule   *LR_Individuum::S          = NULL;
Lisa_Order        *LR_Individuum::Order      = NULL;
Lisa_Matrix<int>  *LR_Individuum::Cross_Mask = NULL;
int                LR_Individuum::Objective  = CMAX;

#ifdef WIN32
#define NULL_DEVICE "C:\\temp\\redirct.txt"
#else
#define NULL_DEVICE "/dev/null"
#endif

struct STDOUT_BLOCK {

  STDOUT_BLOCK(){
    
    redir.open(NULL_DEVICE);
    strm_buffer = std::cout.rdbuf();
    
    std::cout.rdbuf (redir.rdbuf());
  }
  
  ~STDOUT_BLOCK(){
    
    std::cout.rdbuf (strm_buffer);
  }
  
  std::ofstream redir;
  std::streambuf* strm_buffer;

};


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

void LR_Individuum::setCrossMask(){
  double prob = (*GA_Setup::random)(); // is ~(0,1)
  for(int i = 0; i < P->n; i++)
    for(int j = 0; j < P->m; j++)
      (*Cross_Mask)[i][j] = GA_Setup::random->yes_no(prob);
}



LR_Individuum::LR_Individuum(){
  if(P)
    c = new LR(P->n, P->m);
  else
    c = NULL;
  fitness = -1;
  f_valid = false;
}

LR_Individuum::~LR_Individuum(){
  if(c) delete c;
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
			     const LR_Individuum& i2) {
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
  for(std::vector<SeqOp>::iterator it = SeqOps.begin(); it != SeqOps.end(); ++it){
    bool conflict = false;
    for(int i = 0; !conflict && (i < P->n); i++){ //check column
      conflict |= (i!=(*it).first.first && ((*c)[i][(*it).first.second] == (*it).second));
    }
    for(int j = 0; !conflict && (j < P->m); j++){ //check column
      conflict |= (j!=(*it).first.second && ((*c)[(*it).first.first][j] == (*it).second));
    }
    if(conflict){//add one to all ranks from origanal schedule
      for(int i = 0; i < P->n; i++)
	for(int j = 0; j < P->m; j++)
	  if( (!((*Cross_Mask)[i][j])) && //other plan
	      (((*c)[i][j] > (*it).second) ||  //higher rank
	       (((*c)[i][j] == (*it).second) && ((i==(*it).first.first) || (j==(*it).first.second))))) //equal rank and dependent
	    (*c)[i][j]++;
    }
  }
  latinize();
  //std::cout << "CORSSOVER-RESULT:" << *c << std::endl;
}


void LR_Individuum::mutate(){
  //std::cout << "mutating " << *c << std::endl;
  mutate_rotate();
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

//private function to make a valid latin rectangle
void LR_Individuum::latinize(){
  Order->read(c);
  Order->sort();
  S->clear();
  int i,j;
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
  //determine maximum rank
  int k,l,b = 0;
  do {//maybe endless if there is no op in s_ij
    k = (*GA_Setup::random)(P->n);
    l = (*GA_Setup::random)(P->m);
  }while(!(*P->sij)[k][l]);
  
  for(int i = 0; i < P->n; i++)
    b = std::max<int>(b,(*c)[i][l]);
  for(int j = 0; j < P->m; j++)
    b = std::max<int>(b,(*c)[k][j]);
  b = (*GA_Setup::random)(1,b);
  (*c)[k][l] = b;

  for(int i = 0; i < P->n; i++)
    for(int j = 0; j < P->m; j++){
      if(i==k && j==l) continue;
      if( ((*c)[i][j] > b) ||  //higher rank
	  (((*c)[i][j] == b) && ((i==k) || (j==l)))) //equal rank and dependent
	(*c)[i][j]++;
    }
  
  latinize();
  f_valid = false;
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
  {
    STDOUT_BLOCK b;//make nb_iter shut up
    s.improver.osp_iter(s.Values,Starters,Results);
  }
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
  if(c) delete c;
  c = new LR(*i.c);
  fitness = i.fitness;
  f_valid = i.f_valid;
  return *this;
}

void LR_Individuum::initialize(const Lisa_ScheduleNode& s){
  *c = *s.actual_schedule->LR;
  f_valid = false;
}

void LR_Individuum::initialize(GA_Setup& setup){
  
  //randomly initialize plan
  if(setup.mode == INIT_RANDOM){
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
  }
  else {
    setup.initializer->getPlan(*c,setup.mode);
    if(setup.apply_LocalImpr) 
      improve(setup);
  }
  f_valid = false;
}

  
void LR_Individuum::eval() const{
  if(f_valid) return;
  f_valid = true;
  S->clear();
  makePlan(*S);
  /*
  if(LR_Individuum::Objective == SUM_CI_2){
    TIMETYP cj = 0,ci = 0;
    S->ComputeHeadsTails (true, false);
    for (int i=1;i<=P->n; i++ )
      ci += S->GetHead(i,S->GetMOpred(i,SINK));
    for (int j=1;j<=P->m; j++ )
      cj += S->GetHead(S->GetJOpred(SINK,j),j);
    fitness = std::max(ci,cj);
  }
  */
  S->SetValue(LR_Individuum::Objective);
  fitness = S->GetValue();
}

std::ostream& operator<<(std::ostream& out, const LR_Individuum& i){
  return out << *i.c << "F = " << i.fitness << std::flush;
}

