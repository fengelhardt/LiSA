/**
 * @author  LiSA
 * @version 2.3rc1
 */

// Include the header files for the used objects! Standard:
// LisaProblemtype requires global.hpp.
// The file global.hpp is needed by LisaProblemtype.
// The files ctrlpara.hpp, ptype.hpp and lvalues.hpp
// are used for parsing the input file.
// The object Lisa_Schedule (header: schedule.hpp) 
// is necessary for writing the result into the output file.
// The file except.hpp is used for the exception handling.  

#include <unistd.h>
#include <iostream>
#include <fstream>

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]) 
{

    // print a message that the program has started:
    cout << "This is the LiSA Sample Module" << endl;

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
    
    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************
    
    int stages=MAX(my_values->get_n(),my_values->get_m());
    int i,j;
    
    for (i=0; i <my_values->get_n(); i++)
      for (j=0; j <my_values->get_m(); j++)
	if ((*my_values->SIJ)[i][j])
	  (*out_schedule->LR)[i][j]= ((i+j) % stages)+1;

    // The following lines demonstrate how to write into an extra LiSA window:
    // cout << "WARNING: The Problemtype is:" << lpr->output_problem()<< endl;
    // cout << "WARNING: The upper bound is:" << sp->get_double("UPPER_BOUND")<< endl;
    // cout << "WARNING: P(1,2)=" << (*my_values->PT)[1][2]<< endl;
    // cout << "WARNING: Name:" << sp->get_string("NAME")<< endl;

    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    
    // The object out_schedule contain the result of this algorithm,
    // which is written into the output file
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_values;
    delete lpr;
}

//**************************************************************************

