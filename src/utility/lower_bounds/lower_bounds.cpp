/**
 * @author  LiSA
 * @version 3.0
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
#include <cassert>
#include <cmath>


#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"

using namespace std;

//**************************************************************************

typedef TIMETYP TimeType;

TIMETYP lb_Cmax_maxbased(Lisa_Values& values) {

    TIMETYP maximum = 0;

    TIMETYP *sum_row = new double[values.get_m()];
    assert(sum_row);
    
    for (int i=0; i < values.get_m(); ++i) {
	sum_row[i] = 0;
    }

    for (int i=0; i <values.get_n(); ++i) {
	
	TIMETYP sum_col = 0; 
	
	for (int j=0; j <values.get_m(); ++j) {
	 
	    TIMETYP pij = (*values.PT)[i][j];
	    sum_col += pij;
	    sum_row[j] += pij;
	}
	
	if (sum_col > maximum) {
	    maximum = sum_col;
	}
    }

    for (int i=0; i < values.get_m(); ++i) {
	if (maximum < sum_row[i])
	    maximum = sum_row[i];
    }
    
    delete[] sum_row;
    
    return maximum;
}

/* ----- lower bound as described in paper .... ----*/

static int cmp_TIMETYP(const void *pa, const void *pb) {
    TIMETYP a = *( reinterpret_cast<const TIMETYP *>(pa) );
    TIMETYP b = *( reinterpret_cast<const TIMETYP *>(pb) );
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}


int longest_Job(TIMETYP *p_row, TIMETYP *p_col, int num) {
    int longest = -1;
    for (int i = 0; i < num; ++i) {
	
	if ( ( p_row[i] < p_col[i] ) ) {
	    if ( (longest == -1) || (p_row[longest] < p_row[i]) )
		longest = i;
	}
    }
    return longest;
};

int first_M(TIMETYP *p_row,TIMETYP *p_col,int num) {
    for (int i =0; i < num; ++i) {
	if (p_row[i] > p_col[i])
	    return i;
    }
    return -1;
}

TIMETYP lb_sumCi_preemt_based(Lisa_Values& values) {
    
    const unsigned n = values.get_n();
    const unsigned m = values.get_m();
    
    unsigned num = n;
    if (m > num) 
	num = m;
    
    TIMETYP *p_row = new TIMETYP[num]; 
    TIMETYP *p_col = new TIMETYP[num]; 

    assert(p_row);
    assert(p_col);
    
    /* fill the array with zero -
       This should normally be done by the new operator, 
       but we don't trust.
    */
    for (unsigned i = 0; i < num; ++i) {
	p_row[i] = 0;
	p_col[i] = 0;
    }
    
    /* sum up p_row and p_col */
    for (unsigned i=0; i < static_cast<unsigned>( values.get_n() ); ++i) {
	for (unsigned j=0; j < static_cast<unsigned>( values.get_m() ); ++j) {
	    TIMETYP v = (*values.PT)[i][j];
	    p_row[i] += v;
	    p_col[j] += v;
	}
    }
    
    /* sort p_row and p_col in non-decreasing order */
    qsort(p_row, num, sizeof(TIMETYP), cmp_TIMETYP);
    assert(p_row[0] <= p_row[num-1]); /* simple check */
    
    qsort(p_col, num, sizeof(TIMETYP), cmp_TIMETYP);
    assert(p_col[0] <= p_col[num-1]); /* simple check */
    
    TIMETYP lb = 0;
    TIMETYP de, di;
    
    for (unsigned i = 0; i < num; ++i) {
	
	lb += p_row[i];
	
	if (p_row[i] > p_col[i]) {
	    
	    /* case 1 */
	    
	  
	    TIMETYP di = p_col[i] - p_row[i];

	    bool fini = false;
	    do {
		int l = longest_Job(p_row, p_col, num);
		de = p_col[l];
		
		if ( p_row[l] + di < de ) {
		    de = p_row[l] + di;
		    fini = true;
		}
		di = di - de + p_row[l];
		p_row[l] = de;
	    } while ( ! fini );
	
	} else if (p_row[i] > p_col[i]) {
	    
	    /* case 2 */
	    
	    di = p_col[i] - p_row[i];
	    
	    bool fini = false;
	    do {
		int first = first_M(p_row, p_col, num);
		de = p_row[first];
		if ( p_col[first] + di < de) {
		    de = p_col[first] + di;
		    fini = true;
		}
		di -= de + p_col[first];
		p_col[first] = de;
		
	    } while (!fini);
		
	}
	
    } // end for (...)

    delete[] p_row;
    delete[] p_col;
    
    return lb;
}

TIMETYP lb_sumCi_avrg_based(Lisa_Values& values) { 
    
    const unsigned n = values.get_n();
    const unsigned m = values.get_m();
    
    TIMETYP sumP(0);
    for (unsigned i=0; i < n ; ++i) 
	for (unsigned j=0; j < m; ++j) 
	    sumP += (*values.PT)[i][j];
	
    div_t ab = div( (int)n,(int)m);
    unsigned a = ab.quot;
    unsigned b = ab.rem;
    
    assert( n == a * m + b );
    
    unsigned long sumCi = ( ( m * m * ( 1 + a ) * a) / 2 ) + (b * ( a + 1 ) * m );
    return TIMETYP( sumCi ) * sumP / TIMETYP( m * n );

}





int main(int argc, char *argv[]) 
{

    // print a message that the program has started:
    cout << "This is the LiSA Lower Bounds Module" << endl;

    // The Lisa_Exceptionlist is forced for writing
    // error messages to cout. Then LiSA is able
    // to show the error messages of an external module.
    //G_ExceptionList.set_output_to_cout();   

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
    
    int stages=MAX(Values.get_n(),Values.get_m());
    int i,j;
    
    for (i=0; i <Values.get_n(); i++)
      for (j=0; j <Values.get_m(); j++)
	if ((*Values.SIJ)[i][j])
	  (*out_schedule.LR)[i][j]= ((i+j) % stages)+1;

    // The following lines demonstrate how to write into an extra LiSA window:
    //      cout << "WARNING: The Problemtype is:" << Problem.output_problem()<< endl;
    //      cout << "WARNING: The upper bound is:" << Parameter.get_double("UPPER_BOUND")<< endl;
    //      cout << "WARNING: P(1,2)=" << (*Values.PT)[1][2] << endl;
    //      cout << "WARNING: Name:" << Parameter.get_string("NAME")<< endl;
    
    cout << "WARNING: lower bound (max method) : " << lb_Cmax_maxbased(Values)
	     << " (preempt) : " << lb_sumCi_preemt_based(Values)
	     << " (avrg) : " << lb_sumCi_avrg_based(Values) << endl; 
    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    

    //    Parameter.add_key("LOWER_BOUND", (*Values.PT)[1][1] );
    
    // The object out_schedule contain the result of this algorithm,
    // which is written into the output file
    //create xml output handler
    LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
    //pipe objects to this
    xmlOutput << Problem << Values << Parameter << out_schedule;
    //write content to a file
    xmlOutput.write(argv[2]);
}

//**************************************************************************

/* This stuff is for emacs
 * Local variables:
 * mode:c++
 * c-basic-offset: 4
 * End:
 */
