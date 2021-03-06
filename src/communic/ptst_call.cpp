/**
 * @author  Andreas Winkler
 * @version 3.0
 */ 

#include <string>
#include <sstream>
#include <fstream>

#include "ptst_call.hpp"

#include "../main/defaults.hpp"

using namespace std;

//**************************************************************************

extern class Lisa_ProblemType G_ProblemType;
extern class Lisa_Values G_Values;
extern class Lisa_Schedule *G_Schedule;
extern class Lisa_ControlParameters G_Preferences;

//**************************************************************************

/// starts external algorithm parameter test

int TC_startparam(ClientData, //cliet data
																		Tcl_Interp *interp,
																		int,
																		TCL_HACK_CHAR *argv[]) 
{
		
		
		//  long no_of_solutions=0;
		//   TIMETYP upper_bound=0, lower_bound=0;
		//   float ub_value=0,lb_value=0;
		//   string ins_order,bounding,nb_solutions,str;
  string name_of_algo,str="",type,name_of_parameter;
  string var_or_fix;
  int i=0,n=0;
  long int long_var=0;
  double double_var=0;
  string string_var="";
  Lisa_ControlParameters myctrlpara;
  name_of_algo=argv[1];
  //cout << "Algo: " << name_of_algo << " started" << endl;

  sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),"NO_OF_ENTRYS",TCL_GLOBAL_ONLY),"%d",&n);
  Tcl_Eval(interp,"puts e:$bb(NO_OF_ENTRYS)");
  //cout << n << " entrys in controlparameters" << endl;

  str = G_Preferences.get_string(Lisa_Pref::LISACONFIG)+"/proc/" + DEFAULT_IN_FILE;
  
		//XML
		//ofstream fout(str.c_str());
  str = "";

  // write the fixed parameters
  //fout << "<FIX_PARAMETERS>\n";


		int var_no = 1;
		const string fixed_prefix = "fixed_para_";
		const string variable_prefix   = "variable_para_";
		stringstream ss;
  for (i=1;i<=n;i++) 
    {
      str= ztos(i) + ",TYPE";
      type=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),(char*) 
																							str.c_str(),TCL_GLOBAL_ONLY);
      str= ztos(i) + ",NAME_OF_PARAMETER";
      name_of_parameter=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																				c_str(), (char*) str.c_str(),
																																				TCL_GLOBAL_ONLY);
      str= ztos(i) + ",VAR_OR_FIX";
      var_or_fix=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),
																													(char*) str.c_str(),TCL_GLOBAL_ONLY);
      if ( var_or_fix == "fixed" )
								{
										//XML
										//fout << type << " " << name_of_parameter << "\n";
										ss.str("");
										ss << fixed_prefix;
										ss << var_no++;
										myctrlpara.add_key(ss.str(),name_of_parameter);
								}
    }
  
		//XML
		//fout << "</FIX_PARAMETERS>\n\n";
		
  // write the variable parameters
  //fout << "<VAR_PARAMETERS>\n";

		var_no = 1;
  for (i=1;i<=n;i++) 
    {
      str= ztos(i) + ",TYPE";
      type=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),(char*) 
																							str.c_str(),TCL_GLOBAL_ONLY);
      str= ztos(i) + ",NAME_OF_PARAMETER";
      name_of_parameter=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																				c_str(),(char*) str.c_str(),
																																				TCL_GLOBAL_ONLY);
      str= ztos(i) + ",VAR_OR_FIX";
      var_or_fix=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),
																													(char*) str.c_str(),TCL_GLOBAL_ONLY);
      if ( var_or_fix == "variable" )
								{
											//XML
										//fout << type << " " << name_of_parameter << "\n";
										ss.str("");
										ss << variable_prefix;
										ss << var_no++;
										myctrlpara.add_key(ss.str(),name_of_parameter);
								}
    }
			//XML
  //fout << "</VAR_PARAMETERS>\n";
			//XML
  //fout.close();
		
  for (i=1;i<=n;i++) 
    {
      str= ztos(i) + ",TYPE";
      type=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),(char*) 
																							str.c_str(),TCL_GLOBAL_ONLY);
      str= ztos(i) + ",NAME_OF_PARAMETER";
      name_of_parameter=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																				c_str(),(char*) str.c_str(),
																																				TCL_GLOBAL_ONLY);
      str= ztos(i) + ",VAR_OR_FIX";
      var_or_fix=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),
																													(char*) str.c_str(),TCL_GLOBAL_ONLY);
      if (type=="double") 
								{
										if ( var_or_fix == "fixed" )
												{
														sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)(name_of_parameter+"_init").
																																	c_str(),TCL_GLOBAL_ONLY),"%lf",&double_var);
														myctrlpara.add_key(name_of_parameter,double_var);
												}
										if ( var_or_fix == "variable" )
												{
														//XML - add entry to recognize type
														myctrlpara.add_key((name_of_parameter),(double) 0.0);
														sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)(name_of_parameter+"_init").
																																	c_str(),TCL_GLOBAL_ONLY),"%lf",&double_var);
														myctrlpara.add_key((name_of_parameter+"_INIT"),double_var);
														sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)(name_of_parameter+"_max").
																																	c_str(),TCL_GLOBAL_ONLY),"%lf",&double_var);
														myctrlpara.add_key((name_of_parameter+"_MAX"),double_var);
														sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)(name_of_parameter+"_incr").
																																	c_str(),TCL_GLOBAL_ONLY),"%lf",&double_var);
														myctrlpara.add_key((name_of_parameter+"_INCR"),double_var);
														string_var=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																					c_str(),(char*)(name_of_parameter+"_fa").
																																					c_str(), TCL_GLOBAL_ONLY);
														myctrlpara.add_key( (name_of_parameter+"_FA"), string_var );
												}
								}
      if (type=="string") 
								{
										string_var=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)name_of_parameter.c_str(),
																																	TCL_GLOBAL_ONLY);
										myctrlpara.add_key(name_of_parameter,string_var);
								}
      if (type=="long") 
								{
										if ( var_or_fix == "fixed" )
												{
														sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)(name_of_parameter+"_init").
																																	c_str(), TCL_GLOBAL_ONLY),"%ld",&long_var);
														myctrlpara.add_key(name_of_parameter,long_var);
												}
										if ( var_or_fix == "variable" )
												{
														//XML - add entry to recognize type
														myctrlpara.add_key((name_of_parameter),(long) 0);
														
														sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)(name_of_parameter+"_init").
																																	c_str(), TCL_GLOBAL_ONLY),"%ld",&long_var);
														myctrlpara.add_key((name_of_parameter+"_INIT"),long_var);
														sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)(name_of_parameter+"_max").
																																	c_str(), TCL_GLOBAL_ONLY),"%ld",&long_var);
														myctrlpara.add_key((name_of_parameter+"_MAX"),long_var);
														sscanf(Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																	c_str(),(char*)(name_of_parameter+"_incr").
																																	c_str(), TCL_GLOBAL_ONLY),"%lf",&double_var);
														myctrlpara.add_key((name_of_parameter+"_INCR"),double_var);
														string_var=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").
																																					c_str(),(char*)(name_of_parameter+"_fa").
																																					c_str(), TCL_GLOBAL_ONLY);
														myctrlpara.add_key( (name_of_parameter+"_FA"), string_var );
												}
								}
    }
  myctrlpara.add_key( "CONFIG_HOME", G_Preferences.get_string(Lisa_Pref::LISACONFIG) );
  myctrlpara.add_key( "LISA_HOME", G_Preferences.get_string(Lisa_Pref::LISAHOME) );
  string_var=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),"FILE",TCL_GLOBAL_ONLY);
  myctrlpara.add_key( "FILE", string_var );
		
  string algo_call="";
  algo_call=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),"ALGO_CALL",TCL_GLOBAL_ONLY);  
  myctrlpara.add_key( "CALL", algo_call );
  myctrlpara.add_key( "DESCRIPTION", 
																						("Results_of_"+name_of_algo+"_Parametertest") );
  algo_call = "gen_ptst";
  start_paratest(interp,
																	name_of_algo,
																	algo_call, 
																	G_Preferences.get_string(Lisa_Pref::LISACONFIG)+"/proc/" + DEFAULT_IN_FILE,
																	G_Preferences.get_string(Lisa_Pref::LISACONFIG)+"/proc/" + DEFAULT_OUT_FILE,
																	G_Preferences,
																	G_ProblemType,
																	myctrlpara,
																	*G_Schedule,
																	G_Values);
  
  return TCL_OK;
}

//**************************************************************************
		
int start_paratest(Tcl_Interp *interp, 
																			string name_of_algo, 
																			string algo_call, 
																			string output_file, 
																			string result_file,
																			Lisa_ControlParameters& G_Preferences,
																			Lisa_ProblemType & G_ProblemType,
																			Lisa_ControlParameters & parameter,
																			Lisa_Schedule & G_Schedule, 
																			Lisa_Values & G_Values) {
				
		string str="",str2="";
		//XML
		//ofstream fout(output_file.c_str(), ios::app );
				
		//fout << G_ProblemType;
		//fout << parameter;
		//fout << G_Values;
		//fout << G_Schedule;
		//fout.close();
		LisaXmlFile::DOC_TYPE type = (G_Schedule.valid ? 
																																LisaXmlFile::SOLUTION : 
																																LisaXmlFile::INSTANCE);
		LisaXmlFile xmlOutput(type);
		xmlOutput << G_ProblemType << G_Values << parameter;
		if(G_Schedule.valid)
				xmlOutput << G_Schedule;
		xmlOutput.write(output_file);
		
		cout << "paratest:" << algo_call << endl;
		// call the external program
		// TCL/TK does this for us
				
		str="set lsa_status(fid) [open \"| "+G_Preferences.get_string(Lisa_Pref::LISAHOME)+"/bin/" + 
				algo_call + " " + output_file + " " + result_file+ "\" \"r\" ]";
		Tcl_Eval(interp,(char*) str.c_str());
				
		str="set lsa_status(algo_call) "+  algo_call;
		Tcl_Eval(interp,(char*) str.c_str());
		// get pid number of process and
		// test, if algorithm exists
		str="set lsa_status(pid) -1";
		Tcl_Eval(interp,(char*) str.c_str());
		str="set lsa_status(pid) [pid $lsa_status(fid)]";
		Tcl_Eval(interp,(char*) str.c_str());
		str2=Tcl_GetVar2(interp,"lsa_status","pid",TCL_GLOBAL_ONLY);
		if (str2=="-1") {
				G_ExceptionList.lthrow("no file: "+G_Preferences.get_string(Lisa_Pref::LISAHOME)+"/bin/"+algo_call+ " in LiSA path",Lisa_ExceptionList::END_OF_FILE);
				return !OK;
		}
				
		// external Module has started
		// open process window and give the process handling to Tcl
		str="fileevent $lsa_status(fid) readable \"algo_event $lsa_status(fid) " + result_file +" "+ name_of_algo + "\"";
		Tcl_Eval(interp,(char*) str.c_str());
		Tcl_Eval(interp,"Window show .ext_alg_graph");
		return OK;
}

//**************************************************************************
		
		
