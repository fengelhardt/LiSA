/**
 * @author  Mathisas Zimmermann
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
#include <algorithm>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"

using namespace std;

//**************************************************************************

void sort_by_processing_times(Lisa_Matrix<TIMETYP>& P, Lisa_Matrix<int>& LR, string s) {

    if (s == "RAND") return; // nothing to sort then
    
    int n = P.get_n();
    int m = P.get_m();

    // redundant fields for Values.PT and result.LR
    TIMETYP P_new[n][m];
    int LR_new[n][m];
    // initialize the two arrays
    for (int i = 0; i < n; i++) {
	for (int j = 0; j < m; j++) {
	    P_new[i][j] = 0;
	    LR_new[i][j] = 0;
	}
    }
    int pre[n];  // contains the shortest or longest processing time or the sum of
		 // processing times of each job - depends on the chosen criteria
    int comp[n]; // same values as pre[n] but in ordered form
    // initialize the two arrays
    fill(pre, pre + n, 0);
    fill(comp, comp + n, 0);
    if (s == "MINPTMIN" || s == "MINPTMAX") {
	// store the shortest processing time
    	for (int i = 0; i < n; i++) {
	    pre[i] = P[i][0];
	    for (int j = 1; j < m; j++) {
	    	if (pre[i] > P[i][j]) pre[i] = P[i][j];
	    }
	    comp[i] = pre[i];
    	}
    }
    if (s == "MAXPTMIN" || s == "MAXPTMAX") {
	// store the longest processing time
    	for (int i = 0; i < n; i++) {
	    pre[i] = P[i][0];
	    for (int j = 1; j < m; j++) {
	    	if (pre[i] < P[i][j]) pre[i] = P[i][j];
	    }
	    comp[i] = pre[i];
    	}
    }
    if (s == "LPT" || s == "SPT") {
    	// compute the sums
    	for (int i = 0; i < n; i++) {
	    for (int j = 0; j < m; j++) {
	    	pre[i]  = pre[i]  + P[i][j];
	    }
	    comp[i] = pre[i];
    	}
    }
    // sort the processing times
    sort(comp, comp + n);
    // fill the two fields in the right order
    if (s == "LPT" || s == "MINPTMAX" || s == "MAXPTMAX") {
	for (int i = 0; i < n; i++) {
	    int *row = lower_bound(comp, comp + n, pre[i]);
	    int r = row - comp;
	    while (P_new[n-1-r][0] > 0) r++;
	    for (int j = 0; j < m; j++) {
		P_new[n-1-r][j] = P[i][j];
		LR_new[n-1-r][j] = LR[i][j];
	    }
	}
    }

    if (s == "SPT" || s == "MINPTMIN" || s == "MAXPTMIN") {
	for (int i = 0; i < n; i++) {
	    int *row = lower_bound(comp, comp + n, pre[i]);
	    int r = row - comp;
	    while (P_new[r][0] > 0) r++;
	    for (int j = 0; j < m; j++) {
		P_new[r][j] = P[i][j];
		LR_new[r][j] = LR[i][j];
	    }
	}
    }
    // copy the ordered PT and LR values to the original fields
    for (int i = 0; i < n; i++) {
	for (int j = 0; j < m; j++) {
	    P[i][j]  = P_new[i][j];
	    LR[i][j] = LR_new[i][j];
	}
    }
}

void fill_LR_row(Lisa_Matrix<int>& C) {

    int s = C.get_m(); // number of columns
    int z = C.get_n(); // number of rows
    int hilf;          // auxiliary variable
    // fill the LR row by row
    for (int i = 0; i < z; i++) {
      for (int j = 0; j < s; j++) {
        hilf = i+j+1;
	if (hilf > s) hilf = hilf % s;
	C[i][j] = hilf;
      }
    } 
}

void fill_LQ_of_LR(Lisa_Matrix<int>& C, int z0) {

    int s = C.get_m(); // number of columns
    int z = z0+s;      // number of rows
    int hilf;          // auxiliary variable
 
    for (int i = z0; i < z; i++) {
      for (int j = 0; j < s; j++) {
        hilf = i+j+1;
	if (hilf > z) hilf = hilf % z + z0;
	C[i][j] = hilf;
      }
    } 
}

void fill_LR(Lisa_Matrix<int>& M, int z0, int b) {

    int m = M.get_m(); // number of columns
    int n = m + b +z0; // number of rows
    int workaround = 0;
    if (b == 0) {
	b = 1;
	workaround = 1;
    }
    // fill the first m - b columns  
    for (int i = z0; i < n; i++) {
	for(int j = 0; j < m - b + 1; j++) {
	    int hilf = i+j+1;
	    if (hilf > n) hilf = hilf % n + z0;
	    M[i][j] = hilf;
	}
    }
    if (workaround == 1) b = 0;
    // fill the last b columns row by row till row b
    for (int i = z0; i < b +z0; i++) {
	for(int j = m - b + 1; j < m; j++) {
	    int hilf = i+j+1;
	    if (hilf > m +z0) hilf = hilf % (m+z0) + z0;
	    M[i][j] = hilf;
	}			
    }
    // fill the rest row by row
    for (int i = b +z0; i < n; i++) {
	for(int j = m - b + 1; j < m; j++) {
	    int hilf = i+j+1;
	    if (hilf > n) {
		if(hilf > n + m - b) {
		    hilf = hilf - m;
		}
		else {
		    hilf = hilf % n + M[b-1][j];
		}
	    }
	    M[i][j] = hilf;
	}
    }	
}

int main(int argc, char *argv[]) {

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
    
    int astern = -1;
    if (Parameter.defined("ASTERN")) astern = (int)(Parameter.get_long("ASTERN"));
    cout << "astern = " << astern << endl;
    long seed=1234567890;
    if (Parameter.defined("SEED")) seed = Parameter.get_long("SEED");
    cout << "seed = " << seed << endl;
    string sort = "SPT";
    if(Parameter.defined("SORT")) sort = Parameter.get_string("SORT");
    if(Problem.output_beta() == "p_ij=1") sort = "RAND";
    cout << "sort = " << sort << endl;

    // Define the LiSA  schedule object for storing results
    Lisa_Schedule result(Values.get_n(),Values.get_m());
    
    result.make_LR();
    
    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************
    
    int n = Values.get_n(); // number of rows
    int m = Values.get_m(); // number of columns
    
    if (n <= m) {
	fill_LR_row(*(result.LR)); // fill schedule row by row
    }
    else {
	int a = n / m;
	int b = n % m;
	int k = 1; // control variable
	int l = 1; // control variable       
	// if astern is undefinied or to big it will be generated randomly, astern in {0,...,a-1} 
	if (astern < 0 || astern > a-1) {
	    astern=(lisa_random(0,a-1,&seed));
	}
	// fill the first astern latin squares
	for (k = 1; k < astern+1; k++) {
	    fill_LQ_of_LR(*(result.LR), (k-1)*m);
	}
	// fill the next m + b rows
	fill_LR(*(result.LR), (k-1)*m, b);
	// fill the last a-astern-1 latin squares
	for (l = 1; l < a-astern; l++) {
	    fill_LQ_of_LR(*(result.LR), (k+l-1)*m+b);
	}
    }
    // sort the processing times and the plan
    sort_by_processing_times(*(Values.PT), *(result.LR), sort);

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

