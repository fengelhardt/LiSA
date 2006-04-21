#include "./ga_setup.hpp"

selection_params::selection_params(){
  p_mutate = 0.25;
  p_combine = 0.35;
}


//maybe we make this a parameter later
long GA_Setup::SEED = 1234567890;
LisaRandGenerator* GA_Setup::random = new LisaRandGenerator(&GA_Setup::SEED);

GA_Initializer::GA_Initializer(const GA_Setup* setup): Lisa_Dispatcher() {
  target_schedule = new Lisa_Schedule(setup->Values.get_n(),setup->Values.get_m());
  target_schedule->make_LR();
  SetProblem(&setup->Problem,&setup->Values,target_schedule);
  SetRule("RAND");
}

GA_Initializer::~GA_Initializer(){
  delete target_schedule;
}

void GA_Initializer::getPlan(Lisa_Matrix<int>& p, GA_INIT_MODE m){
  if(m==INIT_DISP_ACTIVE){
    dispatch_active();
    p = *target_schedule->LR;
    return;
  }
  dispatch_nondelay();
  p = *target_schedule->LR;
}

GA_Setup::GA_Setup(){
  problem = NULL;
  schedule = NULL;
  initializer = NULL;
  apply_LocalImpr = false;
  mode = INIT_DISP_NON_DELAY;
  clear();
}

void GA_Setup::clear(){
  if(problem) delete problem; 
  if(schedule) delete schedule; 
  if(initializer) delete initializer; 
  problem = NULL;
  schedule = NULL;
  initializer = NULL;
}



bool GA_Setup::init(LisaXmlFile& xmlInput){
  //get Problem
  clear();
  if( !(xmlInput >> Problem))
    {
      std::cout << "ERROR: cannot read ProblemType , aborting program." << std::endl;
      return false;
    }
  //get ControlParameters
  if( !(xmlInput >> Parameter))
    {
      std::cout << "ERROR: cannot read ControlParameters , aborting program." << std::endl;
      return false;
    }
  //get Values
  if( !(xmlInput >> Values))
    {
      std::cout << "ERROR: cannot read Values , aborting program." << std::endl;
      return false;
    }
  // if something else went wrong
  if (!G_ExceptionList.empty())
    {
      std::cout << "ERROR: cannot read input , aborting program." << std::endl;
      return false;
    }
  
  //Lisa_ProblemType problemtype = Problem.get_property(M_ENV);
  
  std::string cannot_handle="";
  if (Problem.get_property(PMTN)) cannot_handle="preemption";
  if (Problem.get_property(PRECEDENCE)!=EMPTY) 
    cannot_handle="precedence constraints"; 
  if (Problem.get_property(BATCH))  cannot_handle="batching"; 
  if (Problem.get_property(NO_WAIT))  cannot_handle="no-wait constraints";
  if (cannot_handle!=""){
    std::cout << "ERROR: ga cannot handle " << cannot_handle << 
      ". Aborting program."<< std::endl;
    return false;
  }  
    
  if (!Parameter.defined("POP_SIZE")) {
    std::cout << "WARNING: \"POP_SIZE\" undefined. Using default " << pop_size << std::endl;
    //return false;
  }
  else {
    pop_size = Parameter.get_long("POP_SIZE");
  }

  if (!Parameter.defined("NUM_GEN")) {
    std::cout << "WARNING: \"NUM_GEN\" undefined. Using default " << n_gen << std::endl;
    //return false;
  }
  else {
    n_gen = Parameter.get_long("NUM_GEN");
  }
  if (!Parameter.defined("M_PROB")) {
    std::cout << "WARNING: \"M_PROB\" undefined. Using default " << sel_params.p_mutate  << std::endl;
    //return false;
  }
  else {
    sel_params.p_mutate = Parameter.get_double("M_PROB");
  }
  if (!Parameter.defined("C_PROB")) {
    std::cout << "WARNING: \"C_PROB\" undefined. Using default " << sel_params.p_combine  << std::endl;
    //return false;
  }
  else {
    sel_params.p_combine = Parameter.get_double("C_PROB");
  }
  if (!Parameter.defined("SEED")) {
    std::cout << "WARNING: \"SEED\" undefined. Using default " << SEED  << std::endl;
    //return false;
  }
  else {
    SEED = Parameter.get_long("SEED");
  }
  if (!Parameter.defined("INIT")) {
    std::cout << "WARNING: \"INIT\" undefined. Using default " << mode  << std::endl;
  }
  else {
    if(Parameter.get_string("INIT") == "RANDOM_ORDER") mode = INIT_RANDOM;
    else if(Parameter.get_string("INIT") == "ACTIVE_DISPATCH") mode = INIT_DISP_ACTIVE;
    else if(Parameter.get_string("INIT") == "NON_DELAY_DISPATCH") mode = INIT_DISP_NON_DELAY;
    else std::cout << "WARNING: \"INIT\" method unknown. Using default " << mode  << std::endl;
  }

  std::string impr_id = "off";
  if (!Parameter.defined("L_IMPR")) {
    std::cout << "WARNING: \"L_IMPR\" undefined and thus disabled."  << std::endl;
    impr_id = "off";
    //return false;
  }
  else  {//initialize the improver
    impr_id = Parameter.get_string("L_IMPR");

    //initialize the improver
    improver.NGBH = 0;
    improver.METHOD = 0;
    improver.PROB = 1;
    improver.MAX_STUCK = NB_Iteration::MAXLONG;
    improver.TABULENGTH = 1;
    improver.NUMB_NGHB = 1;
    improver.TYPE = RAND;
    improver.OBJ_TYPE = 1;
    improver.STEPS = 10;
    improver.NUMB_PROBLEMS = 1;
    improver.NUMB_PLANS = 1;
    improver.NUMB_STUCKS = NB_Iteration::MAXINT;
    improver.ABORT_BOUND = -NB_Iteration::MAXLONG;
    
    //specific stuff
    if (!Parameter.defined("IMPR_STEPS")) {
      std::cout << "WARNING: \"IMPR_STEPS\" undefined. Using default " << improver.STEPS  << std::endl;
      //return false;
    }
    else {
      improver.STEPS = Parameter.get_long("IMPR_STEPS");
    }
    
    improver.OBJ_TYPE = Problem.get_property(OBJECTIVE);
    improver.PROB_TYPE = Problem.get_property(M_ENV);
    
    if ( impr_id     == "API"              ) improver.NGBH = NB_Iteration::API;
    //else if ( NGBH_St     == "SWAP"             ) improver.NGBH = NB_Iteration::SWAP;
    else if ( impr_id    == "SHIFT"            ) improver.NGBH = NB_Iteration::SHIFT;
    else if ( impr_id     == "CR_TRANS"         ) improver.NGBH = NB_Iteration::CR_TRANS;
    else if ( impr_id     == "CR_TRANS_MIX"     ) improver.NGBH = NB_Iteration::CR_TRANS_MIX;
    else if ( impr_id     == "SC_TRANS"         ) improver.NGBH = NB_Iteration::SC_TRANS;
    else if ( impr_id     == "TRANS"            ) improver.NGBH = NB_Iteration::TRANS;
    else if ( impr_id     == "3_API"            ) improver.NGBH = NB_Iteration::_3_API;
    else if ( impr_id     == "3_CR"             ) improver.NGBH = NB_Iteration::_3_CR;
    else if ( impr_id     == "CR_API"           ) improver.NGBH = NB_Iteration::CR_API;
    else if ( impr_id     == "SC_API"           ) improver.NGBH = NB_Iteration::SC_API;
    else if ( impr_id     == "BL_API"           ) improver.NGBH = NB_Iteration::BL_API;
    else if ( impr_id     == "CR_SHIFT"         ) improver.NGBH = NB_Iteration::CR_SHIFT;
    else if ( impr_id     == "CR_SHIFT_MIX"     ) improver.NGBH = NB_Iteration::CR_SHIFT_MIX;
    else if ( impr_id     == "BL_SHIFT"         ) improver.NGBH = NB_Iteration::BL_SHIFT;
    else if ( impr_id     == "CR_TST"           ) improver.NGBH = NB_Iteration::CR_TST;
    else if ( impr_id     == "PI"               ) improver.NGBH = NB_Iteration::PI;
    else impr_id = "off";
    

    
    improver.METHOD   = II;
    
  }
  apply_LocalImpr = (impr_id == "off")?false:true;
  //inittialize other memebers
  problem = new Lisa_OsProblem(&Values);
  schedule = new Lisa_OsSchedule(problem);
  schedule->SetValue(Problem.get_property(OBJECTIVE));
  //std::cout  << RANDOM_BOUND << std::endl;

  initializer = new GA_Initializer(this);

  return true;
}
