/**
 * @author  Lars Dornheim
 * @version 2.3pre3
 */

#include <unistd.h>
#include <fstream>
#include <iostream>

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

#include "dispatcher.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]) 
{

    // print a message that the program has started:
    cout << "This is the LiSA Dispatcher Module" << endl;

    // The Lisa_Exceptionlist is forced for writing
    // error messages to cout. Then LiSA is able
    // to show the error messages of an external module.
    G_ExceptionList.set_output_to_cout();   

    // open files and assure existence:
    if (argc != 3) 
      {
        cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
        exit(1);
      }

    cout << "PID= " << getpid() << endl; 

    // Define the objects for the file communication 
    Lisa_ProblemType * lpr = new Lisa_ProblemType;
    Lisa_ControlParameters * sp = new Lisa_ControlParameters;
    Lisa_Values * my_values=new Lisa_Values;
    
    // The both parameter of the module call are the name of the
    // input file and the name of the output file:
    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);

    // read problem description, the controlparameters and
    // the problem instance:
    i_strm >> (*lpr);
    i_strm >> (*sp);
    i_strm >> (*my_values);

    // Define the LiSA  schedule object for storing results
    Lisa_Schedule * out_schedule = new Lisa_Schedule(my_values->get_n(),
						     my_values->get_m());
    out_schedule->make_LR();
    
    Lisa_Dispatcher * engine=new Lisa_Dispatcher();
    engine->SetProblem(lpr, my_values, out_schedule);
    engine->SetRule((*sp).get_string("RULE"));
    if ((*sp).get_string("ACTIVE")=="TRUE")
      engine->dispatch_active();
    else 
      engine->dispatch();
    delete engine;
    
    // The object out_schedule contain the result of this algorithm,
    // which is written into the output file
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_values;
    delete lpr;
}

//**************************************************************************

