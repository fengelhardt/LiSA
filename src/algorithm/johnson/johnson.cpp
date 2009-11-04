/**
 * @author  Thomas Klemm
 * @version 3.0pre1
 */

#include <unistd.h>

#include <fstream>
#include <iostream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../basics/matrix.hpp"
#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]){
  
  // write errors so lisa can catch them up
  G_ExceptionList.set_output_to_cout();   
  cout << "PID= " << getpid() << endl;
  
  if(argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
  
  
  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  if (!i_strm){
    cout << "ERROR: cannot open file '" << argv[1] << "' for reading." << endl;
    exit(1);
  }
  if(!o_strm){
    cout << "Could not open '"<<argv[2]<<"' for writing." << endl;
    exit(1);
  }
  i_strm.close();
  o_strm.close();
  
  Lisa_ProblemType lpr;
  Lisa_ControlParameters sp;
  Lisa_Values my_values;
  
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
  if( !(xmlInput >> my_values))
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
  
  bool accept=true;
  if(lpr.output_beta() != "" ) accept = false;
  if(lpr.get_property(OBJECTIVE) != CMAX) accept = false;
  const int m_env = lpr.get_property(M_ENV);
  if(m_env != O && m_env != J && m_env != F ) accept = false;
  if(my_values.get_m() != 2) accept = false;
  if(!accept){
    G_ExceptionList.lthrow("Johnson Rule cannot handle "+lpr.output_problem()
                           +" .",Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(1);
  }
  
  const int n=my_values.get_n(); 
  const int m=my_values.get_m();
  // LiSA  schedule object for storing results
  Lisa_Schedule out_schedule(n,m);
  out_schedule.make_LR();
  
  if(m_env == O){ // O2 algorithm
    
    Lisa_Matrix<bool> S(n,m);
    Lisa_Matrix<double> P(n,m);
    Lisa_Matrix<int> PR(n,m); 
    P=(*my_values.PT); 
    S=(*my_values.SIJ);
    Lisa_Vector<int> I1(n), I2(n), I3(n), I4(n); 
    int a=0; int b=0; int c=0; int d=0; 
    int i=0; int j=0;
    
    
    //validate processing times matrix
    for (i=0;i<n;i++){
      if (P[i][0]==0){
        if (S[i][0]==true){
          cout << "WARNING: SIJ is inconsistent with PT !" << endl; 
          cout << "WARNING: results invalid" << endl;
          break;
        }
      } 
      if(P[i][1]==0){
        if (S[i][1]==true){
          cout << "WARNING: SIJ is inconsistent with PT " <<endl;
          cout << "WARNING: results invalid" <<endl;
          break;
        }
      }
    } 
    
    //subdivision in sets
    for (i=0; i<n; i++){ 
      if (P[i][0]<=P[i][1]){
        if (P[i][0]==0){
          I3[c]=i;
          c++;
        }else{
          I1[a]=i;
          a++;
        } 
      }else{
        if(P[i][1]==0){
          I4[d]=i;
          d++;
        }else{
          I2[b]=i;
          b++;
        } 
      } 
    } 
    
    //search maximum 
    int k=I1[0]; int l=I2[0]; int x=0; int y=0; int v=0;
    for (i=0; i<a; i++){
      x=I1[i]; 
      if (P[x][0] > P[k][0]){
        k=x;
      } 
    }// Maximum concerning I1 on M1 at job k   
    for (j=0; j<b; j++){
      x=I2[j]; 
      if (P[x][1] > P[l][1]){
        l=x;
      } 
    }// Maximum concerning I2 on M2 at job l 
    if (P[k][0]>=P[l][1]){
      v=k;
    }else{
      v=l;
    }// Maximum bei Job v 
    
    
    // redeclare  I-lists 
    int aneu=a; int bneu=b; 
    for (i=0; i<a; i++){
      if (I1[i]==v){
        for (j=i; j<a; j++){
          I1[j]=I1[j+1];
        }	
        aneu=a-1; 
      } 
    } 
    for (i=0; i<b; i++){
      if (I2[i]==v){
        for (j=i; j<b; j++){
          I2[j]=I2[j+1];
        }	 
        bneu=b-1;
      } 
    } 
    
    // Computation of latin rectangle PR  
    y=1; 	 
    for (i=0; i<aneu; i++){
      x=I1[i];
      PR[x][0]=y;
      PR[x][1]=y+1;
      y++;	//I1 
    } 
    
    for (i=0; i<bneu; i++){ 
      x=I2[i];
      PR[x][0]=y;
      PR[x][1]=y+1;
      y++;	//I2 
    } 
    PR[v][0]=y;
    PR[v][1]=1;
    y++;
    
    int z=y; 
    for (i=0; i<d; i++){ 
      x=I4[i];
      PR[x][0]=y;
      PR[x][1]=0;
      y++;	//remainder of M1 
    } 
    
    for (i=0; i<c; i++){ 
      x=I3[i];
      PR[x][1]=z;
      PR[x][0]=0;
      z++;	//remainder of M2 
    }
    
    //generate LR-output
    for (i=0; i<n; i++){
      for (j=0; j<m; j++){
        (*out_schedule.LR)[i][j]=PR[i][j];
      }  
    }
    
  }else if(m_env == J){ //J2 algorithm (johnson rule) 
  
    Lisa_Matrix<bool> S(n,m); 
    Lisa_Matrix<double> P(n,m); 
    Lisa_Matrix<int> M(n,m), J(n,m), PR(n,m); 
    P=(*my_values.PT); 
    S=(*my_values.SIJ);
    my_values.MO->write_rank(&M);
    Lisa_Vector<int> I1(n), I2(n), I12(n), I21(n); 
 
    int a=0; int b=0; int c=0; int d=0; 
    int i=0; int j=0;
	 
    //check validity of processing times matrix
	for (i=0;i<n;i++){
      if (P[i][0]==0){
        if (S[i][0]==true){
          cout << "WARNING: SIJ is inconsistent with PT !" << endl; 
		  cout << "WARNING: results invalid!" << endl;
		  break;
        } 
      } 
      if (P[i][1]==0){
        if (S[i][1]==true){
          cout << "WARNING: SIJ is inconsistent with PT ! " << endl;
          cout << "WARNING: results invalid!" << endl; 
          break;
        }
      }
    } 

	for(i=0;i<n;i++){
      if (S[i][0]==false){
        if (S[i][1]==false){
          cout << "WARNING: SIJ is 0 for both machines at Job " << i+1 << endl; 
		  break;
        }
      }
    } 

  //subdivision in sets 
  for (i=0; i<n; i++){ 
	if (M[i][0]<M[i][1]){ 
	    if (M[i][0]==0){
            I2[b]=i;
            b++;
        }else{
          I12[c]=i;
          c++;
       }
    }else{
      if(M[i][1]==1){
        I21[d]=i;
        d++;
      }else{
        I1[a]=i;
        a++;
      } 
    }
  } 

 
  //Johnson step
  Lisa_Matrix<double> P12(c,m), P21(d,m);
  Lisa_Matrix<int> PR12(c,m), PR21(d,m); 
  int x=0; 
 
  for (i=0; i<c; i++){
    x=I12[i];
    P12[i][0]=P[x][0];
    P12[i][1]=P[x][1];
  }  
  
  for (i=0; i<d; i++){
    x=I21[i];
    P21[i][0]=P[x][1];
    P21[i][1]=P[x][0];
  } 
  
  
  //**************************************************************************
  //JOHNSON RULE : F2-algorithm for P12 : n=c and P21 : n=d
  //result:	PR12(c,m) and PR21(d,m)  
  
  int e=1;
  int f=c;
  double Max=0;
  
  while (e<f+1){
    int k=0;
    int l=0; 
    for (i=0; i<c; i++){
      for (int j=0; j<m; j++){
        if (P12[i][j]<P12[k][l]){
          k=i;
          l=j;
        }else{
          if(Max<P12[i][j]){
            Max=P12[i][j]+1;
          } 
        } //Minimum on P12 , stored at k and l; min=P12[k][l] 
      }
    }
    
    if(l==0){					//insert in lists
      PR12[k][0]=e;
      PR12[k][1]=e+1;
      e++;
    }else{
      PR12[k][1]=f+1;
      PR12[k][0]=f;
      f--;
    } 
    
    P12[k][0]=Max;
    P12[k][1]=Max;//remove job k
    
    // PR12 ready
  }
  
  
  e=1;
  f=d;
  Max=0;
  
  while (e<f+1){ 
    int k=0;
    int l=0;
    for (i=0; i<d; i++){
      for (int j=0; j<m; j++){
        if (P21[i][j]<P21[k][l]){
          k=i;
          l=j;
        }else{
          if(Max<P21[i][j]){
            Max=P21[i][j]+1;
          } 
          
        }
      } //Minimum auf P21 bestimmt, abgespeichert bei k und l 
    } 
    
    if(l==0){//insert in lists
      PR21[k][1]=e;
      PR21[k][0]=e+1;
      e++;
    }else{
      PR21[k][0]=f+1;
      PR21[k][1]=f;
      f--;
    }
    
    P21[k][0]=Max;
    P21[k][1]=Max;			//remove job k
  }// PR21 ready
  
  
  //**************************************************************************
  
  // sort results for machine 0 
  int cneu=c;
  for(i=0; i<c; i++){
    x=I12[i];
    J[x][0]=PR12[i][0];
  }//PR12 
  
  for(i=0; i<a; i++){
    x=I1[i];
    J[x][0]=cneu+1; cneu++;
  }//I1 
  
  for(i=0; i<d; i++){
    x=I21[i];
    J[x][0]=PR21[i][0]+cneu-1;
  }//PR21
  
  //sort results for machine 1
  int dneu=d;
  for (i=0; i<d; i++){
    x=I21[i];
    J[x][1]=PR21[i][1];
  }//PR21 
  
  for(i=0; i<b; i++){
    x=I2[i];
    J[x][1]=dneu+1;
    dneu++;
  }//I2 
  
  for(i=0; i<c; i++){
    x=I12[i];
    J[x][1]=PR12[i][1]+dneu-1;
  }//PR12 
  
  
  
  //calculate LR from M and J  
  Lisa_Matrix<int> MJ(n,m);  
  Lisa_Vector<int> z(n), s(m); 
  
  for (i=0;i<n;i++){
    for (j=0;j<m;j++){
      MJ[i][j]=M[i][j]+J[i][j];
    } 
  } 
  
  c=0; 
  int k=1; 
  while (c<n*m){
    for (i=0;i<n;i++){
      z[i]=0;
    } 
    for (j=0;j<m;j++)	{
      s[j]=0;
    } 
    
    f=0; 
    for (i=0;i<n;i++){
      for (j=0;j<m;j++){
        if (MJ[i][j]==2){
          f=1; 
          c++; 
          z[i]=1; 
          s[j]=1; 
          PR[i][j]=k; 
        } 
      } 
    } 
    k++; 
    if (f==0){
      c=n*m;
    }else{
      for (i=0;i<n;i++){
        for (j=0;j<m;j++) {
          if (MJ[i][j]!=0){
            MJ[i][j]=MJ[i][j]-z[i]-s[j];
          } 
        } 
      }
    } 
  }	 
  
  for (i=0;i<n;i++){
    for (j=0; j<m; j++){
      (*out_schedule.LR)[i][j]=PR[i][j];
    } 
  }	  
  
  }else{ // m_env == J ... F2 algorithm(johnson rule)
    
    Lisa_Matrix<double> P(n,m); 
    Lisa_Matrix<int> PR(n,m);
    P=(*my_values.PT);  
    
    int a=1; int b=n; 
    int i; int j;
    double M=0; 
    
    while (a<b+1){ 
      int k=0;
      int l=0;
      
      for (i=0; i<n; i++){
        for (j=0; j<m; j++){
          if (P[i][j]<P[k][l]) {
            k=i; l=j;
          }else{
            if (M<P[i][j]){
              M=P[i][j]+1;
            } 
          } 
        }	  //computed minimum of P, stored in  k, l 
      }  //where min is P[k][l] 
      
      if (l==0){//insert in list 
        PR[k][0]=a;
        PR[k][1]=a+1;
        a++;
      }else{
        PR[k][1]=b+1;
        PR[k][0]=b;
        b--;
      } 
      
      
      P[k][0]=M;
      P[k][1]=M;				//remove job k
    }    	
    
    
    for (i=0;i<n;i++){
      for (int j=0; j<m; j++){
        (*out_schedule.LR)[i][j]=PR[i][j];
      }
    }	 
  }//end F2 
  
  //create xml output handler
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  //pipe objects to this
  xmlOutput << lpr << my_values << sp << out_schedule;
  //write content to a file
  xmlOutput.write(argv[2]);
  
}

//**************************************************************************

