/*
 * ************** pr.cpp *******************************
 * 
 * This are Priorityrules for Single-, Open-Shop
 *
 * Owner: Marco Kleber
 *
 * 21.05.2001
*/

#include<iostream>
#include"../../basics/matrix.hpp"
#include"../../main/global.hpp"
#include"../../lisa/ctrlpara.hpp"
#include"../../scheduling/schedule.hpp"
#include"../../scheduling/mo_jo.hpp"
#include"../../lisa/ptype.hpp"
#include"../../lisa/lvalues.hpp"
#include<unistd.h>
#include<fstream>
#include"../../misc/except.hpp"

using namespace std;

int maxM(Lisa_Matrix<double> M, int s);
int minM(Lisa_Matrix<double> M, int s);
int maxV(Lisa_Vector<double> V);
int minV(Lisa_Vector<double> V);
bool okM(Lisa_Matrix<double> M, int s);
int minM1(Lisa_Matrix<double> M, int s, int z);

int n,m;double T;    

int main(int argc, char *argv[]) 
{

    // comment out following line to send error messages to console 
    //  G_ExceptionList.set_output_to_cout();   

    Lisa_ProblemType * lpr = new Lisa_ProblemType;
    Lisa_ControlParameters * sp = new Lisa_ControlParameters;
    Lisa_Values * my_werte=new Lisa_Values;

    cout << "PID= " << getpid() << endl;

    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);
    i_strm >> (*lpr);      // read problem description
    i_strm >> (*sp);       // read control parameters:
    i_strm >> (*my_werte); // read problem instance:
        
    // LiSA  schedule object for storing results
    Lisa_Schedule * out_schedule = new Lisa_Schedule(my_werte->get_n(),
						     my_werte->get_m());
    out_schedule->make_LR();


    	
    	string Regel;
	n=my_werte->get_n();
	m=my_werte->get_m();

	if (lpr->get_property(M_ENV)==ONE)
	  {if(lpr->get_property(OBJECTIVE)==LMAX)
	    Regel="MS";
	  if(lpr->output_beta()=="r_i") Regel="ERD";}
	if (lpr->get_property(M_ENV)==O)
	  if(m==2) Regel="LAPT";

	if (Regel=="MS")
		{ 
		Lisa_Vector<double> D=(*my_werte->DD),MS(n);
		double t=0;int a,lr=1;

		do{
		   MS.fill(0);
		   for (int i=0;i<n;i++) 
		     {MS[i]=D[i]-(*my_werte->PT)[i][0]-t;
		      if(MS[i]<0) 
			{if(D[i]!=-1) MS[i]=0;
		        else MS[i]=-1;}
		     }
 		   a=maxV(MS);
		   (*out_schedule->LR)[a][0]=lr;
		   t=t+(*my_werte->PT)[a][0];
		   D[a]=-1;
		   lr++;
		  }
		while (lr<n+1);
		}

	if (Regel=="ERD")
 	        {Lisa_Vector<double> R=(*my_werte->RD);
		 double M=R[maxV(R)]+1;int k;
		 for (int l=1;l<n+1;l++)
		  {k=minV(R);
	           (*out_schedule->LR)[k][0]=l;
		   R[k]=M;}
 		}
	
	if (Regel=="LAPT")
		{
		Lisa_Matrix<double> P(n,m),H(n,m),P2(n,m);
		Lisa_Matrix<double> PR(n,m),PR2(n,m);   
		P2=(*my_werte->PT); 
 		int mc,k;
		for (int d=0;d<m;d++)
		  {d==0?mc=1:mc=0;
		   P=P2;
		   for (int p=1;p<n+1;p++)
		     {k=maxM(P,d);
		      for (int x=0;x<n;x++)
		        {if (x!=k && int(P[x][d])==int(P[k][d]))
		            {if (P[x][mc]>P[k][mc]) k=x;}
		        }
		      H[k][mc]=p;
		      P[k][d]=0;
		     }
		  }
		
		for (int j=0;j<m;j++)
		  for (int i=0;i<n;i++)
 		    cout << "WARNING: H= " << H[i][j] << endl;

		P=(*my_werte->PT);
		PR.fill(0);PR2.fill(0);
		T=n+m;
		int la=1,lb=1,a,b,ka,kb,aa,ab;
		double ta=0,tb=0,h;
		bool ma=false,mb=false,gleich=false;
		
		a=minM(H,0);b=minM(H,1);
		
                if (a==b)
		  {if (P[a][0]>P[b][1])
		     {
		      ta=P[a][0];
		      PR[a][0]=PR2[a][0]=la;
		      h=H[a][1];
		      H[a][1]=T;
		      b=minM(H,1);
		      H[a][1]=n+1;
		      PR[b][1]=PR2[b][1]=lb;
		      H[a][0]=H[b][1]=T;
		      tb=P[b][1];
		      la++;lb++;}		
		   else
		     {tb=P[b][1];
		      PR[b][1]=PR2[b][1]=lb;
		      h=H[b][0];
		      H[b][0]=T;
		      a=minM(H,0);
		      H[b][0]=n+1;
		      PR[a][0]=PR2[a][0]=la;
		      H[b][1]=H[a][0]=T;
		      ta=P[a][0];
		      lb++;la++;}}
                else
		    {PR[a][0]=PR[b][1]=PR2[a][0]=PR2[b][1]=1;
		     ta=P[a][0];
		     tb=P[b][1];
		     ka=kb=n;
		     H[a][0]=H[b][1]=T;
		     la++;lb++;}

		do{
		  if (ta<tb && !mb)
		    {A:
		     a=minM(H,0);
		     if ((la==int(PR[a][1])||a==maxM(PR,1))&&gleich==false)
 		       {k=a;
			h=H[k][0];
			H[k][0]=T;
			a=minM(H,0);
			H[k][0]=h;}
		     PR[a][0]=PR2[a][0]=la;
		     if (PR2[a][0]!=0 && PR2[a][1]!=0)
		        PR2[a][0]=PR2[a][1]=0;
		     H[a][0]=T;
		     ta=ta+P[a][0];
		     la++;aa=0;
		     for(int z=0;z<n;z++)
		       if(H[z][0]<(m+n)) aa++;
		     if (aa==0) ma=true;
		     gleich=false;
		    }

		  if (ta>tb && !ma)
		    {B:
		     b=minM(H,1);
		     if ((lb==int(PR[b][0])||b==maxM(PR,0))&&gleich==false)
		       {k=b;
			h=H[k][1];
			H[k][1]=T;
			b=minM(H,1);
			H[k][1]=h;}
		     PR[b][1]=PR2[b][1]=lb;
		     if (PR2[b][1]!=0 && PR2[b][0]!=0)
		        PR2[b][1]=PR2[b][0]=0;
		     H[b][1]=T;
		     tb=tb+P[b][1];
		     lb++;ab=0;
		     for(int z=0;z<n;z++)
		       if(H[z][1]<(m+n))ab++;
		     if (ab==0) mb=true;
		     gleich=false;
		    }

		  if(ta==tb && !ma && !mb)
		    {aa=0,ab=0;
		     gleich=true;
		     a=b=int(T);
		     for(int z=0;z<n;z++)
		       {if(H[z][0]<(m+n))aa++;
		        if(H[z][1]<(m+n))ab++;}
		     if (aa<ab) {goto B;}
		     else {goto A;}
		  }}
		while (ma==false && mb==false);


		if (ma)
		  {do
		     {if (okM(PR2,0))
			{b=minM1(PR2,0,0);
		         if (PR[b][0]==lb) lb++;
		         PR[b][1]=lb;
		         PR2[b][0]=0;
		         H[b][1]=T;
		         lb++;
			}
		      else mb=true;
		     }
		   while (mb==false);
		  }
		else
		  {do
		     {if (okM(PR2,1))
		        {a=minM1(PR2,1,0);
		         if (PR[a][1]==la) la++;
		         PR[a][0]=la;
		         PR2[a][1]=0;
		         H[a][0]=T;
		         la++;
		        }
		      else ma=true;
		     }
		   while (ma==false);
		  }
	
		for (int w=0;w<2;w++)
  		  for (int i=0;i<n;i++)
    		   (*out_schedule->LR)[i][w]=int(PR[i][w]);

		
		}

    // write results to output file:
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_werte;
    delete lpr;
}

int maxM(Lisa_Matrix<double> M, int s)
{
int i=0,j=1;
while (j<n+1)
  {if (M[i][s]<M[j][s]){i=j;j++;}else j++;}
return i;
}

int minM(Lisa_Matrix<double> M, int s)
{
int i=0,j=1;
while (j<n+1)
  {if (M[i][s]>M[j][s]){i=j;j++;}else j++;}
return i;
}

int maxV(Lisa_Vector<double> V)
{
int i=0,j=1;
while (j<n+1)
  {if (V[i]<V[j]){i=j;j++;}else j++;}
return i;
}

int minV(Lisa_Vector<double> V)
{
int i=0,j=1;
while (j<n+1)
  {if (V[i]>V[j]){i=j;j++;}else j++;}
return i;
}

bool okM(Lisa_Matrix<double> M, int s)
{
double x=0;bool ok;
for (int i=0;i<n;i++) x=x+M[i][s];
if (x>0) ok=true; else ok=false;
return ok;
}

int minM1(Lisa_Matrix<double> M, int s,int z)
{
Lisa_Matrix<double> N(n,m);
N=M;
for (int i=0;i<n;i++)
if (N[i][s]==z) N[i][s]=T;
return minM(N,s);
}
