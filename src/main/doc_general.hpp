/** @name Compiling and Starting LiSA\\

In the following #LiSA# will be the location of the LiSA package,
#home# the home directory of the user.\\

1. Compile LiSA\\

The following commands should do that:\\

#cd LiSA/src#\\
#./configure#\\
#make compile#\\
#make install#\\

2. Start LiSA\\

To start LiSA call the shell script #LiSA/bin/Lisa#. The script will
copy the file #LiSA/data/default.lsa# to #home/.lisa# if it does not 
exist there. Then it will call #Main# with the parameter 
#home/.lisa/default.lsa#.\\

The LiSA main program was written using C++ and Tcl/Tk. To have a 
closer look how those languages work together check the files 
#LiSA/src/main/lisa.cpp# and #LiSA/src/desktop/c_tcl.cpp#.\\

The file #lisa.cpp# contains the #main()# function. The parameter 
contains the name and directory of LiSA's description file 
(#default.lsa#). It contains parameters describing the LiSA windows 
and the language in which LiSA will run. If that file can't be opened
the main program will exit.\\ Additionally a problemdefinition can be 
loaded at startup. The name of this file has to be in #default.lsa#
right after #string Startfile#. If such a file is given LiSA will try 
to open it. Then #Tk_myMain()# in #c_tcl.c# will be called.

#Tk_myMain()# creates the Tcl interpreter and calls #Tcl_myAppInit()#.
#Tcl_myAppInit()# then creates the interface between Tcl and C++ and 
calls the Tk main loop:

+ initialization of Tcl and Tk\\
#Tcl_Init()# and #TK_Init()#\\

+ give Tk the adresses of functions than can be called from Tk\\

+ link C++ variables with Tcl Variables\\
#TCL_LinkVar()#\\

+ call the Tcl file #LiSA/tcl/tclinit.tcl#\\
All further operations, like opening the main window and generating the menu
is completly done with the files located in #LiSA/tcl#.

+ main loop (#Tk_MainLoop()#)\\
When this is called C++ gives complete control to Tcl/Tk. The main loop 
will only be exited if exit is selected in the main window.


3. Global Variables\\

C++ funktions called from Tcl can't have any LiSA objects as parameters.
Therefore all functions need access to the main objects which are realised 
as global Variables:\\

+ #class Lisa_Problem G_Problem;# An object describing the current problemtype.\\

+ #class Lisa_Values G_Values;# Contains processing times, weights, 
release dates and so on.\\
       
+ #class Lisa_Schedule G_Schedule;# Contains the schedule, e.g. a 
solution for a scheduling problem.\\

+ #class Lisa_XSchedule G_XSchedule(&G_Schedule);# Is an extension of 
the schedule. It enables for example calculation the objective function.\\
 	 
+ #class Lisa_Preferences G_Preferences;# Contains system preferences.\\

+ #class Lisa_Status G_Status;# Contains information about the current status. 
For example open windows and so on.\\

+ #class Lisa_TCLVar G_TclVar;# Simplifies the communication with the tcl windows.\\	

+ #Tcl_Interp *interp;# A pointer to the Tcl interpreter.\\	
 
These global variables are not neccessarily needed to understand single program
parts. Only the following files are allowed to access them:

+ #lisa.cpp# Here they are created.\\

+ #LiSA/src/desktop/c_tcl.cpp# Needs mostly the tcl interpreter.\\

+ #LiSA/src/desktop/callback.cpp# All functions in this file are a starting
point for intern jobs. They are called as a reaction to various events (e.g. 
#new_mn()# if the machine or job number has changed).\\

+ #LiSA/src/desktop/tcl_c.cpp# Here are all C++ functions 
that are called as Tcl commands.

4.Programroutine

Since all C++ functions are only called from the Tcl/Tk windows, each aktion starts
from #LiSA/src/desktop/tcl_c.cpp#.

*/
