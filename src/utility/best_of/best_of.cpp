/**
 * @author  LiSA
 * @version 3.0pre1
 */

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>


#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../scheduling/os_sched.hpp"

//**************************************************************************

// check if we can handle given problemtype, otherwise exit
void checkProblemType(Lisa_ProblemType &pt){
  Lisa_ProblemType compt;
  
  for(int i=0;i<6;i++){
	  switch(i){
	  case 0:	compt.set_property(M_ENV,O);
				break;
	  case 1:	compt.set_property(M_ENV,J);
				break;
	  case 2:	compt.set_property(M_ENV,F);
				break;
	  case 3:	compt.set_property(RI,true);
            compt.set_property(M_ENV,O);
				break;
	  case 4:	compt.set_property(RI,true);
            compt.set_property(M_ENV,J);
				break;
	  case 5:	compt.set_property(RI,true);
            compt.set_property(M_ENV,F);
				break;
	  }
	  compt.set_property(OBJECTIVE,CMAX);
	  if(pt.output_problem() == compt.output_problem()) 
      return;

	  compt.set_property(OBJECTIVE,LMAX);
	  if(pt.output_problem() == compt.output_problem())
		  return;
	

	  compt.set_property(OBJECTIVE,SUM_CI);
	  if(pt.output_problem() == compt.output_problem()) 
      return;
 
	  compt.set_property(OBJECTIVE,SUM_WICI);
	  if(pt.output_problem() == compt.output_problem())
		  return;

 
	  compt.set_property(OBJECTIVE,SUM_TI);
	  if(pt.output_problem() == compt.output_problem())
		  return;

 
	  compt.set_property(OBJECTIVE,SUM_WITI);
	  if(pt.output_problem() == compt.output_problem())
		  return;

	  compt.set_property(OBJECTIVE,SUM_UI);
	  if(pt.output_problem() == compt.output_problem())
		  return;

 
	  compt.set_property(OBJECTIVE,SUM_WIUI);
	  if(pt.output_problem() == compt.output_problem())
		  return;
  }
  
  G_ExceptionList.lthrow((std::string)"Cannot handle '"+pt.output_alpha()+
                          " / "+pt.output_beta()+" / "+pt.output_gamma()+
                          "'.",Lisa_ExceptionList::INCONSISTENT_INPUT);
  exit(-1);
}

//**************************************************************************

int main(int argc,char* argv[]){
  
  G_ExceptionList.set_output_to_cout();

  // print a message that the programm started:
  std::cout << "This is the LiSA best_of Module." << std::endl;
  if (argc != 3){
    std::cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(7);
  }
 
  std::cout << "PID= " << getpid() << std::endl;  
 
  std::ifstream i_strm(argv[1]);
  if(!i_strm){
    std::cout << "ERROR: cannot open input file " << argv[1] << "." << std::endl;
    exit(1);
  }
  i_strm.close();
  
  LisaXmlFile::initialize();
  LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
  
  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;   
  Lisa_Values Values;
  Lisa_List<Lisa_ScheduleNode> Starters;
  
  xmlInput.read(argv[1]);
  LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
  
  if (!xmlInput || type != LisaXmlFile::SOLUTION){
    std::cout << "ERROR: cannot read input , aborting program." << std::endl;
    exit(1);
  }
  if( !(xmlInput >> Problem)){
    std::cout << "ERROR: cannot read ProblemType , aborting program." << std::endl;
    exit(1);
  }
  if( !(xmlInput >> Parameter)){
    std::cout << "ERROR: cannot read ControlParameters , aborting program." << std::endl;
    exit(1);
  }
  if( !(xmlInput >> Values)){
    std::cout << "ERROR: cannot read Values , aborting program." << std::endl;
    exit(1);
  }
  if( !(xmlInput >> Starters)){
    std::cout << "ERROR: cannot read starting schedule , aborting program." << std::endl;
    exit(1);
  }
  if (!G_ExceptionList.empty()){
    std::cout << "ERROR: cannot read input , aborting program." << std::endl;
    exit(1);
  }
  
  checkProblemType(Problem);
  
  Lisa_OsProblem op(&Values);
  Lisa_OsSchedule os(&op);
  
  os.read_LR(Starters.first().actual_schedule->LR);
  os.SetValue(Problem.get_property(OBJECTIVE));
  TIMETYP best_objective = os.GetValue();
  Lisa_Schedule* best_schedule = Starters.first().actual_schedule;
  
  Starters.reset(); 
  //we already checked first entry, so skip it
  while(Starters.next()){
  
    os.read_LR(Starters.get().actual_schedule->LR);
    os.SetValue(Problem.get_property(OBJECTIVE));
  
    if(os.GetValue() < best_objective){
       best_objective = os.GetValue();
       best_schedule = Starters.get().actual_schedule;
    }
  }
  
  std::ofstream o_strm(argv[2]);
  if(!o_strm){
   std::cout << "ERROR: cannot open output file " << argv[2] << "." << std::endl;
   exit(1);
  }
  o_strm.close();
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  xmlOutput << Problem << Values << Parameter << *best_schedule;
  xmlOutput.write(argv[2]);
  
  return 0;
}

//**************************************************************************

