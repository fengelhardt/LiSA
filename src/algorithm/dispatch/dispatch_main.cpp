/**
 * @author  Lars Dornheim
 * @version 2.3final
 */

#include <unistd.h>
#include <fstream>
#include <iostream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

#include "dispatcher.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]){

  // print a message that the program has started:
  cout << "This is the LiSA Dispatcher Module" << endl;
  cout << "PID= " << getpid() << endl; 

  // The Lisa_Exceptionlist is forced for writing
  // error messages to cout. Then LiSA is able
  // to show the error messages of an external module.
  G_ExceptionList.set_output_to_cout();   

  // open files and assure existence:
  if (argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
    
  // The both parameter of the module call are the name of the
  // input file and the name of the output file:
  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  if (!i_strm){
    cout << "ERROR: cannot open file " << argv[1] << " for reading." << endl;
    exit(1);
  }
  if(!o_strm){
    cout << "ERROR: cannot open file " << argv[2] << " for writing." << endl;
    exit(1);  
  }
  i_strm.close();
  o_strm.close();
   
   // Define the objects for the file communication 
   Lisa_ProblemType lpr;
   Lisa_ControlParameters sp;
   Lisa_Values my_values;
   
   // read problem instance:
   //initialize class
   LisaXmlFile::initialize();
   //create Input handler
   LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
   
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
   //get Problem
   if( !(xmlInput >> lpr))
   {
     cout << "ERROR: cannot read ProblemType , aborting program." << endl;
     exit(1);
   }
   //get ControlParameters
   if( !(xmlInput >> sp))
   {
     cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
     exit(1);
   }
   //get Values
   if( !(xmlInput >> my_values))
   {
     cout << "ERROR: cannot read Values , aborting program." << endl;
     exit(1);
   }
   // if something else went wrong
   if (!G_ExceptionList.empty())
   {
     cout << "ERROR: cannot read input , aborting program." << endl;
     exit(1);
   }
   
   // Define the LiSA  schedule object for storing results
   Lisa_Schedule out_schedule(my_values.get_n(),my_values.get_m());
   out_schedule.make_LR();
   
   Lisa_Dispatcher engine;
   engine.SetProblem(&lpr, &my_values, &out_schedule);
   engine.SetRule(sp.get_string("RULE"));
   
   if(sp.get_string("NONDELAY")=="TRUE"){
     engine.dispatch_nondelay();
   }else{
    if (sp.get_string("ACTIVE")=="TRUE") engine.dispatch_active(); 
    else engine.dispatch();
   }
   // The object out_schedule contain the result of this algorithm,
   // which is written into the output file
   // write results to output file:
   //create xml output handler
   LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
   //pipe objects to this
   xmlOutput << lpr << my_values << sp << out_schedule;
   //write content to a file
   xmlOutput.write(argv[2]);
}

//**************************************************************************

