/**
 * @author  Per Willenius
 * @version 3.0pre1
 */ 

#include <math.h>
#include <string.h>

#include <iostream>

#include "../misc/int2str.hpp"
#include "basicwin.hpp"

using namespace std;

//**************************************************************************

#define MAXSTRING 1200

//**************************************************************************

Lisa_Canvas::Lisa_Canvas(const char * name,Tcl_Interp * tclinterp){
  cv_name = new char[200];
  command = new char[200];
  sprintf(cv_name,"%s",name);
  cv_interp=tclinterp;
  width=500;
  height=500;
  // define some nice colors
  strcpy(color[0],"#eca76f");
  strcpy(color[1],"#1b1dcb");
  strcpy(color[2],"#cad4dc");
  strcpy(color[3],"#fe4d9e");
  strcpy(color[4],"#00774e");
  strcpy(color[5],"#88e1b4");
  strcpy(color[6],"#5d0000");
  strcpy(color[7],"#fefebe");
  strcpy(color[8],"#570070");
  strcpy(color[9],"#ee9ef3");
  strcpy(color[10],"#fee6cc");
  strcpy(color[11],"#495261");
  strcpy(color[12],"#608e20");
  strcpy(color[13],"#008ef6");
  strcpy(color[14],"#fef900");
  strcpy(color[15],"#788594");
  strcpy(color[16],"#00de3e");
  strcpy(color[17],"#ea0000");
  strcpy(color[18],"#98edfe");
  strcpy(color[19],"#feabfe");
  strcpy(color[20],"#921d0b");
  strcpy(color[21],"#fec4c4");
  strcpy(color[22],"#4992a6");
}

//**************************************************************************

void Lisa_Canvas::clear(){
  sprintf(command, "%s delete all",cv_name);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::clear( string tag ) {
  sprintf(command, "%s delete %s",cv_name,(char*) tag.c_str());
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::clear( double xpos,double ypos ) {
  sprintf(command, "%s delete [%s find overlapping %f %f %f %f]",
	  cv_name,cv_name,xpos-3,ypos-3,xpos+3,ypos+3);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void  Lisa_Canvas::line(float x1,float y1,float x2,float y2,int col) {
  line(x1,x2,y1,y2,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void  Lisa_Canvas::line(float x1,float y1,float x2,float y2,const char * col){
  sprintf(command, "%s create line %3.0f %3.0f %3.0f %3.0f -fill %s",
	  cv_name,x1,y1,x2,y2,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void  Lisa_Canvas::line(float x1,float y1,float x2,float y2,
			string tag, const char * col){
  sprintf(command, "%s create line %3.0f %3.0f %3.0f %3.0f -tag {%s} -fill %s",
	  cv_name,x1,y1,x2,y2,(char*) tag.c_str(),col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void  Lisa_Canvas::line_rel(float x1,float y1,float x2,float y2,int col) {
  line_rel(x1,y1,x2,y2,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void  Lisa_Canvas::line_rel(float x1,float y1,float x2,float y2,const char * col){
  sprintf(command, "%s create line %3.0f %3.0f %3.0f %3.0f  -fill %s",
	  cv_name,x1/100.*width,(100.-y1)/100.*height,
	  x2/100.*width,(100.-y2)/100.*height,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::dashed_hline_rel(double x1, double x2, double y)
{
  int x1_int,x2_int,distance_int,i;
  double distance1,distance2;

  x1_int = ((int) x1)+1;
  x2_int = ((int) x2);
  distance1 = x1_int - x1;
  distance2 = x2 - x2_int;
  distance_int = x2_int - x1_int;

  if ((distance1>=0.3))
    line_rel(x1,y,x1_int-0.3,y,"black");
  if((distance2<0.7))
    line_rel(x2_int,y,x2,y,"black");
  else
    line_rel(x2_int,y,x2_int+0.7,y,"black");
  for(i=0;i<distance_int;i++)
    line_rel((double)x1_int+i,y,(double)x1_int+i+0.7,y,"black");
  return;
}

//**************************************************************************

void Lisa_Canvas::arrow(float x1,float y1,float x2,float y2,int col) {
  arrow(x1,x2,y1,y2,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::arrow(float x1,float y1,float x2,float y2,const char* col){
 sprintf(command, 
	 "%s create line %3.0f %3.0f %3.0f %3.0f -arrow last -arrowshape {16 20 5}  -fill %s",
	 cv_name,x1,y1,x2,y2,col);
 Tcl_Eval(cv_interp,command); 
}

//**************************************************************************

void Lisa_Canvas::arrow_rel(float x1,float y1,float x2,float y2,int col) {
  arrow_rel(x1,x2,y1,y2,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::arrow_rel(float x1,float y1,float x2,float y2,const char* col) {
  sprintf(command, 
	  "%s create line %3.0f %3.0f %3.0f %3.0f -arrow last -arrowshape {16 20 5}  -fill %s",
	  cv_name,x1/100.*width,(100.-y1)/100.*height,
	  x2/100.*width,(100.-y2)/100.*height,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::arc_arrow(float x1,float y1,float x2,float y2,float d,const char *col){
  // Eine gerichtete, gebogene Kante eines Graphen  
  // d entspricht den Unterschied zur Linie (d=0 <==> gerade Kante)
  float xzp1,yzp1; // Zwischenpunkt
  float xm,ym,dist;
  dist=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
  xm=(x1+x2)/2.;
  ym=(y1+y2)/2.;  // (xm,ym) Mittelpunkt der Kante
  xzp1=xm-d*((y2-y1)/dist);
  yzp1=ym-d*((x2-x1)/dist );
  
  sprintf(command, 
	  "%s create line %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f -tag arc -smooth true -arrow last -arrowshape {16 20 5} -fill %s",
	  cv_name,x1,y1,xzp1,yzp1,x2,y2,col);  
  Tcl_Eval(cv_interp,command);  
}

//**************************************************************************

void Lisa_Canvas::rect(float x1,float y1,float x2,float y2,int col) {
  rect(x1,x2,y1,y2,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::rect(float x1,float y1,float x2,float y2,const char * col){
  sprintf(command, 
	  "%s create rect %3.0f %3.0f %3.0f %3.0f -fill %s -outline black",
	  cv_name,x1,y1,x2,y2,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::rect_rel(float x1,float y1,float x2,float y2,int col) {
  rect_rel(x1,y1,x2,y2,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::rect_rel(float x1,float y1,float x2,float y2,const char * col){
  sprintf(command, "%s create rect %3.0f %3.0f %3.0f %3.0f -fill %s -outline black",
	  cv_name,x1/100.*width,(100.-y1)/100.*height,
	  x2/100.*width,(100.-y2)/100.*height,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::text(float x,float y,const char *txt, int col){
  text(x,y,txt,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::text(float x,float y,const char *txt, const char * col){
  sprintf(command, 
	  "%s create text %3.0f %3.0f -text \"%s\" -fill %s -justify left",
	  cv_name,x,y,txt,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::text_rel(float x,float y,const char *txt, int col){
  text_rel(x,y,txt,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::text_rel(float x,float y,const char *txt, const char * col){
  sprintf(command, 
	  "%s create text %3.0f %3.0f -text  \"%s\" -fill %s -justify center",
	  cv_name,x/100.*width,(100.-y)/100.*height,txt,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::text(float x,float y,string txt, int col){
  text_rel(x,y,txt,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::text(float x,float y,string text, const char * col){
  sprintf(command, 
	  "%s create text %3.0f %3.0f -text \"%s\" -fill %s -justify left",
	  cv_name,x,y,text.c_str(),col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::text_rel(float x,float y,string text, int col){
  text_rel(x,y,text,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::text_rel(float x,float y,string text, const char * col){
  sprintf(command, 
	  "%s create text %3.0f %3.0f -text  \"%s\" -fill %s -justify center",
	  cv_name,x/100.*width,(100.-y)/100.*height,text.c_str(),col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::point(float x,float y,char col){
 // empty
  cout << "point(" << x << "," << y << "," << col << ")\n";
}

//**************************************************************************

void Lisa_Canvas::circle(float x,float y,float rad,int  col) {
  circle(x,y,rad,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::circle(float x,float y,float rad,const char* col){
  sprintf(command, 
	  "%s create oval %3.0f %3.0f %3.0f %3.0f -fill white -outline %s -width 2",
	  cv_name,x-rad,y-rad,x+rad,y+rad,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::fil_circle(float x,float y,float rad,int  col) {
  fil_circle(x,y,rad,color[col%(MAX_CANV_COLORS)]);
}

//**************************************************************************

void Lisa_Canvas::fil_circle(float x,float y,float rad,const char* col){
  sprintf(command, 
	  "%s create oval %3.0f %3.0f %3.0f %3.0f -fill %s -outline %s -width 2",
	  cv_name,x-rad,y-rad,x+rad,y+rad,col,col);
  Tcl_Eval(cv_interp,command);
}

//**************************************************************************

void Lisa_Canvas::set_wh(int ext_width, int ext_height){
  sprintf(command, "%s config -scrollregion { 0 0 %d %d }",
	  cv_name,ext_width,ext_height);

  Tcl_Eval(cv_interp,command);
  width=ext_width;
  height=ext_height;
}

//**************************************************************************

int Lisa_Canvas::get_width() {
  return( (int) width);
}

//**************************************************************************

int Lisa_Canvas::get_height() {
  return( (int) height);
}

//**************************************************************************

Lisa_Canvas::~Lisa_Canvas(){
  // done  automaticly
}

//**************************************************************************

Lisa_TextObject::Lisa_TextObject(){
  cv_interp=NULL;
  cv_name = new char[200];
  command = "";
  // command = new char[246];
  sprintf(cv_name, "%s",".textaus.fra22.tex23");
 }

//**************************************************************************

Lisa_TextObject::Lisa_TextObject(Tcl_Interp * tclinterp){
  cv_name = new char[200];
  command = "";
  sprintf(cv_name,"%s",".textaus.fra22.tex23");
  cv_interp=tclinterp;
  width=500;
  height=500;
}

//**************************************************************************

Lisa_TextObject::Lisa_TextObject(const char * name,Tcl_Interp * tclinterp){
  cv_name = new char[200];
  command = "";
  sprintf(cv_name,"%s",name);
  cv_interp=tclinterp;
  width=500;
  height=500;
}

//**************************************************************************

void Lisa_TextObject::clear(){
  command= (string) cv_name+ " " + " delete 1.0 end";
  Tcl_Eval(cv_interp,(char*) command.c_str());
}

//**************************************************************************

void Lisa_TextObject::text(const char *text) {
 command= (string) cv_name+ " insert end { " + (string) text + " }"; 
   Tcl_Eval(cv_interp,(char*) command.c_str());
}

//**************************************************************************

void Lisa_TextObject::text(const string text){
  command= (string) cv_name+ " insert end { " + text + " }"; 
  Tcl_Eval(cv_interp,(char*) command.c_str());
}

//**************************************************************************

Lisa_TextObject::~Lisa_TextObject() {
  // nothing to do
}

//**************************************************************************

Lisa_Label::Lisa_Label(string name,Tcl_Interp * tclinterp) {
  label_interp=tclinterp;
  label_name=name;
}

//**************************************************************************

void Lisa_Label::set_text(string text) {
  string command=label_name+" config -text "+  text;
  Tcl_Eval(label_interp,(char*) command.c_str());

}

//**************************************************************************

Lisa_Label::~Lisa_Label() {
  // nothing to do
}

//**************************************************************************

TCGraphic::TCGraphic() {
main_canvas=canv_horizontal=canv_vertikal=NULL;
}

//**************************************************************************

void TCGraphic::clear() {
if ( main_canvas!=NULL) main_canvas->clear();
if (  canv_horizontal!=NULL) canv_horizontal->clear();
if (  canv_vertikal!=NULL)canv_vertikal->clear();
}

//**************************************************************************

void  TCGraphic::set_wh(int width,int height) {
  main_canvas->set_wh(width,height);
  canv_horizontal->set_wh(width,50);
  canv_vertikal->set_wh(50,height);
}

//**************************************************************************

TCGraphic::~TCGraphic() {
// delete main_canvas;
// delete canv_horizontal;
// delete canv_vertikal;
}

//**************************************************************************

TCTable::TCTable() {
   entrywidth= ENTRYWIDTH;
   entryheight=ENTRYHEIGHT ;
 }

//**************************************************************************

void TCTable::draw_table (int m, int n) {
  int i,j;
  int width,height,position;
  string str;
  
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
    canv_vertikal->text(25,entryheight*(j+0.5),(string) "J "+ ztos(j+1) ,"Black");
  }
  for(i=0;i<m;i++) {
    main_canvas->line(entrywidth*(i+1),1,entrywidth*(i+1),entryheight*n,"Grey");
    canv_horizontal->text(entrywidth*(i+0.5),25,"M "+ ztos(i+1) ,"Black");
  }   
  position= entrywidth*m;
}

//**************************************************************************

void TCTable::mark(int row, int column) {

	main_canvas->line(
		      entrywidth*(column)+1,
		      entryheight*(row),
		      entrywidth*(column)+1,
		      entryheight*(row+1),
		      "mark","BLACK");
	main_canvas->line(
		      entrywidth*(column+1)-1,
		      entryheight*(row),
		      entrywidth*(column+1)-1,
		      entryheight*(row+1),
		      "mark","BLACK");

	main_canvas->line(
		      entrywidth*(column),
		      entryheight*(row)+1,
		      entrywidth*(column+1),
		      entryheight*(row)+1,
		      "mark","BLACK");
	main_canvas->line(
		      entrywidth*(column),
		      entryheight*(row+1)-1,
		      entrywidth*(column+1),
		      entryheight*(row+1)-1,
		      "mark","BLACK");

}

//**************************************************************************

