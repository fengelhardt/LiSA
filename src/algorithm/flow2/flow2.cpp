/*
 * ************** flow2.cpp *******************************
 *
 * Owner: Thomas Klemm
 *
 * 13.07.2001
*/

#include<iostream.h>
#include"../../basics/matrix.hpp"
#include"../../main/global.hpp"
#include"../../lisa/ctrlpara.hpp"
#include"../../scheduling/schedule.hpp"
#include"../../lisa/ptype.hpp"
#include"../../lisa/lvalues.hpp"
#include<unistd.h>
#include<fstream.h>
#include"../../misc/except.hpp"

int n,m;

int main(int argc, char *argv[]) 
{

    cout << "PID= " << getpid() << endl; 

    // comment out following line to send error messages to console, 
    //  G_ExceptionList.set_output_to_cout();   

    Lisa_ProblemType * lpr = new Lisa_ProblemType;
    Lisa_ControlParameters * sp = new Lisa_ControlParameters;
    Lisa_Values * my_werte=new Lisa_Values;
    
    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);

    // read problem description 
    i_strm >> (*lpr);

    // read control parameters: 
    i_strm >> (*sp);

    // read problem instance:
    i_strm >> (*my_werte);
    
    // LiSA  schedule object for storing results
    Lisa_Schedule * out_schedule = new Lisa_Schedule(my_werte->get_n(),
						     my_werte->get_m());
    out_schedule->make_LR();
    
    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************


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


    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    
    // write results to output file:
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_werte;
    delete lpr;
}

