#include <fstream.h>
#include "fp_set.hpp"
#include "fp_vect.hpp"
#include "nb_fp.hpp"
#include "../../algorithm/nb_iter/iterate.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ctrlpara.hpp"


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
   n=sp->get_int("DIMENSION");
   sigma=sp->get_int("NUMBER_VECTORS");
   steps=sp->get_int("STEPS");
   raisestuck=sp->get_int("STUCK");
   runs=sp->get_int("NUMBER_RUNS");
   prob0=sp->get_int("PROBAL");
   delete sp;   
  
  
  int i;
  for (i=0; i<runs; i++)
    {
      fp_nb* nb=new fp_nb(n,sigma);
      Lisa_Iterator * it=new Lisa_Iterator();
      cout << "\nstarting run " << i+1;
      nb->solution->random2(&seed);
      it->set_abort_at_bound(0);
      it->set_abort_at_stuck(2*raisestuck);
      it->init(SA,prob0,raisestuck);
      //it->init(TS, 40, 30);      
      it->iterate(nb, 1, steps);
      if (nb->best_solution->get_slack()<=5 )
	o_strm << *(nb->best_solution);
      delete it;
      delete nb;
    }  
}









