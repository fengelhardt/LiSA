/**
 * @author  Thomas Tautenhahn
 * @version 3.0pre1
 */

// replace the following line for a Windows version:
#include <signal.h>

#include "bb_env.hpp"

using namespace std;

//**************************************************************************

bool abort_algorithm;

//**************************************************************************

void set_abort(int i) 
    {  
      abort_algorithm=true;
      cout << "\nSignal " << i << " received, writing results." << endl;
    } 

//**************************************************************************
      
Lisa_GenericBb::Lisa_GenericBb() 
    {
      output_bound=MAXNUMBER;
      steps=steps_ok=0;
      given_upper_bound= (TIMETYP) MAXOBJECTIVE;
      given_lower_bound= (TIMETYP) -MAXOBJECTIVE;
      ins_order_type=RANDOM_ORDER;
    }

//**************************************************************************

void Lisa_GenericBb::count_steps()
 {
   steps++;
   if ((steps % 10000) == 1000)
      cout << "OBJECTIVE= " << upper_bound << endl;
 }    

//**************************************************************************

// WARNING! The following two functions have to be replaced for Windows!
void Lisa_GenericBb::run_start()
 {
   signal(SIGINT, &set_abort);
   upper_bound=given_upper_bound;
   steps=steps_ok=0;
 }

//**************************************************************************

void Lisa_GenericBb::run_stop()
 {
   signal(SIGINT, SIG_IGN);
 }  

//**************************************************************************

