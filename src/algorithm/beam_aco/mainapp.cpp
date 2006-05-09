/*
 *  mainapp.cpp
 *  beamaco
 *
 *  Created by Per Willenius on 03.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

//#include "stdafx.h"
#include <stdlib.h>
//#include <unistd.h>

#include <fstream>
#include <iostream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../basics/list.hpp"
#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"

#include "os_ba.h"
//#include "js_bb.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[])  
//int ba_main(int argc, char *argv[]) 
 {

	 printf("ddff");
//	 exsssit(1);
   OS_BA * my_os_ba=0; 
   //Lisa_GenericBb * my_ba;

   // print a message that the programm started:
	 std::cout << "This is the LiSA-BEAM ACO Module Version 0.1" << std::endl;
   G_ExceptionList.set_output_to_cout();   

   // open files and assure existence:
   if (argc != 3) 
     {
			 std::cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
       exit(1);
     }
//		 std::cout << "PID= " << getpid() << std::endl; 
   ifstream i_strm(argv[1]);
   ofstream o_strm(argv[2]);
	 			 
   if (!i_strm){
     cout << "ERROR: bb cannot find input file " << argv[1] << "." << endl;
		 return -1;
     // exit(1);
   }
   if (!o_strm){
     cout << "ERROR: bb cannot open output file " << argv[2] << "." << endl;
		 return -1;
     //exit(1);
   }
   i_strm.close();
   o_strm.close();

    //initialize class
    LisaXmlFile::initialize();
    //create Input handler
    LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
    //communication objects
    Lisa_ProblemType lpr;
    Lisa_ControlParameters sp;   
    Lisa_Values my_values;
    
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
    
  
   // problem type
   int zfn, problem_type;
   problem_type=lpr.get_property(M_ENV);
   string cannot_handle=""; 
   if (lpr.get_property(PMTN)) cannot_handle="preemption";
   if (lpr.get_property(PRECEDENCE)!=EMPTY) 
     cannot_handle="precedence constraints"; 
   if (lpr.get_property(BATCH))  cannot_handle="batching"; 
   if (lpr.get_property(NO_WAIT))  cannot_handle="no-wait constraints";
   if (cannot_handle!="")  
      {
        cout << "ERROR: bb cannot handle " << cannot_handle << 
	  ". Aborting program."<< endl;
        exit(1);
      }  
   switch (problem_type)
      {
      case O: my_os_ba=new OS_BA;
	break;

      default: G_ExceptionList.lthrow("unknown problem type, aborting");
        exit(1);
      }
   zfn=lpr.get_property(OBJECTIVE);
   if ((zfn==IRREG1)||(zfn==IRREG2))
     {
       cout << "ERROR: ba cannot handle irregular objective functions. This Version is optimized for SumCi" << 
	 "Aborting program." << endl;
       exit(1);
     }

   
   // control parameters: 
	 	/*
	 <integer name="BEAM_WIDTH" caption="Beamweite" default="100"/>
    <real name="LOWER_BOUND" caption="Untere Schranke" default="0"/>
    <real name="UPPER_BOUND" caption="Obere Schranke" default="1e+06"/>
    <choice name="EXTENSION_STRATEGIE" caption="Bestimmung der Beam Extensions">
      <item name="MED"/>
      <item name="LDS"/>
    </choice>
    <integer name="STEPS" caption="Anzahl Durchlaeufe" default="50"/>
    <real name="CONVERGENCE_FACTOR" caption="Konvergenzfaktor fuer Neustart" default="0.9"/>
    <real name="EVAPORATION_RATE" caption="evaporationRate" default="0.7" />
*/

/*
	int para_BEAM_WIDTH;
	double para_LOWER_BOUND;
	double para_UPPER_BOUND;
	int para_EXTENSION_STRATEGY;
	int para_STEPS;
	double para_CONVERGENCE_FACTOR;
	double para_CONVERGENCE_FACTOR;
	*/
   if (sp.defined("BEAM_WIDTH")==Lisa_ControlParameters::LONG)
      my_os_ba->para_BEAM_WIDTH=sp.get_long("BEAM_WIDTH");
	if (sp.defined("UPPER_BOUND")==Lisa_ControlParameters::DOUBLE)
      my_os_ba->set_upper_bound((TIMETYP) sp.get_double("UPPER_BOUND"));
   if (sp.defined("LOWER_BOUND")==Lisa_ControlParameters::DOUBLE)
      my_os_ba->set_lower_bound((TIMETYP) sp.get_double("LOWER_BOUND"));
   if (sp.defined("EXTENSION_STRATEGY")==Lisa_ControlParameters::STRING)
     {
       if (sp.get_string("EXTENSION_STRATEGY")=="MED") 
					my_os_ba->para_EXTENSION_STRATEGY=1;
			 else
					my_os_ba->para_EXTENSION_STRATEGY=2;
     }
		 		 
   if (sp.defined("STEPS")==Lisa_ControlParameters::LONG)
      my_os_ba->para_STEPS=sp.get_long("STEPS");
   if (sp.defined("CONVERGENCE_FACTOR")==Lisa_ControlParameters::DOUBLE)
     my_os_ba->para_CONVERGENCE_FACTOR=sp.get_double("CONVERGENCE_FACTOR");
   if (sp.defined("CONVERGENCE_FACTOR")==Lisa_ControlParameters::DOUBLE)
      my_os_ba->para_CONVERGENCE_FACTOR=sp.get_double("CONVERGENCE_FACTOR");
   
  
   // handle problem instance and cast it into a Lisa_ShopProblem:
   Lisa_OsProblem * my_os_problem=0;
  // Lisa_JsProblem * my_js_problem=0;
   switch (problem_type)
      {
      case O: my_os_problem=new Lisa_OsProblem(&my_values);
	//break;
   //   case ONE:
   //   case F:
   //   case J: my_js_problem=new Lisa_JsProblem(&my_values);
	//break;
      }
   if ((problem_type==F)||(problem_type==ONE))
     G_ExceptionList.lcatch(Lisa_ExceptionList::WARNING);
   if (!G_ExceptionList.empty(Lisa_ExceptionList::INCONSISTENT_INPUT))
     {
       cout << "ERROR: No valid input in bb, aborting program." << endl;
       exit(1);
     }
   int n=my_values.get_n(); 
   int m=my_values.get_m();


	  cout << "vor run_beam_search " << endl;
   // solve the problem and store results in a list:
   Lisa_List<Lisa_Matrix<int> *>* results = new Lisa_List<Lisa_Matrix<int> *>;
   switch (problem_type)
      {
        case O: my_os_ba->run_aco_beam_search(my_os_problem,zfn,results);
                delete my_os_problem;
                break;
    //    case ONE:
     //   case F:
     //   case J: my_js_bb->run(my_js_problem,zfn,results);
	   //     delete my_js_problem;
     //           break;
      }

   // write results to output file:

  cout << "nach run_beam_search " << endl;


   Lisa_Matrix<int> * outputlr;
   Lisa_Schedule * out_schedule = new Lisa_Schedule;
   out_schedule->init(n, m);
   
   type = LisaXmlFile::SOLUTION; 
   if (results->empty()){
     G_ExceptionList.lthrow("no schedules found (maybe upper bound wrong?)");
     type = LisaXmlFile::INSTANCE;
   }
   

	 // debug only: damit es kompiliert wird
	 
	 
   Lisa_List<Lisa_ScheduleNode> SchedList;
    cout << "vor results" << endl;
   while (!results->empty()){ 
     outputlr= results->pop();

		 // cout << "results.append "<< *outputlr << endl;		 
     out_schedule->LR = outputlr;
     SchedList.append(Lisa_ScheduleNode(out_schedule));
		 // Hier Speicherproble,
     //delete outputlr;
     out_schedule->LR=NULL;
		 
   } 
   delete out_schedule;
   delete results;
	 
   
   LisaXmlFile xmlOutput(type);
   xmlOutput << lpr << my_values << sp;
   if(type == LisaXmlFile::SOLUTION)
   xmlOutput << SchedList;
 

xmlOutput.write(argv[2]);

   // end of program:  
   cout << "\nProgram finished after " << my_os_ba->get_nr_insert()
        << " insertions (" << my_os_ba->get_nr_insert_ok() 
        << " of these successfull)." << endl;
   delete my_os_ba;
 }

//**************************************************************************


