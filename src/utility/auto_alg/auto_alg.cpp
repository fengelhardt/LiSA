/**
 * @author  Marc Moerig
 * @version 2.3final
 */ 

//**************************************************************************

#include <stdlib.h>
#include <time.h>

#include <string>
#include <iostream>
#include <fstream>

#include "../../misc/int2str.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../scheduling/schedule.hpp"

//**************************************************************************

int m=10,n=10;
long timeseed,machseed;
int lowerrange=1,upperrange=100;
int numberproblems=1;
int numberalgorithms=1;

const char* algin = "auto_alg.in";
const char* algout = "auto_alg.out";

//**************************************************************************

// get parameters into global var's, if undefined and add default/generated
// values to controlparameters, so they are in the output later
void parseParameters(Lisa_ControlParameters &cp){
  
  if(cp.defined("TIMESEED")==Lisa_ControlParameters::LONG){
    timeseed = cp.get_long("TIMESEED");
  }else{
    timeseed = time(0);
    cp.add_key("TIMESEED",timeseed);
    G_ExceptionList.lthrow((std::string)"No TIMESEED parameter defined,"+
                           " generated '"+ztos(timeseed)+"'.",WARNING);
  }
  
  if(cp.defined("MACHSEED")==Lisa_ControlParameters::LONG){
    machseed = cp.get_long("MACHSEED");
  }else{
    srand(time(0));
    machseed = rand();
    cp.add_key("MACHSEED",machseed);
    G_ExceptionList.lthrow((std::string)"No MACHSEED parameter defined,"+
                           " generated '"+ztos(machseed)+"'.",WARNING);
  }
  
  if(cp.defined("M")==Lisa_ControlParameters::LONG){
    m = cp.get_long("M");
  }else{
    cp.add_key("M",(long)m);
    G_ExceptionList.lthrow((std::string)"No M parameter defined,"+
                           " using default '"+ztos(m)+"'.",WARNING);
  }
  
  if(cp.defined("N")==Lisa_ControlParameters::LONG){
    n = cp.get_long("N");
  }else{
    cp.add_key("N",(long)n);
    G_ExceptionList.lthrow((std::string)"No N parameter defined,"+
                           " using default '"+ztos(n)+"'.",WARNING);
  }

  if(cp.defined("LOWERRANGE")==Lisa_ControlParameters::LONG){
    lowerrange = cp.get_long("LOWERRANGE");
  }else{
    cp.add_key("LOWERRANGE",(long)lowerrange);
    G_ExceptionList.lthrow((std::string)"No LOWERRANGE parameter defined,"+
                           " using default '"+ztos(lowerrange)+"'.",WARNING);
  }  
  
  if(cp.defined("UPPERRANGE")==Lisa_ControlParameters::LONG){
    upperrange = cp.get_long("UPPERRANGE");
  }else{
    cp.add_key("UPPERRANGE",(long)upperrange);
    G_ExceptionList.lthrow((std::string)"No UPPERRANGE parameter defined,"+
                           " using default '"+ztos(upperrange)+"'.",WARNING);
  }

  if(cp.defined("NUMBERPROBLEMS")==Lisa_ControlParameters::LONG){
    numberproblems = cp.get_long("NUMBERPROBLEMS");
  }else{
    cp.add_key("NUMBERPROBLEMS",(long)numberproblems);
    G_ExceptionList.lthrow((std::string)"No NUMBERPROBLEMS parameter "+
                           "defined, using default '"+ztos(numberproblems)+
                           "'.",WARNING);
  }  
  
  if(cp.defined("NUMBERALGORITHMS")==Lisa_ControlParameters::LONG){
    numberalgorithms = cp.get_long("NUMBERALGORITHMS");
  }else{
    cp.add_key("NUMBERALGORITHMS",(long)numberalgorithms);
    G_ExceptionList.lthrow((std::string)"No NUMBERALGORITHMS parameter "+
                           "defined, using default '"+ztos(numberalgorithms)+
                           "'.",WARNING);
  } 
}

//**************************************************************************

// check if we can handle given problemtype, otherwise exit
void checkProblemType(Lisa_ProblemType &pt){
  Lisa_ProblemType compt;
  
  // O||Cmax
  compt.set_property(M_ENV,O);
  compt.set_property(OBJECTIVE,CMAX);
  if(pt.output_problem() == compt.output_problem()) return;

  // O||SumCi
  compt.set_property(OBJECTIVE,SUM_CI);
  if(pt.output_problem() == compt.output_problem()) return;
 
  G_ExceptionList.lthrow((std::string)"Cannot handle '"+pt.output_alpha()+
                          " / "+pt.output_beta()+" / "+pt.output_gamma()+
                          "'.",INCONSISTENT_INPUT);
  exit(-1);
}

//**************************************************************************

//check if controlparameters contain an algorithm and if the executable exists
//exit otherwise
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

// generate values with respect to seed,m,n,lowerrange,upperrange and the 
// problemtype .. only problemtypes that can be handled here should be accepted 
// by checkProblemType()
void generateValues(Lisa_Values &val,Lisa_ProblemType &pt){
  val.init(n,m);
  val.make_PT();
  val.make_SIJ();
  val.SIJ->fill(1);
  
  // taken from tcl_c.cpp, TC_genpt()
  if(lowerrange==upperrange){
    val.PT->fill(lowerrange);
  }else{
    
    Lisa_Vector<int> zeg(m), mg(m);
    for(int j=0; j<n; j++) {
      
      for(int i=0; i<m; i++) {
        zeg[i] = lisa_random((long)lowerrange,(long)upperrange, &timeseed);
        mg[i]=i;
      }
      
      for(int i=0; i<m; i++) {
        int u = lisa_random(i+1, m, &machseed) -1 ;
        int temp = mg[i];
        mg[i]=mg[u];
        mg[u]=temp;
      }
      
      for(int i=0; i<m; i++) if((*val.SIJ)[j][mg[i]]) (*val.PT)[j][mg[i]]= zeg[i];
    }
  
  }

}

//**************************************************************************

//create an input file, clean output file
void writeAlgInput(Lisa_ProblemType &pt,Lisa_ControlParameters &cp,
                   Lisa_Values &val,Lisa_Schedule &sched){

  std::ofstream out_file(algout);
  if(!out_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algout+
                           "' for writing.",FILE_NOT_FOUND);
    exit(-1);
  }
  out_file.close();
  
  std::ofstream in_file(algin);
  if(!in_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algin+
                           "' for writing.",FILE_NOT_FOUND);
    exit(-1);
  }
  
  in_file << pt << std::endl;
  in_file << cp << std::endl;
  in_file << val << std::endl;
  if(sched.valid) in_file << sched << std::endl;
  
  in_file.close();
}

//**************************************************************************
    
void readAlgOutput(Lisa_Schedule &sched){
  
  std::ifstream out_file(algout);
  if(!out_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algout+
                           "' for reading.",FILE_NOT_FOUND);
    exit(-1);
  }
  
  out_file >> sched;
  if(!G_ExceptionList.empty()) exit(-1);
  
  if(sched.get_m() != m || sched.get_n() != n){
    G_ExceptionList.lthrow((std::string)
                           "Schedule size does not match problem.",
                           INCONSISTENT_INPUT);
    exit(-1);
  }
  
  out_file.close();    
}
  
//**************************************************************************

int main(int argc, char *argv[]){
 
 //make sure we report any errors
 G_ExceptionList.set_output_to_cout();
 
 if(argc < 2){
  G_ExceptionList.lthrow((std::string)"Usage: "+argv[0]+
                         " [input file]",ANY_ERROR);
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
 
 // clean up warnings
 do{}while(G_ExceptionList.lcatch(WARNING) != "No error of this kind in list.");
 
 // read parameters for algorithms to call
 Lisa_ControlParameters* cps = new Lisa_ControlParameters[numberalgorithms];
 for(int i=0;i<numberalgorithms;i++){
   in_file >> cps[i];
   if(!G_ExceptionList.empty()) exit(-1);
   checkAlgo(cps[i]);
 }
 
 // were done reading
 in_file.close();
 
  
 for(int i=0;i<numberproblems;i++){
   Lisa_Values val;
   generateValues(val,pt); 
   Lisa_Schedule sched(n,m);
   
   //open output file, write generated problem + comments
   std::ofstream out_file(((std::string)argv[1]+"."+ztos(i+1)+".lsa").c_str());
   if(!out_file){
     G_ExceptionList.lthrow((std::string)"Could not open '"+argv[1]+"."+
                            ztos(i+1)+".lsa"+"' for reading.",FILE_NOT_FOUND);
     exit(-1);
   }
   
   out_file 
   << "This is problem number " << i+1 << " created from the following initial "
   << "values:" << std::endl << std::endl
   << cp
   << pt << std::endl
   << "Generated problem instance:" << std::endl
   << val << std::endl
   << "Now follows a list of schedules for this problem, each preceded by the" 
   << std::endl << "algorithm and parameters that were used to generate it. "
   << "The first algorithm" << std::endl << "was called without an initial "
   << "schedule, while the following algorithms get" << std::endl << "the "
   << "previous schedule as input." << std::endl << std::endl;
   
   // run all the algorithms .. first one should be constructive, the others 
   // can be iterative to improve the solution or contructive to create a new
   // solution ... 
   for(int j=0;j<numberalgorithms;j++){
    
    writeAlgInput(pt,cps[j],val,sched);
    system((cps[j].get_string("EXECUTABLE")+" "+algin+" "+algout).c_str());
    readAlgOutput(sched);
    
    out_file << cps[j] << sched << std::endl;

   }
   
   out_file.close();
 }

 delete[] cps;
 return 0;
}

