/**
 * @author  Per Willenius
 * @version 3.0
 */ 

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <string>
#include <vector>
#include <fstream>

#include "../misc/except.hpp"
#include "../misc/int2str.hpp"
#include "../main/lisapref.hpp"
#include "../main/global.hpp"
#include "../main/status.hpp"
#include "../main/file_io.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"
#include "../scheduling/xsched.hpp"
#include "../scheduling/os_sched.hpp"
#include "../classify/classify.hpp"
#include "../communic/alg_call.hpp"
#include "../basics/order.hpp"
#include "../desktop/tclvar.hpp"

#include "callback.hpp"
#include "gantt.hpp"
#include "tcl_c.hpp"
#include "c_tcl.hpp"

#include "../main/defaults.hpp"

using namespace std;

//**************************************************************************

extern class Lisa_ProblemType G_ProblemType;
extern class Lisa_Values G_Values;
extern class Lisa_Schedule *G_Schedule;
extern class Lisa_XSchedule *G_XSchedule;
extern class Lisa_ControlParameters G_Preferences;
extern class Lisa_TCLVar G_TclVar;
extern class Lisa_Canvas *G_MWCanvas;
extern class Lisa_Status G_Status;
extern class Lisa_List<Lisa_ScheduleNode> *G_ScheduleList;
extern class Lisa_Classify *G_Classify;

//**************************************************************************

// write MO as Matrix in Lisa_MO
int TC_adopt_MO(ClientData /* clientData*/,
		Tcl_Interp * /* interp*/,
		int /*argc*/, TCL_HACK_CHAR ** /* argv[]*/ ) {
  
  int i=0,j=0;
  if (G_TclVar.MO_as_Matrix) {
    if (G_Values.MO==NULL) {G_Values.make_MO();}
    for(i=0;i<G_Values.get_m();i++)
      for (j=0;j<G_Values.get_n();j++)
	if ( (*G_Values.SIJ)[j][i]==false) 
	  (*G_TclVar.MO_as_Matrix)[j][i]=0;
        G_Values.MO->read_rank(G_TclVar.MO_as_Matrix);
    G_TclVar.MO_as_Matrix->fill(0);
    G_Values.MO->write_rank(G_TclVar.MO_as_Matrix);
        if (G_Values.valid_MO()==false) 
      G_ExceptionList.lthrow("Machine Order not valid");
    new_values();
  }
  return TCL_OK;
}

//**************************************************************************

// write Lisa_MO in Matrix in G_TclVar
int TC_make_MO_Matrix(ClientData /*clientData*/,
		      Tcl_Interp * /* interp*/,
		      int /*argc*/, TCL_HACK_CHAR **/* argv[]*/) {
  if (G_Values.MO) {
    if (G_TclVar.MO_as_Matrix==NULL) {
      G_TclVar.make_MO(G_Values.get_n(),G_Values.get_m());
    }
    G_TclVar.MO_as_Matrix->fill(0);
    G_Values.MO->write_rank(G_TclVar.MO_as_Matrix);
  }
  return TCL_OK;
}

//**************************************************************************

int TC_open(ClientData /* clientData*/,
	    Tcl_Interp */* interp*/,
	    int argc, TCL_HACK_CHAR *argv[])
{
  if (argc<2) return TCL_OK; // no filename given
  string ifile;
  // read the filename
  ifile = argv[1];
    // open the file
  if (ifile=="") return TCL_OK;
  if (read_xml(ifile))   return  TCL_OK;
  return TCL_OK;
}

//**************************************************************************

int TC_open_schedule(ClientData /* clientData*/ ,
	    Tcl_Interp * /* interp*/ ,
	    int argc, TCL_HACK_CHAR *argv[])
{
  if (argc<2) return TCL_OK; // no filename given
  ifstream fin( argv[1]);
  if (fin==NULL) { 
    G_ExceptionList.lthrow("file:"+(string) argv[1]+" cannot be opened");
    return TCL_OK;
  }
		fin.close();
  while (G_ExceptionList.empty()==0) print_error();

  if (read_schedule_xml(string(argv[1])))   return  TCL_OK;


  return TCL_OK;
}

//**************************************************************************

int TC_save(ClientData /* clientData */,
	    Tcl_Interp * /* interp */,
	    int argc, TCL_HACK_CHAR *argv[])  
{ 
  if (argc<2) return TCL_OK; // no filename given
  string sfile;
  // read the filename
  sfile = argv[1];
  // save the file
  if (sfile=="") return TCL_OK;
  if (save_xml(sfile)) return  TCL_OK;
  return TCL_OK;
}

//**************************************************************************

// generate randomly processing time matrix with parameters min,max,step,seed1,seed2
int TC_genpt(ClientData /* clientData */,
	     Tcl_Interp * /*interp */,
	     int /*argc*/, TCL_HACK_CHAR *argv[])  
{
  float min,max,step;
  int i,j;
  int temp, u,m,n;
  m=G_Values.get_m(); n=G_Values.get_n();

  min = atof(argv[1]);
  max = atof(argv[2]);
  //sscanf(argv[1],"%f",&min);
  //sscanf(argv[2],"%f",&max);
  if (min==max) {
    for (j=0; j<n; j++) {
      for (i=0; i<m; i++) {
	(*G_Values.PT)[j][i]=min;
      }
    }
  } else {
    sscanf(argv[3],"%f",&step);
    sscanf(argv[4],"%ld",&G_Status.time_seed);
    sscanf(argv[5],"%ld",&G_Status.mach_seed);
    
    Lisa_Vector<int> zeg(m), mg(m);
    for (j=0; j<n; j++) {
      for (i=0; i<m; i++) {
	zeg[i] = lisa_random( (long) min, (long) max, &G_Status.time_seed);
	mg[i]=i;
      }
      for (i=0; i<m; i++) {
	u = lisa_random(i+1, m, &G_Status.mach_seed) -1 ;
	temp = mg[i]; mg[i]=mg[u]; mg[u]=temp;
      }
      for (i=0; i<m; i++) 
	if (  (*G_Values.SIJ)[j][ mg[i] ])
	  (*G_Values.PT)[j][ mg[i] ]= zeg[i];
    }
  }
  //new_values();
  show_output();
  return TCL_OK; 
}

//**************************************************************************

// generate randomly column with parameters min,max,step,seed1,name
int TC_gen_column(ClientData /* clientData */,
	     Tcl_Interp * /*interp*/,
	     int /*argc*/, TCL_HACK_CHAR *argv[])  
{
  float min,max,step;
  int j;
  string name;

  sscanf(argv[1],"%f",&min);
  sscanf(argv[2],"%f",&max);
  sscanf(argv[3],"%f",&step);
  sscanf(argv[4],"%ld",&G_Status.time_seed);
  name = argv[5];
  int temp,n;
  n=G_Values.get_n();
  for (j=0; j<n; j++) {
    temp = lisa_random( (long) min, (long) max, &G_Status.time_seed);
    if (name=="DD") {
      if (G_Values.RD!=NULL) 
	(*G_Values.DD)[j]=temp+(*G_Values.RD)[j];
      else
	(*G_Values.DD)[j]=temp;
    }
    else if (name=="RD") {
      if (G_Values.DD!=NULL) 
	if (max< (*G_Values.DD)[j]) (*G_Values.RD)[j]=temp;
	else (*G_Values.RD)[j]= lisa_random( (long) min, 
					     (long) (*G_Values.DD)[j], 
					     &G_Status.time_seed);
      else (*G_Values.RD)[j]=temp;
    }
    else if (name=="WI") (*G_Values.WI)[j]=temp;
    else cerr << name << " as column name unknown\n";
  } 
  new_values();
  show_output();
  return TCL_OK; 
}

//**************************************************************************

// generate randomly a schedule
int TC_gen_seq(ClientData /* clientData */,
	      Tcl_Interp * /*interp*/,
	      int /*argc*/, TCL_HACK_CHAR **/*argv[]*/)  
{
  int m,n;

  for(n=0;n<G_Values.get_n();n++)
    for(m=0;m<G_Values.get_m();m++) {
      if( (*G_Values.SIJ)[n][m]) 
	(*G_Schedule->LR)[n][m]=
	  lisa_random(1,G_Values.get_n()*G_Values.get_m(),&(G_Status.seed)); 
    }
  update_LR();
  return TCL_OK; 
}

//**************************************************************************

// generate randomly operation set
int TC_gen_sij(ClientData /* clientData */,
	      Tcl_Interp * /*interp*/,
	      int /*argc*/, TCL_HACK_CHAR *argv[])  
{
  float probab=0;
  int m,n,i,j;

  sscanf(argv[1],"%f",&probab);
  sscanf(argv[2],"%ld",&G_Status.time_seed);
  m=G_Values.get_m(); n=G_Values.get_n();

  for (j=0; j<n; j++) {
      for (i=0; i<m; i++) {
	if (lisa_random( (long) 0, (long) 100, &G_Status.time_seed)<=probab )
	  	(*G_Values.SIJ)[j][i]=1;
	else {
	  (*G_Values.SIJ)[j][i]=0;
	   (*G_Values.PT)[j][i]=0;
	}
      }
  }
 no_schedule();
  show_output();
  return TCL_OK; 
}

//**************************************************************************

// generate randomly machine order
int TC_gen_mo(ClientData /* clientData */,
	      Tcl_Interp * interp,
	      int /*argc*/, TCL_HACK_CHAR *argv[])  
{
  int m,n,i,j,no_of_machines,k,index;

  m=G_Values.get_m(); n=G_Values.get_n();
  G_TclVar.make_MO(G_Values.get_n(),G_Values.get_m());
  G_TclVar.MO_as_Matrix->fill(0);

  for (j=0; j<n; j++) {
    no_of_machines=0;
      for (i=0; i<m; i++) {
	  if( (*G_Values.SIJ)[j][i])
	     no_of_machines++;
      }
      for (k=1; k<=no_of_machines; k++) {
	for (index=lisa_random( (long) 0, (long) m, &G_Status.time_seed); index <m;index++)
	  if ((*G_Values.SIJ)[j][index]>0 &&  (*G_TclVar.MO_as_Matrix)[j][index]==0 )
	    break;
	if ( index >=m ) {
	  for (index=0; index <m;index++)
	      if ((*G_Values.SIJ)[j][index]>0 &&  (*G_TclVar.MO_as_Matrix)[j][index]==0 )
	    break;
	}
	(*G_TclVar.MO_as_Matrix)[j][index]=k;
      }
  }
  if (G_Values.MO==NULL) {G_Values.make_MO();}
  G_Values.MO->read_rank(G_TclVar.MO_as_Matrix);
  new_values();
 //  Tcl_Eval(interp,"TC_draw_dat");
//   show_output();
  return TCL_OK; 
}

//**************************************************************************

// reverse a schedule
int TC_rev_seq(ClientData /* clientData */,
	      Tcl_Interp */*interp*/,
	      int /*argc*/, TCL_HACK_CHAR **/*argv[]*/)  
{
  int ni,mi;
  int max=G_Values.get_n()*G_Values.get_m();
  for(ni=0;ni<G_Values.get_n();ni++)
    for(mi=0;mi<G_Values.get_m();mi++)
      if ( (*G_Values.SIJ)[ni][mi] )  (*G_Schedule->LR)[ni][mi]= max-(*G_Schedule->LR)[ni][mi]+1;
  update_LR();
  return TCL_OK; 
}

//**************************************************************************

// update a schedule
int TC_update_seq(ClientData /* clientData */,
	      Tcl_Interp * /*interp*/ ,
	      int /*argc*/, TCL_HACK_CHAR **/*argv[]*/)  
{
  update_LR();
  return TCL_OK; 
}

//**************************************************************************

/// update  C-matrix
int TC_update_cij(ClientData /* clientData */,
		  Tcl_Interp * /*interp*/,
		  int /*argc*/, TCL_HACK_CHAR **/*argv[]*/) {
  Lisa_OsProblem myOsProblem(&G_Values);
  Lisa_OsSchedule myOsSchedule(&myOsProblem);
  myOsSchedule.ComputeHeadsTails(true,true);
  if (myOsSchedule.read_Cij(G_Schedule->CIJ)!=OK) {
    G_ExceptionList.lthrow("Cij-Matrix not valid",Lisa_ExceptionList::ANY_ERROR);
  }
 return TCL_OK;
}

//**************************************************************************

// refresh the output
int TC_draw_output(ClientData /* clientData */,
	      Tcl_Interp *interp,
	      int /*argc*/, TCL_HACK_CHAR **/*argv[]*/)  
{
  if ( ( (string) Tcl_GetVar2(interp,"mw","mwout",0)=="Sequence Graph" ||
	 (string) Tcl_GetVar2(interp,"mw","mwout",0)=="Gantt Diagram")  &&
       (G_Schedule->valid) )
    Tcl_Eval(interp, "activate_zoom");
  else  Tcl_Eval(interp, "disable_zoom");
									
  show_output();
  return TCL_OK; 
}

//**************************************************************************

// Draw Value Window
int TC_draw_dat(ClientData /* clientData */,
	       Tcl_Interp *interp,
	       int /*argc*/, TCL_HACK_CHAR **/*argv[]*/)  {
  TCValues myValues(interp,VW_MAINCANV,VW_HORICANV,VW_VERTCANV,VW_LABEL);
  Lisa_Matrix<int> *myMO=NULL;
  Lisa_Matrix<TIMETYP> *myPT=NULL;
  Lisa_Matrix<bool> *mySIJ=NULL;
  Lisa_Vector<TIMETYP> *myRD=NULL, *myDD=NULL; 
  Lisa_Vector<double> * myWI=NULL;  
  string str;
  
  str = Tcl_GetVar2(interp,"dat","Matrix",TCL_GLOBAL_ONLY); 
  mySIJ=G_Values.SIJ;
  if (str=="MO") {
    if (G_TclVar.MO_as_Matrix==NULL) 
      G_TclVar.make_MO(G_Values.get_n(),G_Values.get_m());
    myMO=G_TclVar.MO_as_Matrix;
  }
  else if (str=="PIJ") myPT=G_Values.PT;
  else if (str=="PrecGraph") { cerr << "PrecGraph output not implemented\n";return TCL_OK;}
  str  = Tcl_GetVar2(interp,"dat","RI",TCL_GLOBAL_ONLY);
  if (str=="1") myRD=G_Values.RD; 
  str  = Tcl_GetVar2(interp,"dat","DI",TCL_GLOBAL_ONLY);
  if (str=="1") myDD=G_Values.DD; 
  str  = Tcl_GetVar2(interp,"dat","WI",TCL_GLOBAL_ONLY);
  if (str=="1") myWI=G_Values.WI; 
  myValues.draw(myMO,myPT,mySIJ,myRD,myDD,myWI);
  return TCL_OK; 
}

//**************************************************************************

/// Update an entry in the value Window
int TC_draw_dat_entry(ClientData /* clientData */,
		      Tcl_Interp *interp,
		      int argc, TCL_HACK_CHAR *argv[]) {
  TCValues myValues(interp,VW_MAINCANV,VW_HORICANV,VW_VERTCANV,VW_LABEL);
  int xpos=0,ypos=0;
  string value;
  if (argc<4) {return TCL_OK;}
  sscanf(argv[2],"%d",&xpos);
  sscanf(argv[1],"%d",&ypos);
  value=argv[3];
  myValues.draw_entry(xpos,ypos,value);
  return TCL_OK; 
}

//**************************************************************************

// Mark an entry in the Value Window
int TC_mark_value_entry(ClientData /* clientData */,
			Tcl_Interp *interp,
			int /*argc*/, TCL_HACK_CHAR *argv[])  {
  int row=0,column=0;
  TCValues myValues(interp,VW_MAINCANV,VW_HORICANV,VW_VERTCANV,VW_LABEL);
  sscanf(argv[1],"%d",&row);
  sscanf(argv[2],"%d",&column);
  myValues.mark(row,column);
  return TCL_OK; 
}

//**************************************************************************

// Draw Schedule Window
int TC_draw_schedule(ClientData /* clientData */,
	       Tcl_Interp *interp,
	       int /*argc*/, TCL_HACK_CHAR **/*argv[]*/)  {
    TCSchedule mySchedule(interp,SW_MAINCANV,SW_HORICANV,SW_VERTCANV,SW_LABEL);
    string str = Tcl_GetVar2(interp,"schedule","Matrix",TCL_GLOBAL_ONLY); 
    Lisa_Matrix<TIMETYP> *myCIJ=NULL;
    Lisa_MO *myMO=NULL;
    Lisa_Matrix<int> *myLR=NULL;
    Lisa_JO *myJO=NULL;
    if (str=="LR")   {
      G_Schedule->make_LR();
      myLR=G_Schedule->LR;
    }
    else if (str=="SCHEDULE") myCIJ=G_Schedule->CIJ;
    else return TCL_OK; 
    mySchedule.draw(myMO,myLR,myJO,myCIJ,G_Values.SIJ);
    return TCL_OK; 
}

//**************************************************************************

// Draw List of Schedules
int TC_draw_schedule_list(ClientData /* clientData */,
	       Tcl_Interp *interp,
	       int /*argc*/, TCL_HACK_CHAR **/*argv[]*/)  {
  int no_schedules=G_ScheduleList->length();
  if (no_schedules>=1) {
    Lisa_ScheduleNode myLisa_ScheduleNode;
    int j=0;
    double objective=0;
    int weights_exists=0,duedates_exists=0, objective_name=0;
    
    objective_name=G_ProblemType.get_property(OBJECTIVE);
    if (objective_name==SUM_WICI||objective_name==SUM_WIUI||objective_name==SUM_WITI) 
      weights_exists=1;
    if (objective_name==LMAX||objective_name==SUM_UI||
	objective_name==SUM_WIUI||objective_name==SUM_TI||
	objective_name==SUM_WITI)
      duedates_exists=1;
    G_ScheduleList->reset();
    for(j=0;j<no_schedules;j++) {
      while (G_ExceptionList.empty()==0) print_error();
      Lisa_OsProblem  myOsProblem(&G_Values);
      Lisa_OsSchedule  myOsSchedule(&myOsProblem);
      G_ExceptionList.lcatch(Lisa_ExceptionList::INCONSISTENT_INPUT);
      //myLisa_ScheduleNode=G_ScheduleList->get();
      G_Schedule=G_ScheduleList->get().actual_schedule;
      (*G_ScheduleList->get().schedule_info)[0]=j+1;
      // CMAX-Objective:
      G_ProblemType.set_property(OBJECTIVE,CMAX);
      objective=update_objective(myOsSchedule);
      (*G_ScheduleList->get().schedule_info)[CMAX]= (int) objective;
      // SUM_CI-Objective:
      G_ProblemType.set_property(OBJECTIVE,SUM_CI);
      objective=update_objective(myOsSchedule);
      (*G_ScheduleList->get().schedule_info)[SUM_CI]=(int) objective;
      if (weights_exists) {
	// SUM_WICI-Objective:
	G_ProblemType.set_property(OBJECTIVE,SUM_WICI);
	objective=update_objective(myOsSchedule);
	(*G_ScheduleList->get().schedule_info)[SUM_WICI]=(int) objective;
      }
      if (duedates_exists) {
	// LMAX-Objective
	G_ProblemType.set_property(OBJECTIVE,LMAX);
	objective=update_objective(myOsSchedule);
	(*G_ScheduleList->get().schedule_info)[LMAX]=(int) objective;
	// SUM_UI-Objective
	G_ProblemType.set_property(OBJECTIVE,SUM_UI);
	objective=update_objective(myOsSchedule);
	(*G_ScheduleList->get().schedule_info)[SUM_UI]=(int) objective;
	// SUM_TI-Objective
	G_ProblemType.set_property(OBJECTIVE,SUM_TI);
	objective=update_objective(myOsSchedule);
	(*G_ScheduleList->get().schedule_info)[SUM_TI]=(int) objective;
	if (weights_exists) {
	  // SUM_WIUI-Objective
	  G_ProblemType.set_property(OBJECTIVE,SUM_WIUI);
	  objective=update_objective(myOsSchedule);
	  (*G_ScheduleList->get().schedule_info)[SUM_WIUI]=(int) objective;
	  // SUM_WITI-Objective
	  G_ProblemType.set_property(OBJECTIVE,SUM_WITI);
	  objective=update_objective(myOsSchedule);
	  (*G_ScheduleList->get().schedule_info)[SUM_WITI]=(int) objective;
	}
      }
	  (*G_ScheduleList->get().schedule_info)[MAXRANK_INFO ]=G_Schedule->get_property(MAXRANK_INFO);
	  (*G_ScheduleList->get().schedule_info)[NO_OF_SOURCES]= G_Schedule->get_property(NO_OF_SOURCES);
	  (*G_ScheduleList->get().schedule_info)[NO_OF_SINKS]=G_Schedule->get_property(NO_OF_SINKS);
	  (*G_ScheduleList->get().schedule_info)[NO_OF_IKL]=(int) 4;
	  //cout << "no sources:" << (*G_ScheduleList->get().schedule_info)[NO_OF_SOURCES]<< endl;
	  G_ScheduleList->next();
    }
    
    G_ProblemType.set_property(OBJECTIVE,objective_name);
    
    TCScheduleList myScheduleList(interp,SLW_MAINCANV,SLW_HORICANV,SLW_VERTCANV,SLW_LABEL);
    myScheduleList.draw(G_ScheduleList);
  }
  return TCL_OK; 
}

//**************************************************************************

// Update an entry in the schedule Window
int TC_draw_schedule_entry(ClientData /* clientData */,
		      Tcl_Interp *interp,
		      int argc, TCL_HACK_CHAR *argv[]) {
  TCSchedule mySchedule(interp,SW_MAINCANV,SW_HORICANV,SW_VERTCANV,SW_LABEL);
  int xpos=0,ypos=0;
  string value;
  if (argc<4) {return TCL_OK;}
  sscanf(argv[2],"%d",&xpos);
  sscanf(argv[1],"%d",&ypos);
  value=argv[3];
  mySchedule.draw_entry(xpos,ypos,value);
  return TCL_OK; 
}

// If new ProblemType defined
int TC_new_problem(ClientData /* clientData */,
		   Tcl_Interp * /*interp*/,
		   int /*argc*/, TCL_HACK_CHAR **/*argv[]*/) {
  new_problemtype();
  return TCL_OK; 
}

//**************************************************************************

// If new Lisa_Values defined
int TC_new_values(ClientData /* clientData */,
		  Tcl_Interp * /*interp*/,
		  int /*argc*/, TCL_HACK_CHAR **/*argv[]*/) {
  new_values();
  return TCL_OK; 
}

//**************************************************************************

// Mark an entry in the schedule window
int TC_mark_schedule_entry(ClientData /* clientData */,
			   Tcl_Interp *interp,
			   int /*argc*/, TCL_HACK_CHAR *argv[])  {
  int row=0,column=0;
  TCSchedule mySchedule(interp,SW_MAINCANV,SW_HORICANV,SW_VERTCANV,SW_LABEL);
  sscanf(argv[1],"%d",&row);
  sscanf(argv[2],"%d",&column);
  mySchedule.mark(row,column);
  return TCL_OK; 
}

//**************************************************************************

int TC_exit(ClientData /* clientData */,
		Tcl_Interp * /*interp*/,
		int /*argc*/, TCL_HACK_CHAR **/*argv[]*/) {  
  G_TclVar.alive=0;
  return TCL_OK;
}

//**************************************************************************

/// Problem Classification
int TC_classify(ClientData /* clientData */,
	    Tcl_Interp * interp ,
		int /*argc*/, TCL_HACK_CHAR **/*argv[]*/) {
  Tcl_Eval(interp, "Window show .textaus");
  mw_set_label ("$Name(Classification");
  Lisa_TextObject mytext(interp);
  mytext.clear();
  std::string str;
  if(G_Classify != 0) str=G_Classify->classify(G_ProblemType);
  mytext.text(str.c_str());
  return TCL_OK; 
}

//**************************************************************************

int TC_references(ClientData /* clientData */,
	    Tcl_Interp * interp ,
		int /*argc*/, TCL_HACK_CHAR **/*argv[]*/) {
  
  std::string str;
  if(G_Classify != 0) str=G_Classify->full_reference(G_ProblemType);
  if (str!="") {
    std::string str2="lisa_text  $Name(References) {" + str + "}" ;
    Tcl_Eval(interp, (char *) str2.c_str());
  }else{
    G_ExceptionList.lthrow("No references found",Lisa_ExceptionList::ANY_ERROR);
  }
  return TCL_OK; 
}

//**************************************************************************

/// If an error in the Tk- Application occour
int TC_error(ClientData /* clientData */,
	           Tcl_Interp * /*interp*/,
		         int /*argc*/, TCL_HACK_CHAR *argv[]) {
               
  G_ExceptionList.lthrow(argv[1],Lisa_ExceptionList::ANY_ERROR);
  return TCL_OK; 
}

//**************************************************************************

/// Delete all global objects
int TC_new(ClientData /* clientData */,
	   Tcl_Interp * /*interp*/,
	   int /*argc*/, TCL_HACK_CHAR **/*argv[]*/) {
  int i;
  for(i=0;i<TUPEL_INDEX;i++) G_ProblemType.set_property(i,EMPTY);
  //  G_ProblemType.valid=false;
   G_Values.init(0,0);
 //   G_Schedule->init(0,0);
   // no_schedule();
   show_output();
   return TCL_OK; 
}

//**************************************************************************

/// write G_Schedule.LR in G_TCLVar.LR
int  TC_save_old_sequence(ClientData /* clientData */,
	   Tcl_Interp * /*interp*/,
	   int /*argc*/, TCL_HACK_CHAR ** /*argv[]*/) {
  G_TclVar.make_LR(G_Values.get_n(),G_Values.get_m());
  *(G_TclVar.LR)=*(G_Schedule->LR);
 return TCL_OK; 
}

//**************************************************************************

/// move operation
int TC_move_operation(ClientData /* clientData */,
	   Tcl_Interp * interp,
	   int /*argc*/, TCL_HACK_CHAR *argv[]) {
  string type="";
  int return_value=ERROR;
  int row=0,column=0;
  int JOsucc=0,MOsucc=0,JOpred=0,MOpred=0;
  type=argv[1];
  if (type=="reset") {
    *(G_Schedule->LR)=*(G_TclVar.LR);
    new_schedule();
return TCL_OK;
  }

  sscanf(argv[2],"%d",&row);
  sscanf(argv[3],"%d",&column);
  
  if (G_Schedule->valid) {
    row++;column++;
    while (G_ExceptionList.empty()==0) print_error();
    Lisa_OsProblem  myOsProblem(&G_Values);
    Lisa_OsSchedule  myOsSchedule(&myOsProblem);
    G_ExceptionList.lcatch(Lisa_ExceptionList::INCONSISTENT_INPUT);
    myOsSchedule.read_LR(G_Schedule->LR);

    JOsucc=myOsSchedule.GetJOsucc(row,column);
    MOsucc=myOsSchedule.GetMOsucc(row,column);
    JOpred=myOsSchedule.GetJOpred(row,column);
    MOpred=myOsSchedule.GetMOpred(row,column);

    G_TclVar.JOsucc= JOsucc;
    G_TclVar.MOsucc= MOsucc;
    G_TclVar.JOpred= JOpred;
    G_TclVar.MOpred= MOpred; 
    myOsSchedule.exclude(row,column);
    if (type=="right") {
      if (MOsucc!=SINK)
      return_value=myOsSchedule.insert(row,column,JOpred,MOsucc);
    } else if (type=="rright") {
      return_value=myOsSchedule.insert(row,column,JOpred,
				       myOsSchedule.GetMOpred(row,SINK));
    } else if (type=="left") {
      if ( MOpred!=SOURCE)
	return_value=myOsSchedule.insert(row,column,JOpred,
					 myOsSchedule.GetMOpred(row,MOpred));
    } else if (type=="lleft") {
      return_value=myOsSchedule.insert(row,column,JOpred,SOURCE);
    } else if (type=="top") {
      if (JOsucc!=SINK)
	return_value=myOsSchedule.insert(row,column,JOsucc,MOpred);
    } else if (type=="ttop") {
      return_value=myOsSchedule.insert(row,column,
				       myOsSchedule.GetJOpred(SINK,column),MOpred);
    } else if (type=="buttom") {
      if (JOpred!=SOURCE)
	return_value=myOsSchedule.insert(row,column,
					 myOsSchedule.GetJOpred(JOpred,column),
					 MOpred);
    }else if (type=="bbuttom") {
      return_value=myOsSchedule.insert(row,column,SOURCE,MOpred);
    } else if (type=="source") {
      return_value=myOsSchedule.insert(row,column,SOURCE,SOURCE);
    } else if (type=="sink") {
      return_value=myOsSchedule.insert(row,column,
				       myOsSchedule.GetJOpred(SINK,column),
				       myOsSchedule.GetMOpred(row,SINK));
    } else return TCL_OK;
    
    if (return_value==CYCLE) {
      G_ExceptionList.lthrow("cycle: modification not possible",
                             Lisa_ExceptionList::ANY_ERROR);
      return TCL_OK; 
    }
    if (return_value==ERROR) {
      return TCL_OK; 
    }
    myOsSchedule.write_LR(G_Schedule->LR);
    new_schedule();
  }
  string zoomflag=Tcl_GetVar2(interp,"mw","zoom",TCL_GLOBAL_ONLY);
  string mw_output= Tcl_GetVar2(interp,"mw","mwout",TCL_GLOBAL_ONLY);
  if (mw_output=="Gantt Diagram") {
    TCGantt myTCGantt(interp,MW_MAINCANV, MW_HORICANV,MW_VERTCANV);
    myTCGantt.zoom=G_Status.zoom;
    myTCGantt.set_wh((int) G_Status.width,(int) G_Status.height);
    myTCGantt.mark(column-1,row-1,&G_Values,G_Schedule,G_Preferences.get_long(Lisa_Pref::GANTT_ORIENT));
  }
  Tcl_Eval(interp, "man_oper_update");
  return TCL_OK; 
}

//**************************************************************************

/// Choose new Sequence
int TC_choose_sequence(ClientData /* clientData */,
		       Tcl_Interp * /*interp*/,
		       int argc, TCL_HACK_CHAR *argv[]) {
   if (argc>1) {
     int seq_no=0;
     sscanf(argv[1],"%d",&seq_no);
     if(seq_no<G_ScheduleList->length()&& seq_no>=0) {
       G_Schedule=(*G_ScheduleList)[seq_no].actual_schedule;
       new_schedule();
     }
  }
    return TCL_OK; 
}

//**************************************************************************

/// Sort list of sequences
int TC_sort_sl(ClientData /* clientData */,
	       Tcl_Interp * interp,
	       int argc, TCL_HACK_CHAR *argv[]) {
  int obj_name=0;
  
  if (argc>1) {
    string str;
    str=argv[1];
    obj_name=stoz(str);
    G_ScheduleList->pop().sinfo_pointer=obj_name;
    G_ScheduleList->sort();
  }
  /*
    Lisa_Node * myLisa_Node=NULL;
    Lisa_ScheduleNode * myLisa_ScheduleNode=NULL;
    if (str=="Objective") {
      myLisa_Node=G_Schedule->ScheduleList->first();
      myLisa_ScheduleNode=(Lisa_ScheduleNode *) myLisa_Node;
      myLisa_ScheduleNode->sinfo_pointer=1;
    }
    G_Schedule->ScheduleList->isort();
  }
  */
  Tcl_Eval(interp, "TC_draw_schedule_list");
  return TCL_OK; 
}

//**************************************************************************

/// starts external algorithm
int TC_startalg(ClientData /* clientData */,
		Tcl_Interp *interp,
		int /*argc*/, TCL_HACK_CHAR *argv[]) {

  
  string name_of_algo,str="",type,name_of_parameter;
  int i=0,n=0;
  long int long_var=0;
  double double_var=0;
  string string_var="";
  string code="";
  Lisa_ControlParameters myctrlpara;
  name_of_algo=argv[1];
    sscanf(Tcl_GetVar2(interp,(char*) name_of_algo.c_str(),"NO_OF_ENTRYS",TCL_GLOBAL_ONLY),"%d",&n);
    for (i=1;i<=n;i++) {
    str= ztos(i) + ",TYPE";
    type=Tcl_GetVar2(interp,(char*) name_of_algo.c_str(),(char*) str.c_str(),TCL_GLOBAL_ONLY);
    str= ztos(i) + ",NAME_OF_PARAMETER";
    name_of_parameter=Tcl_GetVar2(interp,(char*) name_of_algo.c_str(),(char*) str.c_str(),TCL_GLOBAL_ONLY);
    if (type=="double") {
      sscanf(Tcl_GetVar2(interp,(char*) name_of_algo.c_str(),
			 (char*)name_of_parameter.c_str(),TCL_GLOBAL_ONLY),"%lf",&double_var);
      myctrlpara.add_key(name_of_parameter,double_var);
    }
    if (type=="string") {
      string_var=Tcl_GetVar2(interp,(char*) name_of_algo.c_str(),
			     (char*)name_of_parameter.c_str(),TCL_GLOBAL_ONLY);
      myctrlpara.add_key(name_of_parameter,string_var);
    }
   if (type=="long") {
      sscanf(Tcl_GetVar2(interp,(char*) name_of_algo.c_str(),
			 (char*)name_of_parameter.c_str(),TCL_GLOBAL_ONLY),"%ld",&long_var);
      myctrlpara.add_key(name_of_parameter,long_var);
    }
  }
    string algo_call="";
  algo_call=Tcl_GetVar2(interp,(char*) name_of_algo.c_str(),"ALGO_CALL",TCL_GLOBAL_ONLY);
  code=Tcl_GetVar2(interp,(char*) name_of_algo.c_str(),"CODE",TCL_GLOBAL_ONLY);
  if (code=="external") {
    start_ext_algo(interp,name_of_algo,algo_call, G_Preferences.get_string(Lisa_Pref::LISACONFIG)+"/proc/" + DEFAULT_IN_FILE,
																			G_Preferences.get_string(Lisa_Pref::LISACONFIG)+"/proc/" + DEFAULT_OUT_FILE,
																			G_Preferences,G_ProblemType,myctrlpara,*G_Schedule,G_Values);
  } else {
    G_ExceptionList.lthrow( (string) "wrong code:" + code + "in description file external or internal expected (assume external)" ,Lisa_ExceptionList::SYNTAX_ERROR);
    start_ext_algo(interp,name_of_algo,algo_call, G_Preferences.get_string(Lisa_Pref::LISACONFIG)+"/proc/" + DEFAULT_IN_FILE,
																			G_Preferences.get_string(Lisa_Pref::LISACONFIG)+"/proc/" + DEFAULT_IN_FILE,
																			G_Preferences,G_ProblemType,myctrlpara,*G_Schedule,G_Values);
  } 

  return TCL_OK;
}

//**************************************************************************

int TC_problem_reduction(ClientData, // clientData
Tcl_Interp *interp,
int, //argc 
TCL_HACK_CHAR *argv[]){
  
  std::vector<Lisa_ProblemType> Solved;
  
  // set return value to false
  sprintf(interp->result, "%d",0);
  //nothing appropriate found ???
  
  //if(get_problemList(argv[2],argv[1],Solved))
  //return TCL_OK;
  
  if(get_problemList_xml(argv[2],argv[1],Solved))
    return TCL_OK;
  
  
  unsigned num_of_solv_probl = Solved.size();
  for (unsigned j=0;j<num_of_solv_probl;j++)
  {
    // check if problemtypes match exactly
    if(G_ProblemType.output_alpha()==Solved[j].output_alpha() && 
       G_ProblemType.output_beta()==Solved[j].output_beta() && 
       ( Solved[j].get_property(OBJECTIVE) == ANY_OBJECTIVE || G_ProblemType.output_gamma()==Solved[j].output_gamma()))
      {
	sprintf(interp->result, "%d",1);
	return TCL_OK;
	// reduction when algo provides O, current problem is O2 and the like   
      }
    else if(G_ProblemType.output_alpha().find(Solved[j].output_alpha()) != string::npos &&
	    G_ProblemType.output_beta()== Solved[j].output_beta() &&
	    ( Solved[j].get_property(OBJECTIVE) == ANY_OBJECTIVE || G_ProblemType.output_gamma()==Solved[j].output_gamma()))
      {
	sprintf(interp->result, "%d",1);
	return TCL_OK;
      }
    else if(G_ProblemType.output_alpha()==Solved[j].output_alpha() &&
	    ( Solved[j].get_property(OBJECTIVE) == ANY_OBJECTIVE || G_ProblemType.output_gamma()==Solved[j].output_gamma()))
      {
	
        // see if there are unit processing times to handle
        if ((Solved[j].output_beta()+"p_ij=1" == G_ProblemType.output_beta()) ||
	    (G_ProblemType.output_beta() == Solved[j].output_beta()+"; p_ij=1"))
          {
            sprintf(interp->result, "%d",1);
            return TCL_OK;
          }
          
	// see if there are constant processing times to handle : p_ij=p
	if (( Solved[j].output_beta()+ "p_ij=p" == G_ProblemType.output_beta()) ||
            (G_ProblemType.output_beta() == Solved[j].output_beta()+"; p_ij=p")  )
	  {
              sprintf(interp->result, "%d",1);
              return TCL_OK;
	  }              
      }
    
  }  
  return TCL_OK; 
}

//**************************************************************************

/// exclude sequences from list
int TC_exclude(ClientData /* clientData */, Tcl_Interp */*interp*/,
		           int argc, TCL_HACK_CHAR *argv[]) {

  if(argc<2){
    G_ExceptionList.lthrow("Wrong number of arguments in TC_exclude",
                           Lisa_ExceptionList::TCLTK_ERROR);
    return TCL_OK;  
  }
  
  string str= argv[1];
  
  if (str=="clear_list") {
    
    //Lisa_Schedule * my_schedule = new Lisa_Schedule(*G_Schedule);
    // G_ScheduleList->clear(); 
    
    while(!(G_ScheduleList->empty())){
      Lisa_ScheduleNode dummynode=G_ScheduleList->pop();
      if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
    }
    
    // G_Schedule=my_schedule;
    Lisa_ScheduleNode *myLisa_ScheduleNode= new Lisa_ScheduleNode(G_Schedule);
    G_ScheduleList->append(*myLisa_ScheduleNode);
    
    delete myLisa_ScheduleNode;
    delete G_XSchedule;
    
    G_XSchedule = new Lisa_XSchedule(G_Schedule);
  }
  return TCL_OK; 
}

//**************************************************************************

/// save G_Preferences in default.lsa
int TC_save_options(ClientData,	Tcl_Interp *, int , TCL_HACK_CHAR **) 
{
  string str=G_Preferences.get_string(Lisa_Pref::LISACONFIG)+"/" + DEFAULT_SETTINGS_FILE;
  
  LisaXmlFile out(LisaXmlFile::PREFERENCES);
  out << G_Preferences;
  if(!out || !out.write(str))
  {
    G_ExceptionList.lthrow("cannot write file: "+str+"\n",Lisa_ExceptionList::END_OF_FILE);
    return 1;
  }
  
  return TCL_OK; 
}


//**************************************************************************

///  start Enumeration Algorithm
// int TC_cenum(ClientData,	Tcl_Interp *, int , TCL_HACK_CHAR **) {
 
//  cenum(&G_Values);
//  return TCL_OK; 
// }

//**************************************************************************

///send <ctrl-C> Signal to external Algorithm
int TC_stop_extalg(ClientData,	Tcl_Interp *, int , TCL_HACK_CHAR **) {

string str;
str ="dir";
string str2= " ";
 execl(str.c_str(),str2.c_str(),(char*) NULL);
//  cenum(&G_Values);
  return TCL_OK; 
}

//**************************************************************************

// a TCL wrapper for the UNIX kill command
int TC_kill(ClientData /* clientData */,
	 Tcl_Interp * /*interp*/,
	 int /*argc*/, TCL_HACK_CHAR *argv[])  
{
  // two parameters:
  // 1.parameter  signal to send
  // 2.parameter  pid to send signal to
  int signal;
  int pid;
  sscanf(argv[1],"-%d",&signal);
  sscanf(argv[2],"%d",&pid);
  kill(pid, signal);
  return TCL_OK;
}

//**************************************************************************

