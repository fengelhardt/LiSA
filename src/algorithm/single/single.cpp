/*
 * ************** single.cpp *******************************
 * 
 * Owner: Christian Schulz
 *
 * 23.02.2000
*/
#include <stdlib.h>
#include<iostream>
#include<string.h>
#include<time.h>
#include"../../main/global.hpp"
#include"../../lisa/ctrlpara.hpp"
#include"../../scheduling/schedule.hpp"
#include"../../lisa/ptype.hpp"
#include"../../lisa/lvalues.hpp"
#include<unistd.h>
#include<fstream>
#include"../../misc/except.hpp"
#include"../../basics/graph.hpp"

#include"../../scheduling/js_sched.hpp"
#include"../../scheduling/sp_sched.hpp"
#include"../../basics/order.hpp"
#include"Single_Machine_BB.hpp"

using namespace std;

/** @name Solving Single Machine Problems

    This is a branch and bound algorithm to solve 
    single machine problems exactly.

    @author Christian Schulz
    @see SingleMachineBB
 */
//@{
//@Include: Single_Machine_BB.hpp
//@}

int main(int argc, char *argv[]) 
 {
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
       cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
       exit(1);
     }
   
   // read problem description and decide whether program is applicable:
   Lisa_ProblemType * lpr = new Lisa_ProblemType;
   i_strm >> (*lpr);

   //cout<<*lpr<<endl;
   
   if (!G_ExceptionList.empty())
     {
       cout << "ERROR: cannot read problem type, aborting program." << endl;
       exit(1);
     }  
   
 
   if (lpr->get_property(M_ENV)!= 1)
     
     {
       cout << "ERROR: wrong problem type, aborting program." << endl;
       exit(1);
     }

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
   delete lpr;   


   bool mode=true;
   // read control parameters: 
   Lisa_ControlParameters * sp = new Lisa_ControlParameters;
   i_strm >> (*sp);
   if(sp->get_string("SPEED")=="FAST") mode=false;
   delete sp;

   // read problem instance:
   Lisa_Values * my_werte=new Lisa_Values;
   i_strm >> (*my_werte);
   
   // solve the problem and store results in a schedule object
   // Insert your solution algorithm here:
     
   Lisa_Schedule * out_schedule = new Lisa_Schedule(my_werte->get_n(),
                                                    my_werte->get_m());
  
   time_t start;
   time_t end;

   time(&start);

   
   cout<<"single"<<endl;

   SingleMachineBB* smBB=new SingleMachineBB (my_werte, mode);
   smBB->run(out_schedule);

   time(&end);
   cout<<"start "<<start<<" end "<<end<<endl;
   cout<<"runtime "<<end-start<<endl;

   //delete graph;

   out_schedule->make_LR();
  
   // write results to output file:

   o_strm << *out_schedule;
   delete out_schedule;
   delete my_werte;
 }



  










