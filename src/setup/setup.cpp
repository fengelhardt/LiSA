// LiSA setup program
// Copyright Lars Dornheim 2002
// E-Mail: eldeh@web.de
// WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

#include <iostream>
#include <string.h>
#include <unistd.h>

#include <tk.h>

int main(int argc, char* argv[])
     {

     char* tclFileName = "bin/setup.tcl";

     Tcl_Interp* interp = Tcl_CreateInterp();

     // Cygwin needs the following line due to a TCL bug therein
     Tcl_FindExecutable(argv[0]);

     if (Tcl_Init(interp) == TCL_ERROR)
          {
          cerr << "Tcl_Init failed: " << interp->result << endl;
          exit(1);
          }

     if (Tk_Init(interp) == TCL_ERROR)
          {
          cerr << "Tk_Init failed: " << interp->result << endl;
          exit(1);
          }

     char* cmd = strcat(strdup("file dirname "), argv[0]);

     if (Tcl_Eval(interp, cmd) == TCL_OK)
          {
          char* dir = strcat(strdup(interp->result), "/..");
          chdir(dir);
          }
     else
          {
          cerr << "error while executing \"" << cmd << "\": " << interp->result << endl;
          exit(1);
          }

     if (Tcl_EvalFile(interp, tclFileName) != TCL_OK)
          {
          cerr << "error in \"" << tclFileName << "\": " << interp->result << endl;
          exit(1);
          }

     }
