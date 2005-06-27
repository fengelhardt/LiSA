
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

#include "../../scheduling/matching.hpp"
#include "../../main/global.hpp"
#include "../../basics/list.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

#include "../../xml/LisaXmlFile.hpp"

using namespace std;

//**************************************************************************

/// a simple class for calculation heads
/** this class is used to calculate heads for the matching algorithm
    @author  Marc Moerig
    @version 2.3final
*/
class My_Heads {
private:
  int m,n;
  Lisa_Vector<TIMETYP>* jobs;
  Lisa_Vector<TIMETYP>* mach;
public:
  
  /// create object with given size
  My_Heads(const int n_in,const int m_in){
    m = m_in;
    n = n_in;
    jobs = new Lisa_Vector<TIMETYP>(n);
    mach = new Lisa_Vector<TIMETYP>(m);
    jobs->fill(0);
    mach->fill(0);
  }
  
  /// delete object
  ~My_Heads(){
    delete jobs;
    delete mach;
  }

  /// get maximum time from all heads
  TIMETYP get_max() const {
    TIMETYP max; 
    if (n<m){  
      max = (*jobs)[0];
      for (int i=1;i<n;i++)
        if (max<(*jobs)[i]) 
          max = (*jobs)[i];
    }else{
      max = (*mach)[0];
      for (int j=1;j<m;j++)
        if (max<(*mach)[j])
          max = (*mach)[j];
    }
    return max;
  }
  
  /// add an operation at i,j
  void add_operation(const int i,const int j,const TIMETYP value){
    (*mach)[j] = (*jobs)[i] = get_release(i,j)+value;;
  }
  
  /// get release time for operation i,j
  TIMETYP get_release(const int i,const int j) const {
    return (*jobs)[i] < (*mach)[j] ? (*mach)[j] : (*jobs)[i];
  }
  
};

//**************************************************************************

/* Matching Heuristics
 This algorithm is a simple heuristic for the O| |Cmax Problem. It also 
 demonstrates how to use LiSA's matching objects. */
int main(int argc, char *argv[]){ 

   // write errors, so lisa can catch them up
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
 
   // read problem description, algorithm control parameters
   // and problem values
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


   /// problem type applicable ?
  bool accept=true;
  if(lpr.output_beta() != "" && lpr.output_beta() != "p_ij=1" && lpr.output_beta() != "p_ij=p") accept = false;
  if(lpr.get_property(OBJECTIVE) != CMAX && lpr.get_property(OBJECTIVE) != SUM_CI) accept = false; 
  if(lpr.get_property(M_ENV)!=O) accept = false;
  if(!accept){
    G_ExceptionList.lthrow("Matching heuristics cannot handle "+lpr.output_problem()
                           +" .",Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(1);
  }

   //parse control parameters
   bool min = 0;
   bool bottle = 0;
   bool heads = 0;
   if (sp.defined("MINMAX")==Lisa_ControlParameters::STRING){ 
     if (sp.get_string("MINMAX")=="MIN") min = 1;
     else if (sp.get_string("MINMAX")=="MAX") min = 0;
     else if (sp.get_string("MINMAX")=="HEADS"){
       min = 1;
       heads = 1;
     }else cout << "WARNING: MINMAX value out of Range, using MAX" << endl;
   }else cout << "WARNING: Could not read MINMAX parameter, using MAX." << endl;
   
   if (sp.defined("TYPEOF")==Lisa_ControlParameters::STRING){ 
     if (sp.get_string("TYPEOF")=="BOTTLENECK") bottle = 1;
     else if (sp.get_string("TYPEOF")=="WEIGHTED") bottle = 0;
     else cout << "WARNING: TYPEOF value out of Range, using WEIGHTED" << endl;
   }else cout << "WARNING: Could not read TYPEOF parameter, using WEIGHTED." << endl;

   const int m = my_values.get_m();
   const int n = my_values.get_n();
   const int mx = (m<n?n:m);

   Lisa_Schedule out_schedule(n,m);
   out_schedule.make_LR();
   
   My_Heads my_heads(n,m);
   Lisa_Matrix<TIMETYP> op_rel(n,m);

   const Lisa_Vector<int> *I_matched;
   
   cout << "OBJECTIVE= " << 100*mx << endl;
        
   Lisa_Matching* matching;
   
   if (bottle) matching = new Lisa_BottleneckMatching(my_values.PT);  
   else matching = new Lisa_WeightedMatching(my_values.PT);
   
   if (min) matching->invert();
   
   for (int a=1;a<=mx;a++){
     
     I_matched = matching->get_matching_I();
     
     for (int i=0;i<mx;i++){
       const int j = (*I_matched)[i];
       matching->remove(i,j);
       if (j<m&&i<n&&(*my_values.SIJ)[i][j]){
         my_heads.add_operation(i,j,(*my_values.PT)[i][j]);
         (*out_schedule.LR)[i][j]= a;
         (*my_values.SIJ)[i][j] = 0;
       } // if
     }// for	
     
     if(heads){
       for(int i=0;i<n;i++)
         for(int j=0;j<m;j++){
           op_rel[i][j] = (*my_values.PT)[i][j] + my_heads.get_release(i,j);
         }
       matching->set_all_edges(&op_rel);
     }// if
     
     cout << "OBJECTIVE= " << my_heads.get_max() << endl;
     
   } // for
   
   //create xml output handler
   LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
   //pipe objects to this
   xmlOutput << lpr << my_values << sp << out_schedule;
   //write content to a file
   xmlOutput.write(argv[2]);
  
   delete matching;
 }

//**************************************************************************

