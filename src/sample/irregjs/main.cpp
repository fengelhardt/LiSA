/*
 * ************** bb_main.h *******************************
 * 
 * branch and bound external LiSA module
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
*/


#include<iostream.h>
#include<fstream.h>
#include"irregjs.hpp"
#include"../../lisa_dt/ptype.hpp"

int main(int argc, char *argv[]) 
 {
   
   // open files and assure existence:
   if (argc != 3) 
     {
       cerr << "\nUsage: " << argv[0] << " [input file] [output file]\n";
       exit(1);
     }
   ifstream i_strm(argv[1]);
   ofstream o_strm(argv[2]);
   if (!i_strm)
     {
       cerr << "\nERROR: input file " << argv[1] << " not found!\n";
       exit(1);
     }
   

   // make fake problem:
   Lisa_Values * my_werte=new Lisa_Values;
   my_werte->init(20,5);
   my_werte->make_PT;
   
   //read problem:
   NM_JsProblem * Prob=new NM_JsProblem(my_werte);
   delete my_werte;
   i_strm >> Prob;
   
   delete Prob;
   return 0;
 }  












