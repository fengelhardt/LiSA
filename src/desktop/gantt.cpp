/*
 * ******************** gantt.cpp *******************************
 * 
 * description:   draw Gantt Chart in main window
 *
 * owner:         Ines Wasmund
 *
 * date:          08.03.1999
 *
 */

/** @version
 */ 

#include "../scheduling/xsched.hpp"
#include "../lisa/lvalues.hpp"
#include "../scheduling/schedule.hpp"
#include "../basics/matrix.hpp"
#include "../misc/int2str.hpp"
#include "../main/lisapref.hpp"
#include "../desktop/c_tcl.hpp"
#include "../desktop/basicwin.hpp"
#include "../misc/except.hpp"

#include "gantt.hpp"

const bool MACHINE=1;
const bool JOB=0;
const int BEGIN=0;
const int MIDDLE=1;
const int END=2;
const int LINE=3;

void TCGantt::init(Lisa_Values *G_Values, Lisa_Schedule *G_Schedule, 
		   bool orientation)
{
  int i1, i2, counter;
  
  number_y_values = G_Values->get_m();
  number_x_values = G_Values->get_n();
  if (orientation == JOB)
    {
      int tmp=number_x_values;
      number_x_values=number_y_values;number_y_values=tmp;
    }
  last_values = new Lisa_Vector<TIMETYP>(number_y_values); 
  y_geometry = new Lisa_Matrix<float>(number_y_values,4);
  for(i1=0;i1<number_y_values;i1++)
    {
      x_max_value=0;
      for(i2=0;i2<number_x_values;i2++)
	{
	  if (orientation == MACHINE)
	    {
	      if ( (*G_Schedule->CIJ)[i2][i1]>x_max_value)
		x_max_value = (*G_Schedule->CIJ)[i2][i1];
	    }
	  else
	    if ( (*(G_Schedule->CIJ))[i1][i2]>x_max_value)
	      x_max_value = (*(G_Schedule->CIJ))[i1][i2];
	}
      (*last_values)[i1]=x_max_value;
    }
  for(i1=0;i1<number_y_values;i1++)
    if((*last_values)[i1]>x_max_value)
      x_max_value = (*last_values)[i1];
  x_geometry=90/(x_max_value);
  x_extent=(double) mw_width()/main_canvas->get_width();
  set_y_geometry();
  counter = (int) main_canvas->get_width()/70;
  if(counter==0)
    counter=1;
  x_interval=scaling(x_max_value, counter);
  counter = (int) main_canvas->get_height()/45;
  if(counter==0)
    counter=1;
  y_interval=(int) scaling(number_y_values, counter);
  if(y_interval==0)
    y_interval=1;
}


void TCGantt::set_y_geometry()
{
  float row_height=85./(number_y_values), y1, y2, y3, y4;
  int canvas_height=main_canvas->get_height(), height=mw_height(), i1;
  
  y_extent= (double) height/canvas_height;
  for (i1=0;i1<number_y_values;i1++)
    {    
      y1=10.+(i1+1)*row_height;
      y2=10.+(i1)*row_height+100./canvas_height+row_height/20;
      y3=y1-100./canvas_height-row_height/20;
      y4=(y3-y2)/2+y2;
      (*y_geometry)[i1][LINE]=y1;
      (*y_geometry)[i1][BEGIN]=y2;
      (*y_geometry)[i1][END]=y3;
      (*y_geometry)[i1][MIDDLE]=y4-100./canvas_height;
    }
  y_frame_size=row_height/30;
  if (y_frame_size<500./canvas_height)
    y_frame_size=500./canvas_height;
  if (y_frame_size>1000./canvas_height)
    y_frame_size=1000./canvas_height;
  x_frame_size=y_frame_size*canvas_height/main_canvas->get_width();
  box_height=((*y_geometry)[0][END]-(*y_geometry)[0][BEGIN])/y_extent;
}


double TCGantt::scaling(TIMETYP final_value, int step_number)
{
  double curr_value=1, relation = final_value/step_number;
  int signum=1;
  
  if(final_value<0)
    {signum=-1;final_value*=-1;}
  if(step_number<1)
    step_number=1;
  relation = final_value/step_number;
  while(relation<1)
    {
      relation*=2;curr_value/=2;
      if(relation<1)
	{relation*=2.5;curr_value/=2.5;}
      if(relation<1)
	{relation*=2;curr_value/=2;}
    }
  while(relation>2)
    {
      relation/=2;curr_value*=2;
      if(relation>2)
	{relation/=2.5;curr_value*=2.5;}
      if(relation>2)
	{relation/=2;curr_value*=2;}
    }
  return(signum*curr_value);
}


int TCGantt::box_color(int i, int j, Lisa_Matrix<bool> *CP, bool orientation, 
		       int color_type, Lisa_Vector<int> *active_path)
{
  int color, i1;
  int special_color[5]={17,16,13,20,14};    //red, green, blue, brown, yellow
  
  if (color_type==GANTT_NORMAL)
    if (orientation == MACHINE)
      color=i;
    else
      color=j;
  else
    if (color_type==GANTT_CP)
      {
	if ( (*CP)[i][j]) 
	  color=17;                               // red
	else 
	  color=15;                                // grey
      }
    else
      {
	color=11;
	if(orientation==JOB)
	  i=j;
	for(i1=0;i1<5;i1++)
	  if (i+1 == (*active_path)[i1])
	    color=special_color[i1];
      }
  return(color);
}


void TCGantt::x_scaling()
{
  double d1;
  
  main_canvas->arrow_rel(0,10,90+6*x_extent,10,"black");
  main_canvas->text_rel(90+5*x_extent,10-3*y_extent,"t","black");
  main_canvas->text_rel(0.5*x_extent,10-3*y_extent,"0","black");
  canv_horizontal->text_rel(0.5*x_extent,50,"0","black");
  for (d1=x_interval;d1<x_max_value;d1+=x_interval)
    {
      main_canvas->line_rel(d1*x_geometry,10-y_extent,d1*x_geometry,
			    10+y_extent,"black");
      main_canvas->text_rel(d1*x_geometry,10-3*y_extent,ztos(d1),"black");
      canv_horizontal->text_rel(d1*x_geometry,50,ztos(d1),"black");
    }
  main_canvas->text_rel(x_max_value*x_geometry,10-6*y_extent,
			ztos(x_max_value),"black");
  canv_horizontal->text_rel(x_max_value*x_geometry,20,ztos(x_max_value),
			    "black");
  main_canvas->line_rel(x_max_value*x_geometry,10-2*y_extent,
			x_max_value*x_geometry,95+4*y_extent,
			"black");
}


void TCGantt::y_scaling(bool orientation)
{
  int i1;
  
  for (i1=y_interval-1;i1<number_y_values;i1+=y_interval)
    {
      if(orientation == MACHINE)
	canv_vertikal->text_rel(50,(*y_geometry)[i1][MIDDLE],
				(string) "M "+ ztos(i1+1),"black");
      else
	canv_vertikal->text_rel(50,(*y_geometry)[i1][MIDDLE],
				(string) "J "+ ztos(i1+1),"black");
    }
  main_canvas->line_rel(0,10-y_extent,0,95+3*y_extent,"black");  
}


void TCGantt::draw_x_lines()
{
  int i1;
  
  for (i1=0; i1< number_y_values; i1++)
    {
      main_canvas->line_rel(0, (*y_geometry)[i1][LINE],
			    (*last_values)[i1]*x_geometry,
			    (*y_geometry)[i1][LINE],"grey");  //color 15
      if(i1<number_y_values - 1)
	if((*last_values)[i1]<(*last_values)[i1+1])
	  main_canvas->line_rel((*last_values)[i1]*x_geometry,
				(*y_geometry)[i1][LINE],
				(*last_values)[i1+1]*x_geometry,
				(*y_geometry)[i1][LINE],"grey");  //color 15
    }
}


void TCGantt::draw_box(TIMETYP end, TIMETYP time, int x_value, int y_value, 
		       int color, char *text_color)
{
  double x1, x2, x3;

  x2=end*x_geometry;
  x1=x2-time*x_geometry;
  main_canvas->rect_rel(x1,(*y_geometry)[y_value][BEGIN],
			x2,(*y_geometry)[y_value][END],color);
  x3=(x2-x1)/2+x1;
  if(box_height>2) 
    if( main_canvas->get_width()*(x2-x1)/100 > (ztos(x_value+1).length())*8)
      main_canvas->text_rel(x3,(*y_geometry)[y_value][MIDDLE],
			    ztos(x_value+1),text_color);
}


void TCGantt::draw_DD(Lisa_Vector<TIMETYP> *DD)
{
  int i;
  double x1,x2,y1,y2;

  x1=((*DD)[0])*x_geometry-100./(main_canvas->get_width());
  x2=(*DD)[0]*x_geometry;
  y1=10-y_frame_size;
  y2=(*y_geometry)[0][LINE]+y_frame_size;
  main_canvas->rect_rel(x1,y1, x2,y2,"red");
  main_canvas->line_rel(x2, y1, x2, y2, "red");
  main_canvas->line_rel(x2-x_frame_size, y1, x2, y1, "red");
  main_canvas->line_rel(x2-x_frame_size, y2, x2, y2, "red");
  for(i=1;i<number_y_values;i++)
    {
      x1=((*DD)[i])*x_geometry-100./(main_canvas->get_width());
      x2=(*DD)[i]*x_geometry;
      y1=(*y_geometry)[i-1][LINE]-y_frame_size;
      y2=(*y_geometry)[i][LINE]+y_frame_size;
      main_canvas->rect_rel(x1,y1, x2,y2,"red");
      main_canvas->line_rel(x2, y1, x2, y2, "red");
      main_canvas->line_rel(x2-x_frame_size, y1, x2, y1, "red");
      main_canvas->line_rel(x2-x_frame_size, y2, x2, y2, "red");
    }
  
}


void TCGantt::draw_RD(Lisa_Vector<TIMETYP> *RD)
{
  int i;
  double x1,x2,y1,y2;
  
   if ((*RD)[0]!=0)
     {
       x1=(*RD)[0]*x_geometry;
       x2=((*RD)[0])*x_geometry+100./(main_canvas->get_width());
       y1=10-y_frame_size;
       y2=(*y_geometry)[0][LINE]+y_frame_size;
       main_canvas->rect_rel(x1, y1, x2, y2, "blue");
       main_canvas->line_rel(x1, y1, x1, y2, "blue");
       main_canvas->line_rel(x1, y1, x1+x_frame_size,y1,"blue");
       main_canvas->line_rel(x1, y2, x1+x_frame_size,y2,"blue");
     }
  for(i=1;i<number_y_values;i++)
    if ((*RD)[i]!=0)
      {
       x1=(*RD)[i]*x_geometry;
       x2=((*RD)[i])*x_geometry+100./ (main_canvas->get_width());
       y1=(*y_geometry)[i-1][LINE]-y_frame_size;
       y2=(*y_geometry)[i][LINE]+y_frame_size;
       main_canvas->rect_rel(x1, y1, x2, y2,"blue");
       main_canvas->line_rel(x1, y1, x1, y2, "blue");
       main_canvas->line_rel(x1,y1,x1+x_frame_size,y1,"blue");
       main_canvas->line_rel(x1, y2,x1+x_frame_size,y2,"blue");
    }
}  


bool TCGantt::check_x_value(Lisa_Values *G_Values, Lisa_Schedule *G_Schedule,
			    int x, int y, TIMETYP time)
{
  if ((*G_Values->SIJ)[x][y])
    {
      if (time<=(*G_Schedule->CIJ)[x][y])
	if (time >= (*G_Schedule->CIJ)[x][y]
	    - (*G_Values->PT)[x][y])
	  return(0);
    }
  return(1);
}



//---------------------------------------------------------------------------
//                              public:
//---------------------------------------------------------------------------


TCGantt::TCGantt(Tcl_Interp *interp,string canv,string horizontal_canvas, 
		 string vertikal_canvas)
{
  main_canvas =new Lisa_Canvas(canv.c_str(),interp); 
  canv_horizontal =new Lisa_Canvas(horizontal_canvas.c_str(),interp);
  canv_vertikal =new Lisa_Canvas(vertikal_canvas.c_str(),interp);
}


void TCGantt::draw(Lisa_Matrix<bool> *CP,
		   Lisa_Values *G_Values,
		   Lisa_Schedule *G_Schedule,
		   bool orientation, int color_type, 
		   Lisa_Vector<int> *active_path) 
{
  int i1, i2, color; char *text_color;

 
  if(zoom != TRUE)
    main_canvas->set_wh(mw_width(),mw_height()); 
  canv_vertikal->set_wh(40,main_canvas->get_height());
  canv_horizontal->set_wh(main_canvas->get_width(),40);
  main_canvas->clear();canv_horizontal->clear();canv_vertikal->clear();
  if(G_Schedule->valid != TRUE)
    {
      G_ExceptionList.lthrow("Schedule is not valid",ANY_ERROR);
      return;
    }
  
  
  /* Initialisation */
  
  init(G_Values, G_Schedule, orientation);
  
  
  /* Draw the Gantt-Chart */
  
  
  /* X-scaling */
  
  x_scaling();
  
  
  /* Y-scaling */
  
  y_scaling(orientation);
  
  
  /* horizontal lines */
  
  draw_x_lines();
  

  /* Draw the boxes */

  for (i1=0;i1<G_Values->get_n();i1++)
    for (i2=0;i2<G_Values->get_m();i2++)
      if ((*G_Values->SIJ)[i1][i2])
	{
	  color=box_color(i1, i2, CP, orientation, color_type, active_path);
	  if (color%23==1 || color%23==4 || color%23==6 || color%23==8 
	      || color%23==11 || color%23==20)
	    text_color="white"; 
	  else
	    text_color="black"; 
	  if (orientation == MACHINE)
	    draw_box((*G_Schedule->CIJ)[i1][i2], (*G_Values->PT)[i1][i2],
		     i1, i2, color, text_color);
	  else
	    draw_box((*G_Schedule->CIJ)[i1][i2], (*G_Values->PT)[i1][i2],
		     i2, i1, color, text_color);
	}


  /* Draw duedates and releasedates */

  if(orientation == JOB)
    {
      if((G_Values->DD)!=NULL)
	draw_DD((G_Values->DD));
      if((G_Values->RD)!=NULL)
	draw_RD((G_Values->RD));
    }
}


operation_data* TCGantt::get_data(double x_coordinate, double y_coordinate,
				  Lisa_Values *G_Values, 
				  Lisa_Schedule *G_Schedule, bool orientation)
{
  TIMETYP time=-1;
  int x_value=-1, y_value=-1, i, j;
  bool flag = 0;

  my_data.job=-1;
  my_data.machine=-1;
  my_data.time=-1;
  if(G_Schedule->valid != TRUE)
    {
      G_ExceptionList.lthrow("Schedule is not valid",ANY_ERROR);
      return(&my_data);
    }
  
  
  /* Initialisation */
  
  init(G_Values, G_Schedule, orientation);

  /* Calculation */

  time=x_coordinate/x_geometry;
  if (x_coordinate<=0 || x_coordinate>=90)
    time=-1;
  if (time>=0)
    {
      for (i=0; i<number_y_values && flag==0; i++)
	{
	  if((*y_geometry)[i][BEGIN] < y_coordinate)
	    {
	      if((*y_geometry)[i][END] > y_coordinate)
		flag=1;
	    }
	  else
	    {
	      flag=1;
	      i=-1;
	    }
	}
      y_value=i-1;
      if(flag==0)
	y_value=-1;
      if (y_value>=0)
	{
	  flag=1;
	  for (j=0;j<number_x_values && flag==1 ;j++)
	    if (orientation == MACHINE)
	      {
		flag = check_x_value(G_Values, G_Schedule, j, y_value, time);
	      }
	    else
	      {
		flag = check_x_value(G_Values, G_Schedule, y_value, j, time);
	      }
	  if (flag==1)
	    j=0;
	  x_value=j-1;
	}
    }
  if(time>=0 && x_value>=0 && y_value>=0)
    {
      my_data.time=time;
      if (orientation == MACHINE)
	{
	  my_data.job=x_value;
	  my_data.machine=y_value;
	}
      else
	{
	  my_data.job=y_value;
	  my_data.machine=x_value;
	}
    }
  return(&my_data);
}


void TCGantt::mark(int machine,int job,Lisa_Values *G_Values,
		   Lisa_Schedule *G_Schedule,bool orientation)
{
  int y_value;
  double x1,x2,x3,x4;
  double y1,y2,y3,y4;

  if(zoom != TRUE)
    main_canvas->set_wh(mw_width(),mw_height()); 
  canv_vertikal->set_wh(40,main_canvas->get_height());
  canv_horizontal->set_wh(main_canvas->get_width(),40);
  if(G_Schedule->valid != TRUE)
    {
      G_ExceptionList.lthrow("Schedule is not valid",ANY_ERROR);
      return;
    }
  
  
  /* Initialisation */
  
  init(G_Values, G_Schedule, orientation);
  if (orientation == MACHINE)
    {
      if (machine>=0 && machine<number_y_values)
	y_value=machine;
      else	
	{
	  G_ExceptionList.lthrow("Machinenumber is invalid",OUT_OF_RANGE);
	  return;
	}
      if (job<0 || job>=number_x_values)
	{
	  G_ExceptionList.lthrow("Jobnumber is invalid",OUT_OF_RANGE);
	  return;
	}
    }
  else
    {
      if (machine<0 || machine>=number_x_values)
	{
	  G_ExceptionList.lthrow("Machinenumber is invalid",OUT_OF_RANGE);
	  return;
	}
      if (job>=0 && job<number_y_values)
	y_value=job;
      else
	{
	  G_ExceptionList.lthrow("Jobnumber is invalid",OUT_OF_RANGE);
	  return;
	}
    }

  
  /* Mark a box */
     
  if ((*G_Values->SIJ)[job][machine])
    {
     x3=(*G_Schedule->CIJ)[job][machine]*x_geometry;
     x2=x3-(*G_Values->PT)[job][machine]*x_geometry;
     x4=x3+x_frame_size;
     x1=x2-x_frame_size;
     y2=(*y_geometry)[y_value][BEGIN];
     y3=(*y_geometry)[y_value][END];
     y4=y3+y_frame_size;
     y1=y2-y_frame_size;
     main_canvas->rect_rel(x1,y1,x4,y2,"black"); 
     main_canvas->rect_rel(x1,y3,x4,y4,"black"); 
     main_canvas->rect_rel(x1,y2,x2,y3,"black"); 
     main_canvas->rect_rel(x3,y2,x4,y3,"black"); 
    }
  return;
}


TCGantt::~TCGantt()
{
  delete last_values;
  delete y_geometry;
  delete main_canvas; 
  delete canv_horizontal;
  delete canv_vertikal;
}












