/**
 * @author  Frank Engelhardt
 * @version 2.3final
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

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"

using namespace std;

void print(int* a, int n);

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

    // The both parameter of the module call are the name of the
    // input file and the name of the output file:
    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);
    if (!i_strm){
      cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
      exit(1);
    }
    if (!o_strm){
      cout << "ERROR: cannot find output file " << argv[1] << "." << endl;
      exit(1);
    }
    i_strm.close();
    o_strm.close();
    
    //initialize class
    LisaXmlFile::initialize();
    //create Input handler
    LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
    //communication objects
    Lisa_ProblemType Problem;
    Lisa_ControlParameters Parameter;   
    Lisa_Values Values;
    
    //parse xml file
    xmlInput.read(argv[1]);
    //determine document type
    LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
    
    //check for successful parsing and valid document type
    if (!xmlInput || !(type == LisaXmlFile::INSTANCE || type == LisaXmlFile::SOLUTION))
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }
    //get Problem
    if( !(xmlInput >> Problem))
    {
      cout << "ERROR: cannot read ProblemType , aborting program." << endl;
      exit(1);
    }
    //get ControlParameters
    if( !(xmlInput >> Parameter))
    {
      cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
      exit(1);
    }
    //get Values
    if( !(xmlInput >> Values))
    {
      cout << "ERROR: cannot read Values , aborting program." << endl;
      exit(1);
    }
    // if something else went wrong
    if (!G_ExceptionList.empty())
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }
    

    // Define the LiSA  schedule object for storing results
    Lisa_Schedule out_schedule(Values.get_n(),Values.get_m());
    out_schedule.make_LR();
    
    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************
    
    // check the problem type
    bool accept = true;
    if( !(Problem.get_property(M_ENV) == ONE || Values.get_m() == 1) ) accept = false;
    if(Problem.get_property(OBJECTIVE) != SUM_UI) accept = false;
    
    if(!accept) {
        cout << "ERROR: algorithm can only handle 1 | | SumUi problems." << endl;
        exit(1);
    }
    
    //cout << "WARNING: problem type is " << Problem.output_problem() << endl;
    
    int n = Values.get_n();
    int i,j;
    
    int *order = new int[n];
    for(i=0; i<n; i++) order[i] = i;
    
    // sort jobs according to their due dates
    bool swapped = true;
    j=n-1;
    while(swapped) {
        swapped = false;
        for(i=0; i<j; i++) {
             if( (*Values.DD)[order[i]] > (*Values.DD)[order[i+1]] ) {
                 int t = order[i];
                 order[i] = order[i+1];
                 order[i+1] = t;
                 swapped = true;
             }
         }
         j--;
    }
    
    int *odd = new int[n];
    int *scheduled = new int[n];
    int numOdd = 0;
    int numScheduled = 0;
    TIMETYP sumPi = 0;
    
    // schedule jobs
    for(i=0; i<n; i++) {
        scheduled[numScheduled++] = order[i];
        sumPi += (*Values.PT)[order[i]][0];
        
        print(scheduled, numScheduled);
        print(odd, numOdd);
        
        if(sumPi > (*Values.DD)[order[i]]) {
             // the last job got delayed, remove the most time consuming job from the schedule
             int max = 0;
             for(j=0; j<numScheduled; j++) {
                 if((*Values.PT)[scheduled[j]][0] > (*Values.PT)[scheduled[max]][0]) max = j;
             }
             
             odd[numOdd++] = scheduled[max];
             sumPi -= (*Values.PT)[scheduled[max]][0];
             numScheduled--;
             
             // reorganize
             for(j=max; j<numScheduled; j++) {
                 scheduled[j] = scheduled[j+1];
             }
        }
    }
    
    // create latin rectangle
    for(i=0; i<numScheduled; i++) (*out_schedule.LR)[scheduled[i]][0] = i+1;
    for(i=0; i<numOdd; i++) (*out_schedule.LR)[odd[i]][0] = i+numScheduled+1;

    // clean up
    delete[] order;
    delete[] odd;
    delete[] scheduled;

    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    
    // The object out_schedule contain the result of this algorithm,
    // which is written into the output file
    //create xml output handler
    LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
    //pipe objects to this
    xmlOutput << Problem << Values << Parameter << out_schedule;
    //write content to a file
    xmlOutput.write(argv[2]);
}

// debug output
void print(int* a, int n) {
    int i;
    for(i=0; i<n; i++) cout << a[i] << " ";
    cout << endl;
}

//**************************************************************************

