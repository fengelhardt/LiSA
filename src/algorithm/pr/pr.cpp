/**
 * @author  Marco Kleber
 * @version 3.0
 */

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <utility>

#include "../../xml/LisaXmlFile.hpp"

#include "../../basics/matrix.hpp"
#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../scheduling/mo_jo.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

using namespace std;

//**************************************************************************

int n,m;
double T;  

//**************************************************************************

int maxM(Lisa_Matrix<double> M, int s){
  int i=0,j=1;
  while (j<n){
    if(M[i][s]<M[j][s]){
      i=j;
      j++;
    }else j++;
  }
  return i;
}

//**************************************************************************

int minM(Lisa_Matrix<double> M, int s){
  int i=0,j=1;
  while (j<n){
    if (M[i][s]>M[j][s]){
      i=j;
      j++;
    }else j++;
  }
  return i;
}

//**************************************************************************

int maxV(Lisa_Vector<double> V){
  int i=0,j=1;
  while (j<n){
    if (V[i]<V[j]){
      i=j;
      j++;
    }else j++;
  }
  return i;
}

//**************************************************************************

int minV(Lisa_Vector<double> V){
  int i=0,j=1;
  while (j<n){
    if (V[i]>V[j]){
      i=j;
      j++;
    }else j++;
  }
  return i;
}

//**************************************************************************

bool okM(Lisa_Matrix<double> M, int s){
  double x=0;
  bool ok;
  for (int i=0;i<n;i++) x=x+M[i][s];
  if (x>0) ok=true;
  else ok=false;
  return ok;
}

//**************************************************************************

int minM1(Lisa_Matrix<double> M, int s,int z)
{
  Lisa_Matrix<double> N(n,m);
  N=M;
  for (int i=0;i<n;i++)
    if (N[i][s]==z) N[i][s]=T;
  return minM(N,s);
}  

//**************************************************************************

int main(int argc, char *argv[]){
  
  // write errors do lisa can catch them up
  G_ExceptionList.set_output_to_cout();   
  
  if(argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
  
  cout << "PID= " << getpid() << endl;
  
  ofstream o_strm(argv[2]);
  ifstream i_strm(argv[1]);
  if (!i_strm){
    cout << "ERROR: cannot open file '" << argv[1] << "' for reading." << endl;
    exit(1);
  }
  if(!o_strm){
    cout << "Could not open '"<<argv[2]<<"' for writing." << endl;
    exit(1);
  }
  o_strm.close();
  i_strm.close();
  
  Lisa_ProblemType lpr;
  Lisa_ControlParameters sp;
  Lisa_Values my_werte;
  
  // read problem instance:
  //initialize class
  LisaXmlFile::initialize();
  //create Input handler
  LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
  
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
  if( !(xmlInput >> lpr))
  {
    cout << "ERROR: cannot read ProblemType , aborting program." << endl;
    exit(1);
  }
  //get ControlParameters
  if( !(xmlInput >> sp))
  {
    cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
    exit(1);
  }
  //get Values
  if( !(xmlInput >> my_werte))
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
  
  
  // LiSA  schedule object for storing results
  Lisa_Schedule out_schedule(my_werte.get_n(),my_werte.get_m());
  out_schedule.make_LR();
  
  
  string Regel="";
  n=my_werte.get_n();
  m=my_werte.get_m();
  
  if (lpr.get_property(M_ENV)==ONE){
    if(lpr.get_property(OBJECTIVE)==LMAX && lpr.output_beta()=="") Regel="MS";
    if(lpr.get_property(OBJECTIVE)==CMAX && lpr.output_beta()=="r_i") Regel="ERD";
  }
  
  if(lpr.get_property(M_ENV)==O && m==2 &&
    lpr.output_beta()=="" && lpr.get_property(OBJECTIVE)==CMAX ) Regel="LAPT";
  
  if(Regel == ""){
    G_ExceptionList.lthrow("PR can not handle "+lpr.output_problem()+".",
    Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(1);
  }
  
  
  if (Regel=="MS"){ //fixed by marc
    Lisa_Vector<double> duedate(*my_werte.DD);
    Lisa_Vector<double> slack(n);
    
    double t=0;
    for(int lr=1;lr<=n;lr++){
      
      //compute slack
      for(int i=0;i<n;i++){
        slack[i] = std::max<double>(duedate[i]-(*my_werte.PT)[i][0]-t,0);
      }
      
      //find minimal slack
      int a;
      for(int i=0;i<n;i++){
        if(duedate[i] >= 0){
          a = i;
          break;
        }
      }
      
      for(int i=0;i<n;i++){
        if(duedate[i] >= 0 && slack[i] < slack[a]){
          a = i;
        }
      }
      
      //schedule a
      (*out_schedule.LR)[a][0]=lr;
      t = t + (*my_werte.PT)[a][0];
      duedate[a] = -1;
    }
  }
  
  if (Regel=="ERD")
  {Lisa_Vector<double> R=(*my_werte.RD);
    double M=R[maxV(R)]+1;int k;
    for (int l=1;l<n+1;l++)
    {k=minV(R);
      (*out_schedule.LR)[k][0]=l;
    R[k]=M;}
  }
  
  if (Regel=="LAPT"){
    Lisa_Matrix<double> P(n,m),H(n,m),P2(n,m);
    Lisa_Matrix<double> PR(n,m),PR2(n,m);   
    P2=(*my_werte.PT); 
    int mc,k;
    for (int d=0;d<m;d++){
      
      d==0 ? mc=1 : mc=0;
      P=P2;
      
      for (int p=1;p<n+1;p++){
        
        k=maxM(P,d);
        for (int x=0;x<n;x++){
          if (x!=k && int(P[x][d])==int(P[k][d])){
            if (P[x][mc]>P[k][mc]) k=x;
          }
        }
        H[k][mc]=p;
        P[k][d]=0;
      }
    }
    /*
    for (int j=0;j<m;j++)
      for (int i=0;i<n;i++)
        cout << "WARNING: H= " << H[i][j] << endl;
    */
    P=(*my_werte.PT);
    PR.fill(0);PR2.fill(0);
    T=n+m;
    int la=1,lb=1,a,b,ka,kb,aa,ab;
    double ta=0,tb=0,h;
    bool ma=false,mb=false,gleich=false;
    
    a=minM(H,0);
    b=minM(H,1);
    
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
        (*out_schedule.LR)[i][w]=int(PR[i][w]);
    
    
  }
  
  // write results to output file:
  //create xml output handler
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  //pipe objects to this
  xmlOutput << lpr << my_werte << sp << out_schedule;
  //write content to a file
  xmlOutput.write(argv[2]);
  
  return 0;
}

//**************************************************************************

