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
#include "../../scheduling/mo_jo.hpp"

using namespace std;

//**************************************************************************

int n,m;

//**************************************************************************

int main(int argc, char *argv[]) 
{

    //  comment out following line to send error messages to console
    //  G_ExceptionList.set_output_to_cout();   

    Lisa_ProblemType * lpr = new Lisa_ProblemType;
    Lisa_ControlParameters * sp = new Lisa_ControlParameters;
    Lisa_Values * my_werte=new Lisa_Values;
    
    cout << "PID= " << getpid() << endl; 

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
    

//J2-algorithm ( johnson rule for job shop)
 
  n=my_werte->get_n(); 
  m=my_werte->get_m(); 
  Lisa_Matrix<bool> S(n,m); 
  Lisa_Matrix<double> P(n,m); 
  Lisa_Matrix<int> M(n,m), J(n,m), PR(n,m); 
  P=(*my_werte->PT); 
  S=(*my_werte->SIJ);
  my_werte->MO->write_rank(&M);
  Lisa_Vector<int> I1(n), I2(n), I12(n), I21(n); 
 
  int a=0; int b=0; int c=0; int d=0; 
  int i=0; int j=0;
	 

//check validity of processing times matrix
//Zulässigkeit der Bearbeitungszeitenmatrix 
 
	for (i=0;i<n;i++) 
		{if (P[i][0]==0)	 
			{if (S[i][0]==true) 
			{cout << "WARNING: SIJ is inconsistent with PT !" << endl; 
			 cout << "WARNING: results invalid!" << endl;
			 break;} 
			} 
		if (P[i][1]==0) 
			{if (S[i][1]==true) 
			{cout << "WARNING: SIJ is inconsistent with PT ! " << endl;
			 cout << "WARNING: results invalid!" << endl; 
			break;} }
		} 

	 for (i=0;i<n;i++)
		{if (S[i][0]==false)
			{if (S[i][1]==false) 
				{cout << "WARNING: SIJ is 0 for both machines at Job " << i+1 << endl; 
				 break;}}} 


  //subdivision in sets
 
  for (i=0; i<n; i++) 
	{ 
	if (M[i][0]<M[i][1]) 
	   { 
	    if (M[i][0]==0) {I2[b]=i; b++;} 
	    else	    {I12[c]=i; c++;} 
	   } 
	else	 
	   { 
	    if (M[i][1]==1) {I21[d]=i; d++;} 
	    else	    {I1[a]=i; a++;} 
	   } 
	} 

 
  //Johnson step
 
  Lisa_Matrix<double> P12(c,m), P21(d,m);
  Lisa_Matrix<int> PR12(c,m), PR21(d,m); 
  int x=0; 
 
  for (i=0; i<c; i++) 
	{x=I12[i]; P12[i][0]=P[x][0]; P12[i][1]=P[x][1];}  
	 
  for (i=0; i<d; i++) 
	{x=I21[i]; P21[i][0]=P[x][1]; P21[i][1]=P[x][0];} 


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		 
	//JOHNSON RULE : F2-algorithm for 
        //P12 : n=c and P21 : n=d 
	//result:	PR12(c,m) and PR21(d,m)  

	int e=1;
	int f=c;
	double Max=0;

 	while (e<f+1) 
 
	 { int k=0; int l=0; 
	   for (i=0; i<c; i++) 
	       {for (int j=0; j<m; j++) 
	           {if (P12[i][j]<P12[k][l]) {k=i; l=j;} 
		    else  
			{if (Max<P12[i][j])   {Max=P12[i][j]+1;} 
			} //Minimum on P12 , stored at k and l; min=P12[k][l] 
		    }
		}
 
  	if (l==0)					//insert in lists 
 		{PR12[k][0]=e; PR12[k][1]=e+1; e++;} 
 
	else	{PR12[k][1]=f+1; PR12[k][0]=f; f--;} 
		         	
	P12[k][0]=Max; P12[k][1]=Max;			//remove job k
 	    						// PR12 ready

	}


	e=1;
	f=d;
	Max=0;

 	while (e<f+1) 
 
	 { int k=0;int l=0; 
	   for (i=0; i<d; i++) 
	       {for (int j=0; j<m; j++) 
	           {if (P21[i][j]<P21[k][l]) {k=i; l=j;} 
		    else  
			{if (Max<P21[i][j])   {Max=P21[i][j]+1;} 
			} 
		   } //Minimum auf P21 bestimmt, abgespeichert bei k und l 
		} 
 
  	if (l==0)					//insert in lists
 		{PR21[k][1]=e; PR21[k][0]=e+1; e++;} 
 
	else	{PR21[k][0]=f+1; PR21[k][1]=f; f--;} 
		         	
	P21[k][0]=Max; P21[k][1]=Max;			//remove job k
 	}    						// PR21 ready


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // sort results for maschine 0 

	int cneu=c; 	for (i=0; i<c; i++) 
		{x=I12[i]; J[x][0]=PR12[i][0];}				//PR12 
	 
	for (i=0; i<a; i++) 	
		{x=I1[i]; J[x][0]=cneu+1; cneu++;}			//I1 
		 		 
	for (i=0; i<d; i++) 
		{x=I21[i]; J[x][0]=PR21[i][0]+cneu-1;}			//PR21

  //sort results for maschine 1 
 
	int dneu=d;
	for (i=0; i<d; i++) 
		{x=I21[i]; J[x][1]=PR21[i][1];}				//PR21 
	 
	for (i=0; i<b; i++) 
		{x=I2[i]; J[x][1]=dneu+1; dneu++;}			//I2 
		  		 
	for (i=0; i<c; i++) 
		{x=I12[i]; J[x][1]=PR12[i][1]+dneu-1;}			//PR12 
		

	   
  //calculate LR from M and J  
 
	Lisa_Matrix<int> MJ(n,m);  
	Lisa_Vector<int> z(n), s(m); 
 
	  for (i=0;i<n;i++) 
	    	{for (j=0;j<m;j++) 
			{MJ[i][j]=M[i][j]+J[i][j];} 
		} 
	   
	  c=0; 
	  int k=1; 
	  while (c<n*m) 
		{ 
	      for (i=0;i<n;i++)	{z[i]=0;} 
	      for (j=0;j<m;j++)	{s[j]=0;} 
 
	      f=0; 
 	      for (i=0;i<n;i++) 
			{for (j=0;j<m;j++) 
		    		{if (MJ[i][j]==2) 
					{f=1; 
					  c++; 
					  z[i]=1; 
					  s[j]=1; 
					  PR[i][j]=k; 
					} 
		    		} 
			} 
	      k++; 
		if (f==0) {c=n*m;} 
		else
		     {for (i=0;i<n;i++) 
			    {for (j=0;j<m;j++) 
				{if (MJ[i][j]!=0) 
			    		{MJ[i][j]=MJ[i][j]-z[i]-s[j];} 
				} 
		      }   } 
		}	 
 
	for (i=0;i<n;i++)   
		{for (j=0; j<m; j++)  {(*out_schedule->LR)[i][j]=PR[i][j];} 
		}	  
 
    
    // write results to output file:
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_werte;
    delete lpr;
}

//**************************************************************************

