/*
 * ************** bb_main.h *******************************
 * 
 * branch and bound external LiSA module
 *
 * Owner: Thomas Tautenhahn
 *
 * 25.10.1999
*/


#include<iostream>
#include"os_bb.hpp"
#include"js_bb.hpp"
#include"../../basics/list.hpp"
#include"../../main/global.hpp"
#include"../../lisa/ctrlpara.hpp"
#include"../../scheduling/schedule.hpp"
#include"../../lisa/ptype.hpp"
#include"../../lisa/lvalues.hpp"
#include<unistd.h>
#include<fstream>

using namespace std;

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
   if (!i_strm)
     {
       cout << "ERROR: bb cannot find input file " << argv[1] << "." << endl;
       exit(1);
     }
   

   // read problem description and decide whether program is applicable:
   Lisa_ProblemType * lpr = new Lisa_ProblemType;
   i_strm >> (*lpr);
   if (!G_ExceptionList.empty())
     {
       cout << "ERROR: bb cannot read problem type, aborting program." << endl;
       exit(1);
     }  
   int zfn, problem_type;
   problem_type=lpr->get_property(M_ENV);
   string cannot_handle="";
   if (lpr->get_property(PMTN)) cannot_handle="preemption";
   if (lpr->get_property(PRECEDENCE)!=EMPTY) 
     cannot_handle="precedence constraints"; 
   if (lpr->get_property(BATCH))  cannot_handle="batching"; 
   if (lpr->get_property(NO_WAIT))  cannot_handle="no-wait constraints";
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
        G_ExceptionList.lthrow("no algorithm implemented, using job shop",
			       WARNING);
      case J: my_bb=my_js_bb=new JS_BB;
	break;
      default: G_ExceptionList.lthrow("unknown problem type, aborting");
        exit(1);
      }
   zfn=lpr->get_property(OBJECTIVE);
   if ((zfn==IRREG1)||(zfn==IRREG2))
     {
       cout << "ERROR: bb cannot handle irregular objective functions. " << 
	 "Aborting program." << endl;
       exit(1);
     }
   delete lpr;
   
   // read control parameters: 
   Lisa_ControlParameters * sp = new Lisa_ControlParameters;
   i_strm >> (*sp);
   if (sp->defined("INS_ORDER")==STRING)
     {
       if (sp->get_string("INS_ORDER")=="LPT") 
	 my_bb->set_ins_order(LPT_ORDER);
     }
   if (sp->defined("NB_SOLUTIONS")==INT)
      my_bb->set_output_bound(sp->get_int("NB_SOLUTIONS"));
   if (sp->defined("UPPER_BOUND")==DOUBLE)
      my_bb->set_upper_bound((TIMETYP) sp->get_double("UPPER_BOUND"));
   if (sp->defined("LOWER_BOUND")==DOUBLE)
      my_bb->set_lower_bound((TIMETYP) sp->get_double("LOWER_BOUND"));
   delete sp;
      
   // read problem instance and cast it into a Lisa_ShopProblem:
   Lisa_Values * my_werte=new Lisa_Values;
   i_strm >> (*my_werte);
   Lisa_OsProblem * my_os_problem=0;
   Lisa_JsProblem * my_js_problem=0;
   switch (problem_type)
      {
      case O: my_os_problem=new Lisa_OsProblem(my_werte);
	break;
      case ONE:
      case F:
      case J: my_js_problem=new Lisa_JsProblem(my_werte);
	break;
      }
   if ((problem_type==F)||(problem_type==ONE))
     G_ExceptionList.lcatch(WARNING);
   if (!G_ExceptionList.empty(INCONSISTENT_INPUT))
     {
       cout << "ERROR: No valid input in bb, aborting program." << endl;
       exit(1);
     }
   int n=my_werte->get_n(); 
   int m=my_werte->get_m();
   delete my_werte;

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
   if (results->empty())
     G_ExceptionList.lthrow("no schedules found (maybe upper bound wrong?)");
   while (!results->empty())
        { 
          outputlr= results->pop();
          out_schedule->LR = outputlr;
          o_strm << *out_schedule;
	  delete outputlr; out_schedule->LR=NULL;
        } 
   delete out_schedule;
   delete results; 

   // end of program:  
   cout << "\nProgram finished after " << my_bb->get_nr_insert()
        << " insertions (" << my_bb->get_nr_insert_ok() 
        << " of these successfull)." << endl;
   delete my_bb;
 }











