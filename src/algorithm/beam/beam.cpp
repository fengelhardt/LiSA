/**
 * @author : jan tusch, andre herms
 */ 
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <limits>

#include "../../xml/LisaXmlFile.hpp"

#include "b_node.hpp"
#include "beam_orders.hpp"

using namespace std;


//B_Node* beam_search(Lisa_Order *, int, Lisa_OsProblem *);


int main(int argc, char *argv[]){
  
  //G_ExceptionList.set_output_to_cout();   
  
  // open files and assure existence:
  if (argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
  
  cout << "This is the LiSA beam search module." << endl;
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
  
  BeamSearch search;
  if(!search.init(xmlInput))
    exit(1);
  //set progress window height
  cout << "OBJECTIVE= "<< search.guessObjective() << endl;
  if(search.mode == BeamSearch::ATTACH && search.attach == BeamSearch::both){
    //first run with machine order
    search.attach = BeamSearch::machines;
    search.step = 0;
    if(!search.run()) exit(1);
    TIMETYP r1 = search.value;
    Lisa_Schedule* res1 = new Lisa_Schedule(*search.result);
    //second run with jobs
    search.attach = BeamSearch::jobs;
    search.step = 0;
    if(!search.run()) exit(1);
    
    //pick better solution
    if(search.value > r1){
      delete search.result;
      search.result = res1;
      search.value = r1;
    }
    else
      delete res1;
  }
  else if(!search.run())
    exit(1);
  
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  xmlOutput << search.Problem << search.Values << search.Parameter << *(search.result);
  xmlOutput.write(argv[2]);
}

