/**
 * @author  Marc Moerig
 * @version 2.3final
 */ 

//**************************************************************************

#include <stdlib.h>

#include <string>
#include <iostream>
#include <fstream>

#include "../../misc/int2str.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"

//**************************************************************************

int m=10,n=11;
long seed=918273645;
int lowerrange=1,upperrange=100;
int numberproblems=1;
int numberalgorithms=1;

//**************************************************************************

// get parameters into global var's, warn if undefined
void parseParameters(Lisa_ControlParameters &cp){
  
  if(cp.defined("SEED")==Lisa_ControlParameters::LONG){
    seed = cp.get_long("SEED");
  }else{
    G_ExceptionList.lthrow((std::string)"No SEED parameter defined,"+
                           " using default '"+ztos(seed)+"'.",WARNING);
  }
  
  if(cp.defined("M")==Lisa_ControlParameters::LONG){
    m = cp.get_long("M");
  }else{
    G_ExceptionList.lthrow((std::string)"No M parameter defined,"+
                           " using default '"+ztos(m)+"'.",WARNING);
  }
  
  if(cp.defined("N")==Lisa_ControlParameters::LONG){
    n = cp.get_long("N");
  }else{
    G_ExceptionList.lthrow((std::string)"No N parameter defined,"+
                           " using default '"+ztos(n)+"'.",WARNING);
  }

  if(cp.defined("LOWERRANGE")==Lisa_ControlParameters::LONG){
    lowerrange = cp.get_long("LOWERRANGE");
  }else{
    G_ExceptionList.lthrow((std::string)"No LOWERRANGE parameter defined,"+
                           " using default '"+ztos(lowerrange)+"'.",WARNING);
  }  
  
  if(cp.defined("UPPERRANGE")==Lisa_ControlParameters::LONG){
    upperrange = cp.get_long("UPPERRANGE");
  }else{
    G_ExceptionList.lthrow((std::string)"No UPPERRANGE parameter defined,"+
                           " using default '"+ztos(upperrange)+"'.",WARNING);
  }

  if(cp.defined("NUMBERPROBLEMS")==Lisa_ControlParameters::LONG){
    numberproblems = cp.get_long("NUMBERPROBLEMS");
  }else{
    G_ExceptionList.lthrow((std::string)"No NUMBERPROBLEMS parameter "+
                           "defined, using default '"+ztos(numberproblems)+
                           "'.",WARNING);
  }  
  
  if(cp.defined("NUMBERALGORITHMS")==Lisa_ControlParameters::LONG){
    numberalgorithms = cp.get_long("NUMBERALGORITHMS");
  }else{
    G_ExceptionList.lthrow((std::string)"No NUMBERALGORITHMS parameter "+
                           "defined, using default '"+ztos(numberalgorithms)+
                           "'.",WARNING);
  }  
}

//**************************************************************************

// check if we can handle given problemtype, if not exit
void checkProblemType(Lisa_ProblemType &pt){
  Lisa_ProblemType compt;
  compt.set_property(M_ENV,O);
  compt.set_property(OBJECTIVE,SUM_CI);
  
  if(pt.output_problem() != compt.output_problem()){
    G_ExceptionList.lthrow((std::string)"Cannot handle '"+pt.output_alpha()+
                           " / "+pt.output_beta()+" / "+pt.output_gamma()+
                           "'.",INCONSISTENT_INPUT);
    exit(-1);
  }
}

//**************************************************************************

//check if controlparameters contain an algorithm and if the executable
//exists exit otherwise
void checkAlgo(Lisa_ControlParameters &cp){
  std::string executable;

  if(cp.defined("EXECUTABLE")==Lisa_ControlParameters::STRING){
    executable = cp.get_string("EXECUTABLE");
  }else{
    G_ExceptionList.lthrow((std::string)"No EXECUTABLE parameter defined."
                           ,INCONSISTENT_INPUT);
    exit(-1);
  }
  
/*add some useful !!! testing if executable exists here -marc-  
  std::ifstream testfile(executable.c_str());
  if(testfile){
    testfile.close();
  }else{
    G_ExceptionList.lthrow((std::string)"Executable '"+
                           executable+"' doesn't seem to exist.",
                           INCONSISTENT_INPUT);
    exit(-1); 
  } */

}

//**************************************************************************

int main(int argc, char *argv[]){
 
 //make sure we report any errors
 G_ExceptionList.set_output_to_cout();
 
 if(argc < 3){
  G_ExceptionList.lthrow((std::string)"Usage: "+argv[0]+
                         " [input file] [output path]",ANY_ERROR);
  exit(-1);  
 }
 
 //open input file
 std::ifstream in_file(argv[1]);
 if(! in_file){
   G_ExceptionList.lthrow((std::string)"Could not open '"+argv[1]+
                          "' for reading.",FILE_NOT_FOUND);
   exit(-1);   
 }

 // get problemtype
 Lisa_ProblemType pt;
 in_file >> pt;
 if(!G_ExceptionList.empty()) exit(-1);
 checkProblemType(pt);
  
 // get parameters into global variables 
 Lisa_ControlParameters cp;
 in_file >> cp;
 if(!G_ExceptionList.empty()) exit(-1);
 parseParameters(cp);
 
 // read parameters for algorithms to call
 Lisa_ControlParameters* cps = new Lisa_ControlParameters[numberalgorithms];
 for(int i=0;i<numberalgorithms;i++){
   in_file >> cps[i];
   if(!G_ExceptionList.empty()) exit(-1);
   checkAlgo(cps[i]);
 }
 
 // were done reading -> close input file
 in_file.close();

 
 delete[] cps;
 return 0;
}
