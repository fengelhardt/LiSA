 /*
 * ****************** win_obj.h ******************************
 * 
 * description:      objects to draw graphical objects and text
 *
 * owner:            Per Willenius
 *
 * date:             17.9.1998
 *
 * version:          V 1.0
 *
*/ 

#include <math.h>

#include "../misc/int2str.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"

#include "callback.hpp"
#include "win_obj.hpp"
#include "c_tcl.hpp"

#define MAXSTRING 1200

// ************************* Functions ***********************


TCValues::TCValues(Tcl_Interp *interp,string canv,string horizontal_canvas, 
		   string vertical_canvas,string label_name) {
  entrywidth= ENTRYWIDTH;
  entryheight=ENTRYHEIGHT ;
  main_canvas =new Lisa_Canvas(canv.c_str(),interp); 
  canv_horizontal =new Lisa_Canvas(horizontal_canvas.c_str(),interp);
  canv_vertikal =new Lisa_Canvas(vertical_canvas.c_str(),interp);
  label= new Lisa_Label(label_name,interp);
}

TCValues::~TCValues() {
  delete main_canvas;
  delete canv_horizontal; 
  delete canv_vertikal;
}

void TCValues::draw(Lisa_Matrix<int> *myMO,Lisa_Matrix<TIMETYP> *myPT, 
		    Lisa_Matrix<bool> *mySIJ, Lisa_Vector<TIMETYP> *myRD, 
		    Lisa_Vector<TIMETYP> *myDD,  Lisa_Vector<double> * myWI) {
  int i,j,m=0,n=0;
  int width,height,position=0;
  string str;
  if (myMO!=NULL || myPT!=NULL || mySIJ!=NULL) { //Matrix zeichnen
    if (myMO!=NULL) {
      m= myMO->get_m(); n= myMO->get_n();
      label->set_text("$Name(MO)");
    }
    else if (myPT!=NULL) {
      m=myPT->get_m(); n=myPT->get_n();
      label->set_text("$Name(PT)");
    }
    else if (mySIJ!=NULL) {
      m=mySIJ->get_m(); n=mySIJ->get_n();
      label->set_text("$Name(SIJ)");
    }
    width=entrywidth*m+3*entrywidth;
    height=entryheight*n+entryheight;
    
    main_canvas->set_wh(width,height);
    main_canvas->clear();
    canv_horizontal->set_wh(width,50);
    canv_horizontal->clear();
    canv_vertikal->set_wh(50,height);
    canv_vertikal->clear();
    main_canvas->line(1,1,entrywidth*m,1,"Grey");
    main_canvas->line(1,1,1,entryheight*n,"Grey");
    for(j=0;j<n;j++) {
      main_canvas->line(1,entryheight*(j+1),entrywidth*m,entryheight*(j+1),"Grey");  
      canv_vertikal->text(25,entryheight*(j+0.5),"J "+ ztos(j+1) ,"Black");
    }
    
    for(i=0;i<m;i++) {
      main_canvas->line(entrywidth*(i+1),1,entrywidth*(i+1),entryheight*n,"Grey");
      canv_horizontal->text(entrywidth*(i+0.5),25,"M "+ ztos(i+1) ,"Black");
      for(j=0;j<n;j++) {
	str="0";
	if (myMO!=NULL) {str=ztos( (*myMO)[j][i]);}
	else if (myPT!=NULL) str=ztos( (*myPT)[j][i]);
	else if (mySIJ!=NULL) str=ztos( (*mySIJ)[j][i]);
	main_canvas->text(entrywidth*(i+1)-entryheight,entryheight*(j+0.5),str,"Black");
 	if ((mySIJ!=NULL) &&( (myPT!=NULL)||(myMO!=NULL) ) ) {
 	  if ((*mySIJ)[j][i]==FALSE) 
 	    main_canvas->rect(entrywidth*(i),entryheight*(j),entrywidth*(i+1),entryheight*(j+1),"Grey");
 	} 
	position= entrywidth*m;
      }
    }
    if (myRD!=NULL) { //Vektor zeichnen
      main_canvas->line(position,1,position,entryheight*n,"Grey");
      main_canvas->line(position+entrywidth,1,position+entrywidth,entryheight*n,"Grey");  
      canv_horizontal->text(position+entrywidth*(0.5),25,"RD","Black");
      main_canvas->line(position,1,position+entrywidth,1,"Grey");
      for(j=0;j<n;j++) {
	main_canvas->line(position,entryheight*(j+1),position+entrywidth,entryheight*(j+1),"Grey");  
	str="0";
	str=ztos( (*myRD)[j]);
	main_canvas->text(position+entrywidth*(0.5),entryheight*(j+1)-15,str,"Black");   
      }
      position+=entrywidth;
    }
    if (myDD!=NULL) { //Vektor zeichnen
      main_canvas->line(position,1,position,entryheight*n,"Grey");
      main_canvas->line(position+entrywidth,1,position+entrywidth,entryheight*n,"Grey");  
      canv_horizontal->text(position+entrywidth*(0.5),25,"DD","Black");
      main_canvas->line(position,1,position+entrywidth,1,"Grey");
      for(j=0;j<n;j++) {
	main_canvas->line(position,entryheight*(j+1),position+entrywidth,entryheight*(j+1),"Grey");  
	str="0";
	str=ztos( (*myDD)[j]);
	main_canvas->text(position+entrywidth*(0.5),entryheight*(j+0.5),str,"Black");   
      }
      position+=entrywidth;  
      
    }
    
    if(myWI!=NULL) { //Vektor zeichnen
      main_canvas->line(position,1,position,entryheight*n,"Grey");
      main_canvas->line(position+entrywidth,1,position+entrywidth,entryheight*n,"Grey");  
      canv_horizontal->text(position+entrywidth*(0.5),25,"WI","Black");
      main_canvas->line(position,1,position+entrywidth,1,"Grey");
      for(j=0;j<n;j++) {
	main_canvas->line(position,entryheight*(j+1),position+entrywidth,entryheight*(j+1),"Grey");  
	str="0";
	str=ztos( (*myWI)[j]);
	main_canvas->text(position+entrywidth*(0.5),entryheight*(j+0.5),str,"Black");   
      }
      position+=entrywidth;  
    }
  }
}

 /// draw value on row xpos and column ypos
void TCValues::draw_entry(int xpos,int ypos,string value) {
  main_canvas->clear("mark");
  main_canvas->clear(entrywidth*(xpos+1)-entryheight,
		      (float) entryheight*(ypos+0.5));

  main_canvas->text((float) entrywidth*(xpos+1)-entryheight,(float) entryheight*(ypos+0.5),value,"Black");
}

TCScheduleList::TCScheduleList(Tcl_Interp *interp,string canv,
		       string horizontal_canvas, string vertical_canvas,
		       string label_name) {
  entrywidth= ENTRYWIDTH;
  entryheight=ENTRYHEIGHT ;
  main_canvas =new Lisa_Canvas(canv.c_str(),interp); 
  canv_horizontal =new Lisa_Canvas(horizontal_canvas.c_str(),interp);
  canv_vertikal =new Lisa_Canvas(vertical_canvas.c_str(),interp);
  label=new Lisa_Label(label_name,interp);
 }

TCScheduleList::~TCScheduleList() {
  delete main_canvas;
  delete canv_horizontal;
  delete canv_vertikal;
}

void TCScheduleList::draw(Lisa_List<ScheduleNode> * myScheduleList) { 
  Lisa_ProblemType myProblemType;
  int i,j,no_schedules,no_columns;
  int width,height;
  string str;
  main_canvas->clear();
  canv_horizontal->clear();
  canv_vertikal->clear();
  
  no_schedules=myScheduleList->length();
  no_columns=LENGHT_OF_INFO_VECTOR;  
  width=LENGHT_OF_INFO_VECTOR*entrywidth;
  height=no_schedules*entryheight;
  
  main_canvas->set_wh(width,height);
  canv_horizontal->set_wh(width,50);
  canv_vertikal->set_wh(50,height);

  //  cout << (*G_ScheduleList->get().schedule_info)[NO_OF_IKL] << endl;

  ScheduleNode myScheduleNode;
  if (no_schedules<=1) return;
  main_canvas->line(1,1,entrywidth*no_columns,1,"Grey");
  main_canvas->line(1,1,1,entryheight*no_schedules,"Grey");
  myScheduleList->reset();
  for(j=0;j<no_schedules;j++) {
    main_canvas->line(1,entryheight*(j+1),entrywidth*no_columns,entryheight*(j+1),"Grey");
    //myLisa_Node=(*G_Schedule.ScheduleList)[j];
    myScheduleNode=myScheduleList->get();
    canv_vertikal->text(25,entryheight*(j+0.5),ztos((*myScheduleList->get().schedule_info)[0]) ,"Black");
    myScheduleList->next();
  }
  for(i=0;i<LENGHT_OF_OBJECTIVE;i++) {
    main_canvas->line(entrywidth*(i+1),1,entrywidth*(i+1),entryheight*no_schedules,"Grey");
    // NUMBER[OBJECTIVE]-2 because no irregular objective wanted 
    if (i< NUMBER[OBJECTIVE]-2) 
      canv_horizontal->text(entrywidth*(i+0.5),25," "+ myProblemType.name(OBJECTIVE,i+1),"Black");
    myScheduleList->reset();
    for(j=0;j<no_schedules;j++) {
      str="";
      main_canvas->rect(entrywidth*(i),entryheight*(j),entrywidth*(i+1),entryheight*(j+1),"White");
      //myLisa_Node=(*G_Schedule.ScheduleList)[j];
      //myScheduleNode=(ScheduleNode *) myLisa_Node;
         str=ztos((* myScheduleList->get().schedule_info)[i+1]);
	 main_canvas->text(entrywidth*(i+1)-entryheight,entryheight*(j+0.5),str,"Black");
	 myScheduleList->next();
    }
  }
  for(i=LENGHT_OF_OBJECTIVE;i<LENGHT_OF_INFO_VECTOR;i++) {
    main_canvas->line(entrywidth*(i+1),1,entrywidth*(i+1),entryheight*no_schedules,"Grey");
    // NUMBER[OBJECTIVE]-2 because no irregular objective wanted 
    if (i==MAXRANK_INFO) 
      canv_horizontal->text(entrywidth*(i+0.5),25," MaxRank","Black");
    if (i==NO_OF_SOURCES) 
      canv_horizontal->text(entrywidth*(i+0.5),25," NoSourc","Black");
    if (i==NO_OF_SINKS) 
      canv_horizontal->text(entrywidth*(i+0.5),25," NoSink","Black");
    myScheduleList->reset();
    for(j=0;j<no_schedules;j++) {
      str="#";
      main_canvas->rect(entrywidth*(i),entryheight*(j),entrywidth*(i+1),entryheight*(j+1),"White");
      //myLisa_Node=(*G_Schedule.ScheduleList)[j];
      //myScheduleNode=(ScheduleNode *) myLisa_Node;
      str=ztos((* myScheduleList->get().schedule_info)[i]);
      main_canvas->text(entrywidth*(i+1)-entryheight,entryheight*(j+0.5),str,"Blue");
      myScheduleList->next();
    }
  }

}

TCSchedule::TCSchedule(Tcl_Interp *interp,string canv,
		       string horizontal_canvas, string vertical_canvas,
		       string label_name) {
  entrywidth= ENTRYWIDTH;
  entryheight=ENTRYHEIGHT ;
  main_canvas =new Lisa_Canvas(canv.c_str(),interp); 
  canv_horizontal =new Lisa_Canvas(horizontal_canvas.c_str(),interp);
  canv_vertikal =new Lisa_Canvas(vertical_canvas.c_str(),interp);
  label=new Lisa_Label(label_name,interp);
 }

TCSchedule::~TCSchedule() {
  delete main_canvas;
  delete canv_horizontal;
  delete canv_vertikal;
}

void TCSchedule::draw( Lisa_MO *myMO,
		       Lisa_Matrix<int> *myP,
		       Lisa_JO *myJO,
		       Lisa_Matrix<TIMETYP> *myCIJ,
		       Lisa_Matrix<bool> *SIJ) {

  int i,j,m=0,n=0;
  int width,height,position;
  string str;
  main_canvas->clear();
  canv_horizontal->clear();
  canv_vertikal->clear();
  if (myMO!=NULL || myP!=NULL || myJO!=NULL || myCIJ!=NULL) { //draw table
    if (myMO!=NULL) {
      m=myMO->get_m(); n=myMO->get_n();
      label->set_text("$Name(MO)");
    }
    else if (myP!=NULL) {
      m=myP->get_m(); n=myP->get_n();
      label->set_text("$Name(Sequence)");
    }
    else if (myJO!=NULL) {
      m=myJO->get_m(); n=myJO->get_n();
      label->set_text("$Name(JO)");
    }
    else if (myCIJ!=NULL) {
      m=myCIJ->get_m(); n=myCIJ->get_n();
      label->set_text("$Name(Schedule)");
    }
    
    width=entrywidth*m+4*entrywidth;
    height=entryheight*n+2*entryheight;
    
    main_canvas->set_wh(width,height);
    canv_horizontal->set_wh(width,50);
    canv_vertikal->set_wh(50,height);
    
    main_canvas->line(1,1,entrywidth*m,1,"Grey");
    main_canvas->line(1,1,1,entryheight*n,"Grey");
    for(j=0;j<n;j++) {
      main_canvas->line(1,entryheight*(j+1),entrywidth*m,entryheight*(j+1),"Grey");  
      canv_vertikal->text(25,entryheight*(j+0.5),"J "+ ztos(j+1) ,"Black");
    }
    
    for(i=0;i<m;i++) {
      main_canvas->line(entrywidth*(i+1),1,entrywidth*(i+1),entryheight*n,"Grey");
      canv_horizontal->text(entrywidth*(i+0.5),25,"M "+ ztos(i+1) ,"Black");
      for(j=0;j<n;j++) {
	str="0";
	if (myMO!=NULL) //str=ztos( (*myMO)[j][i]);
	  str="0"; 
	else if (myP!=NULL) {
	  if ((*myP)[j][i]==0) str="";
	  else str=ztos( (*myP)[j][i]);
	}
	else if (myJO!=NULL) //str=ztos( (*myJO)[j][i]);
	  str="0";
	else if (myCIJ!=NULL) {
	  if ((*myCIJ)[j][i]==0) str="";
	  else str=ztos( (*myCIJ)[j][i]);
	}
	if ((*SIJ)[j][i]==FALSE) 
	  main_canvas->rect(entrywidth*(i),entryheight*(j),entrywidth*(i+1),entryheight*(j+1),"Grey");
	main_canvas->text(entrywidth*(i+1)-entryheight,entryheight*(j+0.5),str,"Black");
      }
    }   
    position= entrywidth*m;
    }
}

/// draw value on row xpos and column ypos
void TCSchedule::draw_entry(int xpos,int ypos,string value) {
  main_canvas->clear("mark");
  main_canvas->clear( entrywidth*(xpos+1)-entryheight,(float) entryheight*(ypos+0.5));
  main_canvas->text((float) entrywidth*(xpos+1)-entryheight,(float) entryheight*(ypos+0.5),value,"Black");
}

TCCMatrix::TCCMatrix(Tcl_Interp *interp,string canv,string horizontal_canvas, string vertical_canvas) {
  entrywidth= ENTRYWIDTH;
  entryheight=ENTRYHEIGHT ;
  main_canvas =new Lisa_Canvas(canv.c_str(),interp); 
  canv_horizontal =new Lisa_Canvas(horizontal_canvas.c_str(),interp);
  canv_vertikal =new Lisa_Canvas(vertical_canvas.c_str(),interp);
}

TCCMatrix::~TCCMatrix() {
  delete main_canvas;
  delete canv_horizontal;
  delete canv_vertikal;
}

void TCCMatrix::draw(Lisa_Matrix<TIMETYP> *myC, 
		     Lisa_Matrix<bool> *myCP) {  
  int i,j,m,n;
  int width,height,position;
  string str;
  
  if (myC!=NULL) { //draw table
    m=myC->get_m(); n=myC->get_n();
    width=entrywidth*m+4*entrywidth;
    height=entryheight*n+2*entryheight;
    
    main_canvas->set_wh(width,height);
    main_canvas->clear();
    canv_horizontal->set_wh(width,50);
    canv_horizontal->clear();
    canv_vertikal->set_wh(50,height);
    canv_vertikal->clear();
    main_canvas->line(1,1,entrywidth*m,1,"Grey");
    main_canvas->line(1,1,1,entryheight*n,"Grey");
    for(j=0;j<n;j++) {
      main_canvas->line(1,entryheight*(j+1),entrywidth*m,entryheight*(j+1),"Grey");  
      canv_vertikal->text(25,entryheight*(j+0.5),"J "+ ztos(j+1) ,"Black");
    }
    
    for(i=0;i<m;i++) {
      main_canvas->line(entrywidth*(i+1),1,entrywidth*(i+1),entryheight*n,"Grey");
      canv_horizontal->text(entrywidth*(i+0.5),25,"M "+ ztos(i+1) ,"Black");
      for(j=0;j<n;j++) {
	str=ztos( (*myC)[j][i]);
	if ((*myCP)[j][i])
	  main_canvas->text(entrywidth*(i+1)-entryheight,
			    entryheight*(j+0.5),str,"Red");
	else
	  main_canvas->text(entrywidth*(i+1)-entryheight,
			    entryheight*(j+0.5),str,"Black");
      }
    }   
    position= entrywidth*m;
  }
}


TCGraph::TCGraph() {
  vertice_radius=VERTICE_RADIUS;
}

TCGraph::~TCGraph() {
 }

TCSeqGraph::TCSeqGraph(Tcl_Interp *interp,string canv,
		       string horizontal_canvas, string vertical_canvas) {
  main_canvas =new Lisa_Canvas(canv.c_str(),interp); 
  canv_horizontal =new Lisa_Canvas(horizontal_canvas.c_str(),interp);
  canv_vertikal =new Lisa_Canvas(vertical_canvas.c_str(),interp);
}

TCSeqGraph::~TCSeqGraph() {
 delete main_canvas;
 delete canv_horizontal;
 delete canv_vertikal;
}

void TCSeqGraph::draw(Lisa_Matrix<bool> &CP,
		  Lisa_OsSchedule &myOsSchedule,
		  Lisa_Matrix<bool> &SIJ) {
  char color[20];
  int i,j,dist,width=0,height=0;
  int maxm,maxn;
  float d;
  int border=60; // size of the border
  maxm=SIJ.get_m();
  maxn=SIJ.get_n();

  if(zoom == FALSE) {
    main_canvas->set_wh(mw_width(),mw_height()); 
  } 
 main_canvas->clear();
  canv_vertikal->set_wh(40,main_canvas->get_height());
  canv_horizontal->set_wh(main_canvas->get_width(),40);
  width=main_canvas->get_width();
  height=main_canvas->get_height();
  canv_horizontal->clear();
  canv_vertikal->clear();
  // draw the horizontal canvas
 for(i=0;i<maxm;i++) {
   canv_horizontal->text((width-2*border)/maxm*i+border,25,
			 "M "+ ztos(i+1) ,"Black");
 }
 
 // draw the vertical canvas
 for(j=0;j<maxn;j++) { 
   canv_vertikal->text(25,(height-2*border)/maxn*j+border,
		       (string) "J "+ ztos(j+1) ,"Black");
 }
 
 // (bugfix by iroess)
 // go through the operations & do the following:
 // - get the horizontal & vertical predecessor - NOT the successor!!!
 // - do both operations lie on a critical path ?! 
 // - both Schedule-Values have to be compared: curr_head==pred_head+pred_processing_time ?	       
	
 for(i=0;i<maxm;i++) { 
   for(j=0;j<maxn;j++) { 
     if ( (SIJ)[j][i]) {
       if ( (CP)[j][i])
	 main_canvas->fil_circle((width-2*border)/maxm*i+border,
				 (height-2*border)/maxn*j+border,2,"Red");
       else
	 main_canvas->fil_circle((width-2*border)/maxm*i+border,
				 (height-2*border)/maxn*j+border,2,"Black");
       // get the horizontal predecessor       
       int hj=j;
       int hi=myOsSchedule.GetMOpred(j+1,i+1)-1;
       if (hi>=0) {
	 // --> critical-path-check...
	 if ( (CP)[j][i] && ((CP)[hj][hi]) &&  (myOsSchedule.GetHead(j+1,i+1)==myOsSchedule.GetHead(hj+1,hi+1)+(*(myOsSchedule.P->time))[hj+1][hi+1] ))    
	   sprintf(color,"Red");
	 else sprintf(color,"Black");
	 dist=i-hi; if(dist<0) dist=-dist;
	 if(dist==1) d=0; // straight line
	 else d=500/maxn*dist/maxm;   
	 main_canvas->arc_arrow((width-2*border)/maxm*hi+border,
				(height-2*border)/maxn*hj+border,
				(width-2*border)/maxm*i+border,
				(height-2*border)/maxn*j+border,
				d,color);
       }
       // and now the vertical predecessor
       int vi=i;
       int vj=myOsSchedule.GetJOpred(j+1,i+1)-1;
       if (vj>=0) {
	 // --> critical-path-check...	       
	 if ( (CP)[j][i] && ((CP)[vj][vi]) &&  (myOsSchedule.GetHead(j+1,i+1)==myOsSchedule.GetHead(vj+1,vi+1)+(*(myOsSchedule.P->time))[vj+1][vi+1] ))    
	   sprintf(color,"Red");
	 else sprintf(color,"Black");
	 dist=j-vj; if(dist<0) dist=-dist;
	 if(dist==1) d=0; // straight line
	 else d=500/maxm*dist/maxn;
	 main_canvas->arc_arrow((width-2*border)/maxm*vi+border,
				(height-2*border)/maxn*vj+border,
				(width-2*border)/maxm*i+border,
				(height-2*border)/maxn*j+border,
				d,color);
       } 
     }
   }
 }
}





