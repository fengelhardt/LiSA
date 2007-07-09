/**
 * @author  LiSA
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
#include <stdio.h>
#include <stdlib.h>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"

using namespace std;

//**************************************************************************

void fill_LR_row(Lisa_Matrix<int>& C){

    int s = C.get_m(); // Anzahl der Spalten
    int z = C.get_n(); // Anzahl der Zeilen
    int hilf;          // Hilfsvariabe
    for (int i = 0; i < z; i++) {
      for (int j = 0; j < s; j++) {
        hilf = i+j+1;
	if (hilf > s) hilf = hilf % s;
	C[i][j] = hilf;
      }
    } 
}

void fill_LR_column(Lisa_Matrix<int>& C, int z0, int anzahl_z){

    int s = C.get_m(); // Anzahl der Spalten
    int hilf;          // Hilfsvariable
    anzahl_z = anzahl_z+z0;
    for (int j = 0; j < s; j++) {
      for (int i = z0; i < anzahl_z; i++) {
        hilf = i+j+1;
	if (hilf > anzahl_z) hilf = hilf % anzahl_z + z0;
	C[i][j] = hilf;
      }
    }
}

int main(int argc, char *argv[]) 
{
  long seed2 = 10384776;
  for(int i = 0; i< 50; ++i){
     std::cout << lisa_random(0,3,&seed2) << std::endl;
  }

    // print a message that the program has started:
    cout << "This is the LiSA o_unit Module" << endl;

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
    

    long seed=1234567890;
    if (Parameter.defined("SEED")) seed=Parameter.get_long("SEED");
    cout<<"seed="<<seed<<endl;

    // Define the LiSA  schedule object for storing results
    Lisa_Schedule result(Values.get_n(),Values.get_m());
    
    result.make_LR();
    
    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************
    
    int n = Values.get_n(); // Anzahl der Zeilen
    int m = Values.get_m(); // Anzahl der Spalten
    if (n <= m) {
      fill_LR_row(*(result.LR)); // zeilenweises füllen des Ergebnisschedules
  }
    else {
      int a = n / m;
      int b = n % m;
      int astern;
      int k; // Laufvariable
      int l; // Laufvariable
            
      // astern is a random integer, with astern element {0,...,a-1} 
	astern=(lisa_random(0,a-1,&seed));

      for (k = 1; k < astern+1; k++) {
	fill_LR_column(*(result.LR), (k-1)*m, m);
      }
      fill_LR_column(*(result.LR), (k-1)*m, m+b);
      for (l = 1; l < a-astern; l++) {
	fill_LR_column(*(result.LR), (k+l-1)*m+b, m);
      }
    }

    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    
    // The object out_schedule contain the result of this algorithm,
    // which is written into the output file
    //create xml output handler
    LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
    //pipe objects to this
    xmlOutput << Problem << Values << Parameter << result;
    //write content to a file
    xmlOutput.write(argv[2]);
}

//**************************************************************************

