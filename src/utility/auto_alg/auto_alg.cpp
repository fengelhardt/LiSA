/**
 * @author  Marc Moerig
 * @version 2.3final
 */ 

//**************************************************************************

#include <cstdlib>
#include <ctime>

#include <string>
#include <iostream>
#include <fstream>

#include "../../misc/int2str.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../xml/LisaXmlFile.hpp"

//**************************************************************************

int m=10,n=10;
long timeseed,machseed;
int minpt=1,maxpt=100;
int numberproblems=1;
int numberalgorithms=1;

std::string algin,algout;

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
                           " generated '"+ztos(timeseed)+"'.",Lisa_ExceptionList::WARNING);
  }
  
  if(cp.defined("MACHSEED")==Lisa_ControlParameters::LONG){
    machseed = cp.get_long("MACHSEED");
  }else{
    srand(time(0));
    machseed = rand();
    cp.add_key("MACHSEED",machseed);
    G_ExceptionList.lthrow((std::string)"No MACHSEED parameter defined,"+
                           " generated '"+ztos(machseed)+"'.",Lisa_ExceptionList::WARNING);
  }
  
  if(cp.defined("M")==Lisa_ControlParameters::LONG){
    m = cp.get_long("M");
  }else{
    cp.add_key("M",(long)m);
    G_ExceptionList.lthrow((std::string)"No M parameter defined,"+
                           " using default '"+ztos(m)+"'.",Lisa_ExceptionList::WARNING);
  }
  
  if(cp.defined("N")==Lisa_ControlParameters::LONG){
    n = cp.get_long("N");
  }else{
    cp.add_key("N",(long)n);
    G_ExceptionList.lthrow((std::string)"No N parameter defined,"+
                           " using default '"+ztos(n)+"'.",Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("MINPT")==Lisa_ControlParameters::LONG){
    minpt = cp.get_long("MINPT");
  }else{
    cp.add_key("MINPT",(long)minpt);
    G_ExceptionList.lthrow((std::string)"No MINPT parameter defined,"+
                           " using default '"+ztos(minpt)+"'.",Lisa_ExceptionList::WARNING);
  }  
  
  if(cp.defined("MAXPT")==Lisa_ControlParameters::LONG){
    maxpt = cp.get_long("MAXPT");
  }else{
    cp.add_key("MAXPT",(long)maxpt);
    G_ExceptionList.lthrow((std::string)"No MAXPT parameter defined,"+
                           " using default '"+ztos(maxpt)+"'.",Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("NUMBERPROBLEMS")==Lisa_ControlParameters::LONG){
    numberproblems = cp.get_long("NUMBERPROBLEMS");
  }else{
    cp.add_key("NUMBERPROBLEMS",(long)numberproblems);
    G_ExceptionList.lthrow((std::string)"No NUMBERPROBLEMS parameter "+
                           "defined, using default '"+ztos(numberproblems)+
                           "'.",Lisa_ExceptionList::WARNING);
  }  
  
  if(cp.defined("NUMBERALGORITHMS")==Lisa_ControlParameters::LONG){
    numberalgorithms = cp.get_long("NUMBERALGORITHMS");
  }else{
    cp.add_key("NUMBERALGORITHMS",(long)numberalgorithms);
    G_ExceptionList.lthrow((std::string)"No NUMBERALGORITHMS parameter "+
                           "defined, using default '"+ztos(numberalgorithms)+
                           "'.",Lisa_ExceptionList::WARNING);
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
                          "'.",Lisa_ExceptionList::INCONSISTENT_INPUT);
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
                           ,Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(-1);
  }
  
/*add some useful !!! testing if executable exists here -marc-  
  std::ifstream testfile(executable.c_str());
  if(testfile){
    testfile.close();
  }else{
    G_ExceptionList.lthrow((std::string)"Executable '"+
                           executable+"' doesn't seem to exist.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(-1); 
  } */
}

//**************************************************************************

// generate values with respect to seed,m,n,minpt,maxpt and the 
// problemtype .. only problemtypes that can be handled here should be accepted 
// by checkProblemType()
void generateValues(Lisa_Values &val,Lisa_ProblemType &pt){
  val.init(n,m);
  val.make_PT();
  val.make_SIJ();
  val.SIJ->fill(1);
  
  // taken from tcl_c.cpp, TC_genpt()
  if(minpt==maxpt){
    val.PT->fill(minpt);
  }else{
    
    Lisa_Vector<int> zeg(m), mg(m);
    for(int j=0; j<n; j++) {
      
      for(int i=0; i<m; i++) {
        zeg[i] = lisa_random((long)minpt,(long)maxpt, &timeseed);
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

  std::ofstream out_file(algout.c_str());
  if(!out_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algout+
                           "' for writing.",Lisa_ExceptionList::FILE_NOT_FOUND);
    exit(-1);
  }
  out_file.close();
  
  std::ofstream in_file(algin.c_str());
  if(!in_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algin+
                           "' for writing.",Lisa_ExceptionList::FILE_NOT_FOUND);
    exit(-1);
  }
  in_file.close();
  
  LisaXmlFile::DOC_TYPE type = LisaXmlFile::INSTANCE;
  if(sched.valid) type = LisaXmlFile::SOLUTION;
  
  LisaXmlFile xmlOutput(type);
  xmlOutput << pt << val << cp;
  if(sched.valid) xmlOutput << sched;
  xmlOutput.write(algin);
}

//**************************************************************************
    
void readAlgOutput(Lisa_Schedule &sched){
  std::ifstream out_file(algout.c_str());
  if(!out_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algout+
                           "' for reading.",Lisa_ExceptionList::FILE_NOT_FOUND);
    exit(-1);
  }
  out_file.close();
  
 LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
 xmlInput.read(algout.c_str());
 
 //check for successful parsing and valid document type   
 LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
 if( !xmlInput || !(type == LisaXmlFile::SOLUTION) || !(xmlInput >> sched)){
   G_ExceptionList.lthrow("Could not read solution.",
                          Lisa_ExceptionList::INCONSISTENT_INPUT);
   exit(-1);
  }
  
  if(!G_ExceptionList.empty()) exit(-1);
  
  if(sched.get_m() != m || sched.get_n() != n){
    G_ExceptionList.lthrow((std::string)
                           "Schedule size does not match problem.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(-1);
  }
  
}
  
//**************************************************************************

TIMETYP sum(Lisa_Matrix<TIMETYP>* pt){
  TIMETYP retval = 0;
  
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      retval += (*pt)[i][j];
    }
  }
  
  return retval;
}

//**************************************************************************

// convert int to string .. add leading zeros with respect to numberproblems,
// so outputfiles are correctly sorted by filesystem
std::string str_prob(const int i){
  const static unsigned int l = ((std::string) ztos(numberproblems)).length(); 
  
  std::string retval(ztos(i));
  while(retval.length() < l) retval = "0"+retval;
  
  return retval;
}

//**************************************************************************

// convert int to string .. add leading zeros with respect to numberalgorithms,
std::string str_alg(const int i){
  const static unsigned int l = ((std::string) ztos(numberalgorithms)).length(); 
  
  std::string retval(ztos(i));
  while(retval.length() < l) retval = "0"+retval;
  
  return retval;
}
  
//**************************************************************************

int main(int argc, char *argv[]){
 time_t all_start = time(0);
 
 //make sure we report any errors
 G_ExceptionList.set_output_to_cout();
 
 if(argc < 2){
  G_ExceptionList.lthrow((std::string)"Usage: "+argv[0]+
                         " [input file]",Lisa_ExceptionList::ANY_ERROR);
  exit(-1);  
 }
 
 //initialize xml input stuff
 LisaXmlFile::initialize();
 
 algin = (std::string) argv[1]+"in.xml";
 algout = (std::string) argv[1]+"out.xml";
 
 //open input file
 std::ifstream in_file(argv[1]);
 if(! in_file){
   G_ExceptionList.lthrow((std::string)"Could not open '"+argv[1]+
                          "' for reading.",Lisa_ExceptionList::FILE_NOT_FOUND);
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
 do{}while(G_ExceptionList.lcatch(Lisa_ExceptionList::WARNING) != "No error of this kind in list.");
 
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
   
   //save current seeds to pt so problem can be reconstructed
   cp.add_key("TIMESEED",timeseed);
   cp.add_key("MACHSEED",machseed);
   
   // generate instance 
   Lisa_Values val;
   generateValues(val,pt); 
   Lisa_Schedule sched(n,m);
   
   //need those two to calculate objective
   Lisa_OsProblem op(&val);
   Lisa_OsSchedule os(&op);
           
   // run all the algorithms .. first one should be constructive, the others 
   // can be iterative to improve the solution or contructive to create a new
   // solution ...
   TIMETYP old_objective = MAXTIME;
   Lisa_List<Lisa_ScheduleNode> SchedList;
   
   for(int j=0;j<numberalgorithms;j++){
    
    writeAlgInput(pt,cps[j],val,sched);
    time_t start = time(0);
    system((cps[j].get_string("EXECUTABLE")+" "+algin+" "+algout).c_str());
    time_t end = time(0);
    readAlgOutput(sched);
    SchedList.append(Lisa_ScheduleNode(&sched));
    
    os.read_LR(sched.LR);
    os.SetValue(pt.get_property(OBJECTIVE));
    std::cout << "AUTO_ALG: problem " << str_prob(i+1) << " algorithm "
              << str_alg(j+1) << " time " << end-start << " sek objective " 
              << os.GetValue();
    if(old_objective > os.GetValue()) std::cout << " +";
    std::cout << std::endl;
    old_objective = os.GetValue();
    

   }
   
   //open output file, write generated problem + comments
   std::string out_file_name = (std::string)argv[1]+"."+str_prob(i+1)+".xml";
   std::ofstream out_file(out_file_name.c_str());
   if(!out_file){
     G_ExceptionList.lthrow("Could not open '"+out_file_name+"' for writing.",
                            Lisa_ExceptionList::FILE_NOT_FOUND);
     exit(-1);
   }
   out_file.close();
   
   LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
   xmlOutput << pt << val << SchedList;
   xmlOutput.write(out_file_name);
   
   std::ofstream out_file1(out_file_name.c_str(),std::ios::out|std::ios::app);
   if(!out_file1){
     G_ExceptionList.lthrow("Could not open '"+out_file_name+"' for appending.",
                            Lisa_ExceptionList::FILE_NOT_FOUND);
     exit(-1);
   }
   
   out_file1 
   << "<!--" << std::endl
   << "This problem was created from the following initial values:" << std::endl
   << pt << std::endl
   << cp << std::endl  
   << "-->";
   
   out_file1.close();
   
   
 }

 delete[] cps;
 time_t all_end = time(0);
 std::cout << "AUTO_ALG: overall time " << all_end-all_start << " sek" << std::endl;
 return 0;
}

