/**
 * @author  Christian Schulz
 * @version 3.0pre1
 */
 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../basics/order.hpp"
#include "../../basics/graph.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../scheduling/js_sched.hpp"
#include "../../scheduling/sp_sched.hpp"

#include "Single_Machine_BB.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]){
  
   G_ExceptionList.set_output_to_cout();   
   // open files and assure existence:
   if (argc != 3){
     cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
     exit(1);
   }
   
   cout << "PID= " << getpid() << endl; 
   
   ifstream i_strm(argv[1]);
   ofstream o_strm(argv[2]);
   if (!i_strm){
     cout << "ERROR: cannot open file " << argv[1] << "for reading." << endl;
     exit(1);
   }
   if (!o_strm){
     cout << "ERROR: cannot open file " << argv[2] << " for writing." << endl;
     exit(1);
   }
   i_strm.close();    
   o_strm.close();
   
   // read problem description and decide whether program is applicable:
   Lisa_ProblemType lpr;
   Lisa_ControlParameters sp;
   Lisa_Values my_werte;
   
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
    if( !(xmlInput >> my_werte))
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
   
   //check problemtype
   bool accept = true;
   if (lpr.get_property(M_ENV) != 1) accept = false;
   if (lpr.output_beta() != "r_i") accept = false;
   if (lpr.get_property(OBJECTIVE) != LMAX) accept = false;
   if(!accept){
     G_ExceptionList.lthrow("single can not handle "+lpr.output_problem()+".",
                            Lisa_ExceptionList::INCONSISTENT_INPUT);
     exit(1);
   }

   //check parameters
   bool mode=true;
   if(sp.defined("SPEED") == Lisa_ControlParameters::STRING && 
      sp.get_string("SPEED")=="FAST") mode=false;

   
   // solve the problem and store results in a schedule object  
   Lisa_Schedule out_schedule(my_werte.get_n(),my_werte.get_m());
   out_schedule.make_LR();  

   SingleMachineBB* smBB=new SingleMachineBB (&my_werte, mode);
   smBB->run(&out_schedule);

   // write results to output file:
   //create xml output handler
   LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
   //pipe objects to this
   xmlOutput << lpr << my_werte << sp << out_schedule;
   //write content to a file
   xmlOutput.write(argv[2]);
}

//**************************************************************************

