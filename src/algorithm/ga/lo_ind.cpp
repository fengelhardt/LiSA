
#include "./lo_ind.hpp"
#include <functional>

#define MK_OP(i,j,n) ((i)*(n)+(j))
#define GET_I_OP(o,n) ((o)/(n))
#define GET_J_OP(o,n) ((o)%(n))

int LO_Individuum::N_OPS = 0;
int LO_Individuum::N = 0;
int LO_Individuum::M = 0;
int LO_Individuum::P_SIZE = 0;

std::vector<Op> LO_Individuum::OPS;
std::vector<short> LO_Individuum::PERM;

void LO_Individuum::init(const Lisa_OsProblem& P){
  OPS.clear();
  N = P.n;
  M = P.m;
  P_SIZE = N*M;
  for(int i = 0; i < P.n; i++)
    for(int j = 0; j < P.m; j++)
      if( (*P.sij)[i+1][j+1])
	OPS.push_back(MK_OP(i,j,N));
  N_OPS = OPS.size();
  PERM=std::vector<short>(P_SIZE,false);
}

LO_Individuum::LO_Individuum(){
  c = OPS;
  fitness = -1;
}

LO_Individuum::LO_Individuum(const LO_Individuum& i){
  c = i.c;
  fitness = i.fitness;
}

LO_Individuum::LO_Individuum(LO_Individuum& i){
  c = i.c;
  fitness = i.fitness;
}


void LO_Individuum::combine(const LO_Individuum& i2){
  
  int i = (*GA_Setup::random)(N_OPS);
  int j = (*GA_Setup::random)(N_OPS);
  int p = 0;
  if(j<i)std::swap(i,j);
  //std::cout << "----- crossing at [" << i << "," <<j << "]" << std::endl;
  //std::cout << '\t' << *this << std::endl;
  //std::cout << "x\t" << i2 << std::endl;
  std::fill(PERM.begin(),PERM.end(),false);
  fitness = -1;
  for(p = i; p <= j; p++)
    PERM[c[p]]=true;
  p = 0; //postion to sequence
  for(int pp = 0; pp < N_OPS; pp++){
    if(PERM[i2.c[pp]]) continue;
    if(p==i) p = j+1;
    c[p++] = i2.c[pp];
  }
  //std::cout << '\t' << *this << std::endl;
}

void LO_Individuum::mutate(){mutate_swap();}


//mutation
void LO_Individuum::mutate_swap(){
  int i = (*GA_Setup::random)(N_OPS);
  int j = (*GA_Setup::random)(N_OPS);
  std::swap(c[i], c[j]);
}

void LO_Individuum::mutate_rotate(){
  int pos[3] = { (*GA_Setup::random)(N_OPS),(*GA_Setup::random)(N_OPS),(*GA_Setup::random)(N_OPS)};
  std::sort(pos,pos+3);
  std::rotate(c.begin() + pos[0],c.begin() + pos[1],c.begin() + pos[2]);
}


void LO_Individuum::improve(GA_Setup&){
  
}


LO_Individuum& LO_Individuum::operator=(const LO_Individuum& i){
  c = i.c;
  fitness = i.fitness;
  return *this;
}

LO_Individuum& LO_Individuum::operator=(LO_Individuum& i){
  c = i.c;
  fitness = i.fitness;
  return *this;
}

void LO_Individuum::initialize(){
  std::random_shuffle(c.begin(),c.end(),*GA_Setup::random);

}


void LO_Individuum::makePlan(Lisa_OsSchedule& plan) const {
  int i,j;
  for(LO_Individuum::Chromosome::const_iterator it = c.begin(); it != c.end(); it++){
    i = GET_I_OP(*it,N)+1;
    j = GET_J_OP(*it,N)+1;
    plan.insert(i,j,plan.GetJOpred(SINK,j),plan.GetMOpred(i,SINK));
  }
}

std::ostream& operator<<(std::ostream& out, const LO_Individuum& i){
    out << "[";
    for(LO_Individuum::Chromosome::const_iterator it = i.c.begin(); it != i.c.end(); it++)
      out << *it << ',';
    out << "] : " << i.fitness;
    return out << std::flush;
}

