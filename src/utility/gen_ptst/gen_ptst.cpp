/**
 * @author  Andreas Winkler
 * @version 3.0pre1
 */ 

#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <iomanip> 
 
#include "../../basics/matrix.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ptype.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../lisa/ctrlpara.hpp"

#include "../../scheduling/os_sched.hpp"
#include "../../scheduling/js_sched.hpp"
#include "../../scheduling/m1_sched.hpp"

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

//this is neccessary because of the weird LiSa-API
double getObjective(Lisa_Schedule& schedule,
																	Lisa_ProblemType& problem,
																	Lisa_Values& values)
{
		cout << "getObjective() ... ";
		if(!schedule.valid)
				{
						cout << "invalid schedule" << endl;
						return -1;
				}
		int env = problem.get_property(M_ENV);
		if(env == O)
				{
						Lisa_OsProblem osp(&values);
						Lisa_OsSchedule oss(&osp);
						if(schedule.LR)
								{
										oss.read_LR(schedule.LR);
										oss.SetValue(problem.get_property(OBJECTIVE));
										cout << oss.GetValue() << endl;
										return oss.GetValue();
								}
						else
								{
								cout << " no LR" << endl;
								return -1;
								}
				}
		else if(env == ONE)
				{
						
						Lisa_OsProblem osp(&values);
						Lisa_OsSchedule oss(&osp);
						if(schedule.LR)
								{
										if(values.get_m() != 1)
												{
														cout << "wrong size" << endl;
														return -1;
												}
										oss.read_LR(schedule.LR);
										oss.SetValue(problem.get_property(OBJECTIVE));
										cout << oss.GetValue() << endl;
										return oss.GetValue();
								}
						else
								{
								cout << " no LR" << endl;
								return -1;
								}
				}
		else if(env == J || env == F)
				{
						Lisa_JsProblem jsp(&values);
						Lisa_JsSchedule jss(&jsp);
						if(schedule.LR)
								{
										jss.read_LR(schedule.LR);
										jss.SetValue(problem.get_property(OBJECTIVE));
										cout << jss.GetValue() << endl;
										return jss.GetValue();
								}
						else
								{
										cout << " no LR" << endl;
										return -1;
								}
				}
		cout << " FAILED" << endl;
		return -1;
}




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
		//XML
		LisaXmlFile::initialize(".");
		
		LisaXmlFile myConfig;
		ifstream i_strm(argv[1]);      // this file contains the problem, the schedule and the
		ofstream o_strm(argv[2]);
		if (!i_strm)
				{
						G_ExceptionList.lthrow("could not open file "+string(argv[1]));
						exit(7);
				}
		if (!o_strm)
				{
						G_ExceptionList.lthrow("could not open file "+string(argv[2]));
						exit(7);
				}
		o_strm.close();
		i_strm.close();
		Lisa_ProblemType Problem;
		Lisa_ControlParameters Parameters;
		Lisa_Values Values;
      
		myConfig.read(argv[1]);
		LisaXmlFile::DOC_TYPE doc_type = myConfig.getDocumentType();
		
  if (!myConfig || (doc_type != LisaXmlFile::INSTANCE && doc_type != LisaXmlFile::SOLUTION))
    {
						G_ExceptionList.lthrow("cannot read file , aborting program.");
						exit(7);
				}
		if( !(myConfig >> Problem))
				{
						G_ExceptionList.lthrow("cannot read ProblemType , aborting program.");
						exit(7);
				}
		if( !(myConfig >> Parameters))
				{
						G_ExceptionList.lthrow("cannot read ControlParameters , aborting program.");
						exit(7);
				}
		if( !(myConfig >> Values))
				{
						G_ExceptionList.lthrow(" cannot read Values , aborting program.");
						exit(7);
				}
		if( !(G_ExceptionList.empty()))
				{
						G_ExceptionList.lthrow("failed reading input, aborting.");
						exit(7);
				}
		
		//strm.open(argv[1]); // startparameter (standard is para_in.lsa)
		
		//thes files are now xml

		//ofstream nb_in;     // these two file are necessary to communicate with 
		//ifstream nb_out;    // the external algorithm
		
		// at first read the names of all parameters
		// create two lists of string for fixed and variable parameters

		Lisa_List<string> *Fix_Param_long;
		if ( !(Fix_Param_long = new Lisa_List<string>()) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_List<string> *Fix_Param_double;
		if ( !(Fix_Param_double = new Lisa_List<string>()) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_List<string> *Fix_Param_string;
		if ( !(Fix_Param_string = new Lisa_List<string>()) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_List<string> *Var_Param;
		if ( !(Var_Param = new Lisa_List<string>()) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_List<string> *Var_Param_type;
		if ( !(Var_Param_type = new Lisa_List<string>()) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		
		// recoded for xml all is inside Parameters

		numb_fix_l = 0; numb_fix_d = 0; numb_fix_s = 0;
		int get_try = 0;
		const string fixed_prefix = "fixed_para_";
		const string variable_prefix   = "variable_para_";
		int type = Lisa_ControlParameters::UNDEFINED;
		stringstream ss;
		//fixed stuff
		string var_name;
		Lisa_List<string> *target;
		//read fixed stuff
		while(1) 
				{
						get_try++;
						ss.str("");
						ss << fixed_prefix << get_try;
						if(Parameters.defined(ss.str()) != Lisa_ControlParameters::STRING)
								break;
						var_name = Parameters.get_string(ss.str());
						type = Parameters.defined(var_name);
						if(type == Lisa_ControlParameters::UNDEFINED) //someone is tricking ;-)
								{
										cerr << var_name << "declared fixed, but not present. -> ignored" << endl;
										continue;
								}
						switch(type)
								{
								case Lisa_ControlParameters::LONG:
										target = Fix_Param_long;
										numb_fix_l++;
										break;
								case Lisa_ControlParameters::DOUBLE:
										target = Fix_Param_double;
										numb_fix_d++;
										break;
								case Lisa_ControlParameters::STRING:
										target = Fix_Param_string;
										numb_fix_s++;
										break;
								default:
										target = NULL;
										break;
								}
						if(target)
								target->append(var_name);
				}
		
		// read the variable parameters
		numb_var = 0;
		get_try = 0;
		while(1) 
				{
						get_try++;
						ss.str("");
						ss << variable_prefix << get_try;
						if(Parameters.defined(ss.str()) != Lisa_ControlParameters::STRING)
								break;
						var_name = Parameters.get_string(ss.str());
						type = Parameters.defined(var_name);
						if(type == Lisa_ControlParameters::UNDEFINED) //someone is tricking ;-)
								continue;
						switch(type)
								{
								case Lisa_ControlParameters::LONG:
										Var_Param_type->append("long");
										Var_Param->append(var_name);
										numb_var++;
										break;
								case Lisa_ControlParameters::DOUBLE:
										Var_Param_type->append("double");
										Var_Param->append(var_name);
										numb_var++;
										break;
								default:
										G_ExceptionList.lthrow("Cannot handle type for variable");
										exit( 7 );
										break;
								}
				}
	
		cout << "fix: " << (numb_fix_l+numb_fix_d+numb_fix_s) << "\n";
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
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_Vector<double> *FP_double;
		if ( !(FP_double = new Lisa_Vector<double>(numb_fix_d)) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_Vector<string> *FP_string;
		if ( !(FP_string = new Lisa_Vector<string>(numb_fix_s)) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_Vector<double> *VP_init;
		if ( !(VP_init = new Lisa_Vector<double>(numb_var)) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_Vector<double> *VP_max;
		if ( !(VP_max = new Lisa_Vector<double>(numb_var)) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_Vector<double> *VP_incr;
		if ( !(VP_incr = new Lisa_Vector<double>(numb_var)) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_Vector<string> *VP_FA;
		if ( !(VP_FA = new Lisa_Vector<string>(numb_var)) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_Vector<double> *variable;
		if ( !(variable = new Lisa_Vector<double>(numb_var)) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}
		Lisa_Vector<double> *VP_best;
		if ( !(VP_best = new Lisa_Vector<double>(numb_var)) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						exit( 7 );
				}

		if ( numb_fix_l > 0 )
				for ( i=0; i<numb_fix_l; i++ )
						(*FP_long)[i] = Parameters.get_long( (*Fix_Param_long)[i].c_str() );
		if ( numb_fix_d > 0 )
				for ( i=0; i<numb_fix_d; i++ )
						(*FP_double)[i] 
								= Parameters.get_double( (*Fix_Param_double)[i].c_str() );
		if ( numb_fix_s > 0 )
				for ( i=0; i<numb_fix_s; i++ )
						(*FP_string)[i] 
								= Parameters.get_string( (*Fix_Param_string)[i].c_str() );
		if ( numb_var > 0 )
				for ( i=0; i<numb_var; i++ )
						{
								if ( (*Var_Param_type)[i] == "long" )
										{
												(*VP_init)[i] 
														= Parameters.get_long( ((*Var_Param)[i]+"_INIT").c_str() );
												(*VP_max)[i] 
														= Parameters.get_long( ((*Var_Param)[i]+"_MAX").c_str() );
										}
								else
										{
												(*VP_init)[i] 
														= Parameters.get_double( ((*Var_Param)[i]+"_INIT").c_str());
												(*VP_max)[i] 
														= Parameters.get_double( ((*Var_Param)[i]+"_MAX").c_str() );
										}
								(*VP_incr)[i] 
										= Parameters.get_double( ((*Var_Param)[i]+"_INCR").c_str() );
								(*VP_FA)[i] 
										= Parameters.get_string( ((*Var_Param)[i]+"_FA").c_str() );
						}
   
		CONFIG_HOME_St = Parameters.get_string( "CONFIG_HOME" );
		LISA_HOME_St   = Parameters.get_string( "LISA_HOME" );
		FILE_St        = Parameters.get_string( "FILE" );
		DESCRIPTION    = Parameters.get_string( "DESCRIPTION" );
		CALL           = Parameters.get_string( "CALL" );

		// all input values read
		
		if ( FILE_St == " " )
				S = CONFIG_HOME_St+"/proc/ptst_tab.lsa";
		else
				S = FILE_St;

		
		ofstream tab_out(S.c_str()); // this file returns the table of the results
		
		//XML : we keep the tabular output in old fashioned style
		

		if (!tab_out)
				{
						G_ExceptionList.lthrow("could not open file "+ S );
						exit(1);
				}
		
		Lisa_Schedule *plan_in;
		if ( !( plan_in = new Lisa_Schedule() ) )
				{
						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
						return !OK;
				} 
		if(doc_type == LisaXmlFile::SOLUTION)
				myConfig >> *plan_in;
		
		// the best of all created schedules
		Lisa_Schedule *best_schedule;
		best_schedule = new Lisa_Schedule();
		if(doc_type == LisaXmlFile::SOLUTION)
				myConfig >> *best_schedule;
		
		
		// prepare the output table file
		tab_out << Problem;
		tab_out << Values;
		
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
		
		
		string alg_in_file = CONFIG_HOME_St +"/proc/in~temp.xml";
		string alg_out_file = CONFIG_HOME_St+"/proc/out~temp.xml";
		string alg_best_file(argv[2]);
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
	       
																		//nb_in.open((CONFIG_HOME_St+"/proc/in~temp.lsa").c_str());
																		//nb_in << Problem;
																		LisaXmlFile algInput(doc_type);
																		algInput << Problem << Values;
																		
																		Lisa_ControlParameters *my_special;
																		if ( !( my_special = new Lisa_ControlParameters ) )
																				{
																						G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
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

																		//nb_in << (*my_special);
																		algInput << (*my_special);
																		delete my_special;
																		//nb_in << Values;
																		
																		//nb_in << (*plan_in);
																				if(doc_type == LisaXmlFile::SOLUTION)
																						algInput << (*plan_in);
																		algInput.write(alg_in_file);
																		//nb_in.close();
																		
																		// now call the neighborhood main program
	       
																		S=LISA_HOME_St+"/bin/"+CALL+" ";
																		S=S+CONFIG_HOME_St+"/proc/in~temp.xml";
																		S=S+CONFIG_HOME_St+"/proc/out~temp.xml";
																		system(S.c_str());
																		
																		LisaXmlFile algOutput(LisaXmlFile::SOLUTION);
																		if(! algOutput.read(alg_out_file))
																				cerr << "Warning result cannot be read !!!" << endl;
																						
																		//nb_out.open((CONFIG_HOME_St+"/proc/out~temp.lsa").c_str());
																		
																		OBJ_VALUE = MAXLONG;
																																				
																		//we have to get objective value
																		Lisa_Schedule current_schedule;
																		algOutput >> current_schedule;
																		double obj;
																		if(!algOutput || !current_schedule.valid)
																				cerr << "no schedule returned" << endl;
																		else if((obj = getObjective(current_schedule,Problem,Values)) < 0)
																				cerr << "cannot determine objective value" << endl;
																		else
																				OBJ_VALUE = obj;
																		tab_out << OBJ_VALUE << " ";
																		
																		if ( OBJ_VALUE < BEST_OBJECTIVE )
																				{
																						BEST_OBJECTIVE = OBJ_VALUE;
																						for ( i=0; i<numb_var; i++ )
																								(*VP_best)[i] = (*variable)[i];
																						algOutput >> *best_schedule;
																				}
																		
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
										
										//nb_in.open((CONFIG_HOME_St+"/proc/in~temp.lsa").c_str());
										//nb_in << Problem;
										LisaXmlFile algInput(doc_type);
										algInput << Problem << Values;
									
										Lisa_ControlParameters *my_special;
										if ( !( my_special = new Lisa_ControlParameters ) )
												{
														G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
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
	    
										
										//nb_in << (*my_special);
										cout << "Call Config : " << endl << *my_special << endl;
										algInput << (*my_special);
										delete my_special;
										//nb_in << Values;
										
										//nb_in << (*plan_in);
										if(doc_type == LisaXmlFile::SOLUTION)
												algInput << (*plan_in);
										algInput.write(alg_in_file);
										//nb_in.close();
										
										// now call the neighborhood main program
	   
										S=LISA_HOME_St+"/bin/"+CALL+" ";
										S=S+CONFIG_HOME_St+"/proc/in~temp.xml";
										S=S+CONFIG_HOME_St+"/proc/out~temp.xml";
										system(S.c_str());
										
										LisaXmlFile algOutput(LisaXmlFile::SOLUTION);
										if(! algOutput.read(alg_out_file))
												cerr << "Warning result cannot be read !!!" << endl;
										
										//nb_out.open((CONFIG_HOME_St+"/proc/out~temp.lsa").c_str());
										
										OBJ_VALUE = MAXLONG;
										
										//we have to get objective value
										Lisa_Schedule current_schedule;
										algOutput >> current_schedule;
										double obj;
										if(!algOutput || !current_schedule.valid)
												cerr << "no schedule returned" << endl;
										else if((obj = getObjective(current_schedule,Problem,Values)) < 0)
												cerr << "cannot determine objective value" << endl;
										else
												OBJ_VALUE = obj;
										tab_out << OBJ_VALUE << " ";
										
										if ( OBJ_VALUE < BEST_OBJECTIVE )
												{
														BEST_OBJECTIVE = OBJ_VALUE;
														(*VP_best)[0] = (*variable)[0];
														algOutput >> *best_schedule;
												}
										
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
						//nb_in.open((CONFIG_HOME_St+"/proc/in~temp.lsa").c_str());
						//nb_in << Problem;
						LisaXmlFile algInput(doc_type);
						algInput << Problem << Values;



						Lisa_ControlParameters *my_special;
						if ( !( my_special = new Lisa_ControlParameters ) )
								{
										G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
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
       
						//nb_in << (*my_special);
						algInput << (*my_special);
						delete my_special;
						//nb_in << Values;
						
						//nb_in << (*plan_in);
								if(doc_type == LisaXmlFile::SOLUTION)
										algInput << (*plan_in);
						algInput.write(alg_in_file);
						//nb_in.close();
						
						// now call the neighborhood main program
						
						S=LISA_HOME_St+"/bin/"+CALL+" ";
						S=S+CONFIG_HOME_St+"/proc/in~temp.xml";
						S=S+CONFIG_HOME_St+"/proc/out~temp.xml";
						system(S.c_str());
										

						cout << "fertig \n";

						OBJ_VALUE = MAXLONG;
										
						LisaXmlFile algOutput(LisaXmlFile::SOLUTION);
						if(! algOutput.read(alg_out_file))
								cerr << "Warning result cannot be read !!!" << endl;
										
						//we have to get objective value
						Lisa_Schedule current_schedule;
															
						algOutput >> current_schedule;
						double obj;
						if(!algOutput || !current_schedule.valid)
								cerr << "no schedule returned" << endl;
						else if((obj = getObjective(current_schedule,Problem,Values)) < 0)
								cerr << "cannot determine objective value" << endl;
						else
								OBJ_VALUE = obj;
						tab_out << OBJ_VALUE << " ";
						
						if ( OBJ_VALUE < BEST_OBJECTIVE )
								{
										BEST_OBJECTIVE = OBJ_VALUE;
										
										algOutput >> (*best_schedule);
								}
						//nb_out.close();
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
		
		//cout << (*best_schedule);
		
		LisaXmlFile bestOut(LisaXmlFile::SOLUTION);
		bestOut << Problem 
										<< Values 
										<< Parameters 
										<< *best_schedule;

		if(!bestOut || !		bestOut.write(alg_best_file))
				cout << "cannot write result file" << endl;

		tab_out << (*best_schedule);

		//strm.close();
		//strm.open((CONFIG_HOME_St+"/proc/para_out.lsa").c_str());
		//strm.close();
   
		//delete problem_in;
		delete plan_in;
		//delete prob_type;
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

