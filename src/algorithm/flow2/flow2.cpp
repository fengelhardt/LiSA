/**
 * @author  Thomas Klemm
 * @version 2.3pre3
 */

#include <unistd.h>

#include <fstream>
#include <iostream>

#include "../../basics/matrix.hpp"
#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

using namespace std;

//**************************************************************************

int n,m;

//**************************************************************************

int main(int argc, char *argv[]){

    // write errors so lisa can catch them up
    G_ExceptionList.set_output_to_cout();   

    if(argc != 3){
     cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
     exit(1);
    }
    
    Lisa_ProblemType * lpr = new Lisa_ProblemType;
    Lisa_ControlParameters * sp = new Lisa_ControlParameters;
    Lisa_Values * my_werte=new Lisa_Values;
    
    cout << "PID= " << getpid() << endl; 

    ifstream i_strm(argv[1]);
    if (!i_strm){
     cout << "ERROR: cannot open file '" << argv[1] << "' for reading." << endl;
     exit(1);
    }

    // read problem description 
    i_strm >> (*lpr);

    // read control parameters: 
    i_strm >> (*sp);

    // read problem instance:
    i_strm >> (*my_werte);
    i_strm.close();
    
    if (!G_ExceptionList.empty()){
     cout << "ERROR: cannot read problem data, aborting program." << endl;
     exit(1);
    } 
    
    // LiSA  schedule object for storing results
    Lisa_Schedule * out_schedule = new Lisa_Schedule(my_werte->get_n(),
						     my_werte->get_m());
    out_schedule->make_LR();

//F2 algorithm(johnson rule)
 
  n=my_werte->get_n(); 
  m=my_werte->get_m(); 
  Lisa_Matrix<double> P(n,m); 
  Lisa_Matrix<int> PR(n,m);
  P=(*my_werte->PT);  
 
  int a=1; int b=n; 
  int i; int j;
  double M=0; 
 
  while (a<b+1) 
 
 { int k=0; int l=0; 
   for (i=0; i<n; i++) 
       {for (j=0; j<m; j++) 
           {if (P[i][j]<P[k][l]) {k=i; l=j;} 
	    else  
		{if (M<P[i][j])	{M=P[i][j]+1;} 
	   	} 
	   }	  //computed minimum of P, stored in  k, l 
		  //where min is P[k][l] 
	} 
 
  if (l==0)					//insert in list 
 	{PR[k][0]=a; PR[k][1]=a+1; a++;} 
 
  else	{PR[k][1]=b+1; PR[k][0]=b; b--;} 
		
         
  P[k][0]=M; P[k][1]=M;				//remove job k
  }    	


  for (i=0;i<n;i++)  
	{for (int j=0; j<m; j++)  {(*out_schedule->LR)[i][j]=PR[i][j];}
		}	 
								//end F2 
    
    // write results to output file:
    ofstream o_strm(argv[2]);
    if(!o_strm){
     cout << "Could not open '"<<argv[2]<<"' for writing." << endl;
     exit(1);
    }
    o_strm << *out_schedule;
    o_strm.close();
    
    delete out_schedule;
    delete my_werte;
    delete lpr;
}

//**************************************************************************

