/**
 * @author  Andreas Winkler
 * @version 2.3pre3
 */ 

#include <stdlib.h>

#include <fstream>
#include <iomanip> 
 
#include "../../basics/matrix.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ptype.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../lisa/ctrlpara.hpp"

using namespace std;

//**************************************************************************

const int MAXINT  = 214748000;
const int MAXLONG = 214748000;

// definition of parameters for the file-controlling
// following parameters are possible:
TIMETYP OBJ_VALUE, BEST_OBJECTIVE;

int n;  // number of jobs
int i, j, k;
int count, succ;
int numb_fix_l, numb_fix_d, numb_fix_s, numb_var;
int no_objective;
std::string S;
std::string CONFIG_HOME_St, LISA_HOME_St, FILE_St;
std::string DESCRIPTION, CALL;

//**************************************************************************

/// test some parameters of an external algorithm
int main(int argc, char *argv[])
 {
   BEST_OBJECTIVE   = MAXLONG;
   // print a message that the programm started:
   cout << "\n\nThis is the general LiSA Parameter Test Module Version 10.09.1999" << endl;

   long cnt;
   
   if (argc != 3) 
     {
       cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
       exit(7);
     }

   ifstream strm;      // this file contains the problem, the schedule and the
   strm.open(argv[1]); // startparameter (standard is para_in.lsa)
   ofstream nb_in;     // these two file are necessary to communicate with 
   ifstream nb_out;    // the external algorithm
   
   if (!strm)
     {
       G_ExceptionList.lthrow("could not open file "+string(argv[1]));
       exit(7);
     }

   // read the parameters from file

   Lisa_ControlParameters *ptest_special;
   if ( !( ptest_special = new Lisa_ControlParameters ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }

   // at first read the names of all parameters
   // create two lists of string for fixed and variable parameters
   Lisa_List<string> *Fix_Param_long;
   if ( !(Fix_Param_long = new Lisa_List<string>()) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
      Lisa_List<string> *Fix_Param_double;
   if ( !(Fix_Param_double = new Lisa_List<string>()) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
      Lisa_List<string> *Fix_Param_string;
   if ( !(Fix_Param_string = new Lisa_List<string>()) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_List<string> *Var_Param;
   if ( !(Var_Param = new Lisa_List<string>()) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_List<string> *Var_Param_type;
   if ( !(Var_Param_type = new Lisa_List<string>()) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }

   // read the fixed parameters
   numb_fix_l = 0; numb_fix_d = 0; numb_fix_s = 0;
   strm.seekg(0);
   do 
     {
       S=""; strm >> S;
       if (S=="") 
	 {
	   cerr<<"ERROR: no <FIX_PARAMETERS> found in input file.\n";
	   return !OK;
	 } 
     }  
   while (S!="<FIX_PARAMETERS>");
   strm >> S;
   while (S!="</FIX_PARAMETERS>") 
     {
       if ( S == "long" )
	 {
	   strm >> S;
	   Fix_Param_long->append( S );
	   numb_fix_l += 1;
	 }
       if ( S == "double" )
	 {
	   strm >> S;
	   Fix_Param_double->append( S );
	   numb_fix_d += 1;
	 }
       if ( S == "string" )
	 {
	   strm >> S;
	   Fix_Param_string->append( S );
	   numb_fix_s += 1;
	 }
       strm >> S;
     }  

   // read the variable parameters
   numb_var = 0;
   strm.seekg(0);
   do 
     {
       S=""; strm >> S;
       if (S=="") 
	 {
	   cerr<<"ERROR: no <VAR_PARAMETERS> found in input file.\n";
	   return !OK;
	 } 
     }  
   while (S!="<VAR_PARAMETERS>");
   strm >> S;
   while (S!="</VAR_PARAMETERS>") 
     {
       if ( S == "long" )
	 {
	   strm >> S;
	   Var_Param->append( S );
	   Var_Param_type->append( "long" );
	   numb_var += 1;
	 }
       else if ( S == "double" )
	 {
	   strm >> S;
	   Var_Param->append( S );
	   Var_Param_type->append( "double" );
	   numb_var += 1;
	 }
       else 
	 {
	   G_ExceptionList.lthrow("Cannot handle the type "+S+" in <Var_Parameters>");
	   exit( 7 );
	 }
       strm >> S;
     }  

   //cout << "fix: " << (numb_fix_l+numb_fix_d+numb_fix_s) << "\n";
   if ( numb_fix_l > 0 )
     for ( i=0; i<numb_fix_l; i++ )
       cout << (*Fix_Param_long)[i] << "\n";
   if ( numb_fix_d > 0 )
     for ( i=0; i<numb_fix_d; i++ )
       cout << (*Fix_Param_double)[i] << "\n";
   if ( numb_fix_s > 0 )
     for ( i=0; i<numb_fix_s; i++ )
       cout << (*Fix_Param_string)[i] << "\n";
   //cout << "var: " << numb_var << "\n";
   if ( numb_var > 0 )
     for ( i=0; i<numb_var; i++ )
       cout<<(*Var_Param_type)[i]<<" "<<(*Var_Param)[i]<<"\n";

   // the following vectors contains the input values
   Lisa_Vector<long> *FP_long;
   if ( !(FP_long = new Lisa_Vector<long>(numb_fix_l)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_Vector<double> *FP_double;
   if ( !(FP_double = new Lisa_Vector<double>(numb_fix_d)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_Vector<string> *FP_string;
   if ( !(FP_string = new Lisa_Vector<string>(numb_fix_s)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_Vector<double> *VP_init;
   if ( !(VP_init = new Lisa_Vector<double>(numb_var)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_Vector<double> *VP_max;
   if ( !(VP_max = new Lisa_Vector<double>(numb_var)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_Vector<double> *VP_incr;
   if ( !(VP_incr = new Lisa_Vector<double>(numb_var)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_Vector<string> *VP_FA;
   if ( !(VP_FA = new Lisa_Vector<string>(numb_var)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_Vector<double> *variable;
   if ( !(variable = new Lisa_Vector<double>(numb_var)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }
   Lisa_Vector<double> *VP_best;
   if ( !(VP_best = new Lisa_Vector<double>(numb_var)) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       exit( 7 );
     }


   strm >> (*ptest_special);

   if ( numb_fix_l > 0 )
     for ( i=0; i<numb_fix_l; i++ )
       (*FP_long)[i] = ptest_special->get_int( (*Fix_Param_long)[i].c_str() );
   if ( numb_fix_d > 0 )
     for ( i=0; i<numb_fix_d; i++ )
       (*FP_double)[i] 
	 = ptest_special->get_double( (*Fix_Param_double)[i].c_str() );
   if ( numb_fix_s > 0 )
     for ( i=0; i<numb_fix_s; i++ )
       (*FP_string)[i] 
	 = ptest_special->get_string( (*Fix_Param_string)[i].c_str() );
   if ( numb_var > 0 )
     for ( i=0; i<numb_var; i++ )
       {
	 if ( (*Var_Param_type)[i] == "long" )
	   {
	     (*VP_init)[i] 
	       = ptest_special->get_int( ((*Var_Param)[i]+"_INIT").c_str() );
	     (*VP_max)[i] 
	       = ptest_special->get_int( ((*Var_Param)[i]+"_MAX").c_str() );
	   }
	 else
	   {
	     (*VP_init)[i] 
	       = ptest_special->get_double( ((*Var_Param)[i]+"_INIT").c_str());
	     (*VP_max)[i] 
	       = ptest_special->get_double( ((*Var_Param)[i]+"_MAX").c_str() );
	   }
	 (*VP_incr)[i] 
	   = ptest_special->get_double( ((*Var_Param)[i]+"_INCR").c_str() );
	 (*VP_FA)[i] 
	   = ptest_special->get_string( ((*Var_Param)[i]+"_FA").c_str() );
       }
   
   CONFIG_HOME_St = ptest_special->get_string( "CONFIG_HOME" );
   LISA_HOME_St   = ptest_special->get_string( "LISA_HOME" );
   FILE_St        = ptest_special->get_string( "FILE" );
   DESCRIPTION    = ptest_special->get_string( "DESCRIPTION" );
   CALL           = ptest_special->get_string( "CALL" );

   delete ptest_special;
   // all input values readed

   if ( FILE_St == " " )
     S = CONFIG_HOME_St+"/proc/ptst_out.lsa";
   else
     S = FILE_St;
   ofstream tab_out(S.c_str()); // this file returns the table of the results
   ofstream ostrm(argv[2]);  // this file returns the best schedule

   if (!ostrm)
     {
       G_ExceptionList.lthrow("could not open file "+string(argv[2]));
       exit(1);
     }

   Lisa_ProblemType *prob_type;
   if ( !( prob_type = new Lisa_ProblemType ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       return !OK;
     }
   strm.seekg(0);
   strm >> (*prob_type);
   ostrm << (*prob_type);
   
   Lisa_Values *problem_in;
   if ( !( problem_in = new Lisa_Values() ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       return !OK;
     }	  
   strm >> (*problem_in);
   ostrm << (*problem_in);

   Lisa_Schedule *plan_in;
   if ( !( plan_in = new Lisa_Schedule() ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       return !OK;
     } 
   strm >> (*plan_in);

   // the best of all created schedules
   Lisa_Schedule *best_schedule;
   best_schedule = new Lisa_Schedule();
   strm.seekg(0);
   strm >> (*best_schedule);

   // prepare the output table file
   tab_out << (*prob_type);
   tab_out << (*problem_in);
   tab_out << "\n<FIX_PARAMETERS>\n";
   if ( numb_fix_l > 0 )
     for ( i=0; i<numb_fix_l; i++ )
       tab_out<<"long "<<(*Fix_Param_long)[i]<<" "<<(*FP_long)[i]<<"\n";
   if ( numb_fix_d > 0 )
     for ( i=0; i<numb_fix_d; i++ )
       tab_out<<"double "<<(*Fix_Param_double)[i]<<" "<<(*FP_double)[i]<<"\n";
   if ( numb_fix_s > 0 )
     for ( i=0; i<numb_fix_s; i++ )
       tab_out<<"string "<<(*Fix_Param_string)[i]<<" "<<(*FP_string)[i]<<"\n";
   tab_out << "</FIX_PARAMETERS>\n\n";

   if ( numb_var > 0 )
     {
       tab_out << "<CONTROLPARAMETERS>\n";
       tab_out << "string TAB_NAME TABELLE=\n";

       tab_out << "string COL_NAME " << (*Var_Param)[numb_var-1] << "=\n";
       if ( numb_var > 1 )
	 tab_out << "string ROW_NAME " << (*Var_Param)[numb_var-2] << "=\n";
       tab_out << "long NB_COL ";
       // how many table entrys for the parameter [numb_var-1] ?
       (*variable)[numb_var-1] = (*VP_init)[numb_var-1]; cnt = 0;
       if ( (*VP_FA)[numb_var-1]=="FACTOR" )
	 do 
	   {
	     cnt += 1;
	     if ( (*Var_Param_type)[numb_var-1] == "long" )
	       (*variable)[numb_var-1] 
		 = long( (*variable)[numb_var-1] * (*VP_incr)[numb_var-1] );
	     else
	       (*variable)[numb_var-1] *= (*VP_incr)[numb_var-1];
	   }
	 while ( (*variable)[numb_var-1] <= (*VP_max)[numb_var-1] );
       else
	 do
	   {
	     cnt +=1;
	     if ( (*Var_Param_type)[numb_var-1] == "long" )
	       (*variable)[numb_var-1] 
		 = long( (*variable)[numb_var-1] + (*VP_incr)[numb_var-1] );
	     else
	       (*variable)[numb_var-1] += (*VP_incr)[numb_var-1];
	   }
	 while ( (*variable)[numb_var-1] <= (*VP_max)[numb_var-1] );
       tab_out << cnt << "\n";
       tab_out << "long NB_ROW ";
       if ( numb_var > 1 )
	 {
	   // how many table entrys for the parameter [numb_var-2] ?
	   (*variable)[numb_var-2] = (*VP_init)[numb_var-2]; cnt = 0;
	   if ( (*VP_FA)[numb_var-2]=="FACTOR" )
	     do 
	       {
		 cnt += 1;
		 if ( (*Var_Param_type)[numb_var-2] == "long" )
		   (*variable)[numb_var-2] 
		     = long((*variable)[numb_var-2] * (*VP_incr)[numb_var-2]);
		 else
		   (*variable)[numb_var-2] *= (*VP_incr)[numb_var-2];
	       }
	     while ( (*variable)[numb_var-2] <= (*VP_max)[numb_var-2] );
	   else
	     do
	       {
		 cnt +=1;
		 if ( (*Var_Param_type)[numb_var-2] == "long" )
		   (*variable)[numb_var-2] 
		     = long((*variable)[numb_var-2] + (*VP_incr)[numb_var-2]);
		 else
		   (*variable)[numb_var-2] += (*VP_incr)[numb_var-2];
	       }
	     while ( (*variable)[numb_var-2] <= (*VP_max)[numb_var-2] );
	   tab_out << cnt << "\n";
	 }
       else 
	 tab_out << "1\n";
       tab_out << "</CONTROLPARAMETERS>\n\n";
     }

   if ( numb_var > 0 )
     {
       tab_out << "<DATA>\n";
       tab_out << (*Var_Param)[numb_var-1] << "= {\n";
       (*variable)[numb_var-1] = (*VP_init)[numb_var-1];
       if ( (*VP_FA)[numb_var-1]=="FACTOR" )
	 do 
	   {
	     tab_out << (*variable)[numb_var-1] << " ";
	     if ( (*Var_Param_type)[numb_var-1] == "long" )
	       (*variable)[numb_var-1] 
		 = long( (*variable)[numb_var-1] * (*VP_incr)[numb_var-1] );
	     else
	       (*variable)[numb_var-1] *= (*VP_incr)[numb_var-1];
	   }
	 while ( (*variable)[numb_var-1] <= (*VP_max)[numb_var-1] );
       else
	 do
	   {
	     tab_out << (*variable)[numb_var-1] << " ";
	     if ( (*Var_Param_type)[numb_var-1] == "long" )
	       (*variable)[numb_var-1] 
		 = long( (*variable)[numb_var-1] + (*VP_incr)[numb_var-1] );
	     else
	       (*variable)[numb_var-1] += (*VP_incr)[numb_var-1];
	   }
	 while ( (*variable)[numb_var-1] <= (*VP_max)[numb_var-1] );
       tab_out << "\n}\n\n";
       
       if ( numb_var > 1 )
	 {
	   tab_out << (*Var_Param)[numb_var-2] << "= {\n";
	   (*variable)[numb_var-2] = (*VP_init)[numb_var-2];
	   if ( (*VP_FA)[numb_var-2]=="FACTOR" )
	     do 
	       {
		 tab_out << (*variable)[numb_var-2] << " ";
		 if ( (*Var_Param_type)[numb_var-2] == "long" )
		   (*variable)[numb_var-2] 
		     = long((*variable)[numb_var-2] * (*VP_incr)[numb_var-2]);
		 else
		   (*variable)[numb_var-2] *= (*VP_incr)[numb_var-2];
	       }
	     while ( (*variable)[numb_var-2] <= (*VP_max)[numb_var-2] );
	   else
	     do
	       {
		 tab_out << (*variable)[numb_var-2] << " ";
		 if ( (*Var_Param_type)[numb_var-2] == "long" )
		   (*variable)[numb_var-2] 
		     = long((*variable)[numb_var-2] + (*VP_incr)[numb_var-2]);
		 else
		   (*variable)[numb_var-2] += (*VP_incr)[numb_var-2];
	       }
	     while ( (*variable)[numb_var-2] <= (*VP_max)[numb_var-2] );
	   tab_out << "\n}\n\n";
	 }

       // now the main loop to test all parameters
       
       for ( i=0; i<numb_var; i++ )
	 (*variable)[i] = (*VP_init)[i];
     } // if ( numb_var > 0 )


   if ( numb_var > 1 )
     {
       // the big do-while loop 
       do
	 {
	   for ( i=0; i<numb_var-2; i++ )
	     tab_out << (*Var_Param)[i] << " = " << (*variable)[i] << "\n";
	   tab_out << "TABELLE= {\n";
	   
	   // now variate the last two parameters:

	   (*variable)[numb_var-2] = (*VP_init)[numb_var-2];
	   do  // [numb_var-2]
	     {
	       tab_out << " { ";
	       (*variable)[numb_var-1] = (*VP_init)[numb_var-1];
	       do  // [numb_var-1]
		 {
		   for ( i=0; i<numb_var; i++ )
		     cout<<(*Var_Param)[i]<<"="<<(*variable)[i]<<", ";
		   cout<<"\n";
	       
		   nb_in.open((CONFIG_HOME_St+"/proc/in~temp.lsa").c_str());
		   nb_in << (*prob_type);

		   Lisa_ControlParameters *my_special;
		   if ( !( my_special = new Lisa_ControlParameters ) )
		     {
		       G_ExceptionList.lthrow("out of memory",2);
		       return !OK;
		     }
	       
		   // the fixed controlparameters
		   if ( numb_fix_l > 0 )
		     for ( i=0; i<numb_fix_l; i++ )
		       my_special
			 ->add_key((*Fix_Param_long)[i], long((*FP_long)[i]));
		   if ( numb_fix_d > 0 )
		     for ( i=0; i<numb_fix_d; i++ )
		       my_special
			 ->add_key( (*Fix_Param_double)[i], (*FP_double)[i] );
		   if ( numb_fix_s > 0 )
		     for ( i=0; i<numb_fix_s; i++ )
		       my_special
			 ->add_key( (*Fix_Param_string)[i], (*FP_string)[i] );

		   // the variable controlparameters
		   for ( i=0; i<numb_var; i++ )
		     {
		       if ( (*Var_Param_type)[i] == "long" )
			 my_special
			   ->add_key((*Var_Param)[i],long((*variable)[i]));
		       else
			 my_special
			   ->add_key((*Var_Param)[i],double((*variable)[i])); 
		     }

		   nb_in << (*my_special);
		   delete my_special;
		   nb_in << (*problem_in);
		   nb_in << (*plan_in);
	       
		   nb_in.close();

		   // now call the neighborhood main program
	       
		   S=LISA_HOME_St+"/bin/"+CALL+" ";
		   S=S+CONFIG_HOME_St+"/proc/in~temp.lsa ";
		   S=S+CONFIG_HOME_St+"/proc/out~temp.lsa";
		   system(S.c_str());

		   nb_out.open((CONFIG_HOME_St+"/proc/out~temp.lsa").c_str());
	       
		   OBJ_VALUE = MAXLONG;
		   no_objective = 0;
		   do 
		     {
		       S=""; nb_out >>S;
		       if (S=="") 
			 {
			   G_ExceptionList.lthrow
			     ("WARNING: no <OBJECTIVE> found in input file.");
			   no_objective = 1;
			 } 
		     }  
		   while ( (S!="<OBJECTIVE>") && (S!="") );
		   if ( no_objective == 0 )
		     {
		       nb_out >> S;
		       nb_out >> OBJ_VALUE;
		     }
		   tab_out << OBJ_VALUE << " ";

		   if ( OBJ_VALUE < BEST_OBJECTIVE )
		     {
		       BEST_OBJECTIVE = OBJ_VALUE;
		       for ( i=0; i<numb_var; i++ )
			 (*VP_best)[i] = (*variable)[i];
		       nb_out.seekg(0);
		       nb_out >> (*best_schedule);
		     }
		   nb_out.close();
		   
		   if ( (*VP_FA)[numb_var-1] == "FACTOR" )
		     {
		       if ( (*Var_Param_type)[numb_var-1] == "long" )
			 (*variable)[numb_var-1] 
			   = long((*variable)[numb_var-1]
				  *(*VP_incr)[numb_var-1]);
		       else
			 (*variable)[numb_var-1] *= (*VP_incr)[numb_var-1];
		     }
		   else
		     {
		       if ( (*Var_Param_type)[numb_var-1] == "long" )
			 (*variable)[numb_var-1] 
			   = long((*variable)[numb_var-1]
				  +(*VP_incr)[numb_var-1]);
		       else
			 (*variable)[numb_var-1] += (*VP_incr)[numb_var-1];
		     }
		 }
	       while ( (*variable)[numb_var-1] <= (*VP_max)[numb_var-1] );
	       tab_out << "}\n";
	       if ( (*VP_FA)[numb_var-2] == "FACTOR" )
		 {
		   if ( (*Var_Param_type)[numb_var-2] == "long" )
		     (*variable)[numb_var-2] 
		       = long( (*variable)[numb_var-2] 
			       * (*VP_incr)[numb_var-2] );
		   else
		     (*variable)[numb_var-2] *= (*VP_incr)[numb_var-2];
		 }
	       else
		 {
		   if ( (*Var_Param_type)[numb_var-2] == "long" )
		     (*variable)[numb_var-2] 
		       = long( (*variable)[numb_var-2] 
			       + (*VP_incr)[numb_var-2] );
		   else
		     (*variable)[numb_var-2] += (*VP_incr)[numb_var-2];
		 }
	     }
	   while ( (*variable)[numb_var-2] <= (*VP_max)[numb_var-2] );
	   tab_out << "}\n";

	   // increase the other parameters:

	   if ( (*VP_FA)[numb_var-3]=="FACTOR" )
	     {
	       if ( (*Var_Param_type)[numb_var-3] == "long" )
		 (*variable)[numb_var-3] 
		   = long( (*variable)[numb_var-3] * (*VP_incr)[numb_var-3] );
	       else
		 (*variable)[numb_var-3] *= (*VP_incr)[numb_var-3];
	     }
	   else
	     {
	       if ( (*Var_Param_type)[numb_var-3] == "long" )
		 (*variable)[numb_var-3] 
		   = long( (*variable)[numb_var-3] + (*VP_incr)[numb_var-3] );
	       else
		 (*variable)[numb_var-3] += (*VP_incr)[numb_var-3];
	     }

	   if ( numb_var > 3 )
	     for ( i=numb_var-3; i>0; i-- )
	       if ( (*variable)[i] > (*VP_max)[i] )
		 {
		   (*variable)[i] = (*VP_init)[i];
		   if ( (*VP_FA)[i-1]=="FACTOR" )
		     {
		       if ( (*Var_Param_type)[i-1] == "long" )
			 (*variable)[i-1] 
			   = long( (*variable)[i-1] * (*VP_incr)[i-1] );
		       else
			 (*variable)[i-1] *= (*VP_incr)[i-1];
		     }
		   else
		     {
		       if ( (*Var_Param_type)[i-1] == "long" )
			 (*variable)[i-1] 
			   = long( (*variable)[i-1] + (*VP_incr)[i-1] );
		       else
			 (*variable)[i-1] += (*VP_incr)[i-1];
		     }
		 }
	 } 
       while ( (*variable)[0] <= (*VP_max)[0] );
       // end of the big do-while loop
       
       tab_out << "</DATA>\n\n";
     } // if ( numb_var > 1 )

   if ( numb_var == 1 )
     {
       // the big do-while loop 
       tab_out << "TABELLE= {\n";
	   
       // now variate the last one parameters:

       (*variable)[0] = (*VP_init)[0];
       do  // [0]
	 {
	   tab_out << " { ";
	   cout<<(*Var_Param)[0]<<"="<<(*variable)[0]<<", ";
	   cout<<"\n";
	   
	   nb_in.open((CONFIG_HOME_St+"/proc/in~temp.lsa").c_str());
	   nb_in << (*prob_type);

	   Lisa_ControlParameters *my_special;
	   if ( !( my_special = new Lisa_ControlParameters ) )
	     {
	       G_ExceptionList.lthrow("out of memory",2);
	       return !OK;
	     }
	       
	   // the fixed controlparameters
	   if ( numb_fix_l > 0 )
	     for ( i=0; i<numb_fix_l; i++ )
	       my_special
		 ->add_key((*Fix_Param_long)[i], long((*FP_long)[i]));
	   if ( numb_fix_d > 0 )
	     for ( i=0; i<numb_fix_d; i++ )
	       my_special
		 ->add_key( (*Fix_Param_double)[i], (*FP_double)[i] );
	   if ( numb_fix_s > 0 )
	     for ( i=0; i<numb_fix_s; i++ )
	       my_special
		 ->add_key( (*Fix_Param_string)[i], (*FP_string)[i] );

	   // the variable controlparameters
	   if ( (*Var_Param_type)[0] == "long" )
	     my_special
	       ->add_key((*Var_Param)[0],long((*variable)[0]));
	   else
	     my_special
	       ->add_key((*Var_Param)[0],double((*variable)[0])); 
	    
	   nb_in << (*my_special);
	   delete my_special;
	   nb_in << (*problem_in);
	   nb_in << (*plan_in);
	       
	   nb_in.close();

	   // now call the neighborhood main program
	   
	   S=LISA_HOME_St+"/bin/"+CALL+" ";
	   S=S+CONFIG_HOME_St+"/proc/in~temp.lsa ";
	   S=S+CONFIG_HOME_St+"/proc/out~temp.lsa";
	   system(S.c_str());

	   nb_out.open((CONFIG_HOME_St+"/proc/out~temp.lsa").c_str());
	       
	   OBJ_VALUE = MAXLONG;
	   no_objective = 0;
	   do 
	     {
	       S=""; nb_out >>S;
	       if (S=="") 
		 {
		   G_ExceptionList.lthrow
		     ("WARNING: no <OBJECTIVE> found in input file.");
		   no_objective = 1;
		 } 
	     }  
	   while ( (S!="<OBJECTIVE>") && (S!="") );
	   if ( no_objective == 0 )
	     {
	       nb_out >> S;
	       nb_out >> OBJ_VALUE;
	     }
	   tab_out << OBJ_VALUE << " ";

	   if ( OBJ_VALUE < BEST_OBJECTIVE )
	     {
	       BEST_OBJECTIVE = OBJ_VALUE;
	       (*VP_best)[0] = (*variable)[0];
	       nb_out.seekg(0);
	       nb_out >> (*best_schedule);
	     }
	   nb_out.close();
		   
	   tab_out << "}\n";
	   if ( (*VP_FA)[0] == "FACTOR" )
	     {
	       if ( (*Var_Param_type)[0] == "long" )
		 (*variable)[0] 
		   = long( (*variable)[0] 
			   * (*VP_incr)[0] );
	       else
		 (*variable)[0] *= (*VP_incr)[0];
	     }
	   else
	     {
	       if ( (*Var_Param_type)[0] == "long" )
		 (*variable)[0] 
		   = long( (*variable)[0] 
			   + (*VP_incr)[0] );
	       else
		 (*variable)[0] += (*VP_incr)[0];
	     }
	 }
       while ( (*variable)[0] <= (*VP_max)[0] );
       tab_out << "}\n";
       // end of the big do-while loop
       
       tab_out << "</DATA>\n\n";
     } // if ( numb_var == 1 ) 

   if ( numb_var == 0 )
     {
       nb_in.open((CONFIG_HOME_St+"/proc/in~temp.lsa").c_str());
       nb_in << (*prob_type);

       Lisa_ControlParameters *my_special;
       if ( !( my_special = new Lisa_ControlParameters ) )
	 {
	   G_ExceptionList.lthrow("out of memory",2);
	   return !OK;
	 }
       
       // the fixed controlparameters
       if ( numb_fix_l > 0 )
	 for ( i=0; i<numb_fix_l; i++ )
	   my_special
	     ->add_key((*Fix_Param_long)[i], long((*FP_long)[i]));
       if ( numb_fix_d > 0 )
	 for ( i=0; i<numb_fix_d; i++ )
	   my_special
	     ->add_key( (*Fix_Param_double)[i], (*FP_double)[i] );
       if ( numb_fix_s > 0 )
	 for ( i=0; i<numb_fix_s; i++ )
	   my_special
	     ->add_key( (*Fix_Param_string)[i], (*FP_string)[i] );
       
       nb_in << (*my_special);
       delete my_special;
       nb_in << (*problem_in);
       nb_in << (*plan_in);
       
       nb_in.close();

       // now call the neighborhood main program
       
       S=LISA_HOME_St+"/bin/"+CALL+" ";
       S=S+CONFIG_HOME_St+"/proc/in~temp.lsa ";
       S=S+CONFIG_HOME_St+"/proc/out~temp.lsa";
       system(S.c_str());

       cout << "fertig \n";
       nb_out.open((CONFIG_HOME_St+"/proc/out~temp.lsa").c_str());
       
       OBJ_VALUE = MAXLONG;
       no_objective = 0;
       do 
	 {
	   S=""; nb_out >>S;
	   if (S=="") 
	     {
	       G_ExceptionList.lthrow
		     ("WARNING: no <OBJECTIVE> found in input file.");
	       no_objective = 1;
	     } 
	 }  
       while ( (S!="<OBJECTIVE>") && (S!="") );
       if ( no_objective == 0 )
	 {
	   nb_out >> S;
	   nb_out >> OBJ_VALUE;
	 }
       if ( OBJ_VALUE < BEST_OBJECTIVE )
	 {
	   BEST_OBJECTIVE = OBJ_VALUE;
	   nb_out.seekg(0);
	   nb_out >> (*best_schedule);
	 }
       nb_out.close();
     }


   tab_out << "<REMARKS>\n";
   tab_out << "Best Objective = " << BEST_OBJECTIVE << "\n";
   if ( numb_var > 0 )
     {
       tab_out << "by the follow parameters:\n";
       for ( i=0; i<numb_var; i++ )
	 tab_out << (*Var_Param)[i] << " = " << (*VP_best)[i] << "\n";
     }
   tab_out << "</REMARKS>\n\n";
    
   cout << (*best_schedule);
   ostrm << (*best_schedule);
   tab_out << (*best_schedule);
   ostrm.close();
   strm.close();
   strm.open((CONFIG_HOME_St+"/proc/para_out.lsa").c_str());
   strm.close();
   
   delete problem_in;
   delete plan_in;
   delete prob_type;
   delete best_schedule;
   delete Fix_Param_long;
   delete Fix_Param_double;
   delete Fix_Param_string;
   delete Var_Param;
   delete Var_Param_type;
   delete FP_long;
   delete FP_double;
   delete FP_string;
   delete VP_init;
   delete VP_max;
   delete VP_incr;
   delete VP_FA;
   delete variable;
   delete VP_best;

   return !OK;
 }

//**************************************************************************

