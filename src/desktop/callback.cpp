/**
 * @author  Per Willenius
 * @version 2.3pre3
 */

#include "../desktop/c_tcl.hpp" 
#include "../desktop/gantt.hpp"
#include "../scheduling/xsched.hpp"
#include "../main/lisapref.hpp"
#include "../main/status.hpp"
#include "../misc/int2str.hpp"
#include "../basics/order.hpp"
#include "../lisa/ptype.hpp"

#include "callback.hpp"
#include "tclvar.hpp"

using namespace std;

//**************************************************************************

extern class Lisa_ProblemType G_ProblemType;
extern class Lisa_Values G_Values;
extern class Lisa_Schedule *G_Schedule;
extern class Lisa_Status G_Status;
extern class Lisa_XSchedule *G_XSchedule;
extern class Lisa_List<Lisa_ScheduleNode> *G_ScheduleList;
extern class Lisa_Preferences G_Preferences;
extern class Lisa_Canvas *G_MWCanvas;
extern Tcl_Interp *interp;
extern class Lisa_TCLVar G_TclVar;

//**************************************************************************

void new_problemtype() {
  if (G_ProblemType.output_problem()!="no valid Problem") {
    Tcl_Eval(interp,"tupelinit; dpinit");
    Tcl_Eval(interp,"problem_exist");
    if (G_ProblemType.get_property(M_NUMBER)==M_VAL && G_ProblemType.get_property(M_NO)>0)
      G_Values.init(G_Values.get_n(),G_ProblemType.get_property(M_NO));
    if (G_Values.get_m()<=0 || G_Values.get_n()<=0) {
      no_mn();
    }
  }
  //  no_schedule();
  no_values();
  if (G_ProblemType.get_property(M_ENV)==F) 
    gen_fs();
  if (G_ProblemType.get_property(M_ENV)!=F && G_ProblemType.get_property(M_ENV)!=J) {
    G_Values.no_MO();
  }
  if (!G_ProblemType.get_property(RI)) 
    G_Values.no_RD();
  else G_Values.make_RD();
  if (!G_ProblemType.get_property(DI)&&
      G_ProblemType.get_property(OBJECTIVE)!=LMAX&&
      G_ProblemType.get_property(OBJECTIVE)!=SUM_UI&&
      G_ProblemType.get_property(OBJECTIVE)!=SUM_WIUI&&
      G_ProblemType.get_property(OBJECTIVE)!=SUM_TI&&
      G_ProblemType.get_property(OBJECTIVE)!=SUM_WITI&&
      G_ProblemType.get_property(OBJECTIVE)!=IRREG1&&
      G_ProblemType.get_property(OBJECTIVE)!=IRREG2
      )
    G_Values.no_DD();
  else G_Values.make_DD();
  if (G_ProblemType.get_property(OBJECTIVE)!=SUM_WIUI&&
      G_ProblemType.get_property(OBJECTIVE)!=SUM_WICI&&
      G_ProblemType.get_property(OBJECTIVE)!=SUM_WITI)
    G_Values.no_WI();
  else G_Values.make_WI();
  // show_output();
}

//**************************************************************************

void no_schedule() {
  //G_ScheduleList->clear();
   while(!(G_ScheduleList->empty())) 
	{
          Lisa_ScheduleNode dummynode;
          dummynode=G_ScheduleList->pop();
          if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
	}
   int m=G_Values.get_m();
   int n=G_Values.get_n();
  G_Schedule = new Lisa_Schedule(n,m);
  Lisa_ScheduleNode *myLisa_ScheduleNode;
  myLisa_ScheduleNode= new Lisa_ScheduleNode(G_Schedule);
  G_ScheduleList->append(*myLisa_ScheduleNode);
  G_Schedule->valid=FALSE;
  delete G_XSchedule;
  G_XSchedule = new Lisa_XSchedule(G_Schedule);
  if (interp!=NULL) {
    Tcl_Eval(interp,"mw_no_schedule");
  }
  show_output();
}

//**************************************************************************

void no_values() {
  G_Values.valid=FALSE;
  if (interp!=NULL) {
    Tcl_Eval(interp,"mw_no_values");
  }
  show_output();
}

//**************************************************************************

void new_schedule() {
  G_Schedule->valid_LR(G_Values.SIJ);
  if (G_Schedule->valid) {
    Tcl_Eval(interp,"schedule_exist");
    Tcl_Eval(interp,"TC_draw_schedule");
    //  Tcl_Eval(interp,"TC_draw_schedule_list");
    show_output();
  } 
}

//**************************************************************************

/* some entries in Lisa_Value has changed*/
void new_values() {
  // test if values valid:
  int m_env=G_ProblemType.get_property(M_ENV);
  if (m_env==X || m_env==G || m_env==P || m_env==Q || m_env==R) {
    Tcl_Eval(interp,"mw_no_values"); return; 
  }
  if (m_env==J || m_env==F)
    if (G_Values.valid_MO()!=OK) {
      Tcl_Eval(interp,"mw_no_values"); return;
    } 
  if (interp!=NULL) {
    Tcl_Eval(interp,"values_exist");
  }
  G_Values.valid=TRUE;
 //  G_ScheduleList->clear();
//   G_Schedule = new Lisa_Schedule(G_Values.get_n(),G_Values.get_m());
//   Lisa_ScheduleNode *myLisa_ScheduleNode;
//   myLisa_ScheduleNode= new Lisa_ScheduleNode(G_Schedule);
//   G_ScheduleList->append(*myLisa_ScheduleNode);
//  delete G_XSchedule;
//   G_XSchedule = new Lisa_XSchedule(G_Schedule);
}

//**************************************************************************

// the number of machines or jobs has changed:
void new_mn() {
  G_Values.init(G_Values.get_n(),G_Values.get_m());
  G_TclVar.init();
  
  // and now, change the corresponding tcl variables
  if (interp!=NULL) {
    Tcl_Eval(interp,"mw_no_values ");
    Tcl_Eval(interp,"new_mn");
  }
  //G_ScheduleList->clear();
  while(!(G_ScheduleList->empty())) 
    {
      Lisa_ScheduleNode dummynode;
      dummynode=G_ScheduleList->pop();
      if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
    }
  G_Schedule = new Lisa_Schedule(G_Values.get_n(),G_Values.get_m());
  Lisa_ScheduleNode *myLisa_ScheduleNode;
  myLisa_ScheduleNode= new Lisa_ScheduleNode(G_Schedule);
  G_ScheduleList->append(*myLisa_ScheduleNode);
   delete G_XSchedule;
  G_XSchedule = new Lisa_XSchedule(G_Schedule);
}

//**************************************************************************

// machine or job number is 0
void no_mn() {
  Tcl_Eval(interp,"mw_no_mn");
}

//**************************************************************************

void show_output() {
  string mw_output;
  TIMETYP objective;
  mw_set_objective("");
  // clear all canvas:
  mw_set_label ("");
  TCCMatrix myCMatrix(interp,MW_MAINCANV, MW_HORICANV,MW_VERTCANV);
  myCMatrix.clear();
  if (G_Values.valid==FALSE || G_ProblemType.valid()==FALSE )
    Tcl_Eval(interp,"mw_add_logo");
  else {
    mw_output= Tcl_GetVar2(interp,"mw","mwout",TCL_GLOBAL_ONLY);
    if (mw_output=="Sequence") {
      mw_set_label ("$Name(Sequence)");
      TCSchedule mySchedule(interp,MW_MAINCANV,MW_HORICANV,MW_VERTCANV,MW_LABEL);
      Lisa_Matrix<int> *myLR=NULL;
      myLR=G_Schedule->LR;
      mySchedule.draw(NULL,myLR,NULL,NULL,G_Values.SIJ);
      return;
    }
    if (mw_output=="Parameter") {
      mw_set_label ("$Name(Parameter)");
      TCValues myValues(interp,MW_MAINCANV,MW_HORICANV,MW_VERTCANV,MW_LABEL);
      Lisa_Matrix<TIMETYP> *myPT=NULL;
      Lisa_Matrix<bool> *mySIJ=NULL;
      Lisa_Vector<TIMETYP> *myRD=NULL, *myDD=NULL;
      if (G_ProblemType.get_property(RI)!=EMPTY)
	myRD=G_Values.RD;
      if (G_ProblemType.get_property(DI)!=EMPTY)
	myDD=G_Values.DD;
      Lisa_Vector<double> * myWI=NULL;
      if (G_ProblemType.get_property(OBJECTIVE)==SUM_WICI ||
	  G_ProblemType.get_property(OBJECTIVE)==SUM_WIUI ||
	  G_ProblemType.get_property(OBJECTIVE)==SUM_WITI )
	myWI=G_Values.WI;
      string str;
      myPT=G_Values.PT;
      mySIJ=G_Values.SIJ;
      myValues.draw(NULL,myPT,mySIJ,myRD,myDD,myWI);
      return;
    }
    // all other output routines needs a valid schedule
    // G_Schedule->valid_LR(G_Values.SIJ);
    if (G_Schedule->valid) {
      while (G_ExceptionList.empty()==0) print_error();
      Lisa_OsProblem  myOsProblem(&G_Values);
      Lisa_OsSchedule  myOsSchedule(&myOsProblem);
      G_ExceptionList.lcatch(INCONSISTENT_INPUT);
      objective=update_objective(myOsSchedule);
      if (mw_output=="Gantt Diagram") {
	double maximum=0;
	for (int i=0;i<G_Values.get_n();i++) {
	  for (int j=0;j<G_Values.get_m();j++) {
	    if (maximum<(*G_Values.PT)[i][j])
	      maximum=(*G_Values.PT)[i][j];
	  }
	}
	if (maximum>0) {
	  Tcl_Eval(interp,"activate_zoom");
	  mw_set_label ("$Name(Gantt_Diagram)");
	  Lisa_Vector<int> active_path(5);
	  active_path.fill(0);
	  TCGantt myTCGantt(interp,MW_MAINCANV, MW_HORICANV,MW_VERTCANV);
	  myTCGantt.zoom=G_Status.zoom;
	  myTCGantt.set_wh((int) G_Status.width,(int) G_Status.height);
	  myTCGantt.draw(G_XSchedule->CP,&G_Values,G_Schedule,
			 G_Preferences.gantt_orient,
			 G_Preferences.gantt_col_type,
			 G_Preferences.gantt_colors);
	}
      }
      else if(mw_output=="Sequence Graph") {
	Tcl_Eval(interp,"activate_zoom");
	mw_set_label ("$Name(Sequence_Graph)");
	TCSeqGraph mySeqGraph(interp,MW_MAINCANV, MW_HORICANV,MW_VERTCANV);
	mySeqGraph.zoom=G_Status.zoom;
	mySeqGraph.set_wh((int) G_Status.width,(int) G_Status.height);
	mySeqGraph.draw(*G_XSchedule->CP, myOsSchedule,*G_Values.SIJ);
      }
      // else if(mw_output=="Reduced Sequence Graph") output_RSG(G_MWCanvas);
      // else if(mw_output=="Comparability Graph") output_CG(G_MWCanvas);
      //       else if(mw_output=="Interval Graph") output_IG(G_MWCanvas);
      else if(mw_output=="Schedule") {
	mw_set_label ("$Name(Schedule)");
	TCCMatrix myCMatrix(interp,MW_MAINCANV, MW_HORICANV,MW_VERTCANV);
	myCMatrix.draw(G_Schedule->CIJ,G_XSchedule->CP);
      }
      //    else G_ExceptionList.lthrow((string) "identifier:"+(string) mw_output+ (string)" unknown",OUT_OF_RANGE);
    }
    //  else { 
    //       if (G_Values.valid) 
    // 	Tcl_Eval(interp,"set mw(mwout) Parameter; TC_draw_output");
    //else Tcl_Eval(interp,"set mw(mwout) 0 ");
    //    } 
  }
}

//**************************************************************************

TIMETYP update_objective(Lisa_OsSchedule &myOsSchedule ) {
  TIMETYP objective=0;
  
  if (G_Schedule->valid) {
    int i,j;
    myOsSchedule.ComputeHeadsTails(TRUE,TRUE);
    if(G_Schedule->semiactive) {
      myOsSchedule.read_LR(G_Schedule->LR);
    }
    else {
      if (myOsSchedule.read_Cij(G_Schedule->CIJ)!=OK) {
	G_ExceptionList.lthrow("Cij-Matrix not valid",ANY_ERROR);
	return 0;
      }
    } 
    myOsSchedule.SetValue(G_ProblemType.get_property(OBJECTIVE));    
    objective= myOsSchedule.GetValue();
    
    // construct C_Matrix in Lisa_Schedule
    G_XSchedule->make_CP();
    G_XSchedule->CP->fill(0);
    G_Schedule->make_CIJ();
    for (i=0;i<G_Values.get_n();i++)
      for (j=0;j<G_Values.get_m();j++) {
	(*G_Schedule->CIJ)[i][j]=myOsSchedule.GetHead(i+1,j+1)+(*G_Values.PT)[i][j];
      }
    
    if (G_ProblemType.get_property(OBJECTIVE)==CMAX) {
      for (i=0;i<G_Values.get_n();i++)
	for (j=0;j<G_Values.get_m();j++) {
	  if (myOsSchedule.GetHead(i+1,j+1)+(*G_Values.PT)[i][j]+
	      myOsSchedule.GetTail(i+1,j+1)>=objective) { 
	    (*G_XSchedule->CP)[i][j]=TRUE; 
	  }
	  else 
	    (*G_XSchedule->CP)[i][j]=FALSE;
	}
    }
    mw_set_objective 
      (G_ProblemType.name
       (OBJECTIVE,G_ProblemType.get_property(OBJECTIVE))
       +": "+ztos(objective));
  }
  return objective;
}

//**************************************************************************

void update_LR() {
  int i=0, j=0, n=G_Values.get_n(), m= G_Values.get_m();
  int number_of_operations=0,rank=0;
  double value=0;  
  Lisa_Order myOrder(n,m);

  for(i=0;i<n;i++) 
    for(j=0;j<m;j++) 
      if ((*G_Values.SIJ)[i][j])
	number_of_operations++;
  
  for(i=0;i<n;i++) 
    for(j=0;j<m;j++) {
      value=(*G_Schedule->LR)[i][j];
      if ((*G_Values.SIJ)[i][j])
	myOrder.read_one_key(i,j,value);
      else myOrder.read_one_key(i,j,20000);
    }
  myOrder.sort();
  G_Schedule->make_LR();
  G_Schedule->LR->fill(0);
  for (rank=0; rank<number_of_operations;rank++) {
    (*G_Schedule->LR)[myOrder.row(rank)][ myOrder.col(rank)]=rank+1;
  }
  while (G_ExceptionList.empty()==0) print_error();
  Lisa_OsProblem  myOsProblem(&G_Values);
  Lisa_OsSchedule  myOsSchedule(&myOsProblem);
  G_ExceptionList.lcatch(INCONSISTENT_INPUT);
  myOsSchedule.read_LR(G_Schedule->LR);
  myOsSchedule.write_LR(G_Schedule->LR);
  new_schedule();
}

//**************************************************************************

/// generate MO with Flow Shop Order
void gen_fs( ) {
  int n=G_Values.get_n();
  int m=G_Values.get_m();
  int i,j;
  G_Values.SIJ->fill(1);
  G_TclVar.make_MO(n,m);
  G_Values.make_MO();
  for (i=0;i<n;i++)
    for (j=0;j<m;j++)
      (*G_TclVar.MO_as_Matrix)[i][j]=j+1;
  G_Values.MO->read_rank(G_TclVar.MO_as_Matrix);
}

//**************************************************************************

