/**
 * @author  Marc Moerig
 * @version 2.3final
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
  
  // file exits ?
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
  int NUMB_STUCKS = 999999;  
  double ABORT_BOUND = 1; 
  string ngbh_type_in = "ENUM";
  int NGBH_TYPE = ENUM; // ENUM,RAND ..  only for II,TS 
  int PROB = 50; // 0..100 only for SA,SA_anti,TA
  int MAX_STUCK = 2000; // only for SA,SA_anti,TA
  int TABU_LENGTH = 1; // length of tabulist ... only TS
  int NUMB_NGB = 1; // how many NGB's to generate in each step ... only TS

  // algorithm
  if (!param.defined("METHOD")){ 
    G_ExceptionList.lthrow("You must define an algorithm (METHOD) in the input file.");
    exit(7);
  }else{
    method_in = param.get_string("METHOD");

    if(method_in=="II") METHOD = II;
    else if(method_in=="SA") METHOD = SA;
    else if(method_in=="SA_anti") METHOD = SA_anti;
    else if(method_in=="TS") METHOD = TS ;
    else if(method_in=="TA") METHOD = TA;
    else {
      std::cout << method_in << " is not a valid algorithm." << std::endl;
      std::cout << "Must be II,SA,SA_anti,TS or TA." << std::endl;
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
    STEPS = param.get_int("STEPS");
  }
  
  // stucks after which to abort algorithm
  if (!param.defined("NUMB_STUCKS")){ 
    G_ExceptionList.lthrow("You must define a maximum number of stucks (NUMB_STUCKS) in the input file.");
    exit(7);
  }else{
    NUMB_STUCKS = param.get_int("NUMB_STUCKS");
  }
  
  // lower bound
  if (!param.defined("ABORT_BOUND")){
    G_ExceptionList.lthrow("You must define a lower bound (ABORT_BOUND) for the objective in the input file.");
    exit(7);
  }else{
    ABORT_BOUND = param.get_double("ABORT_BOUND");
  }
  
  // type of neighbours to generate (ENUM or RAND)
  if(METHOD==II||METHOD==TS){

    if (!param.defined("NGBH_TYPE")){
      G_ExceptionList.lthrow("You must define a neighbourhood type (NGBH_TYPE) for II and TS in the input file.");
      exit(7);
    }else{
      ngbh_type_in = param.get_string("NGBH_TYPE");
      if(ngbh_type_in == "ENUM") NGBH_TYPE = ENUM;
      else if(ngbh_type_in == "RAND") NGBH_TYPE = RAND;
      else{
	std::cout << ngbh_type_in << " is no valid neighbourhood type." << std::endl;
	std::cout << "Must be ENUM or RAND." << std::endl;
	exit(7);
      }
    }

  }
    
  // start probability and number of stucks after which to increase temperature/threshold
  if(METHOD==SA||METHOD==SA_anti||METHOD==TA){
    
    if(!param.defined("PROB")){
      G_ExceptionList.lthrow("You must define a start probability (PROB) for SA, SA_anti and TA in the input file.");
      exit(7);
    }else{
      PROB = param.get_int("PROB");
    }

    if(!param.defined("MAX_STUCK")){
      G_ExceptionList.lthrow
	("You must define a number of stucks (MAX_STUCK) for SA, SA_anti and TA in the input file.");
      exit(7);
    }else{
      MAX_STUCK = param.get_int("MAX_STUCK");
    }
      
  }
  
  // length of tabulist and number of neighbours to generate in each step
  if(METHOD==TS){
     if(!param.defined("TABU_LENGTH")){
      G_ExceptionList.lthrow("You must define the length of the tabu list (TABU_LENGTH) for TS in the input file.");
      exit(7);
    }else{
      TABU_LENGTH = param.get_int("TABU_LENGTH");
    }
     
     if(!param.defined("NUMB_NGB")){
      G_ExceptionList.lthrow
	("You must define the number of neighbours to generate in each step (NUMB_NGB) for TS in the input file.");
      exit(7);
    }else{
      NUMB_NGB = param.get_int("NUMB_NGB");
    }
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
  Lisa_Iterator it;
  
  // init aborts
  it.set_abort_at_stuck(NUMB_STUCKS);
  it.set_abort_at_bound(ABORT_BOUND);

  // init algottihm type and parameters 
  if(METHOD==II) it.init(METHOD,NGBH_TYPE);
  else if(METHOD==SA||METHOD==SA_anti||METHOD==TA) it.init(METHOD,PROB,MAX_STUCK);
  else if(METHOD==TS) it.init(METHOD,TABU_LENGTH,NUMB_NGB,NGBH_TYPE);
  
  //go for it ;)
  it.iterate(nbh,1,STEPS);

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
  
