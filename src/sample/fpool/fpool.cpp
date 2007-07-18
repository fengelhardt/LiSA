/**
 * @author  Thomas Tautenhahn
 * @version 2.3final
 */
 
#include <stdlib.h>

#include <fstream>

#include "../../algorithm/nb_iter/iterate.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ctrlpara.hpp"

#include "fp_set.hpp"
#include "fp_vect.hpp"
#include "nb_fp.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[])
 { 
   long seed=1234567123l;
   G_ExceptionList.set_output_to_cerr();
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

   // read special entry: 
   int n, sigma;
   long steps;
   long raisestuck;
   int runs;
   int prob0;
   Lisa_ControlParameters * sp = new Lisa_ControlParameters;
   i_strm >> (*sp);
   n=sp->get_long("DIMENSION");
   sigma=sp->get_long("NUMBER_VECTORS");
   steps=sp->get_long("STEPS");
   raisestuck=sp->get_long("STUCK");
   runs=sp->get_long("NUMBER_RUNS");
   prob0=sp->get_long("PROBAL");
   delete sp;   
  
  
  int i;
  for (i=0; i<runs; i++)
    {
      FP_Neighbourhood* nb=new FP_Neighbourhood(n,sigma);
      
      Lisa_ControlParameters CP;
      CP.add_key("PROB",(long)prob0);
      CP.add_key("TYPE","RAND");
      CP.add_key("MAX_STUCK",raisestuck);
      CP.add_key("ABORT_BOUND",(long)0);
      CP.add_key("NUMB_STUCKS",(long)2*raisestuck);
      //Iterator(SA,prob0,raisestuck);
      Lisa_Iter * it=new Lisa_OldSimulatedAnnealing(&CP);
      // Lisa_Iterator * it=new Lisa_Iterator(TS,40,30);
      cout << "\nstarting run " << i+1;
      nb->solution->random2(&seed);
      //it->set_abort_at_bound(0);
      //it->set_abort_at_stuck(2*raisestuck);    
      it->iterate(nb, 1, steps);
      if (nb->best_solution->get_slack()<=5 )
	o_strm << *(nb->best_solution);
      delete it;
      delete nb;
    }  
}

//**************************************************************************

