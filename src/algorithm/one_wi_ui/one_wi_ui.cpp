/**
 * @author  LiSA
 * @version 2.3final
 */

// Include the header files for the used objects! Standard:
// LisaProblemtype requires global.hpp.
// The file global.hpp is needed by LisaProblemtype.
// The files ctrlpara.hpp, ptype.hpp and l.hpp
// are used for parsing the input file.
// The object Lisa_Schedule (header: schedule.hpp) 
// is necessary for writing the result into the output file.
// The file except.hpp is used for the exception handling.  

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <queue>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../scheduling/m1_sched.hpp"
#include "../../basics/order.hpp"
#include "../../basics/matrix.hpp"

using namespace std;


/////////////////////////////
struct Compare_PT{
	Compare_PT(Lisa_Values* V):Values(V){}
	
	bool operator()(int i, int j){
		return (*(Values->PT))[i][0] > (*(Values->PT))[j][0];
	}
	private: Lisa_Values* Values;
};

typedef std::priority_queue <int, vector<int>, Compare_PT> MyQueue;	//MyQueue neu definiert
//////////////////////////////




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
    
    //Mengen verwalten
    Lisa_Order I(Values.get_n());
    std::list <int> I_v;
    
    MyQueue I_star = MyQueue(Compare_PT(&Values));
    int d_i = -1;    
    //std::vector V(Values.get_n());    //V intialisieren, in V stehen 2-Tupel (index des Jobs i, p_i)
    
    
    
    //Merker für Indizes
    int i, j, i_star, k, l;
    int  temp, ell = 0;
    TIMETYP d_i_star;
    TIMETYP sum = 0;
    i_star = -1;
    

    	for (i = 0; i < Values.get_n(); i++){
    		I.read_one_key(i, (*Values.DD)[i]);
		cout << (*Values.DD)[i] << '\t';		
	}
		
	I.sort();		//Duedates sortiert
	
	sum = 0;
	for (i = 0; i < Values.get_n(); i++){
		i_star = I.row(i);
		cout << "istern = " << i_star << "[" << (*Values.DD)[i_star] << "]" << endl;
		d_i_star = (*Values.DD)[i_star];
		I_star.push(i_star);
		
		sum += (*Values.PT)[i_star][0];
		
		if (sum > d_i_star){
			//suche Job aus I_star mit größter PT
			cout << sum << " > " << d_i_star << endl;
			cout << "removing job " << I_star.top() << "[" << (*Values.PT)[I_star.top()] << "]" << std::endl;
			ell = I_star.top();
			I_star.pop();
			

			//Element l hat größte Bearbeitungszeiten der schon angeordneten Jobs			
			sum -= (*Values.PT)[ell][0];
			//I_v.push_back(ell);
		}
	}
	
	i = 1;	
	while (!I_star.empty()){
		(*(out_schedule.LR))[I_star.top()][0]=i++;
		I_star.pop();
	}
	for (k = 0; k < Values.get_n(); k++){
		if (!(*(out_schedule.LR))[k][0])
			(*(out_schedule.LR))[k][0]=i++;
	}
	
	
	cout << "i =" <<i;
 
    

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

//**************************************************************************

