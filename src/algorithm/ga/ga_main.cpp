#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <signal.h>


#include <iostream>
#include <fstream>
#include <limits>

#include "../../xml/LisaXmlFile.hpp"

#include "ga.hpp"
#include "../../scheduling/schedule.hpp"

using namespace std;

static bool cancel_flag = false;

void interupted(int){
  cancel_flag = true;
}

int main(int argc, char *argv[]){
  
  G_ExceptionList.set_output_to_cout();   
  
  // open files and assure existence:
  if (argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
  
  cout << "This is the LiSA GenAlg module." << endl;
  cout << "PID= " << getpid() << endl;
  
  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  if (!i_strm){
    cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
    exit(1);
  }
  if (!o_strm){
    cout << "ERROR: cannot write output file " << argv[1] << "." << endl;
    exit(1);
  }
  i_strm.close();
  o_strm.close();
		
  //initialize class
  LisaXmlFile::initialize();
  //create Input handler
  LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
  //communication objects
  
  //parse xml file
  xmlInput.read(argv[1]);
  //determine document type
  LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
  
  //check for successful parsing and valid document type
  if (!xmlInput || !(type == LisaXmlFile::INSTANCE || type == LisaXmlFile::SOLUTION))
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }
  //read the setup and stop if failes
  
  //typedef LO_Individuum Individuum;
  typedef LR_Individuum Individuum;
  typedef GA<Individuum> GA_Type;
  
  GA_Type ga;
  if(!ga.readSetup(xmlInput)) exit(1);
  ga.canceled = &cancel_flag;
  signal(SIGINT, &interupted);
  ga.run();
  signal(SIGINT, SIG_IGN);
  
  Lisa_Schedule result(ga.setup.Values.get_n(),ga.setup.Values.get_m());
  result.make_LR();
  
  LR_Individuum::Objective = ga.setup.Problem.get_property(OBJECTIVE);
  ga.eval_pop(true);
  ga.getBest().makePlan(*ga.setup.schedule);

  ga.setup.schedule->write_LR(result.LR);
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  xmlOutput << ga.setup.Problem << ga.setup.Values << ga.setup.Parameter << result;
  if(!xmlOutput.write(argv[2])){
    std::cout << "ERROR: Failed to write \"" << argv[2] << "\" !!!" << std::endl;
  }
}

