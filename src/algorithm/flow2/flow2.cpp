/**
 * @author  Thomas Klemm
 * @version 2.3final
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
 
  // LiSA  schedule object for storing results
  Lisa_Schedule out_schedule(my_values.get_n(),my_values.get_m());
  out_schedule.make_LR();
  
  //F2 algorithm(johnson rule)
  
  const int n=my_values.get_n(); 
  const int m=my_values.get_m(); 
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
  //end F2 
  
  //create xml output handler
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  //pipe objects to this
  xmlOutput << lpr << my_values << sp << out_schedule;
  //write content to a file
  xmlOutput.write(argv[2]);
  
}

//**************************************************************************

