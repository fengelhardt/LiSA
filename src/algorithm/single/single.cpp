/**
 * @author  Christian Schulz
 * @version 2.3final
 */
 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

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
   if (!i_strm){
     cout << "ERROR: cannot open file " << argv[1] << "for reading." << endl;
     exit(1);
   } 
    
   // read problem description and decide whether program is applicable:
   Lisa_ProblemType * lpr = new Lisa_ProblemType;
   i_strm >> (*lpr);

   
   if (!G_ExceptionList.empty()){
     cout << "ERROR: cannot read problem type, aborting program." << endl;
     exit(1);
   }  
   
 
   if (lpr->get_property(M_ENV)!= 1){
     cout << "ERROR: wrong problem type, aborting program." << endl;
     exit(1);
   }

   string cannot_handle="";
   if (lpr->get_property(PMTN)) cannot_handle="preemption";
   if (lpr->get_property(PRECEDENCE)!=EMPTY) 
     cannot_handle="precedence constraints"; 
   if (lpr->get_property(BATCH))  cannot_handle="batching"; 
   if (lpr->get_property(NO_WAIT))  cannot_handle="no-wait constraints";
   if (cannot_handle!=""){
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
   i_strm.close();
   
   // solve the problem and store results in a schedule object  
   Lisa_Schedule * out_schedule = new Lisa_Schedule(my_werte->get_n(),
                                                    my_werte->get_m());
  
   //time_t start;
   //time_t end;

   //time(&start);


   SingleMachineBB* smBB=new SingleMachineBB (my_werte, mode);

   smBB->run(out_schedule);


   //time(&end);
   
   //cout<<"start "<<start<<" end "<<end<<endl;
   //cout<<"runtime "<<end-start<<endl;

   //delete graph;

   out_schedule->make_LR();
  
   // write results to output file:
   ofstream o_strm(argv[2]);
   if (!o_strm){
     cout << "ERROR: cannot open file " << argv[2] << " for writing." << endl;
     exit(1);
   }    
   
   o_strm << *out_schedule;
   o_strm.close();
   
   delete out_schedule;
   delete my_werte;
 }

//**************************************************************************

