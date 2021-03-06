/**
 * @author  Thomas Tautenhahn
 * @version 3.0
 */

#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../basics/list.hpp"
#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"

#include "os_bb.hpp"
#include "js_bb.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]) 
 {
   OS_BB * my_os_bb=0; 
   JS_BB * my_js_bb=0;
   Lisa_GenericBb * my_bb;

   // print a message that the programm started:
   cout << "This is the LiSA-Branch&Bound Module Version 10.11.1999" << endl;
   G_ExceptionList.set_output_to_cout();   

   // open files and assure existence:
   if (argc != 3) 
     {
       cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
       exit(1);
     }
   cout << "PID= " << getpid() << endl; 
   ifstream i_strm(argv[1]);
   ofstream o_strm(argv[2]);
   if (!i_strm){
     cout << "ERROR: bb cannot find input file " << argv[1] << "." << endl;
     exit(1);
   }
   if (!o_strm){
     cout << "ERROR: bb cannot open output file " << argv[2] << "." << endl;
     exit(1);
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
      case O: my_bb=my_os_bb=new OS_BB;
	break;
      case F:
      case ONE:
      case J: my_bb=my_js_bb=new JS_BB;
	break;
      default: G_ExceptionList.lthrow("unknown problem type, aborting");
        exit(1);
      }
   zfn=lpr.get_property(OBJECTIVE);
   if ((zfn==IRREG1)||(zfn==IRREG2))
     {
       cout << "ERROR: bb cannot handle irregular objective functions. " << 
	 "Aborting program." << endl;
       exit(1);
     }

   
   // control parameters: 
   if (sp.defined("INS_ORDER")==Lisa_ControlParameters::STRING)
     {
       if (sp.get_string("INS_ORDER")=="LPT") 
	 my_bb->set_ins_order(LPT_ORDER);
     }
   if (sp.defined("NB_SOLUTIONS")==Lisa_ControlParameters::LONG)
      my_bb->set_output_bound(sp.get_long("NB_SOLUTIONS"));
   if (sp.defined("UPPER_BOUND")==Lisa_ControlParameters::DOUBLE)
      my_bb->set_upper_bound((TIMETYP) sp.get_double("UPPER_BOUND"));
   if (sp.defined("LOWER_BOUND")==Lisa_ControlParameters::DOUBLE)
      my_bb->set_lower_bound((TIMETYP) sp.get_double("LOWER_BOUND"));
  
  
   // handle problem instance and cast it into a Lisa_ShopProblem:
   Lisa_OsProblem * my_os_problem=0;
   Lisa_JsProblem * my_js_problem=0;
   switch (problem_type)
      {
      case O: my_os_problem=new Lisa_OsProblem(&my_values);
	break;
      case ONE:
      case F:
      case J: my_js_problem=new Lisa_JsProblem(&my_values);
	break;
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

   // solve the problem and store results in a list:
   Lisa_List<Lisa_Matrix<int> *>* results = new Lisa_List<Lisa_Matrix<int> *>;
   switch (problem_type)
      {
        case O: my_os_bb->run(my_os_problem,zfn,results);
                delete my_os_problem;
                break;
        case ONE:
        case F:
        case J: my_js_bb->run(my_js_problem,zfn,results);
	        delete my_js_problem;
                break;
      }

   // write results to output file:
   Lisa_Matrix<int> * outputlr;
   Lisa_Schedule * out_schedule = new Lisa_Schedule;
   out_schedule->init(n, m);
   
   type = LisaXmlFile::SOLUTION; 
   if (results->empty()){
     G_ExceptionList.lthrow("no schedules found (maybe upper bound wrong?)");
     type = LisaXmlFile::INSTANCE;
   }
   
   Lisa_List<Lisa_ScheduleNode> SchedList;
   
   while (!results->empty()){ 
     outputlr= results->pop();
     out_schedule->LR = outputlr;
     SchedList.append(Lisa_ScheduleNode(out_schedule));
     delete outputlr;
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
   cout << "\nProgram finished after " << my_bb->get_nr_insert()
        << " insertions (" << my_bb->get_nr_insert_ok() 
        << " of these successfull)." << endl;
   delete my_bb;
 }

//**************************************************************************

