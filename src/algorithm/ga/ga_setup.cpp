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
  objective = 0;
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
    G_ExceptionList.lthrow("You have to define POP_SIZE in the input file!");
    return false;
  }
  else {
    pop_size = Parameter.get_long("POP_SIZE");
  }

  if (!Parameter.defined("NUM_GEN")) {
    G_ExceptionList.lthrow("You have to define NUM_GEN in the input file!");
    return false;
  }
  else {
    n_gen = Parameter.get_long("NUM_GEN");
  }
  if (!Parameter.defined("M_PROB")) {
    G_ExceptionList.lthrow("You have to define M_PROB in the input file!");
    return false;
  }
  else {
    sel_params.p_mutate = Parameter.get_double("M_PROB");
  }
  if (!Parameter.defined("C_PROB")) {
    G_ExceptionList.lthrow("You have to define C_PROB in the input file!");
    return false;
  }
  else {
    sel_params.p_combine = Parameter.get_double("C_PROB");
  }
  if (!Parameter.defined("SEED")) {
    std::cout << "WARNING: \"SEED\" undefined. Using default " << SEED  << std::endl;
  }
  else {
    SEED = Parameter.get_long("SEED");
  }
  objective = Problem.get_property(OBJECTIVE);
  /*
    if (!Parameter.defined("FITNESS")) {
     std::cout << "WARNING: \"FITNESS\" undefined. Using default " << "OBJETIVE"  << std::endl;
  } 
    else { 
    if(Parameter.get_string("FITNESS") == "SUM_Ci2") objective = SUM_CI_2; 
    else if(Parameter.get_string("FITNESS") == "OBJECTIVE") ; 
    else std::cout << "WARNING: \"FITNESS\" method unknown. Using default " << "OBJECTIVE"  << std::endl; 
  } 
  */
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
  }else if (Parameter.get_string("L_IMPR") == "(disabled)"){
    impr_id = "off";
  } else  {
    
    //create  controlparameters for II    
    ParameterII.add_key("METHOD","IterativeImprovement");
    
    
    if (!Parameter.defined("IMPR_STEPS")) {
      std::cout << "WARNING: \"IMPR_STEPS\" undefined. Using default 10."<< std::endl;
      ParameterII.add_key("STEPS",(long)10);
    }else {
      ParameterII.add_key("STEPS",Parameter.get_long("IMPR_STEPS"));
    }
    
    
    ParameterII.add_key("NGBH",Parameter.get_string("L_IMPR"));   
    ParameterII.add_key("TYPE","RAND");
    ParameterII.add_key("NUMB_STUCKS",ParameterII.get_long("STEPS"));
    ParameterII.add_key("ABORT_BOUND",0.0);

    //initialize the improver
    improver.configure(Problem, ParameterII, Values);
    
    impr_id = Parameter.get_string("L_IMPR");
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
