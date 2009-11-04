/**
 * @author  Manuela Vogel
 * @version 3.0pre1
 */

#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../scheduling/js_sched.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../basics/order.hpp"

using namespace std;

//**************************************************************************

const int NORMAL=0;
const int EXTENDED=1;
const int INCREASE=1;
const int DECREASE=0;
const int FAST=1;
const int SLOW=0;
int objective_function;

//**************************************************************************

int main(int argc, char *argv[]) 
{
  
  G_ExceptionList.set_output_to_cout();   
  // open files and assure existence:
  if (argc != 3){
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
  
  cout << "PID= " << getpid() << endl; 
  
  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  if (!i_strm){
    cout << "ERROR: cannot open file " << argv[1] << " for reading." << endl;
    exit(1);
  }
  if(!o_strm){
    cout << "ERROR: cannot open file " << argv[2] << " for writing." << endl;
  }
  i_strm.close();
  o_strm.close();
  
  // read problem description and decide whether program is applicable:
  Lisa_ProblemType lpr;
  Lisa_ControlParameters sp;
  Lisa_Values my_values;
  
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
  
  
  if (lpr.get_property(M_ENV)!=F){
    cout << "ERROR: cannot handle problem type, aborting program." << endl;
    exit(1);
  }
  
  if (lpr.get_property(OBJECTIVE)==CMAX){
    objective_function=FAST;
  }else{
    objective_function=SLOW;
  }
  
  string cannot_handle="";
  if (lpr.get_property(PMTN)) cannot_handle="preemption";
  if (lpr.get_property(PRECEDENCE)!=EMPTY) cannot_handle="precedence constraints"; 
  if (lpr.get_property(BATCH))  cannot_handle="batching"; 
  if (lpr.get_property(NO_WAIT))  cannot_handle="no-wait constraints";
  if (cannot_handle!=""){
    cout << "ERROR: bb cannot handle " << cannot_handle 
    << ". Aborting program."<< endl;
    exit(1);
  }
  
  int Job_insertion_order=DECREASE;
  if (sp.defined("JOB_INSERTION_ORDER")==Lisa_ControlParameters::STRING){
    if (sp.get_string("JOB_INSERTION_ORDER")=="INCREASE")
      Job_insertion_order=INCREASE;
  }
  
  int type_of_lower_bound=NORMAL;
  if (objective_function==FAST){
    if (sp.defined("TYPE_OF_LOWER_BOUND")==Lisa_ControlParameters::STRING){
      if (sp.get_string("TYPE_OF_LOWER_BOUND")=="EXTENDED")
        type_of_lower_bound=EXTENDED;
    }
  }
  
  
  
  Lisa_JsProblem * myjsproblem;
  myjsproblem= new Lisa_JsProblem(&my_values);
  
  if (objective_function==FAST)
  {
    Lisa_JsSchedule *myjsschedule;
    myjsschedule= new Lisa_JsSchedule(myjsproblem);
    myjsschedule->ComputeHeadsTails(true,true);
    if (type_of_lower_bound==EXTENDED)
      myjsschedule->InitHeadsTails();
    
    
    // solve the problem and store results in a schedule object
    // Insert your solution algorithm here:
    
    
    int i,j,bestpos=0,inspos,k;
    int n=my_values.get_n(),m=my_values.get_m();
    TIMETYP row_i,min,z;
    Lisa_Vector<TIMETYP> my_head(m+1);
    Lisa_Vector<TIMETYP> my_tail(m);
    
    
    Lisa_Order myOrder(n);
    // calculate sums of row i (i=0,...,n-1) of the processing time matrix  
    for (i=0; i <n; i++) 
    {
      row_i=0;
      for (j=0; j <m; j++) 
      {
        row_i+=(*myjsproblem->time)[i+1][j+1];
      }
      if (Job_insertion_order==INCREASE)
      {
        myOrder.read_one_key(i,row_i);
      }
      else
      {
        myOrder.read_one_key(i,-row_i);
      }
    }
    
    //sort row_i; begin with the smallest sum
    myOrder.sort();
    // The Order, in which the jobs are inserted, is given by
    //INCREASE: job with smallest sum is inserted first and so on
    //DECREASE: job with largest sum  is inserted first and so on
    for (j=1; j <=m;j++)
    {
      myjsschedule->insert(myOrder[0]+1,j,SOURCE) ;
    }
    cout << "OBJECTIVE= " << 100*(n+m-1) << endl;
    myjsschedule->SetValue(CMAX);
    
    // i is the number of jobs, which are already inserted
    // my_head[1],...,my_head[m] save the heads of job myOrder[i]+1 on machine
    // 1,...,m if this job is inserted after job inspos in myjsschedule
    // in the same way: my_tail[0],...,my_tail[m-1]
    // (my_head[0] is needed for SOURCE)
    // min is the best known value for objective so far and
    // bestpos is the JO predecessor of job myOrder[i]+1 in myjsschedule,
    // which guarantee the minimal objective min .
    for (i=1;i<n;i++)
    {
      min = MAXTIME;
      myjsschedule->SetValue(CMAX);
      inspos=SOURCE; 
      do
      {
        //calculating the heads of job myOrder[i]+1 on machine 1,...,m , if
        //job myOrder[i]+1 is inserted after job inspos in myjsschedule
        my_head[0]=0;
        if (inspos==SOURCE)
        {
          for (j=1; j<=m; j++)
          {
            my_head[j]= my_head[j-1]+(*myjsproblem->time)[myOrder[i]+1][j-1];
          }
        }
        else
        {
          for (j=1;j<=m;j++) 
          {
            if (myjsschedule-> GetHead(inspos,j)+(*myjsproblem->time)[inspos][j] > my_head[j-1]+(*myjsproblem->time)[myOrder[i]+1][j-1] )
            {
              my_head[j] =myjsschedule->GetHead(inspos,j)+(*myjsproblem->time)[inspos][j];
            }
            else
            {
              my_head[j]=my_head[j-1]+(*myjsproblem->time)[myOrder[i]+1][j-1]; 
            } 
          }
        }
        k=myjsschedule->GetJOsucc(inspos,m);
        //calculating the tails of job myOrder[i]+1 on machine 1,...,m , if
        //job myOrder[i]+1 is inserted after job inspos in myjsschedule 
        if (k!=SINK)
        {
          my_tail[m-1]= myjsschedule->GetTail(k,m)+(*myjsproblem->time)[k][m];
          for (j=m-1;j>=1;j--)
          {
            if (my_tail[j]+(*myjsproblem->time)[myOrder[i]+1][j+1] > myjsschedule->GetTail(k,j)+(*myjsproblem->time)[k][j])
            {
              my_tail[j-1]=my_tail[j]+(*myjsproblem->time)[myOrder[i]+1][j+1];
            }
            else
            {
              my_tail[j-1]=myjsschedule->GetTail(k,j)+(*myjsproblem->time)[k][j];
            }
          }
        }
        else
        {
          my_tail[m-1]= 0;
          for(j=m-1;j>=1;j--)
          {
            my_tail[j-1]= my_tail[j]+(*myjsproblem->time)[myOrder[i]+1][j+1];
          }
        }
        
        //myjsschedule->InitHeadsTails();
        z= myjsschedule->GetValue();
        // calculate new value of the objective function, if job 
        //myOrder[i]+1 is inserted in myjsschedule
        //use G=H+P+T
        //G, H, P, T : matrix with weights of longest way through the operation, matrix of heads, processing time, tails  of the operation
        //For job myOrder[i]+1 on the machines 1,..,m the elements in 
        //H, P, T are added and each sum compared with z
        for (j=1;j<=m;j++)
        {
          if (z < my_head[j]+(*myjsproblem->time)[myOrder[i]+1][j]+my_tail[j-1] )
          {
            z=my_head[j]+(*myjsproblem->time)[myOrder[i]+1][j]+my_tail[j-1] ;
          }
        }
        // The minimal value z of the objective is saved as min.
        // In this case bestpos is the JO predecessor of Job myOrder[i]+1. 
        if (z< min) 
        {
          min= z;
          bestpos = inspos;
        }
        inspos= myjsschedule->GetJOsucc(inspos,1);
      }
      while (inspos != SINK);
      //Job myOrder[i]+1 is inserted after Job bestpos; value of objective is 
      //min (minimal).
      for (j=1;j<=m;j++)
      {
        myjsschedule -> insert(myOrder[i]+1,j,bestpos);
      }
      cout << "OBJECTIVE= " << min  << endl;
    }      
    
    Lisa_Schedule out_schedule(my_values.get_n(),my_values.get_m());
    out_schedule.make_LR();
    
    
    myjsschedule->write_LR(out_schedule.LR);
    
    //write results to output file:
    //create xml output handler
    LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
    //pipe objects to this
    xmlOutput << lpr << my_values << sp << out_schedule;
    //write content to a file
    xmlOutput.write(argv[2]);
    
    delete myjsproblem;
    delete myjsschedule;
    
  }else{
    
    Lisa_JsSchedule *schedule_1;
    schedule_1= new Lisa_JsSchedule(myjsproblem);
    
    Lisa_JsSchedule *schedule_3;
    schedule_3= new Lisa_JsSchedule(myjsproblem);
    
    // solve the problem and store results in a schedule object
    // Insert your solution algorithm here:
    
    
    int i,j,bestpos=0,inspos;
    int n=my_values.get_n(),m=my_values.get_m();
    TIMETYP row_i,min;
    
    Lisa_Order myOrder(n);
    
    // calculate sums of row i (i=0,...,n-1) of the processing time matrix  
    for (i=0; i <n; i++) 
    {
      row_i=0;
      for (j=0; j <m; j++) 
      {
        row_i+=(*myjsproblem->time)[i+1][j+1];
      }
      myOrder.read_one_key(i,row_i);
      if (Job_insertion_order==INCREASE)
      {
        myOrder.read_one_key(i,row_i);
      }
      else
      {
        myOrder.read_one_key(i,-row_i);
      }
    }
    
    //sort sums of rows ; begin with the smallest sum
    myOrder.sort();
    
    // The Order, in which the jobs are inserted, is given by
    //INCREASE: job with smallest sum is inserted first and so on
    //DECREASE: job with largest sum  is inserted first and so on
    //schedule_3 contains the order of the i already inserted jobs
    for (j=1; j <=m;j++)
    {
      schedule_3->insert(myOrder[0]+1,j,SOURCE) ;
    }
    cout << "OBJECTIVE= " << 100*(n+m-1) << endl;
    // The (i+1)-th  job (with number myOrder[i]+1) is inserted:
    for (i=1;i<n;i++)
    {
      min = MAXTIME; 
      inspos=SOURCE;
      //schedule_1 is for testing which insertion position has best (minimal)
      //result for objective function
      //The insertion position inspos runs through all possible positions,
      //in which the (i+1)-th job can be inserted in schedule_1.
      //The smallest objective, which is got by inserting the (i+1)-th job
      //is called min .
      // bestpos is the JO-predecessor of the (i+1)-th job, if the insertion 
      //position with smallest objective min is choosen .
      
      
      do
      {
        (* schedule_1) = (* schedule_3);
        for (j=1;j<=m;j++) 
        {
          schedule_1 -> insert(myOrder[i]+1,j,inspos);
        }
        schedule_1-> SetValue(CMAX);
        //compare the values of the objective            
        if (schedule_1->GetValue()< min) 
        { 
          min=schedule_1->GetValue() ;
          bestpos = inspos; 
        }
        inspos=schedule_3->GetJOsucc(inspos,1); 
      }
      while (inspos != SINK);
      
      // The (i+1)-th job is inserted in schedule_3 after job bestpos .
      
      for (j=1;j<=m;j++)
      {
        schedule_3 -> insert(myOrder[i]+1,j,bestpos); 
      }
      cout << "OBJECTIVE= " << min  << endl;
    } //endfor i                                               
    
    
    Lisa_Schedule out_schedule(my_values.get_n(),my_values.get_m());
    out_schedule.make_LR();
    
    schedule_1->write_LR(out_schedule.LR);
    schedule_3->write_LR(out_schedule.LR);
    
    //write results to output file:
    //create xml output handler
    LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
    //pipe objects to this
    xmlOutput << lpr << my_values << sp << out_schedule;
    //write content to a file
    xmlOutput.write(argv[2]);
    
    delete myjsproblem;
    delete schedule_1;
    delete schedule_3;
  }
}

//**************************************************************************

