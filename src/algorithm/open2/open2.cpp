/*
 * ************** open2.cpp *******************************
 *
 * Owner: Thomas Klemm
 *
 * 13.07.2001
*/

#include<iostream.h>
#include"../../basics/matrix.hpp"
#include"../../basics/global.hpp"
#include"../../lisa_dt/ctrlpara.hpp"
#include"../../lisa_dt/schedule.hpp"
#include"../../lisa_dt/ptype.hpp"
#include"../../lisa_dt/lvalues.hpp"
#include<fstream.h>
#include"../../basics/except.hpp"

int n,m;

int main(int argc, char *argv[]) 
{

    // Auskommentieren, falls die Fehlermeldungen weitergesendet werden sollen
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
    
   
//O2-Algorithmus 
 
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
 


	//Zulässigkeit der Bearbeitungszeitenmatrix 
 
	for (i=0;i<n;i++) 
		{if (P[i][0]==0)	 
			{if (S[i][0]==true) 
			{cout << "WARNING: SIJ bzgl PT unzulaessig!" << endl; 
			 cout << "WARNING: Ergebnisse sind falsch" << endl;
   	 			break;}} 
 		if(P[i][1]==0) 
			{if (S[i][1]==true) 
			{cout << "WARNING: SIJ bzgl PT unzulaessig!" <<endl;
			 cout << "WARNING: Ergebnisse sind falsch" <<endl;
			 break;}}  		} 
 
	//Mengeneinteilungen 
 
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
	
	
  	//Maximumsuche 
 
	int k=I1[0]; int l=I2[0]; int x=0; int y=0; int v=0;
	for (i=0; i<a; i++) 
		{x=I1[i]; 
		if (P[x][0] > P[k][0])	{k=x;} 
		}			// Maximum bzgl I1 auf M1 bei Job k   
	for (j=0; j<b; j++) 
		{ x=I2[j]; 
		if (P[x][1] > P[l][1])	{l=x;} 
		}			// Maximum bzgl I2 auf M2 bei Job l 
 
	if (P[k][0]>=P[l][1])	{v=k;} 
	else			{v=l;}	// Maximum bei Job v 

	
	//Neudeklaration der I-Listen 
 
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

	
	// Bestimmung vom lat. Rechteck PR 
	 
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
		x=I4[i]; PR[x][0]=y; PR[x][1]=0; y++;	//Rest von M1 
		} 
	 
	for (i=0; i<c; i++) 
		{ 
		x=I3[i]; PR[x][1]=z; PR[x][0]=0; z++;	//Rest von M2 
		}
	
	
 	//LR-AUSGABE

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

