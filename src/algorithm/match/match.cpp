
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

using namespace std;

//**************************************************************************

/// a simple class for calculation heads
/** this class is used to calculate heads for the matching algorithm
    @author  Marc Moerig
    @version 2.3rc1
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
   
   if(argc != 3){
     cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
     exit(1);
   }
     
   cout << "PID= " << getpid() << endl; 
   
   ifstream i_strm(argv[1]);
   if (!i_strm){
     cout << "ERROR: cannot open file '" << argv[1] << "' for reading." << endl;
     exit(1);
   }
   
   // read problem description, algorithm control parameters
   // and problem values
   Lisa_ProblemType * lpr = new Lisa_ProblemType;
   i_strm >> (*lpr);
 
   Lisa_ControlParameters * sp = new Lisa_ControlParameters;
   i_strm >> (*sp);
 
   Lisa_Values * my_werte=new Lisa_Values;
   i_strm >> (*my_werte);
  
   i_strm.close();
 
   if (!G_ExceptionList.empty()){
     cout << "ERROR: cannot read problem data, aborting program." << endl;
     exit(1);
   }  
   
   /// problem type applicable ?
   if (lpr->get_property(M_ENV)!=O){
        cout << "ERROR: cannot read problem type, aborting program." << endl;
        exit(1);
   }
   string cannot_handle="";
   if (lpr->get_property(PMTN)) cannot_handle="preemption";
   if (lpr->get_property(PRECEDENCE)!=EMPTY) 
        cannot_handle="precedence constraints"; 
   if (lpr->get_property(BATCH))  cannot_handle="batching"; 
   if (lpr->get_property(NO_WAIT))  cannot_handle="no-wait constraints";
   if (cannot_handle!=""){
     cout << "ERROR: MATCH cannot handle " << cannot_handle << 
             ". Aborting program."<< endl;
     exit(1);
   }  
   delete lpr;   

   //parse control parameters
   bool min = 0;
   bool bottle = 0;
   bool heads = 0;
   if (sp->defined("MINMAX")==STRING){ 
     if (sp->get_string("MINMAX")=="MIN") min = 1;
     else if (sp->get_string("MINMAX")=="MAX") min = 0;
     else if (sp->get_string("MINMAX")=="HEADS"){
       min = 1;
       heads = 1;
     }else cout << "WARNING: MINMAX value out of Range, using MAX" << endl;
   }else cout << "WARNING: Could not read MINMAX parameter, using MAX." << endl;
   
   if (sp->defined("TYPEOF")==STRING){ 
     if (sp->get_string("TYPEOF")=="BOTTLENECK") bottle = 1;
     else if (sp->get_string("TYPEOF")=="WEIGHTED") bottle = 0;
     else cout << "WARNING: TYPEOF value out of Range, using WEIGHTED" << endl;
   }else cout << "WARNING: Could not read TYPEOF parameter, using WEIGHTED." << endl;
   
   delete sp;

   const int m = my_werte->get_m();
   const int n = my_werte->get_n();
   const int mx = (m<n?n:m);

   Lisa_Schedule * out_schedule = new Lisa_Schedule(n,m);
   out_schedule->make_LR();
   
   My_Heads* my_heads = new My_Heads(n,m);
   Lisa_Matrix<TIMETYP>* op_rel = new Lisa_Matrix<TIMETYP>(n,m);

   const Lisa_Vector<int> *I_matched;
   
   cout << "OBJECTIVE= " << 100*mx << endl;
        
   Lisa_Matching* matching;
   
   if (bottle) matching = new Lisa_BottleneckMatching(my_werte->PT);  
   else matching = new Lisa_WeightedMatching(my_werte->PT);
   
   if (min) matching->invert();
   
   for (int a=1;a<=mx;a++){
     
     I_matched = matching->get_matching_I();
     
     for (int i=0;i<mx;i++){
       const int j = (*I_matched)[i];
       matching->remove(i,j);
       if (j<m&&i<n&&(*my_werte->SIJ)[i][j]){
         my_heads->add_operation(i,j,(*my_werte->PT)[i][j]);
         (*out_schedule->LR)[i][j]= a;
         (*my_werte->SIJ)[i][j] = 0;
       } // if
     }// for	
     
     if(heads){
       for(int i=0;i<n;i++)
       for(int j=0;j<m;j++){
         (*op_rel)[i][j] = (*my_werte->PT)[i][j] + my_heads->get_release(i,j);
       }
       matching->set_all_edges(op_rel);
     }// if
     
     cout << "OBJECTIVE= " << my_heads->get_max() << endl;
     
   } // for
   
   ofstream o_strm(argv[2]);
   if(!o_strm){
     cout << "Could not open '"<<argv[2]<<"' for writing." << endl;
     exit(1);
   }
   o_strm << *out_schedule;
   o_strm.close();
   
   delete matching;
   delete op_rel;
   delete out_schedule;
   delete my_werte;
   delete my_heads;
 }

//**************************************************************************

