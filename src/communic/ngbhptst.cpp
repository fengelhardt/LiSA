/*
 * ******************* ngbhptst.cpp ******************************
 * 
 * description:        functions which was called as tcl/tk command
 *                     parametertest of the neighborhood iteration
 *
 * owner:              Andreas Winkler
 *
 * date:               14.4.1999
 *
 * version:            V 1.0
*/ 


// ********************* System Includes ********************
#include "../basics/lisa_str.hpp"
#include <fstream.h>

// ************************ Local Includes ********************
#include "ngbhptst.hpp"

// ********************** Global Variables ********************
extern class Lisa_ProblemType G_ProblemType;
extern class Lisa_Values G_Values;
extern class Lisa_Schedule *G_Schedule;
extern class Lisa_Preferences G_Preferences;

// *********************** functions *************************

/// test some parameters of neighborhood search methods
int  TC_nb_ptst(ClientData /*clientData*/,
	    Tcl_Interp *interp,
	       int /*argc*/, char *argv[]) 
 {
   string S;

// definition of parameters for the file-controlling
// following parameters are possible:
   int    PROB_init, PROB_max,
	  MAX_STUCK_init, MAX_STUCK_max,
	  TABULENGTH_init, TABULENGTH_max,
	  NUMB_NGHB_init, NUMB_NGHB_max;
   long   STEPS_init, STEPS_max;
   float  PROB_incr, MAX_STUCK_incr, TABULENGTH_incr, 
	  NUMB_NGHB_incr, STEPS_incr;
   string NGBH_St,    //  possible is M1_API, M1_SHIFT, OSP_API, OSP_SHIFT, 
                      //  OSP_3_API, OSP_CR_API, OSP_BL_API, OSP_CR_SHIFT, 
                      //  OSP_BL_SHIFT, OSP_CR_TST, OSP_3_CR, JSP_API, 
                      //  JSP_SHIFT, JSP_3_API, JSP_CR_API, JSP_BL_API, 
                      //  JSP_CR_SHIFT, JSP_BL_SHIFT, JSP_3_CR
          METHOD_St,
          TYPE_St,    //  possible is ENUM, RAND
          OBJ_TYPE_St,//  possible is CMAX, LMAX, SUM_CI, SUM_WICI, SUM_UI,
		      //  SUM_WIUI, SUM_TI, SUM_WITI
          LISA_HOME_St, CONFIG_HOME_St,
          STEPS_FA_St, PROB_FA_St, MAX_STUCK_FA_St,
          TABULENGTH_FA_St, NUMB_NGHB_FA_St,
          FILE_St;
   float  ABORT_BOUND;

   NGBH_St          = "";
   METHOD_St        = "";
   TYPE_St          = "";
   STEPS_FA_St      = "";
   PROB_FA_St       = "";
   MAX_STUCK_FA_St  = "";
   TABULENGTH_FA_St = "";
   NUMB_NGHB_FA_St  = "";
   FILE_St          = "";
   STEPS_init       = 1;
   STEPS_max        = 1;
   STEPS_incr       = 2;
   PROB_init        = 1;
   PROB_max         = 1;
   PROB_incr        = 2;
   MAX_STUCK_init   = 1;
   MAX_STUCK_max    = 1;
   MAX_STUCK_incr   = 2;
   TABULENGTH_init  = 1;
   TABULENGTH_max   = 1;
   TABULENGTH_incr  = 2;
   NUMB_NGHB_init   = 1;
   NUMB_NGHB_max    = 1;
   NUMB_NGHB_incr   = 2;
   ABORT_BOUND      = 0;

   NGBH_St = Tcl_GetVar2(interp,"neighbor","NGBH",TCL_GLOBAL_ONLY);
   METHOD_St = Tcl_GetVar2(interp,"neighbor","METHOD",TCL_GLOBAL_ONLY);
   TYPE_St = Tcl_GetVar2(interp,"neighbor","TYPE",TCL_GLOBAL_ONLY);
   STEPS_FA_St = Tcl_GetVar2(interp,"nb","STEPS_FA",TCL_GLOBAL_ONLY);
   PROB_FA_St = Tcl_GetVar2(interp,"nb","PROB_FA",TCL_GLOBAL_ONLY);
   MAX_STUCK_FA_St  = Tcl_GetVar2(interp,"nb","MAX_STUCK_FA",TCL_GLOBAL_ONLY);
   TABULENGTH_FA_St =Tcl_GetVar2(interp,"nb","TABULENGTH_FA",TCL_GLOBAL_ONLY);
   NUMB_NGHB_FA_St  = Tcl_GetVar2(interp,"nb","NUMB_NGHB_FA",TCL_GLOBAL_ONLY);
   FILE_St  = Tcl_GetVar2(interp,"nb","FILE",TCL_GLOBAL_ONLY);
   sscanf(Tcl_GetVar2(interp,"neighbor","ABORT_BOUND",TCL_GLOBAL_ONLY),
	  "%f",&ABORT_BOUND);
   sscanf(Tcl_GetVar2(interp,"nb","STEPS_init",TCL_GLOBAL_ONLY),
	  "%ld",&STEPS_init);
   if ( STEPS_init < 1 )
     {
       G_ExceptionList.lthrow("STEPS_init must be positive");
       return TCL_OK;
     }
   sscanf(Tcl_GetVar2(interp,"nb","STEPS_max",TCL_GLOBAL_ONLY),
	  "%ld",&STEPS_max);
   sscanf(Tcl_GetVar2(interp,"nb","STEPS_incr",TCL_GLOBAL_ONLY),
	  "%f",&STEPS_incr);
   if ( ((STEPS_FA_St=="FACTOR")&&(long(STEPS_init*STEPS_incr)<=STEPS_init))
	|| ((STEPS_FA_St=="ADDEND")&&(STEPS_incr<1)) )
     {
       G_ExceptionList.lthrow("STEPS_factor is too small");
       return TCL_OK;
     }
   if (METHOD_St == "SA"|| METHOD_St == "TA") 
     {
       sscanf(Tcl_GetVar2(interp,"nb","PROB_init",TCL_GLOBAL_ONLY),
	      "%d",&PROB_init);
       if ( PROB_init < 0 )
	 {
	   G_ExceptionList.lthrow("PROB_init must be nonnegative");
	   return TCL_OK;
	 }
       sscanf(Tcl_GetVar2(interp,"nb","PROB_max",TCL_GLOBAL_ONLY),
	      "%d",&PROB_max);
       sscanf(Tcl_GetVar2(interp,"nb","PROB_incr",TCL_GLOBAL_ONLY),
	      "%f",&PROB_incr);
       if ( ((PROB_FA_St=="FACTOR")&&(long(PROB_init*PROB_incr)<=PROB_init))
	    || ((PROB_FA_St=="ADDEND")&&(PROB_incr<1)) )
	 {
	   G_ExceptionList.lthrow("PROB-factor is too small");
	   return TCL_OK;
	 }
       sscanf(Tcl_GetVar2(interp,"nb","MAX_STUCK_init",TCL_GLOBAL_ONLY),
	      "%d",&MAX_STUCK_init);
       if ( MAX_STUCK_init < 1 )
	 {
	   G_ExceptionList.lthrow("MAX_STUCK_init must be positive");
	   return TCL_OK;
	 }
       sscanf(Tcl_GetVar2(interp,"nb","MAX_STUCK_max",TCL_GLOBAL_ONLY),
	      "%d",&MAX_STUCK_max);
       sscanf(Tcl_GetVar2(interp,"nb","MAX_STUCK_incr",TCL_GLOBAL_ONLY),
	      "%f",&MAX_STUCK_incr);
       if ( ((MAX_STUCK_FA_St=="FACTOR")
	           &&(long(MAX_STUCK_init*MAX_STUCK_incr)<=MAX_STUCK_init))
	    || ((MAX_STUCK_FA_St=="ADDEND")&&(MAX_STUCK_incr<1)) )
	 {
	   G_ExceptionList.lthrow("MAX_STUCK-factor is too small");
	   return TCL_OK;
	 }
     }
   if (METHOD_St == "TS")
     {
       sscanf(Tcl_GetVar2(interp,"nb","TABULENGTH_init",TCL_GLOBAL_ONLY),
	      "%d",&TABULENGTH_init);
       if ( TABULENGTH_init < 1 )
	 {
	   G_ExceptionList.lthrow("TABULENGTH_init must be positive");
	   return TCL_OK;
	 }
       sscanf(Tcl_GetVar2(interp,"nb","TABULENGTH_max",TCL_GLOBAL_ONLY),
	      "%d",&TABULENGTH_max);
       sscanf(Tcl_GetVar2(interp,"nb","TABULENGTH_incr",TCL_GLOBAL_ONLY),
	      "%f",&TABULENGTH_incr);
       if ( ((TABULENGTH_FA_St=="FACTOR")
	         &&(long(TABULENGTH_init*TABULENGTH_incr)<=TABULENGTH_init))
	    || ((TABULENGTH_FA_St=="ADDEND")&&(TABULENGTH_incr<1)) )
	 {
	   G_ExceptionList.lthrow("TABULENGTH-factor is too small");
	   return TCL_OK;
	 }
       sscanf(Tcl_GetVar2(interp,"nb","NUMB_NGHB_init",TCL_GLOBAL_ONLY),
	      "%d",&NUMB_NGHB_init);
       if ( NUMB_NGHB_init < 1 )
	 {
	   G_ExceptionList.lthrow("NUMB_NGHB_init must be positive");
	   return TCL_OK;
	 }
       sscanf(Tcl_GetVar2(interp,"nb","NUMB_NGHB_max",TCL_GLOBAL_ONLY),
	      "%d",&NUMB_NGHB_max);
       sscanf(Tcl_GetVar2(interp,"nb","NUMB_NGHB_incr",TCL_GLOBAL_ONLY),
	      "%f",&NUMB_NGHB_incr);
       if ( ((NUMB_NGHB_FA_St=="FACTOR")
	             &&(long(NUMB_NGHB_init*NUMB_NGHB_incr)<=NUMB_NGHB_init))
	    || ((NUMB_NGHB_FA_St=="ADDEND")&&(NUMB_NGHB_incr<1)) )
	 {
	   G_ExceptionList.lthrow("NUMB_NGHB-factor is too small");
	   return TCL_OK;
	 }
     }

   CONFIG_HOME_St = G_Preferences.CONFIG_HOME;
   LISA_HOME_St   = G_Preferences.LISA_HOME;
	  
   // now create the output file for handling neighborhood search:

   S = G_Preferences.CONFIG_HOME+"/proc/para_in.lsa";
   ofstream ostrm(S.c_str());
   ostrm << G_ProblemType;

   if (G_ProblemType.get_property(M_ENV)==ONE)   NGBH_St = "M1_" + NGBH_St;
   else if(G_ProblemType.get_property(M_ENV)==O) NGBH_St = "OSP_" + NGBH_St;
   else 
     if(G_ProblemType.get_property(M_ENV)==J||G_ProblemType.get_property(M_ENV)==F)  
       NGBH_St = "JSP_" + NGBH_St;
     else 
       {
	 G_ExceptionList.lthrow("no Neighborhood for the actual Problem defined");
	 return TCL_OK;
       }

   // schreibe die Parameter in die Datei:
   Lisa_ControlParameters *ptest_special;
   if ( !( ptest_special = new Lisa_ControlParameters ) )
     {
       G_ExceptionList.lthrow("out of memory",2);
       return TCL_OK;
     };

   ptest_special->add_key( string("NGBH"),            NGBH_St );
   ptest_special->add_key( string("METHOD"),          METHOD_St );
   ptest_special->add_key( string("TYPE"),            TYPE_St );
   ptest_special->add_key( string("CONFIG_HOME"),     CONFIG_HOME_St );
   ptest_special->add_key( string("LISA_HOME"),       LISA_HOME_St );
   ptest_special->add_key( string("STEPS_FA"),        STEPS_FA_St );
   ptest_special->add_key( string("PROB_FA"),         PROB_FA_St );
   ptest_special->add_key( string("MAX_STUCK_FA"),    MAX_STUCK_FA_St );
   ptest_special->add_key( string("TABULENGTH_FA"),   TABULENGTH_FA_St );
   ptest_special->add_key( string("NUMB_NGHB_FA"),    NUMB_NGHB_FA_St );
   ptest_special->add_key( string("STEPS_INIT"),      long(STEPS_init));
   ptest_special->add_key( string("STEPS_MAX"),       long(STEPS_max));
   ptest_special->add_key( string("STEPS_INCR"),      float(STEPS_incr));
   ptest_special->add_key( string("PROB_INIT"),       long(PROB_init));
   ptest_special->add_key( string("PROB_MAX"),        long(PROB_max));
   ptest_special->add_key( string("PROB_INCR"),       float(PROB_incr));
   ptest_special->add_key( string("MAX_STUCK_INIT"),  long(MAX_STUCK_init));
   ptest_special->add_key( string("MAX_STUCK_MAX"),   long(MAX_STUCK_max));
   ptest_special->add_key( string("MAX_STUCK_INCR"),  float(MAX_STUCK_incr));
   ptest_special->add_key( string("TABULENGTH_INIT"), long(TABULENGTH_init));
   ptest_special->add_key( string("TABULENGTH_MAX"),  long(TABULENGTH_max));
   ptest_special->add_key( string("TABULENGTH_INCR"), float(TABULENGTH_incr));
   ptest_special->add_key( string("NUMB_NGHB_INIT"),  long(NUMB_NGHB_init));
   ptest_special->add_key( string("NUMB_NGHB_MAX"),   long(NUMB_NGHB_max));
   ptest_special->add_key( string("NUMB_NGHB_INCR"),  float(NUMB_NGHB_incr));
   ptest_special->add_key( string("ABORT_BOUND"),     double(ABORT_BOUND));
   ptest_special->add_key( string("FILE"),            FILE_St);
   ptest_special->add_key( string("NUMB_PROBLEMS"),   long(1) );
   ptest_special->add_key( string("NUMB_PLANS"),      long(1) );

   ostrm << (*ptest_special);
   delete ptest_special;

   ostrm << G_Values;
   ostrm << (*G_Schedule);
   ostrm.close(); 

   // now call the neighborhood main program
   /*
   S=G_Preferences.LISA_HOME+"/bin/nb_ptest ";
   S=S+G_Preferences.CONFIG_HOME+"/proc/para_in.lsa ";
   S=S+G_Preferences.CONFIG_HOME+"/proc/para_out.lsa";
   system(S.c_str());
   */
   
   // This is a try for executing in background
   S="set lsa_status(fid) [open \"| $env(LISAHOME)/bin/nb_ptest ";
   S=S+G_Preferences.CONFIG_HOME+"/proc/para_in.lsa ";
   S=S+G_Preferences.CONFIG_HOME+"/proc/para_out.lsa";
   S=S+"\" \"r\" ]";
   Tcl_Eval(interp,(char*) S.c_str());
   
   S="set nb(pid) [pid $lsa_status(fid)]";
   Tcl_Eval(interp,(char*) S.c_str()); 
   
   if (  Tcl_GetVar2(interp,"nb","pid",TCL_GLOBAL_ONLY)==NULL )
     {
       G_ExceptionList.lthrow("no file: "+G_Preferences.LISA_HOME+"/nb_iter in LiSA path",END_OF_FILE);
       return !OK;
     }
   
   
   S="fileevent $lsa_status(fid) readable \"nbptevent $lsa_status(fid) ";
   S=S+G_Preferences.CONFIG_HOME+"/proc/out~temp.lsa"+ "\"";
   Tcl_Eval(interp,(char*) S.c_str());
   Tcl_Eval(interp,"Window show .nbpt_graph");
   

   S = G_Preferences.CONFIG_HOME+"/proc/para_out.lsa";
   ifstream istrm(S.c_str());
   //istrm >> G_Values;
   //istrm >> G_Schedule;
   istrm.close();
   return TCL_OK;
 }


/// starts external algorithm parameter test
int TC_startparam(ClientData /* clientData */,
		Tcl_Interp *interp,
		int /*argc*/, char *argv[]) {


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

  str = G_Preferences.CONFIG_HOME+"/proc/algo_in.lsa";
  ofstream fout(str.c_str());
  str = "";

  // write the fixed parameters
  fout << "<FIX_PARAMETERS>\n";
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
	fout << type << " " << name_of_parameter << "\n";
    }
  fout << "</FIX_PARAMETERS>\n\n";

  // write the variable parameters
  fout << "<VAR_PARAMETERS>\n";
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
	fout << type << " " << name_of_parameter << "\n";
    }
  fout << "</VAR_PARAMETERS>\n";

  fout.close();

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
  myctrlpara.add_key( "CONFIG_HOME", G_Preferences.CONFIG_HOME );
  myctrlpara.add_key( "LISA_HOME", G_Preferences.LISA_HOME );
  string_var=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),"FILE",
				 TCL_GLOBAL_ONLY);
  myctrlpara.add_key( "FILE", string_var );

  string algo_call="";
  algo_call=Tcl_GetVar2(interp,(char*) (name_of_algo+"_ptst").c_str(),
			"ALGO_CALL",TCL_GLOBAL_ONLY);
  myctrlpara.add_key( "CALL", algo_call );
  myctrlpara.add_key( "DESCRIPTION", 
		      ("Results_of_"+name_of_algo+"_Parametertest") );
  algo_call = "gen_ptst";
  start_paratest(interp,name_of_algo,algo_call, G_Preferences.CONFIG_HOME+"/proc/algo_in.lsa",
	     G_Preferences.CONFIG_HOME+"/proc/algo_out.lsa",
	     G_Preferences,G_ProblemType,myctrlpara,*G_Schedule,G_Values);
  
  return TCL_OK;
}


int start_paratest(Tcl_Interp *interp, string name_of_algo, string algo_call, string output_file, string result_file,
	     Lisa_Preferences & G_Preferences,
	     Lisa_ProblemType & G_ProblemType,
	     Lisa_ControlParameters & parameter,
	     Lisa_Schedule & G_Schedule, Lisa_Values & G_Values) {

  string str="",str2="";
  ofstream fout(output_file.c_str(), ios::app );

  fout << G_ProblemType;
  fout << parameter;
  fout << G_Values;
  fout << G_Schedule;
  fout.close();
  
  cout << "paratest:" << algo_call << endl;
  // call the external program
  // TCL/TK does this for us
  
  str="set lsa_status(fid) [open \"| "+G_Preferences.LISA_HOME+"/bin/" + 
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
    G_ExceptionList.lthrow("no file: "+G_Preferences.LISA_HOME+"/bin/"+algo_call+ " in LiSA path",END_OF_FILE);
    return !OK;
  }
  
  // external Module has started
  // open process window and give the process handling to Tcl
  str="fileevent $lsa_status(fid) readable \"algo_event $lsa_status(fid) " + result_file +" "+ name_of_algo + "\"";
  Tcl_Eval(interp,(char*) str.c_str());
  Tcl_Eval(interp,"Window show .ext_alg_graph");
  return OK;
}












