#ifndef _LISA_GA__H_
#define _LISA_GA__H_

#include "./ga_setup.hpp"
//#include "./lo_ind.hpp"
#include "./lr_ind.hpp"

template <class Ind>
class GA {

public:
  
  GA_Setup setup;
  
  bool readSetup(LisaXmlFile& input){ return setup.init(input);}

  //algorithm completed ?
  bool done(){ return (setup.n_gen  - gen) <= 0;}

  void print_progress(){
    const int MAX_CLAIMS = 400;
    const int mm = setup.n_gen/MAX_CLAIMS;
    if(mm<2 || !(gen%mm)){
      std::cout << "steps= " << setup.n_gen-gen;
      std::cout << " OBJECTIVE= " << getBest().getFitness();
      std::cout << " best= " << getBest().getFitness();
      std::cout << " ready " << 100*gen /setup.n_gen  << "%" << std::endl;
    }
  }

  //start the GA
  void run(){
    Ind::init(*setup.problem, setup.Problem.get_property(OBJECTIVE));
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
    //std::copy(population.begin(),population.end(), std::ostream_iterator<Ind>(std::cout, "\n"));
  }

  const Ind& getBest(){
    return *best;
  }

  const Ind& getWorst(){
    return *worst;
  }
  
private:
  
  typedef std::vector<Ind> PopType;
  PopType population;
  PopType intermediates;
  
  typename PopType::iterator best;
  typename PopType::iterator worst;

  TIMETYP sum_abs;

  int gen;
  
  //initialize population 
  void init_pop(){
    gen = 0;
    population = PopType(setup.pop_size);
    for_each(population.begin(), population.end(), mem_fun_ref(&Ind::initialize));
    intermediates.clear();
    if(setup.apply_LocalImpr) 
      //the following does not work, don't know why (stl bug/design issue, I guess)
      //for_each(population.begin(), population.end(), std::bind2nd(mem_fun(&Ind::improve),setup));
      for(typename PopType::iterator it = population.begin(); it != population.end(); ++it) it->improve(setup);
  }
  
  //select 
  void select_pop(){
    while(signed(intermediates.size()) < signed(setup.pop_size - 1)){
      int i1 = (*GA_Setup::random)(setup.pop_size), i2 =  (*GA_Setup::random)(setup.pop_size);
      
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
    if(signed(intermediates.size()) == signed(setup.pop_size)) intermediates.pop_back();
  }
    
    //replace the current generation by its descendents
    void alter_pop(){
      intermediates.push_back(*best); //elitist
      population=intermediates;
      intermediates.clear();
      gen++;
    }

  //evaluate the population
  void eval_pop(){
    worst = best = population.begin();
    sum_abs = 0;
    for(typename PopType::iterator it = population.begin(); it != population.end(); ++it){
      if((*best  > *it)) best = it;
      if((*worst < *it)) worst = it;
      sum_abs += it->getFitness();
    }
  }
  

};
 


#endif
