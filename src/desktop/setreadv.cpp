/**                   
 * @author  Per Willenius
 * @version 2.3rc1
 */ 

#include <string>

#include "../misc/int2str.hpp"
#include "../misc/except.hpp"
#include "../main/status.hpp"
#include "../main/global.hpp"
#include "../main/lisapref.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/xsched.hpp"
#include "../scheduling/schedule.hpp"
#include "../scheduling/os_sched.hpp"
#include "../classify/classify.hpp"

#include "tcl_c.hpp"
#include "gantt.hpp"
#include "c_tcl.hpp"
#include "tclvar.hpp"
#include "callback.hpp"

using namespace std;

//**************************************************************************

extern class Lisa_ProblemType G_ProblemType;
extern class Lisa_Values G_Values;
extern class Lisa_Schedule *G_Schedule;
extern class Lisa_XSchedule *G_XSchedule;
extern class Lisa_Preferences G_Preferences;
extern class Lisa_TCLVar G_TclVar;
extern class Lisa_Canvas *G_MWCanvas;
extern class Lisa_Status G_Status;
extern class Lisa_List<Lisa_ScheduleNode> *G_ScheduleList;

//**************************************************************************

int TC_set_p(ClientData /* clientData */,
	 Tcl_Interp * /* interp*/,
	 int /* argc*/, TCL_HACK_CHAR *argv[])  
{
  string ms,ns,ws;
  int m,n,value;
  
  sscanf(argv[1],"%d",&n);
  sscanf(argv[2],"%d",&m);
  sscanf(argv[3],"%d",&value);
  (*G_Schedule->LR)[n][m]=value;
  show_output();
  return TCL_OK;
}

//**************************************************************************

int TC_get_p(ClientData /* clientData */,
	 Tcl_Interp *interp,
	 int /*argc*/, TCL_HACK_CHAR *argv[])  
{
int n,m;

 sscanf(argv[1],"%d",&n);
 sscanf(argv[2],"%d",&m);
 sprintf(interp->result, "%d", (*G_Schedule->LR)[n][m]);
  return TCL_OK; 
}

//**************************************************************************

int TC_set_prob(ClientData /* clientData */,
	       Tcl_Interp *interp,
	       int /*argc*/, TCL_HACK_CHAR ** /*argv[]*/)  
{
  const char *ms,*ns;
  int m=0,n=0,mold,nold;
  
  ms = Tcl_GetVar2(interp,"glob","machines",TCL_GLOBAL_ONLY);
  ns = Tcl_GetVar2(interp,"glob","jobs",TCL_GLOBAL_ONLY);
  
  if(ms != 0) sscanf(ms,"%d",&m);
  if(ns != 0) sscanf(ns,"%d",&n);
  
  mold=G_Values.get_m();
  nold=G_Values.get_n();
  
  if(mold!=m||nold!=n) {
    G_Values.init(n,m);
    new_mn();
  }
  
  return TCL_OK;
}

//**************************************************************************

// set processing times
int TC_set_pt(ClientData /* clientData */,
	     Tcl_Interp *interp,
	     int /*argc*/, TCL_HACK_CHAR *argv[])  
{
  string ms,ns,ws;
  int m,n;
  float value;
  
  sscanf(argv[1],"%d",&n);
  sscanf(argv[2],"%d",&m);
  sscanf(argv[3],"%f",&value);
  (*G_Values.PT)[n][m]=value;
  if ((*G_Values.SIJ)[n][m]==FALSE) {
    if (value>0) {
      (*G_Values.SIJ)[n][m]=1;
      Tcl_Eval(interp,"TC_draw_dat");
    }
  }
  show_output();
  return TCL_OK;
}

//**************************************************************************

// read processing time
int TC_get_pt(ClientData /* clientData */,
	     Tcl_Interp *interp,
	     int /*argc*/, TCL_HACK_CHAR *argv[])  
{
  int n,m;
  sscanf(argv[1],"%d",&n);
  sscanf(argv[2],"%d",&m);
  if (G_Values.PT) {
    if(inttst( (*G_Values.PT)[n][m])) sprintf(interp->result, "%.0f", (*G_Values.PT)[n][m]);
    else sprintf(interp->result, "%f", (*G_Values.PT)[n][m]);
  } else sprintf(interp->result, "0");
  return TCL_OK; 
}

//**************************************************************************

// set an entry in Lisa_Problen.tupel
int TC_set_Tupel(ClientData /* clientData */,
	 Tcl_Interp * /*interp*/,
	 int /*argc*/, TCL_HACK_CHAR *argv[])  
{
  // two parameters:
  // 1.parameter  number of tupel-entry
  // 2.parameter  the corresponding entry
  string ms,ns,ws;
  int index;
  int entry;
  sscanf(argv[1],"%d",&index);
  sscanf(argv[2],"%d",&entry);
  if (index<M_NO)
    G_ProblemType.set_property(index,entry);
  else if (index==M_NO)
    G_ProblemType.m_no=entry;
  else if (index==N_NO)
    G_ProblemType.n_no=entry;
  else cerr << "error in TC_set_Tupel: entry " << index << "to big\n";
  return TCL_OK;
}

//**************************************************************************

// returns the values of C++ variables
int TC_getvar(ClientData /* clientData */,
	      Tcl_Interp *interp,
	      int argc, TCL_HACK_CHAR *argv[]){
          
  int temp=0,row=0,column=0,n=0,m=0,i=0;
  float xpos=0,ypos=0;
  string name=argv[1],str="NOT_DEFINED",str2="",str3="",str4="";
  
  if (name=="LANGUAGE") {
    str= G_Preferences.get_string("LANGUAGE");
  }
  else if (name=="HTML_VIEWER") {
    str= G_Preferences.get_string("HTML_VIEWER");
  }
  else if (name=="problemtype") {
    str=G_ProblemType.output_problem();
    if (str!="no valid Problem"&&G_Values.get_m()!=0&&G_Values.get_n()!=0) {
      str+="      ";
      if (G_Values.get_m()>1)
	str+=ztos(G_Values.get_m()) + " " + translate("Machines")+"   "; 
      str+=ztos(G_Values.get_n()) +" "+ translate("Jobs");
    }
  }
  else if (name=="mwwidth") sprintf(interp->result, "%d",mw_width() );
  //G_Preferences.get_int("WIDTH"));
  else if (name=="mwheight") sprintf(interp->result, "%d",mw_height() );
  //G_Preferences.get_int("HEIGHT"));
  else if (name=="get_m") sprintf(interp->result, "%d",G_Values.get_m());
  else if (name=="get_n") sprintf(interp->result, "%d",G_Values.get_n());
  else if (name=="get_mn") sprintf(interp->result, "%d",
				   MIN(G_Values.get_n(),G_Values.get_m()));
  else if (name=="alpha") {
    str=G_ProblemType.output_alpha();
      }
  else if (name=="beta") {
    str=G_ProblemType.output_beta();
      }
  else if (name=="gamma") {
    str=G_ProblemType.output_gamma();
  }
  else if (name=="schedule_row") {
    sscanf(argv[2],"%d",&temp);
    sprintf(interp->result, "%ld", (long) temp/ENTRYHEIGHT);
  }
 else if (name=="schedulelist_row") {
    sscanf(argv[2],"%d",&temp);
    sprintf(interp->result, "%ld", (long) temp/ENTRYHEIGHT);
  }
  else if (name=="schedule_column") {
    sscanf(argv[2],"%d",&temp);
    sprintf(interp->result, "%ld", (long) temp/ENTRYWIDTH);
  }
  else if (name=="schedule_identifier") { 
       if (argv[2] ==NULL || argv[3]==NULL) 
      {interp->result="0";return TCL_OK;} 
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    if (row<G_Values.get_n()&& row>=0) {
      if (column<G_Values.get_m()&& column>=0) {// valid row an column
	str = Tcl_GetVar2(interp,"schedule","Matrix",TCL_GLOBAL_ONLY); 
      }
    }
  }
  else if (name=="semiactive_flag") {
    str = ztos(G_Schedule->semiactive);
  } 
  else if (name=="w_row") {
    sscanf(argv[2],"%d",&temp);
    sprintf(interp->result, "%ld", (long) temp/ENTRYHEIGHT);
  }
  else if (name=="w_column") {
    sscanf(argv[2],"%d",&temp);
    sprintf(interp->result, "%ld", (long) temp/ENTRYWIDTH);
  }
  else if (name=="w_identifier") {
    str="";
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    if (row<G_Values.get_n() && row>=0) {
      if (column<G_Values.get_m() && column>=0) {// either PIJ, SIJ or MO
	str = Tcl_GetVar2(interp,"dat","Matrix",TCL_GLOBAL_ONLY); 
      }
      else {// DD, WI RD
	int dd=0,wi=0,rd=0;
	str2  = Tcl_GetVar2(interp,"dat","RI",TCL_GLOBAL_ONLY);
	if (str2=="1") rd=1; 
	str2  = Tcl_GetVar2(interp,"dat","DI",TCL_GLOBAL_ONLY);
	if (str2=="1") dd=1; 
	str2  = Tcl_GetVar2(interp,"dat","WI",TCL_GLOBAL_ONLY);
	if (str2=="1") wi=1;
	if (column == G_Values.get_m()) {// first one 
	  if (rd) str = "RD";
	  else if (dd) str = "DD";
	  else if (wi) str = "WI";
	  	}
	else if(column == G_Values.get_m()+1) {// second one 
	  if (rd) {
	    if (dd) str = "DD";
	    else if (wi)  str = "WI";
	  }
	  else {
	    if (dd+wi==2) str = "WI";
	  }
	}
	else if(column == G_Values.get_m()+2) {// third one
	  if (rd+dd+wi==3) str = "WI";
	}  
      }
    }
  }
  else if (name=="w_entry") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str2 = Tcl_GetVar2(interp,"dat","Matrix",TCL_GLOBAL_ONLY);
    if (row<G_Values.get_n()&& column<G_Values.get_m() && row>=0 && column>=0 ) {
      if (str2=="PIJ") {
	str=ztos((*G_Values.PT)[row][column]);
      }
      if (str2=="SIJ") {
	str=ztos((*G_Values.SIJ)[row][column]);
      }
      if (str2=="MO") {
	if (G_TclVar.MO_as_Matrix!=NULL)
	  str=ztos((*G_TclVar.MO_as_Matrix)[row][column]);
	else str="0";
      }
    }
  }
  else if (name=="Cij") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str=ztos((*G_Schedule->CIJ)[row][column]);
  }
 else if (name=="pij") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str=ztos((*G_Values.PT)[row][column]);
  }
 else if (name=="critical") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    if (G_XSchedule->CP) {
    str=ztos((*G_XSchedule->CP)[row][column]);
    } else str="0";
  }
  else if (name=="schedule_entry") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str2 = Tcl_GetVar2(interp,"schedule","Matrix",TCL_GLOBAL_ONLY);
    if (row<G_Values.get_n()&& column<G_Values.get_m() && row>=0 && column>=0) {
      if (str2=="LR") {
 	str=ztos((*G_Schedule->LR)[row][column]);
      }
      if (str2=="SCHEDULE") {
 	str=ztos((*G_Schedule->CIJ)[row][column]);
      }
      if (str2=="MO") {
	//	puts "MO gefragt" << endl;
	if (G_TclVar.MO_as_Matrix!=NULL)
	  str=ztos((*G_TclVar.MO_as_Matrix)[row][column]);
	else str="0";
	}
      if (str2=="JO") {
	str="0";
      }
    }
  }
  else if (name=="entry_in_ptupel") {
    str3=argv[2];
    str=ztos(G_ProblemType.get_property(stoz(str3)));
  }
  else if (name=="DD") {
    sscanf(argv[2],"%d",&row);
    str=ztos((*G_Values.DD)[row]);
  }
  else if (name=="RD") {
    sscanf(argv[2],"%d",&row);
    str=ztos((*G_Values.RD)[row]);
  }
  else if (name=="WI") {
    sscanf(argv[2],"%d",&row);
    str=ztos((*G_Values.WI)[row]);
  }
  else if (name=="time_seed") {
    str=ztos(G_Status.time_seed);
  }
 else if (name=="mach_seed") {
    str=ztos(G_Status.mach_seed);
  }
   else if (name=="upper_bound") {
     if (G_Schedule->valid) {
       while (G_ExceptionList.empty()==0) print_error();
       Lisa_OsProblem  myOsProblem(&G_Values);
       Lisa_OsSchedule  myOsSchedule(&myOsProblem);
       G_ExceptionList.lcatch(INCONSISTENT_INPUT);
       str=ztos(update_objective(myOsSchedule));
     } else str= "schedule_not_valid";
 }else if (name=="lower_bound"){
    str=" ";
 }else if (name=="job_from_gantt"){
   
   sscanf(argv[2],"%f",&xpos);
   sscanf(argv[3],"%f",&ypos);
   show_output();
   TCGantt myTCGantt(interp,MW_MAINCANV, MW_HORICANV,MW_VERTCANV);
   myTCGantt.zoom=G_Status.zoom;
   myTCGantt.set_wh((int) G_Status.width,(int) G_Status.height);
   operation_data  *mydata;
   mydata=new operation_data;
   double normalx;
   double normaly;
   if (G_Status.zoom) {
     normalx=xpos/G_Status.width*100.;
     normaly=100.-ypos/G_Status.height*100.;
   } else {
     normalx=xpos/mw_width()*100.;
     normaly=100.-ypos/mw_height()*100.;
   }
   mydata=myTCGantt.get_data(normalx,normaly,&G_Values,G_Schedule, G_Preferences.gantt_orient);
   
   str=ztos(mydata->job);
   myTCGantt.mark(mydata->machine,mydata->job,&G_Values,G_Schedule,G_Preferences.gantt_orient);
 }else if (name=="machine_from_gantt") {
    sscanf(argv[2],"%f",&xpos);
    sscanf(argv[3],"%f",&ypos);
    TCGantt myTCGantt(interp,MW_MAINCANV, MW_HORICANV,MW_VERTCANV);
    myTCGantt.zoom=G_Status.zoom;
    myTCGantt.set_wh((int) G_Status.width,(int) G_Status.height);
    operation_data  *mydata;
    mydata=new operation_data;
    
    double normalx;
    double normaly;
    if (G_Status.zoom) {
      normalx=xpos/G_Status.width*100.;
      normaly=100.-ypos/G_Status.height*100.;
    } else {
      normalx=xpos/mw_width()*100.;
      normaly=100.-ypos/mw_height()*100.;
    }
    
    mydata=myTCGantt.get_data(normalx,normaly,&G_Values,G_Schedule, G_Preferences.gantt_orient);
    str=ztos(mydata->machine);
  }
  else if (name=="no_solutions") {
    str=ztos(G_ScheduleList->length());
  }
  else if (name=="PROGRESS_INDICATOR_STEPS") {
    str=ztos(PROGRESS_INDICATOR_STEPS);
  }
  else if (name=="M_ENV") {
    str=ztos(G_ProblemType.get_property(M_ENV));
  }
  else if (name=="ptupel_number") {
    if (argc==3) {
      str2= argv[2];
      if (str2=="alpha") str=ztos(NUMBER_ALPHA);
      else if (str2=="beta") str=ztos(NUMBER_BETA);
      else if (str2=="gamma") str=ztos(NUMBER_GAMMA);
      else if (str2=="misc") str=ztos(NUMBER_MISC);
      else {
	for (i=0;i<TUPEL_INDEX;i++) {
	  if (str2== (string) name_of_tupelindex[i])
	    str=ztos(NUMBER[i]);
	}
      }
    }
  //   if (argc==4) {
//       str2= argv[2];
//       str3= argv[3];
//     }
    
  }
  else if (name=="ptupel_name") {
    if (argc==3) {
      str2= argv[2];
      n=stoz(str2);
      str=name_of_tupelindex[n];
    }
    if (argc==4) {
      str2= argv[2];
      n=stoz(str2);
      str3= argv[3];
      m=stoz(str3);
      str=name_of_tupelentry[n][m];
    }
  }
  else if (name=="JO_succ") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str=ztos(G_TclVar.JOsucc);
  }
  else if (name=="MO_succ") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str=ztos(G_TclVar.MOsucc);
  }
  else if (name=="MO_pred") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str=ztos(G_TclVar.MOpred);
  }
  else if (name=="JO_pred") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str=ztos(G_TclVar.JOpred);
  }
   else if (name=="REFERENCES") {
     str=Lisa_full_ref(&G_ProblemType,G_Preferences.LISA_HOME,
		       "/data/classify/classify.bib");
     int stringlength=str.length();
     if (stringlength>250) {
       str.erase(250,stringlength);
     }
   }
  else if (name=="SIJ") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    str=ztos((*G_Values.SIJ)[row][column]);
  }
  else if (name=="VALID_SCHEDULE") {
    str=ztos(G_Schedule->valid);
  }else if(name=="VALID_PROBLEMTYPE"){
    str=ztos(G_ProblemType.valid());
  }

  else cerr << "in TC_getvar: Variable " <<name <<"unknown\n";
  if (str!="NOT_DEFINED") sprintf(interp->result, "%s", str.c_str() );
  return TCL_OK; 
}

//**************************************************************************

// Set C++ variables 
int TC_setvar(ClientData /* clientData */,
	      Tcl_Interp *interp,
	      int /*argc*/, TCL_HACK_CHAR *argv[])  {
  int int_value=1;
  int width=0,height=0;
  int row=0, column=0;
  float value=0;
  int entry=0;
  string name,str="";
  string str2="",str3="";
  name=argv[1];
 //  cout << "setvar" << name << endl;
  if (name=="CANVGEO") { 
    sscanf(Tcl_GetVar2(interp,"cw","width",0),"%d",&width);
    sscanf(Tcl_GetVar2(interp,"cw","height",0),"%d",&height);
    G_Status.zoom=TRUE;
    G_Status.width=MIN(MAX_CANV_WIDTH,width);
    G_Status.height=MIN(MAX_CANV_HEIGHT,height);
    show_output();
  }
  else if (name=="LR") { 
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    sscanf(argv[4],"%d",&entry);
    if (row>=0 && column>=0 && row<G_Values.get_n() && column<G_Values.get_m()) 
	(*G_Schedule->LR)[row][column]=entry;
    //show_output();
  }
  else if (name=="Schedule") { 
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    sscanf(argv[4],"%f",&value);
    if (row>=0 && column>=0 && row<G_Values.get_n() && column<G_Values.get_m() )
	(*G_Schedule->CIJ)[row][column]= (TIMETYP) value ;
    show_output();
  }

else if (name=="SIJ") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    sscanf(argv[4],"%d",&entry);
    if (row>=0 && column>=0 && row<G_Values.get_n() && column<G_Values.get_m() )
      (*G_Values.SIJ)[row][column]=entry;
    if (entry==0) (*G_Values.PT)[row][column]=0;
     no_schedule();
    // Tcl_Eval(interp,"TC_draw_dat");
    //show_output();
  }
  else if (name=="MO") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    sscanf(argv[4],"%d",&entry);
    if (G_TclVar.MO_as_Matrix!=NULL) 
      (*G_TclVar.MO_as_Matrix)[row][column]=entry;
    //show_output();
  }
  else if (name=="semiactive_flag") {
    int semiactive=0;
    sscanf(argv[2],"%d",&semiactive);
    G_Schedule->semiactive=semiactive;
  }
  else if (name=="RD") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%f",&value);
    if ( G_Values.RD!=NULL ) {
      if ( G_Values.DD!=NULL ) {
	if ( (*G_Values.DD)[row]>=value) (*G_Values.RD)[row]=value;
	else {
	  G_ExceptionList.lthrow("Release Dates cannot be greater than Due Dates",
				  INCONSISTENT_INPUT);
	  (*G_Values.RD)[row]=(*G_Values.DD)[row];
	  Tcl_Eval(interp,"TC_draw_dat");
	}
      }
      else (*G_Values.RD)[row]=value;
    }
    else  G_ExceptionList.lthrow("Release Dates not defined",UNDEFINED_OBJECT);
    show_output();
  }
  else if (name=="DD") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%f",&value);
    if ( G_Values.DD!=NULL ) {
      if ( G_Values.RD!=NULL ) {
	if ( (*G_Values.RD)[row]<=value) (*G_Values.DD)[row]=value;
	else {
	  G_ExceptionList.lthrow("Release Dates cannot be greater than Due Dates",
				 INCONSISTENT_INPUT);
	  (*G_Values.DD)[row]=(*G_Values.RD)[row];
	  Tcl_Eval(interp,"TC_draw_dat");
	 }
       }
       else  (*G_Values.DD)[row]=value; 
    }
    else  G_ExceptionList.lthrow("Due Dates not defined",UNDEFINED_OBJECT);
    show_output();
  }
  else if (name=="WI") {
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%f",&value);
    (*G_Values.WI)[row]=value;
    show_output();
  }
  //else if (name=="Lisa_Problem.valid") {
   //   G_ProblemType.valid=TRUE;
  // }
  else if (name=="man_oper") {
    int row=0,column=0;
    int JOsucc=0,MOsucc=0,JOpred=0,MOpred=0;
    //type=argv[1];
    sscanf(argv[2],"%d",&row);
    sscanf(argv[3],"%d",&column);
    if (G_Schedule->valid) {
      row++;column++;
      while (G_ExceptionList.empty()==0) print_error();
      Lisa_OsProblem  myOsProblem(&G_Values);
      Lisa_OsSchedule  myOsSchedule(&myOsProblem);
      G_ExceptionList.lcatch(INCONSISTENT_INPUT);
      myOsSchedule.read_LR(G_Schedule->LR);
      
      JOsucc=myOsSchedule.GetJOsucc(row,column);
      MOsucc=myOsSchedule.GetMOsucc(row,column);
      JOpred=myOsSchedule.GetJOpred(row,column);
      MOpred=myOsSchedule.GetMOpred(row,column);
      
      G_TclVar.JOsucc= JOsucc;
      G_TclVar.MOsucc= MOsucc;
      G_TclVar.JOpred= JOpred;
      G_TclVar.MOpred= MOpred;   
    }
 }
 else if (name=="zoom") {
   int zoom;
   sscanf(argv[2],"%d",&zoom);
   if (zoom==1) { 
     G_Status.zoom=TRUE;
     // 
   G_Status.width=mw_width();
   G_Status.height=mw_height();
   }
   else {
     G_Status.zoom=FALSE;
     //show_output();
   }
 }
 else if (name=="zoom_area") {
   float x1=1,y1=1,x2=1,y2=2,xd,yd,temp;
   sscanf(argv[2],"%f",&x1);
   sscanf(argv[3],"%f",&y1);
   sscanf(argv[4],"%f",&x2);
   sscanf(argv[5],"%f",&y2);
   if (x2<x1) {temp=x1;x1=x2;x2=temp;}
   if (y2<y1) {temp=y1;y1=y2;y2=temp;}
   xd=x2-x1; if (xd <0) xd=-xd;
   yd=y2-y1; if (yd <0) yd=-yd;
   if (xd>4 && yd >4 ) {
     if ( MAX_CANV_WIDTH<G_Status.width*mw_width()/xd || 
	  MAX_CANV_HEIGHT<G_Status.height*mw_height()/yd) { show_output(); return TCL_OK;}
     float oldwidth=G_Status.width;
     float oldheight=G_Status.height;
     G_Status.width=  G_Status.width*mw_width()/xd;
     G_Status.height= G_Status.height*mw_height()/yd;
     show_output();
     str= (string) MW_MAINCANV + (string) " xview moveto " + ztos(x1/oldwidth);
     Tcl_Eval(interp, (char*) str.c_str());
     str= (string) MW_HORICANV + (string) " xview moveto " + ztos(x1/oldwidth);
     Tcl_Eval(interp, (char*) str.c_str());
     str= (string) MW_MAINCANV + (string) " yview moveto " + ztos(y1/oldheight);
     Tcl_Eval(interp, (char*) str.c_str()); 
     str= (string) MW_VERTCANV + (string) " yview moveto " + ztos(y1/oldheight);
     Tcl_Eval(interp, (char*) str.c_str());
   }
   else show_output();
 }
  else if (name=="gantt") {
    str=Tcl_GetVar2(interp,"gantt","orientation",TCL_GLOBAL_ONLY);
    if (str=="machine")
      G_Preferences.gantt_orient=GANTT_MACHINE;
    else G_Preferences.gantt_orient=GANTT_JOB;
    str=Tcl_GetVar2(interp,"gantt","special",TCL_GLOBAL_ONLY);
    if (str=="critical_path") 
      G_Preferences.gantt_col_type=GANTT_CP;
    else if (str=="colors") // only other value than GANTT_CP necessary
      {
	G_Preferences.gantt_col_type=GANTT_COLOR; 
	sscanf(Tcl_GetVar2(interp,"gantt","red",0),"%d",&int_value);
	(*G_Preferences.gantt_colors)[GANTT_RED]=int_value;
	sscanf(Tcl_GetVar2(interp,"gantt","green",0),"%d",&int_value);
	(*G_Preferences.gantt_colors)[GANTT_GREEN]=int_value;
	sscanf(Tcl_GetVar2(interp,"gantt","blue",0),"%d",&int_value);
	(*G_Preferences.gantt_colors)[GANTT_BLUE]=int_value;
	sscanf(Tcl_GetVar2(interp,"gantt","brown",0),"%d",&int_value);
	(*G_Preferences.gantt_colors)[GANTT_BROWN]=int_value;
	sscanf(Tcl_GetVar2(interp,"gantt","yellow",0),"%d",&int_value);
	(*G_Preferences.gantt_colors)[GANTT_YELLOW]=int_value;
      }
    else 
      G_Preferences.gantt_col_type=GANTT_NORMAL;
    show_output();
  }
  else if (name=="options") {
    str2=argv[2];
     str3=argv[3];
     if ( str2=="LANGUAGE" ) {
       G_Preferences.contents.add_key("LANGUAGE",str3);
     }
    if ( str2=="HTML_VIEWER" ) {
      G_Preferences.contents.add_key("HTML_VIEWER",str3);
    }
  } 
  else cerr << "in TC_setvar: Variable " <<name <<" unknown\n";
  return TCL_OK; 
}

//**************************************************************************

