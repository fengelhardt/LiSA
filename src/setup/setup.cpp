// LiSA setup program
// Copyright Lars Dornheim 2002
// E-Mail: eldeh@web.de
// WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

#include <iostream>
#include <unistd.h>
#include <libgen.h>

#include <tk.h>

int main(int argc, char* argv[])
     {

     char* TclFileName = "setup.tcl";

     char* dir = dirname(argv[0]);
     chdir(dir);

     Tcl_Interp* interp = Tcl_CreateInterp();

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

     if (Tcl_EvalFile(interp, TclFileName) != TCL_OK)
          {
          cerr << "error in \"" << TclFileName << "\": " << interp->result << endl;
          exit(1);
          }

     }
