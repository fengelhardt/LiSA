/*
 * ************** open2.cpp *******************************
 *
 * Owner: Thomas Klemm
 *
 * 13.07.2001
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

int n,m;

int main(int argc, char *argv[]) 
{

    // comment out following line to send error messages to console,
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
    
   
    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************
    
   
//O2-algorithm 
 
	n=my_werte->get_n(); 
	m=my_werte->get_m(); 
	Lisa_Matrix<bool> S(n,m);
	Lisa_Matrix<double> P(n,m);
	Lisa_Matrix<int> PR(n,m); 
	P=(*my_werte->PT); 
	S=(*my_werte->SIJ);
	Lisa_Vector<int> I1(n), I2(n), I3(n), I4(n); 
	int a=0; int b=0; int c=0; int d=0; 
	int i=0; int j=0;
 


	//validate processing times matrix
 
	for (i=0;i<n;i++) 
		{if (P[i][0]==0)	 
			{if (S[i][0]==true) 
			{cout << "WARNING: SIJ is inconsistent with PT !" << endl; 
			 cout << "WARNING: results invalid" << endl;
   	 			break;}} 
 		if(P[i][1]==0) 
			{if (S[i][1]==true) 
			{cout << "WARNING: SIJ is inconsistent with PT " <<endl;
			 cout << "WARNING: results invalid" <<endl;
			 break;}}  		} 
 
	//subdivision in sets
 
	for (i=0; i<n; i++) 
		{ 
		if (P[i][0]<=P[i][1])	 
			{if (P[i][0]==0)	{I3[c]=i; c++;} 
			else			{I1[a]=i; a++;} 
			} 
		else	{if (P[i][1]==0)	{I4[d]=i; d++;} 
			else			{I2[b]=i; b++;} 
			} 
		} 
	
	
  	//search maximum 
 
	int k=I1[0]; int l=I2[0]; int x=0; int y=0; int v=0;
	for (i=0; i<a; i++) 
		{x=I1[i]; 
		if (P[x][0] > P[k][0])	{k=x;} 
		}			// Maximum concerning I1 on M1 at job k   
	for (j=0; j<b; j++) 
		{ x=I2[j]; 
		if (P[x][1] > P[l][1])	{l=x;} 
		}			// Maximum concerning I2 on M2 at job l 
 
	if (P[k][0]>=P[l][1])	{v=k;} 
	else			{v=l;}	// Maximum bei Job v 

	
	// redeclare  I-lists 
 
	int aneu=a; int bneu=b; 
	for (i=0; i<a; i++) 
		{if (I1[i]==v)  
			{for (j=i; j<a; j++)	{I1[j]=I1[j+1];}	
			aneu=a-1; 
			} 
		} 
	for (i=0; i<b; i++) 
		{if (I2[i]==v)  
			{for (j=i; j<b; j++)	{I2[j]=I2[j+1];}	 
			bneu=b-1; 
			} 
		} 

	
	// Computation of latin rectangle PR 
	 
	y=1; 	 
	for (i=0; i<aneu; i++) 
		{
		x=I1[i]; PR[x][0]=y; PR[x][1]=y+1; y++;	//I1 
		} 
	 
	for (i=0; i<bneu; i++) 
		{ 
		x=I2[i]; PR[x][0]=y; PR[x][1]=y+1; y++;	//I2 
		} 
	PR[v][0]=y; PR[v][1]=1; y++;
 
	int z=y; 
	for (i=0; i<d; i++) 
		{ 
		x=I4[i]; PR[x][0]=y; PR[x][1]=0; y++;	//remainder of M1 
		} 
	 
	for (i=0; i<c; i++) 
		{ 
		x=I3[i]; PR[x][1]=z; PR[x][0]=0; z++;	//remainder of M2 
		}
	
	
 	//generate LR-output

	 for (i=0; i<n; i++)  
		{for (j=0; j<m; j++) 
			{(*out_schedule->LR)[i][j]=PR[i][j];}  
		}

 
    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    
    // write results to output file:
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_werte;
    delete lpr;
}

