/**
 * @author  Marc Moerig
 * @version 3.0
 */


#include <iostream>
#include <fstream>

#include "../../algorithm/nb_iter/iterate.hpp"
#include "../../lisa/ctrlpara.hpp"

#include "travel_graph.hpp"
#include "travel_neighbourhood.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]){
  
  // print any errors
  G_ExceptionList.set_output_to_cout();   
  
  // arguments ?
  if(argc != 3){
    std::cout << "Usage: " << argv[0] << " [input file] [output file]" <<  std::endl;
    exit(1);
  }
  
  // open input file
  std::ifstream i_strm(argv[1]);
  
  // file exists ?
  if(!i_strm){
    std::cout << "File '"<< argv[1] <<"' not found. Exiting."<< std::endl;
    exit(1);
  }
  
  // create an object holding parameters and read it
  Lisa_ControlParameters param;
  i_strm >> param;
      
  // parse our parameters  
  string method_in = "II";
  int METHOD = II; // which algorithm: II,SA,SA_anti,TA,TS
  string NGBH = "API"; // which neighbourhood: API,RPI
  int STEPS = 100; 
  //int NUMB_STUCKS = 999999;  
  //double ABORT_BOUND = 1; 
  //string ngbh_type_in = "ENUM";
  //int NGBH_TYPE = ENUM; // ENUM,RAND ..  only for II,TS 
  //int PROB = 50; // 0..100 only for SA,SA_anti,TA
  //int MAX_STUCK = 2000; // only for SA,SA_anti,TA
  //int TABU_LENGTH = 1; // length of tabulist ... only TS
  //int NUMB_NGB = 1; // how many NGB's to generate in each step ... only TS

  // algorithm
  if (!param.defined("METHOD")){ 
    G_ExceptionList.lthrow("You must define an algorithm (METHOD) in the input file.");
    exit(7);
  }else{
    method_in = param.get_string("METHOD");

    if(method_in=="II") METHOD = II;
    else if(method_in=="SA") METHOD = SA;
    else if(method_in=="TS") METHOD = TS ;
    else if(method_in=="TA") METHOD = TA;
    else {
      std::cout << method_in << " is not a valid algorithm." << std::endl;
      std::cout << "Must be II,SA,TS or TA." << std::endl;
      exit(7);
    }
  }

  // neighbourhood
  if (!param.defined("NGBH")){ 
    G_ExceptionList.lthrow("You must define a neighbourhood (NGBH) in the input file.");
    exit(7);
  }else{
    NGBH = param.get_string("NGBH");
    if(NGBH!="API"&&NGBH!="RPI"){
      std::cout << NGBH << " is not a valid Neighbourhood." << std::endl;
      std::cout << "Must be API or RPI." << std::endl;
      exit(7);
    }
  }
  
  //steps
  if (!param.defined("STEPS")){ 
    G_ExceptionList.lthrow("You must define a number of steps (STEPS) in the input file.");
    exit(7);
  }else{
    STEPS = param.get_long("STEPS");
  }
  

  // create an object holding our problem and read it
  Travel_Graph tr1;
  i_strm >> tr1;
  
  // close input stream/file
  i_strm.close();

  // create neighbourhood object
  Travel_RPI_Neighbourhood* nbh=0;
  if(NGBH=="API") nbh = new Travel_API_Neighbourhood(&tr1);
  else if(NGBH=="RPI") nbh = new Travel_RPI_Neighbourhood(&tr1);

  // create iteration object 
  Lisa_Iter* it=0;
  
  // init algorithm type and parameters 
  if(METHOD==II) it = new Lisa_IterativeImprovement(&param);
  else if(METHOD==SA) it = new Lisa_OldSimulatedAnnealing(&param);
  else if(METHOD==TA) it = new Lisa_ThresholdAccepting(&param);
  else if(METHOD==TS) it = new Lisa_TabuSearch(&param);
    
  //go for it ;)
  it->iterate(nbh,1,STEPS);

  delete it;
  
  // put our best result back into our problem object
  nbh->write_best();
  
  // open file for output
  std::ofstream o_strm(argv[2]);
  
  // done ?
  if(!o_strm){
    std::cout << "Could not open file '" << argv[2] << "'. Exiting." << std::endl;
    exit(1);
  }
  
  // write parameters so we can use the outputfile as inputfile again
  o_strm << param;
  
  // write problem .. which now contains a solution
  o_strm << tr1;

  // cleanup
  delete nbh;
  
  // done
  return 0;
}

//**************************************************************************
  
