/*
 * ************** irj_main.cpp *******************************
 * 
 * part of neumann project
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
*/


#include<iostream.h>
#include<fstream.h>
#include"irregjs.hpp"
#include"insert.hpp"
#include"append.hpp"
#include"../../lisa_dt/problem.hpp"

int main(int argc, char *argv[]) 
 {
   //   G_ExceptionList.set_output_to_cerr(); 

   // open files and assure existence:
   if (argc != 4) 
     {
       cerr << "\nUsage: " << argv[0] << 
       " [input file 1 (problem)] [input file 2 (schedule)] [output file]\n";
       exit(1);
     }
   ifstream i_strm(argv[1]);
   ifstream i2_strm(argv[2]);
   ofstream o_strm(argv[3]);
   if (!i_strm)
     {
       cerr << "\nERROR: input file " << argv[1] << " not found!\n";
       exit(1);
     }
   

   // make fake problem:
   Lisa_Values * my_werte=new Lisa_Values;
   my_werte->init(20,5);
   my_werte->make_PT();
   
   //read problem:
   NM_JsProblem * Prob=new NM_JsProblem(my_werte);
   delete my_werte;
   i_strm >> *Prob;

   /*  // make it harder:
   int ji, jj; 
   for (ji=1; ji<=Prob->n; ji++)
     { 
       (*Prob->di)[ji]= 125 + (int) ((*Prob->di)[ji]*0.5);
       for (jj=1; jj<=Prob->m; jj++)
	 (*Prob->time)[ji][jj]+=1;
     }
     */
   //read schedule
   NM_JsSchedule * Sched=new NM_JsSchedule(Prob);
   i2_strm >> *Sched;
   Sched->SetValue(IRREG2);
   cout.width(10);
   cout << Sched->GetValue() << "    ";
   delete Sched;

   // now my try:
   Sched=new NM_JsSchedule(Prob);
   js_inserter* engine=new js_inserter;
   engine->init(Sched, IRREG2);
   engine->insert();
   Sched->SetValue(IRREG2);
   cout.width(10);   
   cout << Sched->GetValue();
   delete engine;  
   delete Sched;

   // easier method:
   Sched=new NM_JsSchedule(Prob);
   ECT_reverse(Sched);
   Sched->SetValue(IRREG2);
   cout.width(10);   
   cout << Sched->GetValue();
   delete Sched;

   cout << endl;
   delete Sched;
   delete Prob;
   return 0;
 }  




