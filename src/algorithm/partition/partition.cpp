/*
 * ************** partition.cpp *******************************
 * 
 * partition algorithm for LiSA
 *
 * Owner: unknown
 *
 * 30.04.2001
*/

#include<iostream>
#include"../../main/global.hpp"
#include"../../basics/matrix.hpp"
#include"../../lisa/ctrlpara.hpp"
#include"../../scheduling/schedule.hpp"
#include"../../lisa/ptype.hpp"
#include"../../lisa/lvalues.hpp"
#include<unistd.h>
#include<fstream>
#include"../../misc/except.hpp"

using namespace std;

int main(int argc, char *argv[]) 
{

    // comment out following line to send error messages to console
    //G_ExceptionList.set_output_to_cout();   

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

    //calculating the sum of processing times
    int sum_p = 0;
    int max_n = my_werte->get_n();
    for(int i=0;i<max_n;i++)
      sum_p += (int) (*my_werte->PT)[i][0];

    //optimal partition would need half of that time
    int sum_half = (int) sum_p/2;

    //generate table
    Lisa_Matrix<bool> * table= new Lisa_Matrix<bool>(max_n,sum_half+1);

    //fill table with false
    table->fill(FALSE);
    //initiate with 0 and p1 true
    (*table)[0][0] = TRUE;
    (*table)[0][((*my_werte->PT)[0][0])] = TRUE;

    //fill iterating
    for(int i=1;i<max_n;i++)
      for(int j=0;j<=sum_half;j++){
	//a partition of this size was already possible
	if((*table)[i-1][j])
	  (*table)[i][j] = TRUE;
	//a partition of size-pj was already possible
	else if(((*my_werte->PT)[i][0]<=j) && ((*table)[i-1][j-((*my_werte->PT)[i][0])]))
	  (*table)[i][j] = TRUE;
      }

    //find last T in Table
    int part1 = sum_half;
    while(!(*table)[max_n-1][part1])
      part1--;

    //initialize solution vector
    Lisa_Vector<bool> * partition=new Lisa_Vector<bool>(max_n);
    partition->fill(false);

    //find partition
    int i=max_n-1;
    int j=part1;

    //trace first partition
    while(j>0){
      while(i>0 && (*table)[i-1][j]) i--;
      (*partition)[i]=true;
      j = j - (int)(*my_werte->PT)[i][0];
    }

    //fill LR
    int pos1=1;
    int pos2=1;
    for(i=0;i<max_n;i++)
      if((*partition)[i]){
	(*out_schedule->LR)[i][0]=pos1++;
      }else{
	(*out_schedule->LR)[i][1]=pos2++;
      }

    //try to trick the system
    
    for(i=0;i<max_n;i++)
      if((*partition)[i]){
	(*my_werte->SIJ)[i][1]=false;
	(*my_werte->PT)[i][1]=0;
      }else{
	(*my_werte->SIJ)[i][0]=false;
	(*my_werte->PT)[i][1]=(*my_werte->PT)[i][0];
	(*my_werte->PT)[i][0]=0;
      }

    // that way parameters are displayed in LiSA main window/console:
    // cout << "WARNING: the problemtype is:" << lpr->output_problem()<< endl;
    cout << "WARNING: view solution in ~/.lisa/proc/algo_out.lsa !" << endl;
    //cout << "WARNING: sum= " << sum_p <<endl;
    //cout << "WARNING: S/2=" << sum_half << endl;
    //cout << "WARNING: N=" << max_n << endl;
    //cout << "WARNING: partition1=" << part1 << endl;
    //cout << "WARNING: partition2=" << sum_p-part1 << endl;
    //cout << "Table: " << *table;
    //cout << "Partition: " << *partition;
    //cout << "PT: " << *my_werte->PT;
    //cout << "SIJ: " << *my_werte->SIJ;
    //cout << "schedule: " << *out_schedule;
    //cout << "valid: " << out_schedule->valid_LR(my_werte->SIJ) << endl;
    //cout << "WARNING: There are  " << sp->get_int("NB_STEPS") << " steps to do " << endl;
    //cout << "WARNING: Iss" << sp->get_string("NAME")<< endl;


    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    
    // write results to output file:
    o_strm << *lpr;
    o_strm << *my_werte;
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_werte;
    delete lpr;
}


