#ifndef _LISA_GA__H_
#define _LISA_GA__H_

#include "./ga_setup.hpp"
//#include "./lo_ind.hpp"
#include "./lr_ind.hpp"
#include <numeric>
#include <set>

template <class Ind>
class GA {

public:
  
  GA_Setup setup;
  bool * canceled;
  Lisa_List<Lisa_ScheduleNode> *initials;
  std::vector<int> perm;
  
  bool readSetup(LisaXmlFile& input){
    if(setup.init(input)){
      if(input.getDocumentType() == LisaXmlFile::SOLUTION){
	if(initials) delete initials;
	initials = new Lisa_List<Lisa_ScheduleNode>();
	input >> *initials;
	if(!input || initials->empty()) {
	  delete initials;
	  initials = NULL;
	  return false;
	}
	initials->reset();
      }
      return true;
    }
    return false;
  }

  GA(){
    canceled = NULL;
    initials = NULL;
  }

  ~GA(){
    if(initials) delete initials;
  }
  
  //algorithm completed ?
  bool done(){ return (canceled && *canceled) || ((setup.n_gen  - gen) <= 0);}

  void print_progress(){
    const int MAX_CLAIMS = 400;
    const int mm = setup.n_gen/MAX_CLAIMS;
    if(mm<2 || !(gen%mm)){
      std::cout << "steps= " << setup.n_gen-gen;
      std::cout << " OBJECTIVE= " << getBest().getFitness();
      std::cout << " best= " << getBest().getFitness();
      std::cout << " worst= " << getWorst().getFitness();
      std::cout << " ready " << 100*gen /setup.n_gen  << "%" << std::endl;
    }
  }

  //start the GA
  void run(){
    Ind::init(*setup.problem, setup.objective);
    init_pop();
    eval_pop();
    while(!done()){
      print_progress();
      select_pop();
      alter_pop();
      eval_pop();
      //std::cout << "Best  = " << best  - population.begin() << " value = " << best->getFitness() << std::endl;
      //std::cout << "Worst = " << worst - population.begin() << " value = " << worst->getFitness() << std::endl;
    }
    //std::cout << "---------- DONE -----------" << std::endl;
    std::copy(population.begin(),population.end(), std::ostream_iterator<Ind>(std::cout, "\n"));
  }

  const Ind& getBest(){
    return best;
  }

  const Ind& getWorst(){
    return worst;
  }
  
private:
  
  typedef std::vector<Ind> PopType;
  PopType population;
  PopType intermediates;
  
  Ind best;
  Ind worst;

  TIMETYP sum_abs;

  int gen;
  
  //initialize population 
  void init_pop(){
    gen = 0;
    population = PopType(setup.pop_size);
    perm = std::vector<int>(setup.pop_size);
    for(int i = 0; i < signed(perm.size()); i++) perm[i]=i;
    typename PopType::iterator it = population.begin();
    if(initials && setup.pop_size > 0)
      do {
	it->initialize(initials->get());
	it++;
      }while(initials->next() && it != population.end());
    
    for(; it != population.end(); ++it)
      it->initialize(setup);
    
    intermediates.clear();
  }
  
  //select 

  void select_pop(){
    select_turnament();
    //select_turnament_with_stochastic_sampling();
    //select_ranking();
  }
  
  //CAUTION - THIS DOES NOT WORK ---- FIXME !!! Jan
  void select_turnament_with_stochastic_sampling(){
    //bring individual in random order
    std::random_shuffle(perm.begin(),perm.end(),(*GA_Setup::random));
    
    for(int i = 0; i < setup.pop_size; i+=2){ //create two offspring from adjacent pairs
      int j = i+1;
      while( j>=setup.pop_size || j == i) j = (*GA_Setup::random)(setup.pop_size);
      
      Ind p1 = population[i];
      Ind p2 = population[j];
      
      //mutate/crossover
      if(GA_Setup::random->yes_no(setup.sel_params.p_combine)) {
	if(GA_Setup::random->yes_no(setup.sel_params.p_mutate))
	  p1.mutate();
	if(GA_Setup::random->yes_no(setup.sel_params.p_mutate))
	  p2.mutate();
	Ind::combine(p1,p2,  setup, std::back_inserter(intermediates));
      }
      else{
	p1.mutate();
	p2.mutate();
	intermediates.push_back(p1);
	intermediates.push_back(p2);
      }
      
      if(setup.apply_LocalImpr){
	(intermediates.rbegin()  )->improve(setup);
	(intermediates.rbegin()+1)->improve(setup);
      }
      
      intermediates.push_back(population[i]);
      intermediates.push_back(population[j]);
      
      //unisample on the four individuals
      TIMETYP F[4];
      for(int p = 0; p < 4; p++) F[p] = (intermediates.rbegin()+p)->getFitness();
      TIMETYP min = *std::min_element(F,F+4);
      TIMETYP max = *std::max_element(F,F+4);
      TIMETYP pressure = double(gen)/double(setup.n_gen);
      for(int p = 0; p < 4; p++) F[p] = ((max-min)!=0.0)?(1.0 - ((F[p] - min)/(max-min))):1.0;
      std::set<int> selected_set;

      while(selected_set.size() < 2){
	for(int p = 0; p<4; p++){
	  double prob = 0.5 + 0.5*pressure*F[p];
	  std::cout << "prop = " << prob << std::endl;
	  if(GA_Setup::random->yes_no(0.5 + 0.5*pressure*F[p]))
	    selected_set.insert(p);
	}
      }
      std::vector<int> selected(selected_set.size());
      std::copy(selected_set.begin(),selected_set.end(),selected.begin());
      std::sort(selected.begin(),selected.end());
      for(int p = 0; p < signed(selected.size()); p++){
	std::cout << "offset = "  << 4-selected.size()+p << std::endl;
	std::cout << *(intermediates.rbegin()+4-selected.size()+p) << std::endl;
	  *(intermediates.rbegin()+4-selected.size()+p) = *(intermediates.rbegin()+selected[p]);
      }
      
      
      
      //keep the best two solutions
	  
      std::sort(intermediates.rbegin()+4-selected.size(),intermediates.rbegin()+4,std::greater<Ind>());
      intermediates.pop_back();intermediates.pop_back();
    }
    //leave space for elitist strategy
    if(signed(intermediates.size()) == signed(setup.pop_size)) 
      intermediates.pop_back();
  }
  

  void select_ranking(){
   
  }


  void select_turnament(){
    while(signed(intermediates.size()) < signed(setup.pop_size - 1)){
      int i1, i2;
      do {
	i1 = (*GA_Setup::random)(setup.pop_size);
	i2 =  (*GA_Setup::random)(setup.pop_size);
      } while(i1!=i2);
      
      Ind p1 = population[i1];
      Ind p2 = population[i2];
      
      //mutate/crossover
      if(GA_Setup::random->yes_no(setup.sel_params.p_combine)) {
	if(GA_Setup::random->yes_no(setup.sel_params.p_mutate))
	  p1.mutate();
	if(GA_Setup::random->yes_no(setup.sel_params.p_mutate))
	  p2.mutate();
	Ind::combine(p1,p2,  setup, std::back_inserter(intermediates));
      }
      else{
	p1.mutate();
	p2.mutate();
	intermediates.push_back(p1);
	intermediates.push_back(p2);
      }
      
      if(setup.apply_LocalImpr){
	(intermediates.rbegin()  )->improve(setup);
	(intermediates.rbegin()+1)->improve(setup);
      }
      
      intermediates.push_back(population[i1]);
      intermediates.push_back(population[i2]);
  
      //keep the best two solutions
      std::sort(intermediates.rbegin(),intermediates.rbegin()+4,std::greater<Ind>());
      intermediates.pop_back();intermediates.pop_back();
    }
    //leave space for elitist strategy
    if(signed(intermediates.size()) == signed(setup.pop_size)) 
      intermediates.pop_back();
  }
    
    //replace the current generation by its descendents
    void alter_pop(){
      intermediates.push_back(best); //elitist
      population=intermediates;
      intermediates.clear();
      gen++;
    }

  //evaluate the population
public: void eval_pop(bool force = false){
    if(force)
      for(typename PopType::iterator it = population.begin(); it != population.end(); ++it)
	it->forgetFitness();
	
    typename PopType::iterator b_it = population.begin(), w_it = population.begin();
    sum_abs = 0;
    for(typename PopType::iterator it = population.begin(); it != population.end(); ++it){
      if((b_it->getFitness()  > it->getFitness())) b_it = it;
      if((w_it->getFitness() < it->getFitness()))  w_it = it;
      sum_abs += it->getFitness();
    }
    if(b_it == population.end()) return;
    best = *b_it;
    worst = *w_it;

  }
  

};
 


#endif
