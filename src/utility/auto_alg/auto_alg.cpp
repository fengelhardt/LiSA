/**
 * @author  Marc Moerig
 * @version 2.3final
 */ 

//**************************************************************************

#include <string>
#include <iostream>
#include <fstream>

#include "../../misc/except.hpp"
#include "../../lisa/ctrlpara.hpp"

//**************************************************************************

int m=10,n=10;
long seed=918273645;
int lowerrange=1,upperrange=100;
int numberproblems=1;
int numberalgorithms=1;

//**************************************************************************

void parseParameters(Lisa_ControlParameters &cp){
   
}

//**************************************************************************

int main(int argc, char *argv[]){
 
 G_ExceptionList.set_output_to_cout();
 
 if(argc < 3){
  G_ExceptionList.lthrow((std::string)"Usage: "+argv[0]+
                         " [input file] [output path]",ANY_ERROR);
  exit(-1);  
 }
 
 std::ifstream in_file(argv[1]);
 if(! in_file){
   G_ExceptionList.lthrow((std::string)"Could not open '"+argv[1]+
                          "' for reading.",FILE_NOT_FOUND);
   exit(-1);   
 }
  
 Lisa_ControlParameters cp;
 in_file >> cp;
 
 if(!G_ExceptionList.empty()) exit(-1);
 
 parseParameters(cp);
 
 
 return 0;
}
