/*
 * ************** inout.cpp **********************************************
 * 
 * description:   Dient dazu, in einem externen Modul Ein- und Ausgaben 
 *                auf dem Bildschirm realisieren zu koennen. 
 *
 * owner:         Ines Wasmund
 *
 * date:          10.03.1999
*/ 


#include "inout.hpp"
#include "../../desktop/basicwin.hpp"
#include "evaluate.hpp"


/// der Tcl-Interpreter
extern Tcl_Interp *interp;
/// Das Hauptfenster
extern class Lisa_Canvas *HW_canv;
/// Variablen (vor allem zur Bildschirmsteuerung)  
extern class TCLVar C_Tcl;
extern class correlation *mycorrelation;
extern class character *mycharacter;


int Tk_myMain(int argc, char **argv) {
  interp = Tcl_CreateInterp();
  if (  Tcl_myAppInit( argc, argv, interp ) != TCL_OK ) 
    {
      fprintf( stderr, "\nUnexpected application error!" );
    }
  return 0;
}


int Tcl_myAppInit(int, char**, Tcl_Interp *interp) 
{
  string      LISA_HOME,file;
  int         retcode;
  Tk_Window   mainWindow;
  
  if( Tcl_Init(interp) == TCL_ERROR ) 
    {
      fprintf(stderr, "Tcl_Init failed: %s\n", interp->result);
      exit(1);
    }
  
  if( Tk_Init(interp) == TCL_ERROR ) 
    {
      fprintf(stderr, "Tk_Init failed: %s\n", interp->result);
      exit(1);
    }
  
  // Commands, which can be called in Tcl/Tk:
  
  Tcl_CreateCommand(interp, "TC_Table", TC_Table, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateCommand(interp, "TC_PointXY", TC_PointXY, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
 
  Tcl_CreateCommand(interp, "TC_export", TC_export, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
 
  Tcl_CreateCommand(interp, "TC_characte", TC_characte, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
 
  Tcl_CreateCommand(interp, "TC_confid", TC_confid, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
 
  Tcl_CreateCommand(interp, "TC_corr", TC_corr, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  
  Tcl_CreateCommand(interp, "TC_Bar2", TC_Bar2, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
 
  Tcl_CreateCommand(interp, "TC_Bar3", TC_Bar3, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
 
  Tcl_CreateCommand(interp, "TC_pie", TC_pie, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);
  
  Tcl_CreateCommand(interp, "TC_trend", TC_trend, (ClientData) 
 		    Tk_MainWindow(interp),(Tcl_CmdDeleteProc *)NULL);



  LISA_HOME = Tcl_GetVar2(interp,"env","LISAHOME",0);
  mainWindow = Tk_MainWindow(interp);
  
  file=LISA_HOME+ "/tcl/external/evaluate/menu.tcl";
  retcode = Tcl_EvalFile( interp, (char*) file.c_str() );
  
  if(retcode != TCL_OK) 
    {
      fprintf(stderr, "\nGUI: Initialization ERROR: ");
      fprintf(stderr, "%s\n", interp->result);
      exit(1);
    }
  Tk_MainLoop(); 
  Tcl_Eval(interp, "exit");
  return 0;  
}

int tcl_hwin(Tcl_Interp *interp)
{
  Tcl_Eval(interp,"source tclinit.tcl");
  return TCL_OK;
}





