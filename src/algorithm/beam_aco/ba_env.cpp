/*
 *  ba_env.cpp
 *  beamaco
 *
 *  Created by Per Willenius on 03.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


//#include "stdafx.h"
// replace the following line for a Windows version:
#include <signal.h>
#include "ba_env.hpp"

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
      
Lisa_GenericBeamACO::Lisa_GenericBeamACO() 
    {
      output_bound=MAXNUMBER;
      steps=steps_ok=0;
      given_upper_bound= (TIMETYP) MAXOBJECTIVE;
      given_lower_bound= (TIMETYP) -MAXOBJECTIVE;
      ins_order_type=RANDOM_ORDER;
    }

//**************************************************************************

void Lisa_GenericBeamACO::count_steps()
 {
   steps++;
   if ((steps % 10000) == 1000)
      cout << "OBJECTIVE= " << upper_bound << endl;
 }    

//**************************************************************************

// WARNING! The following two functions have to be replaced for Windows!
void Lisa_GenericBeamACO::run_start()
 {
   signal(SIGINT, &set_abort);
   upper_bound=given_upper_bound;
   steps=steps_ok=0;
 }

//**************************************************************************

void Lisa_GenericBeamACO::run_stop()
 {
   signal(SIGINT, SIG_DFL);
 }  

//**************************************************************************

