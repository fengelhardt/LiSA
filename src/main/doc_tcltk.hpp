/** @name Some Infos About The Tcl Tk Stuff

Since doc++ can only generate a documentation from c++ source code files, 
here's a short insight into the Tcl/Tk source. Nearly all files in the 
#LiSA/tcl# directory have been generated using visual tcl and edited 
afterwards.\\
To enable support for different languages LiSA uses the files 
#LiSA/tcl/language/<language>/main.tcl#.\\
To enable an interface to the C++ code C++ functions have been written, 
that look like Tcl commands. Their names start with #TC_# to differ them 
from Tcl functions.

The different Tcl files:\\

#canvdim.tcl#:\\
+ opens a window, in which the size of the canvas can be entered\\
+ calls the c++ function #TC_setvar(CANVGEO)#\\

#dia_class.tcl#:\\
+ used only by the classification routine so far\\
+ opens a textwindow\\
+ window name: #.textaus#\\

#dia_gantt.tcl#:\\
+ dialog window with various parameters for the gantt chart\\
+ C++ function: #TC_setvar()#\\
+ window name: #.gant#\\

#dia_prob.tcl#:\\
+ dialog window for editing the problem type, job and machine number, ...\\
+ C++ functions: #TC_setvar()#, #TC_getvar()#, #TC_set_tubel()# and #TC_set_prob#\\
+ window name: #.dprob#\\
+ to convert the tupelentrys defined in #global.hpp# the file #global.tcl# is used\\

#dialog.tcl#:\\
+ varios small dialog windows\\
+ window names: #.erzpt# and #.erzspal#\\
+ #.erzpt# is used for random generation of processing times\\
+ #.erzspal# is used for random generation of deadlines, release dates and weights\\
+ C++ functions: #TC_genpt()#, #TC_draw_dat()# and #TC_gen_column()#\\

#global.tcl#:\\
+ definition of the global #GLOB()# variable with entrys according to #global.hpp#\\
+ definition of the global #MACRO()# variable with the names from #global.hpp#\\
+ used in #dia_prob.tcl#\\

#main_win.tcl#:\\
+ opens the main window and the base window named #.#\\
+ window name: #.lisa#\\
+ C++ functions:  #TC_open()#, #TC_save()#, #TC_heuristic()#,  #TC_neighbor()#,
#TC_draw_output()#, #TC_getvar()#, #TC_draw_schedule()# and #TC_draw_dat()#\\
+ tcl functions:\\
- #lw_scv()#, #lw_sch()#: the scrollbars have to move two canvas, therefore 
these functions are needed\\
- #showseq{}#: opens the schedule window or destroys it if it exists already\\
- #showdat{}#: same for the value window\\
- #new_mn{}#: refresh the content of the schedule and values window\\
- #mw_no_problem{}#: make menuentrys unselectable which are useless without a
defined problem\\
- #mw_no_values{}#: same if no values are defined\\
- #mw_no_schedule{}#: and again if no schedule exists\\
- #problem_exist{}#, #values_exist{}# and #schedule_exist{}#: make the 
according menuentrys selectable again\\
- #mw_remove_unofficial_entrys{}#: does nothing so far\\
- #mw_set_label{}#: writes a message to the lower status bar\\

#message.tcl#:\\
+ opens a window for short notifications or error messages\\
+ window name: #.message#\\
+ function #showmessage{title text}# opens a #.message# window 
with the according title and text\\

#neighbor.tcl#:\\
+ dialogwindow for calling the neighbourhood algorithms\\
+ window name #.nachbar#\\

#schedule.tcl#:\\
+ enables in- and output of a schedule in a table\\
+ windowname: #.schedulewin#\\
+ c++ functions: #TC_draw_schedule()#, #TC_gen_seq()#, #TC_rev_seq()#, #TC_getvar()#, 
  #TC_mark_schedule_entry()#, #TC_setvar()# and  #TC_draw_output()#

#start.tcl#:\\
+ not directly used by LiSA but a helpfull coding tool, the LiSA desktop can be started 
without the underlying c++ program with #wish start.tcl#\\

#startbb.tcl#:\\
+ dialogwindow for starting the branch and bound algorithm\\
+ windowname: #.startbb#\\
+ c++ function: #TC_start_bb()#\\

#tclinit.tcl#:\\
+ contains the #Window{}# function\\
+ reads all tcl source files and displys the main window\\
+ #tclinit.tcl# is called from the c++ program as tcl source\\

#values.tcl#:\\
+ enables in- and output of vectors and matrices in #Lisa_Values# as a table\\
+ window name: #.datawin#\\
+ c++ functions: #TC_draw_dat()#, #TC_draw_output()#, #TC_getvar()#, #TC_mark_value_entry()#,
  #TC_set_pt()# and #TC_setvar()#\\ 

#main.tcl#:\\
+ located in #LiSA/tcl/language/german/main.tcl# (or #LiSA/tcl/language/english/main.tcl#)\\
+ definition of the global #Name# field with german or english names, messages, ...\\
+ all strings can be requested with #$Name(identifier)#\\

#messages.tcl#:\\
+ located in #LiSA/tcl/language/german/messages.tcl# (or #LiSA/tcl/language/english/messages.tcl#)\\
+ definition of the global #Name# field with german or english names, messages, ...\\
+ all strings can be requested with #$Name(identifier)#\\


*/
