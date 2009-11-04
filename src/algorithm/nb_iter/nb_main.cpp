/**
 * @author  Andreas Winkler
 * @version 3.0pre1
 */ 
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <fstream>
#include <time.h>

#include "../../xml/LisaXmlFile.hpp"

#include "nb_iter.hpp"
using namespace std;

//************************************************************************

NB_Iteration *task;

//************************************************************************

void interupted(int){
 task->abort();
}

//************************************************************************

int main(int argc, char *argv[]){
   
  G_ExceptionList.set_output_to_cout();

  // print a message that the programm started:
  cout << "This is the LiSA-Neighbourhood-Search Module Version 05.01.1999" << endl;
  if (argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(7);
  }
 
  cout << "PID= " << getpid() << endl;  
 
  ifstream i_strm(argv[1]);
  if(!i_strm){
    cout << "ERROR: cannot open input file " << argv[1] << "." << endl;
    exit(1);
  }
  i_strm.close();
  
  LisaXmlFile::initialize();
  LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
  
  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;   
  Lisa_Values Values;
  Lisa_List<Lisa_ScheduleNode> Starters;
  Lisa_List<Lisa_ScheduleNode> Results;
  
  xmlInput.read(argv[1]);
  LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
  
  if (!xmlInput || type != LisaXmlFile::SOLUTION){
    cout << "ERROR: cannot read input , aborting program." << endl;
    exit(1);
  }
  if( !(xmlInput >> Problem)){
    cout << "ERROR: cannot read ProblemType , aborting program." << endl;
    exit(1);
  }
  if( !(xmlInput >> Parameter)){
    cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
    exit(1);
  }
  if( !(xmlInput >> Values)){
    cout << "ERROR: cannot read Values , aborting program." << endl;
    exit(1);
  }
  if( !(xmlInput >> Starters)){
    cout << "ERROR: cannot read starting schedule , aborting program." << endl;
    exit(1);
  }
  if (!G_ExceptionList.empty()){
    cout << "ERROR: cannot read input , aborting program." << endl;
    exit(1);
  }

  task = new NB_Iteration();
  if(!task->configure(Problem,Parameter,Values)) exit(7);
  signal(SIGINT, &interupted);
  task->nb_iter(Values, Starters, Results);
  signal(SIGINT, SIG_IGN);
  delete task;
  
  //output solution ....
  ofstream o_strm(argv[2]);
  if(!o_strm){
   cout << "ERROR: cannot open output file " << argv[2] << "." << endl;
   exit(1);
  }
  o_strm.close();
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  xmlOutput << Problem << Values << Parameter << Results;
  xmlOutput.write(argv[2]);

  return OK;
 }

//**************************************************************************

