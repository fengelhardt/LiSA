/*
 * ************** evaluate.cpp ********************************************
 * 
 * description:   realizes the evaluation of tables and the graphical 
 *		  representation of it 
 *
 * owner:         Ines Wasmund
 * 
 * date:          12.10.1999
*/

/** @version
 */ 


#include <fstream.h>
#include <math.h>
#include "../../lisa_dt/ctrlpara.hpp"
#include "evaluate.hpp"
#include "inout.hpp"
#include "../../misc/int2str.hpp"
#include "../../basics/except.hpp"


//-------------------------------------------------------------------------
//                         Local Constants
//-------------------------------------------------------------------------


const int EXP_VALUE=0, MINIMUM=1, MAXIMUM=2, VARIANCE=3, ST_DEVIATION=4;


//-------------------------------------------------------------------------


Tcl_Interp *interp;  
class Lisa_Canvas *HW_canv;
class correlationtable *mycorrelationtable;
class charactertable *mycharactertable;
class preference *mypreference;
class Lisa_ControlParameters *mycontrol;

//-------------------------------------------------------------------------


int main(int argc, char *argv[]) 
{ 
  G_ExceptionList.set_output_to_cerr();
  mypreference = new preference(argc, argv);
  Tk_myMain( argc, argv );
  delete mypreference;
}

//-------------------------------------------------------------------------

int Data_find(string str, istream & fein) 
{  
  string Compare;
  do 
    {
      fein >> Compare;
    } 
  while(Compare!=str&&Compare!="");
  if (Compare=="") return 0;
  return 1;
}

//-------------------------------------------------------------------------


int TC_Table(ClientData, Tcl_Interp *interp, int, char**)
{  
  mypreference->draw_type = "table";
  mypreference->draw(interp, MAINCANV, HORICANV, VERTCANV);
  return TCL_OK;
}


int TC_PointXY(ClientData, Tcl_Interp*, int, char**)
{ 
  mypreference->draw_type = "pointxy";
  mypreference->draw(interp, MAINCANV, HORICANV, VERTCANV);
  return TCL_OK;
}


int TC_export(ClientData, Tcl_Interp* interp, int, char**) 
{ 
  mypreference->export_to(interp, MAINCANV, HORICANV, VERTCANV);
  return TCL_OK;
}


int TC_characte(ClientData, Tcl_Interp*, int, char**)
{ 
  mypreference->values_type = "characteristica";
  mypreference->draw(interp, MAINCANV, HORICANV, VERTCANV);
  return TCL_OK;
}


int TC_confid(ClientData, Tcl_Interp*, int, char**)
{ 
  return TCL_OK;
}


int TC_corr(ClientData, Tcl_Interp*, int, char**)
{ 
  mypreference->values_type = "correlation";
  mypreference->draw(interp, MAINCANV, HORICANV, VERTCANV);
  return TCL_OK;
}


int TC_Bar2(ClientData, Tcl_Interp*, int, char**)
{ 
  mypreference->draw_type = "bar2";
  mypreference->draw(interp, MAINCANV, HORICANV, VERTCANV);
  return TCL_OK;
}


int TC_Bar3(ClientData, Tcl_Interp*, int, char**)
{ 
  return TCL_OK;
}



int TC_pie(ClientData, Tcl_Interp*, int, char**)
{ 
  return TCL_OK;
}


int TC_trend(ClientData, Tcl_Interp*, int, char**) 
{ 
  return TCL_OK;
}

//-------------------------------------------------------------------------
//                              preference
//-------------------------------------------------------------------------

preference::preference(int argc, char *argv[])
{ 
  if (argc <2) {cerr << "es muss ein Dateiname angegeben werden\n"; exit(1);}
  ifstream fein(argv[1]);
  mycontrol = new Lisa_ControlParameters;
  fein >> *mycontrol;
  number_original_columns = mycontrol->get_int("NB_COL");
  number_original_rows = mycontrol->get_int("NB_ROW");
  draw_type = "table";
  values_type = "characteristica";
  original_columns = new Lisa_Vector<string> (number_original_columns);
  original_values = new Lisa_Matrix<TIMETYP> (number_original_rows,
					    number_original_columns);
  if (Data_find(mycontrol->get_string("COL_NAME"),fein))
    {
      fein >> *original_columns;
    }
  else cerr << mycontrol->get_string("COL_NAME") << " nicht gefunden\n";
  if (Data_find(mycontrol->get_string("TAB_NAME"),fein))
  {
    fein >> *original_values;
  } 
  else cerr << mycontrol->get_string("TAB_NAME") << " nicht gefunden\n";
};


int preference::get_number_original_columns()
{
  return number_original_columns;
};


int preference::get_number_original_rows()
{
  return number_original_rows;
};


void preference::draw(Tcl_Interp *interp, string MC ,string HC, string VC)
{
  if(draw_type=="table")
    {
      if(values_type == "characteristica")
	{
	  mycharactertable = new charactertable(interp, MC, HC, VC);
	  mycharactertable->draw_table();
	  delete mycharactertable;
	}
      if(values_type == "correlation")
	{
	  mycorrelationtable = new correlationtable(interp, MC, HC, VC);
	  mycorrelationtable->draw_table();
	  delete mycorrelationtable;
	}
    }
  if(draw_type=="pointxy")
    {
      if(values_type == "characteristica")
	{
	  mycharactertable = new charactertable(interp, MC, HC, VC);
	  mycharactertable->draw_pointxy();
	  delete mycharactertable;
	}
      if(values_type == "correlation")
	{
	  mycorrelationtable = new correlationtable(interp, MC, HC, VC);
	  mycorrelationtable->draw_pointxy();
	  delete mycorrelationtable;
	}
    }
  if(draw_type=="bar2")
    {
      if(values_type == "characteristica")
	{
	  mycharactertable = new charactertable(interp, MC, HC, VC);
	  mycharactertable->draw_bar2();
	  delete mycharactertable;
	}
      if(values_type == "correlation")
	{
	  mycorrelationtable = new correlationtable(interp, MC, HC, VC);
	  mycorrelationtable->draw_bar2();
	  delete mycorrelationtable;
	}
    }
};


void preference::export_to(Tcl_Interp *interp, string MC, string HC, string VC)
{
  string type, file;

  type = Tcl_GetVar2(interp, "export", "doc", 0);
  file = Tcl_GetVar2(interp, "export", "filename", 0);
  if (file == "")
    file = "evaluate";
  if(type=="ps") 
    {
      if (file.find(".")<0)
	file = file + ".ps";
      string str=
	".menu.frame_for_all_canvas.fra32.cpd34.03 postscript -file " + file;
      Tcl_Eval(interp, (char*) str.c_str());
    }
  if (type=="latex")
    {
      if (values_type == "characteristica")
	{
	  mycharactertable = new charactertable(interp, MC, HC, VC);
	  mycharactertable->export_latex(file);
	  delete mycharactertable;
	};
      if (values_type == "confidence")
	{

	};
      if (values_type == "correlation")
	{
	  mycorrelationtable = new correlationtable(interp, MC, HC, VC);
	  mycorrelationtable->export_latex(file);
	  delete mycorrelationtable;
	};
    }
  if (type=="excel")
    {
      if (values_type == "characteristica")
	{
	  mycharactertable = new charactertable(interp, MC, HC, VC);
	  mycharactertable->export_excel(file);
	  delete mycharactertable;
	};
      if (values_type == "confidence")
	{

	};
      if (values_type == "correlation")
	{
	  mycorrelationtable = new correlationtable(interp, MC, HC, VC);
	  mycorrelationtable->export_excel(file);
	  delete mycorrelationtable;
	};
    };
};

preference::~preference()
{
  delete original_columns;
  delete original_values;
};

//-------------------------------------------------------------------------
//                              menudata
//-------------------------------------------------------------------------


bool menudata::str2bool(string convert)
{
  return (convert=="1" ? 1 : 0);
}


Lisa_Vector<bool>* menudata::get_columns
(Tcl_Interp *interp, Lisa_Vector<bool> *columns, char *structure, char *type)
{
  int column=0, i, buffer;
  string columnstring;

  columns->fill(0);
  columnstring = Tcl_GetVar2(interp, structure, type,0);
  for(i=0;i<columnstring.length();i++)
    {
      buffer = (int) columnstring[i];
      buffer-=48;
      if(buffer<10 && buffer>=0)
	{
	  column*=10;
	  column+=buffer;
	}
      else
	{
	  if(column!=0)
	    {
	      if(column <= mypreference->get_number_original_columns())
		(*columns)[column-1]=1;
	      column=0;
	    }
	}
    }    
  if(column!=0)   
    if(column <= mypreference->get_number_original_columns() && column>0)
      (*columns)[column-1]=1;
   return columns;
};


int menudata::count_bool_values(Lisa_Vector<bool> *values)
{
  int i, number=0;
  for(i=0;i<values->get_m();i++)
    if ( (*values)[i] == 1)
      number++;
  return number;
};


int menudata::get_column(Tcl_Interp *interp, char *structure, char *type)
{
  bool number_flag=FALSE,break_flag=FALSE;
  int column=0, i, buffer;
  string columnst;

  columnst = Tcl_GetVar2(interp, structure, type,0);
  for(i=0;i<columnst.length();i++)
    {
      buffer = (int) columnst[i];
      buffer-=48;
      if((buffer<10) && (buffer>=0))
	{
	  if(break_flag)
	    if(column > mypreference->get_number_original_columns())
	      {
	    	break_flag=FALSE;
	    	column=0;
	      }
	    else
	      G_ExceptionList.lthrow("Something is wrong with the input",
				     ANY_ERROR);
	  column*=10;
	  column+=buffer;
	  number_flag=TRUE;
	}
      else
      	if(number_flag)
      	  break_flag=TRUE;
    }
  if(column > mypreference->get_number_original_columns())
    G_ExceptionList.lthrow("Something is wrong with the input",ANY_ERROR);
  return column;
};

//-------------------------------------------------------------------------
//                              character
//-------------------------------------------------------------------------


character::character(Tcl_Interp *interp)
{ 
  columns = new Lisa_Vector<bool> 
    (mypreference->get_number_original_columns());
  characteristica = new Lisa_Vector<bool> (5);
  get_values(interp);
};

void character::get_values(Tcl_Interp *interp)
{
  (*characteristica)[EXP_VALUE] = 
    str2bool(Tcl_GetVar2(interp,"characte","cexpv",0));
  (*characteristica)[MINIMUM] = 
    str2bool(Tcl_GetVar2(interp,"characte","cmin",0));
  (*characteristica)[MAXIMUM] = 
    str2bool(Tcl_GetVar2(interp,"characte","cmax",0));
  (*characteristica)[VARIANCE] = 
    str2bool(Tcl_GetVar2(interp,"characte","cvar",0));
  (*characteristica)[ST_DEVIATION] = 
    str2bool(Tcl_GetVar2(interp,"characte","cstdev",0));
  radio_columns = Tcl_GetVar2(interp,"characte","rc",0);
  if(radio_columns=="all")
    columns->fill(1);
  else
    columns = get_columns(interp, columns, "characte", "columns");
}

character::~character()
{
  delete columns;
};


//-------------------------------------------------------------------------
//                             correlation
//-------------------------------------------------------------------------


correlation::correlation(Tcl_Interp *interp)
{
  columns1 = new Lisa_Vector<bool> 
    (mypreference->get_number_original_columns());
  columns2 = new Lisa_Vector<bool> 
    (mypreference->get_number_original_columns());
  get_values(interp);
};


void correlation::get_values(Tcl_Interp *interp)
{
  radio_columns1=Tcl_GetVar2(interp,"corr","rc1",0);
  radio_columns2=Tcl_GetVar2(interp,"corr","rc2",0);
  if (radio_columns1 == "all")
    columns1->fill(1);
  else
    columns1 = get_columns(interp, columns1, "corr", "columns1");
  if (radio_columns2 == "all")
    columns2->fill(1);
  else
    columns2 = get_columns(interp, columns2,  "corr", "columns2");
  radio_type = Tcl_GetVar2(interp,"corr","type",0);
};


correlation::~correlation()
{
  delete columns1;
  delete columns2;
};


//-------------------------------------------------------------------------
//                protected           table
//-------------------------------------------------------------------------


bool table::str2bool(string convert)
{
  return (convert=="1" ? 1 : 0);
}


int table::entry_length(string dec_point)
{
  int length=0, i,j, compare;

  for (i=0;i<number_rows;i++)
    if (((*row_names)[i]).length() > length)
      length = (((*row_names)[i]).length());
  for (j=0;j<number_rows;j++)
    if (((*column_names)[i]).length() > length)
      length = ((*column_names)[i]).length();
  for (i=0;i<number_rows;i++)
    for (j=0;j<number_rows;j++)
      if (dec_point==".")
	{
	  if (ztos((*table_values)[i][j]).length() > length)
	    length=ztos((*table_values)[i][j]).length();
	}
      else
	{
	  string str =ztos((*table_values)[i][j]); 
	  compare = str.length();
	  if (str.find("."))
	    compare = compare - 1 + dec_point.length();
	  if (compare > length)
	    length=compare;
	}
  return length;
};


//-------------------------------------------------------------------------
//                public              table
//-------------------------------------------------------------------------


void table::export_latex(string file)
{
  int i,j;

  if (file.find(".")<0)
    file = file + ".tex";
  ofstream *out = new ofstream((char*) file.c_str());
  *out << "\\documentclass{article}\n"
       << "\\begin{document}\n"
       << "\\begin{tabular}{|r||";
  for(i=0;i<number_columns;i++)
    *out << "c|";
  *out << "}\\hline\n";
  for(i=0;i<number_columns;i++)
    *out << " & " << (*column_names)[i];
  *out << "\\\\ \\hline \\hline \n";  
  for(i=0;i<number_rows;i++)
    {  
      *out << (*row_names)[i]; 
      for(j=0;j<number_columns;j++)
	*out << " & "<< (*table_values)[i][j];
      *out << "\\\\ \\hline \n";
    }
  *out << "\\end{tabular}\n"
       << "\\end{document}\n";
  delete out;
};


void table::export_excel(string file)
{
  string separator, dec_point, str;
  int length, i,j,k, find;
  bool tab;

  if (file.find(".")<0)
    file = file + ".xls";
  ofstream *out = new ofstream((char*) file.c_str());
  dec_point = Tcl_GetVar2(interp, "export", "dec_point", 0);
  if (dec_point=="")
    dec_point=".";
  separator = Tcl_GetVar2(interp, "export", "separator", 0);
  tab = str2bool(Tcl_GetVar2(interp,"export","ctab",0));
  if (separator=="" && tab==0)
    {
      length = entry_length(dec_point)+1;
      for(i=0;i<=length;i++)
	*out << " ";
      for (i=0;i<number_columns;i++)
	{
	  *out << (*column_names)[i];
	  for (j=(*column_names)[i].length();j<=length;j++)
	    *out << " ";
	}
      *out << "\n";
      for (i=0;i<number_rows;i++)
	{
	  *out << (*row_names)[i];
	  for (j=(*row_names)[i].length();j<=length;j++)
	    *out << " ";
	  for (j=0;j<number_columns;j++)
	    {
	      str = ztos((*table_values)[i][j]);
	      if (dec_point!=".")
		{
		  find = str.find(".");
		  if (find >= 0)
		    str.replace(find,1,dec_point);
		}
	      *out << str;
	      for (k=str.length();k<=length;k++)
		*out << " ";
	    }
	  *out << "\n";
	}
    }
  else
    {
      *out << separator;
      if (tab)
	*out << "	";
      else
	*out << " ";
      for (i=0;i<number_columns;i++)
	{
	  *out << (*column_names)[i] << separator;
	  if (tab)
	    *out << "	";
	  else
	    *out << " ";
	}
      *out << "\n";
      for (i=0;i<number_rows;i++)
	{
	  *out << (*row_names)[i] << separator;
	  if (tab)
	    *out << "	";
	  else
	    *out << " ";	  
	  for (j=0;j<number_columns;j++)
	    {
	      str = ztos((*table_values)[i][j]);
	      if (dec_point!=".")
		{
		  find = str.find(".");
		  if (find >= 0)
		    str.replace(find,1,dec_point);
		}
	      *out << str << separator;
	      if (tab)
		*out << "	";
	      else
		*out << " ";

	    }
	  *out << "\n";
	}
    }
  delete out;
};


//-------------------------------------------------------------------------
//                protected           graphic
//-------------------------------------------------------------------------


TIMETYP graphic::get_minimum()
{
  int i,j;
  TIMETYP value=0;

  for(i=0;i<number_rows;i++)
    for(j=0;j<number_columns;j++)
      if((*table_values)[i][j] < value)
	value = (*table_values)[i][j];
  return value;
};


TIMETYP graphic::get_maximum()
{
  int i,j;
  TIMETYP value=0;

  for(i=0;i<number_rows;i++)
    for(j=0;j<number_columns;j++)
      if((*table_values)[i][j] > value)
	value = (*table_values)[i][j];
  return value;
};


//-------------------------------------------------------------------------
//                public              graphic
//-------------------------------------------------------------------------
                               

graphic::graphic(Tcl_Interp *interp, string MC, string HC, string VC)
{
  entrywidth= ENTRYWIDTH;
  entryheight=ENTRYHEIGHT;
  main_canvas =new Lisa_Canvas(MC.c_str(),interp); 
  canv_horizontal =new Lisa_Canvas(HC.c_str(),interp);
  canv_vertikal =new Lisa_Canvas(VC.c_str(),interp);
  frame=10;
  x_number=0;
  y_value=0;
  width=800;
  height=600;
};


void graphic::init_canvas()
{
  main_canvas->set_wh(width,height);
  main_canvas->clear();
  canv_horizontal->set_wh(width,50);
  canv_horizontal->clear();
  canv_vertikal->set_wh(50,height);
  canv_vertikal->clear();
};


double graphic::scaling(TIMETYP final_value, int step_number)
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


bool graphic::init_scale(TIMETYP mini, TIMETYP maxi)
{
  minimum=mini;
  maximum=maxi;

  x_scale = (100.-2.*frame)/x_number;
  if (maximum>0)
    y_value+=maximum;
  if (minimum<0)
    y_value+=-minimum;
  if(y_value==0)
    {
      G_ExceptionList.lthrow("No values to draw",ANY_ERROR);
      return 1;
    }
  y_scale=(100-2*frame)/y_value;
  y_step=scaling((double) y_value,10);
  y0 = -minimum * y_scale + frame;
  return 0;
};


void graphic::error_set()
{
  init_scale(0,10);
  x_number=0;
};


void graphic::coordinate_system()
{
  int i;
  double d,x;
  string str;

  main_canvas->line_rel(frame/4*3,y0,100-frame/2,y0,"black");
  main_canvas->arrow_rel(frame,frame/2,frame,100-frame/2,"black");
  for(d=0; d>minimum;d-=y_step)
    {
      main_canvas->text_rel(frame/2,y0+d*y_scale,ztos(d),"black");
      canv_vertikal->text_rel(50,y0+d*y_scale,ztos(d),"black");
      main_canvas->line_rel(frame-.7,y0+d*y_scale,frame+.7,y0+d*y_scale,
			    "black");
    }
  for(d=0; d<maximum;d+=y_step)
    {
      main_canvas->text_rel(frame/2,y0+d*y_scale,ztos(d),"black");
      canv_vertikal->text_rel(50,y0+d*y_scale,ztos(d),"black");
      main_canvas->line_rel(frame-.7,y0+d*y_scale,frame+.7,y0+d*y_scale,
			    "black");
    }
  for(i=0,x=first_row_name;i<number_values;i++)
    {
      str=(*row_names)[i];
      main_canvas->text_rel(x,y0-frame/2,(char*) str.c_str(),"black");
      canv_horizontal->text_rel(x,50,(char*) str.c_str(),"black");
      x+= step_row_name;
    }
  x+=dist_between;
  for(;i<number_rows;i++)
    {
      str=(*row_names)[i];
      main_canvas->text_rel(x,y0-frame/2,(char*) str.c_str(),"black");
      canv_horizontal->text_rel(x,50,(char*) str.c_str(),"black");
      x+= step_row_name;
    }
};


void graphic::draw_table()
{
  int i,j;
  int width,height;
  string str;

  if (number_columns==0 || number_values==0)
    G_ExceptionList.lthrow("No values to draw",ANY_ERROR);    
  width=entrywidth * number_columns + 4*entrywidth;
  height=entryheight * number_rows + 2*entryheight;
  init_canvas();
  main_canvas->line(1,1,entrywidth*number_columns,1,"Grey");
  main_canvas->line(1,1,1,entryheight*number_rows,"Grey");
  for(j=0;j<number_rows;j++) 
    {
      main_canvas->line(1,entryheight*(j+1),entrywidth*number_columns,
			entryheight*(j+1),"Grey");  
      canv_vertikal->text(21,entryheight*(j+0.5),(*row_names)[j] ,"Black");
    }
  for(i=0;i<number_columns;i++) 
    {
      main_canvas->line(entrywidth*(i+1),1,entrywidth*(i+1),
			entryheight*number_rows,"Grey");
      canv_horizontal->text(entrywidth*(i+0.5),25,(*column_names)[i] ,
			    "Black");
    }   
  for(i=0;i<number_columns;i++) 
    {
      for(j=0;j<number_rows;j++) 
	main_canvas->text(entrywidth*(i+1)-entryheight,entryheight*(j+0.5),
			  ztos((*table_values)[j][i]),"Black");
    }
};


void graphic::draw_pointxy()
{
  bool error_flag;
  int color,i,j;
  double x,y;

  init_canvas();
  x_number=number_rows+1;
  if(number_rows>number_values)
    x_number+=2;
  error_flag = init_scale(get_minimum(), get_maximum());
  dist_between=2*x_scale;
  if(error_flag)
    error_set();
  else
    {
      x=frame+x_scale; 
      for(i=0;i<number_values;i++)
	{
	  for(j=0;j<number_columns;j++)
	    {
	      color=j%23;
	      y=(*table_values)[i][j]*y_scale+y0;
	      main_canvas->rect_rel(x-.5,y-.5,x+.5,y+.5,color);
	    }
	  x+=x_scale;
	}
      x+=dist_between;
      for(;i<number_rows;i++)
	{
	  for(j=0;j<number_columns;j++)
	    {
	      color=j%23;
	      y=(*table_values)[i][j]*y_scale+y0;
	      main_canvas->rect_rel(x-.5,y-.5,x+.5,y+.5,color);
	    }
	  x+=x_scale;
	}
    }
  first_row_name=frame+x_scale;
  step_row_name=x_scale;
  coordinate_system();
};


void graphic::draw_bar2()
{
  bool error_flag;
  int color,i,j;
  double x1,y2;
  
  init_canvas();
  x_number=(number_columns+1)*number_rows; 
  if(number_rows>number_values)
    x_number+=3;
  error_flag = init_scale(get_minimum(), get_maximum());
  dist_between=3*x_scale;
  if(error_flag)
    error_set();
  else
    {
      x1= frame + x_scale;
      for(i=0;i<number_values;i++)
	{
	  for(j=0;j<number_columns;j++)
	    {
	      color=j%23;
	      y2=(*table_values)[i][j]*y_scale+y0;
	      main_canvas->rect_rel(x1,y0,x1+x_scale,y2,color);
	      x1+=x_scale;
	    }
	  x1+=x_scale;
	}
      x1+=dist_between;
      for(;i<number_rows;i++)
	{
	  for(j=0;j<number_columns;j++)
	    {
	      color=j%23;
	      y2=(*table_values)[i][j]*y_scale+y0;
	      main_canvas->rect_rel(x1,y0,x1+x_scale,y2,color);
	      x1+=x_scale;
	    }
	  x1+=x_scale;
	}
    }
  first_row_name=frame+(number_columns/2.+1)*x_scale;
  step_row_name=(number_columns+1)*x_scale;
  coordinate_system();
};


graphic::~graphic()
{
  delete main_canvas; 
  delete canv_horizontal;
  delete canv_vertikal;
};

//-------------------------------------------------------------------------
//                               valuation
//-------------------------------------------------------------------------


valuation::valuation(Tcl_Interp *interp, string MC, string HC, string VC)
  :graphic(interp, MC, HC, VC)
{
}; 


void valuation::exp_values(int k, Lisa_Matrix <TIMETYP> *values_from,
			   Lisa_Matrix <TIMETYP> *values_to, int n_columns, 
			   int n_rows)
{
  int i,j;
  TIMETYP exp_v;
  
  for(i=0; i<n_columns; i++)
    {
      exp_v = 0;
      for(j=0; j<n_rows; j++)
	exp_v += (*values_from)[j][i];
      exp_v /= n_rows;
      (*values_to)[k][i] = exp_v;
    }
};


void valuation::variances(int k, int row_exp_values,
			  Lisa_Matrix <TIMETYP> *values_from,
			  Lisa_Matrix <TIMETYP> *exp_values_from,
			  Lisa_Matrix <TIMETYP> *values_to, int n_columns, 
			  int n_rows)
{
  int i,j;
  TIMETYP var;
  
  for(i=0; i<n_columns; i++)
    {
      var=0;
      for(j=0; j<n_rows; j++)
	var += pow((*values_from)[j][i]
		   -(*exp_values_from)[row_exp_values][i],2);
      var /= (n_rows-1);
      (*values_to)[k][i] = var;
    }
};


void valuation::st_deviations(int k, int row_variances,
			      Lisa_Matrix <TIMETYP> *variances_from,
			      Lisa_Matrix <TIMETYP> *values_to, int n_columns)
{
  int i;
  TIMETYP st_dev;

  for(i=0;i<n_columns;i++)
    {
      st_dev = sqrt ((*variances_from)[row_variances][i]);
      (*values_to)[k][i] = st_dev;
    }
};


//-------------------------------------------------------------------------
//                             charactertable
//-------------------------------------------------------------------------

charactertable::charactertable(Tcl_Interp *interp, string MC, string HC, string VC)
  :character(interp), valuation(interp, MC, HC, VC)
{
  number_columns = count_bool_values(columns);
  column_names = new Lisa_Vector<string> (number_columns);
  number_values = mypreference->get_number_original_rows();
  if(number_values == 0)
    {
      G_ExceptionList.lthrow("No values",ANY_ERROR);
      number_rows = 0;
    }
 else
   {
     number_rows = number_values + count_bool_values(characteristica);
     row_names = new Lisa_Vector<string> (number_rows);
     table_values = new Lisa_Matrix<TIMETYP> (number_rows,number_columns);
     init(interp);
   }
};


void charactertable::init(Tcl_Interp *interp)
{
  int i,j,k;

  for(j=0,k=0;j<mypreference->get_number_original_columns();j++)
    if((*columns)[j] == 1)
      {
	(*column_names)[k] = (*mypreference->original_columns)[j];
	for(i=0;i<number_values;i++)
	  (*table_values)[i][k] = (*mypreference->original_values)[i][j];
	k++;
      }
  row_names->fill("");
  k=number_values;
  if((*characteristica)[EXP_VALUE] == 1)
    {
      (*row_names)[k] = Tcl_GetVar2(interp,"Name","Exp_v",0);
      exp_values(k, table_values, table_values,number_columns, number_values);
      row_exp_values = k;
      k++;
    }
   if((*characteristica)[MINIMUM] == 1)
     {
       (*row_names)[k] = Tcl_GetVar2(interp,"Name","Min",0);
       minimums(k);
       k++;
     }
   if((*characteristica)[MAXIMUM] == 1)
     {
       (*row_names)[k] = Tcl_GetVar2(interp,"Name","Max",0);
       maximums(k);
       k++;
     }
   if((*characteristica)[VARIANCE] == 1)
     {
       (*row_names)[k] = Tcl_GetVar2(interp,"Name","Var",0);
       if((*characteristica)[EXP_VALUE] == 1)
	 variances(k, row_exp_values, table_values, table_values,
		   table_values, number_columns, number_values);
       else
	 {
	   store_exp_values = new Lisa_Matrix<TIMETYP> (1,number_columns);
	   exp_values(0, table_values, store_exp_values, number_columns, 
		      number_values); 
	   variances(k, 0, table_values, store_exp_values, table_values, 
		     number_columns, number_values);
	   delete store_exp_values;
	 }
       row_variances = k;
       k++;
     }
   if((*characteristica)[ST_DEVIATION] == 1)
     {
       (*row_names)[k] = Tcl_GetVar2(interp,"Name","St_dev",0);
       if((*characteristica)[VARIANCE] == 1)
	 st_deviations(k, row_variances, table_values, table_values, 
		       number_columns);
       else
	 {
	   store_variances = new Lisa_Matrix<TIMETYP> (1,number_columns);
	   if((*characteristica)[EXP_VALUE] == 1)
	     {
	       variances(0, row_exp_values, table_values, table_values, 
			 store_variances, number_columns, number_values);
	       st_deviations(k, 0, store_variances, table_values,
			     number_columns);
	     }
	   else
	     {
	       store_exp_values = new Lisa_Matrix<TIMETYP> (1,number_columns);
	       exp_values(0, table_values, store_exp_values, number_columns, 
			  number_values); 
	       variances(0, 0, table_values, store_exp_values,
			 store_variances, number_columns, number_values);
	       st_deviations(k,0,store_variances,table_values,number_columns);
	       delete store_exp_values;
	     }
	   delete store_variances;
	 }
       k++;
     }
};


void charactertable::minimums(int k)
{
  int i,j;
  TIMETYP min;
  
  for(i=0; i<number_columns; i++)
    {
      min = (*table_values)[0][i];
      for(j=1; j<number_values; j++)
	if ((*table_values)[j][i]<min)
	  min = (*table_values)[j][i];
      (*table_values)[k][i] = min;
    }
};


void charactertable::maximums(int k)
{
  int i,j;
  TIMETYP max;
  
  for(i=0; i<number_columns; i++)
    {
      max = (*table_values)[0][i];
      for(j=1; j<number_values; j++)
	if ((*table_values)[j][i]>max)
	  max = (*table_values)[j][i];
      (*table_values)[k][i] = max;
    }
};

charactertable::~charactertable()
{
  delete column_names;
  delete row_names;
  delete table_values;
};


//-------------------------------------------------------------------------
//                            correlationtable
//-------------------------------------------------------------------------


correlationtable::correlationtable(Tcl_Interp *interp, string MC, string HC,
				   string VC)
  :correlation(interp), valuation(interp, MC, HC, VC)
{
  number_rows = count_bool_values(columns1);
  number_columns = count_bool_values(columns2);
  number_original_columns = mypreference->get_number_original_columns();
  number_values = number_rows;
  row_names = new Lisa_Vector<string> (number_rows);
  column_names = new Lisa_Vector<string> (number_columns);
  table_values = new Lisa_Matrix<TIMETYP> (number_rows,number_columns);
  store_exp_values = new Lisa_Matrix<TIMETYP> (1, number_original_columns);
  if(radio_type == "corrc")
    {
      store_variances = new Lisa_Matrix<TIMETYP> (1, number_original_columns);
      store_st_deviations = new Lisa_Matrix<TIMETYP> 
	(1, number_original_columns);
      store_covariance =  new Lisa_Matrix<TIMETYP> 
	(number_rows, number_columns);
    }
  init();
};


void correlationtable::init()
{
  int i,k;

  for (i=0,k=0; i<number_original_columns; i++)
    {
      if((*columns1)[i] == 1)
	{
	  (*row_names)[k] = (*mypreference->original_columns)[i];
	  k++;
	}
    }
  for (i=0,k=0; i<number_original_columns; i++)
    {     
      if((*columns2)[i] == 1)
	{
	  (*column_names)[k] = (*mypreference->original_columns)[i];
	  k++;
	}
    }
  exp_values(0, mypreference->original_values, store_exp_values, 
	     number_original_columns, 
	     mypreference->get_number_original_rows());
  if(radio_type == "cov")
    covariance(table_values, mypreference->get_number_original_rows());
  else
    {
      variances(0, 0, mypreference->original_values, store_exp_values, 
		store_variances, number_original_columns, 
		mypreference->get_number_original_rows());
      st_deviations(0, 0, store_variances, store_st_deviations,
		    number_original_columns);
      covariance(store_covariance,mypreference->get_number_original_rows()-1);
      corr_coeffi();
    }
};


void correlationtable::covariance(Lisa_Matrix<TIMETYP> *where_to, int divisor)
{
  int i,j,k, i1, j1;
  TIMETYP cov;

  for(i=0, i1=0; i<number_original_columns; i++)
    if((*columns1)[i] == 1)
      {
	for(j=0, j1=0; j<number_original_columns; j++)
	  if((*columns2)[j] == 1)
	    {
	      cov = 0;
	      for(k=0;k<mypreference->get_number_original_rows();k++)
		cov += ((*mypreference->original_values)[k][i] 
			- (*store_exp_values)[0][i])
		  * ((*mypreference->original_values)[k][j] 
		     - (*store_exp_values)[0][j]); 
	      cov /= divisor;
	      (*where_to)[i1][j1] = cov;
	      j1++;
	    }
	i1++;
      }
};


void correlationtable::corr_coeffi()
{
  int i,j,i1,j1;
  
  for(i=0,i1=0; i<number_original_columns; i++)
    if((*columns1)[i] == 1)
      {
	for(j=0,j1=0; j<number_original_columns; j++)
	  {
	    if((*columns2)[j] == 1)
	      {
		(*table_values)[i1][j1] = (*store_covariance)[i1][j1]
		  /((*store_st_deviations)[0][i]
		    *(*store_st_deviations)[0][j]);
		j1++;
	      }
	  }
	i1++;
      }
};


correlationtable::~correlationtable()
{
  delete row_names;
  delete column_names;
  delete table_values;
  delete store_exp_values;
  if(radio_type == "corrc")
    {
      delete store_variances;
      delete store_st_deviations;
      delete store_covariance;
    }
};





