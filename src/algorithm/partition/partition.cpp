/**
 * @author  unknown
 * @version 3.0pre1
 */
 
#include <unistd.h>
#include <fstream>
#include <iostream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../basics/matrix.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]){
  
  //write errors so lisa can pick them up
  G_ExceptionList.set_output_to_cout(); 
  
  if(argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
  
  cout << "PID= " << getpid() << endl; 
  
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
  
  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;
  Lisa_Values Values;    
  
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
  
  // LiSA  schedule object for storing results
  Lisa_Schedule out_schedule(Values.get_n(),Values.get_m());
  out_schedule.make_LR();
  
  //calculating the sum of processing times
  int sum_p = 0;
  int max_n = Values.get_n();
  for(int i=0;i<max_n;i++)
    sum_p += (int) (*Values.PT)[i][0];
  
  //optimal partition would need half of that time
  int sum_half = (int) sum_p/2;
  
  //generate table
  Lisa_Matrix<bool> table(max_n,sum_half+1);
  
  //fill table with false
  table.fill(false);
  //initiate with 0 and p1 true
  (table)[0][0] = true;
  (table)[0][ (int) ((*Values.PT)[0][0])] = true;
  
  //fill iterating
  for(int i=1;i<max_n;i++)
  for(int j=0;j<=sum_half;j++){
    //a partition of this size was already possible
    if((table)[i-1][j])
      (table)[i][j] = true;
    //a partition of size-pj was already possible
    else if(((*Values.PT)[i][0]<=j) && ((table)[i-1][ (int) (j-(*Values.PT)[i][0]) ]))
      (table)[i][j] = true;
  }
  
  //find last T in Table
  int part1 = sum_half;
  while(!(table)[max_n-1][part1])
    part1--;
  
  //initialize solution vector
  Lisa_Vector<bool> partition(max_n);
  partition.fill(false);
  
  //find partition
  int i=max_n-1;
  int j=part1;
  
  //trace first partition
  while(j>0){
    while(i>0 && (table)[i-1][j]) i--;
    (partition)[i]=true;
    j = j - (int)(*Values.PT)[i][0];
  }
  
  //fill LR
  int pos1=1;
  int pos2=1;
  for(i=0;i<max_n;i++)
  if((partition)[i]){
    (*out_schedule.LR)[i][0]=pos1++;
  }else{
    (*out_schedule.LR)[i][1]=pos2++;
  }
  
  //try to trick the system
  
  for(i=0;i<max_n;i++)
  if((partition)[i]){
    (*Values.SIJ)[i][1]=false;
    (*Values.PT)[i][1]=0;
  }else{
    (*Values.SIJ)[i][0]=false;
    (*Values.PT)[i][1]=(*Values.PT)[i][0];
    (*Values.PT)[i][0]=0;
  }
  
  // that way parameters are displayed in LiSA main window/console:
  cout << "WARNING: view solution in ~/.lisa/proc/algo_out.lsa !" << endl;
  
  // write results to output file:
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  //pipe objects to this
  xmlOutput << Problem << Values << Parameter << out_schedule;
  //write content to a file
  xmlOutput.write(argv[2]);
}

//**************************************************************************

