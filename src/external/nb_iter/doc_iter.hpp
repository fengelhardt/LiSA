/** @name To Call
To call the main program, use:\\
\\
#nb_iter [input file] [output file]#\\
\\
The program is controlled by an input-file, which contains the parameters for 
the iteration.\\
If the output-file exists, it will be overwrite, else it will be create.\\
\\
When you work with the LiSA-mainprogram, the input-file will be create 
automaticly. 
*/
//@{
//@}


/** @name Parameters in the Input-File and Output-File
The input-file must have the following structure:\\
\\
#<PROBLEMTYPE>#\\
#Lisa_ProblemType= { alpha / betta / gamma }#\\
#<\PROBLEMTYPE>#\\

#<CONTROLPARAMETERS>#\\
	// number of considered problems\\
#long   NUMB_PROBLEMS [ int-Wert > 0 ]#\\
	// number of schedules per problem\\
#long   NUMB_PLANS    [ int-Wert > 0 ]#\\
	// name of the Neighborhood\\
#String NGBH          [ M1_API, M1_SHIFT, OSP_API, OSP_SHIFT, OSP_3_API, OSP_CR_API, OSP_CR_SHIFT ]#\\
	// name of the search method\\
#String METHOD 	     [ II, SA, TA, TS ]#\\
	// propability for SA or treshold value for TA\\
#long   PROB 	     [ int-Wert aus [0..100] ]#\\
	// restart value for SA or TA\\
#long   RESTARTS      [ int-Wert > 0 ]#\\
	// tabulist-length for TS\\
#long   TABULENGTH    [ int-Wert > 0 ]#\\
	// number of considered neighbors for TS\\
#long   NUMB_NGHB     [ int-Wert > 0 ]#\\
	// enumerative or random search\\
#String TYPE 	     [ ENUM, RAND ]#\\
	// number of generated neighbors\\
#long   STEPS 	     [ int-Wert > 0 ]#\\
	// max number of stucks (not necessary to give)\\
#long   NUMB_STUCKS   [ int-Wert > 0 ]#\\
	// abort-value for the objective (not necessary to give)\\
#float  ABORT_BOUND   [ float-Wert > 0 ]#\\
#<\CONTROLPARAMETERS>#\\
\\
#<VALUES>#\\
...\\
	//see class Lisa-Values\\
...\\
#<\VALUES>#\\
\\
#<SCHEDULE># ... #<\SCHEDULE>#\\
...	corresponding to the number of schedules per problem (see Lisa_Schedule)\\
#<SCHEDULE># ... #<\SCHEDULE>#\\
\\
...     and so on, corresponding to the number of problems\\
\\
\\
to the methods:\\
\\
- #II# (iteartiv improvement) requires only #TYPE# \\
- #SA# (simulated annealing) requires #PROB#, #RESTARTS# and #TYPE#\\
- #TA# (threshold accepting) requires #PROB#, #RESTARTS# and #TYPE#\\
- #TS# (tabu search) requires #TABULENGTH#, #NUMB_NGHB# and #TYPE#\\
\\
the names of the neighborhoods:\\
\\
  #M1_API#: API_Neighborhood \\
  #M1_SHIFT#: shift_Neighborhood\\
  #OSP_API#: OSHOP_API_Ngbh\\
  #OSP_SHIFT#: OSHOP_shift_Ngbh\\
  #OSP_3_API#: OSHOP_3_API_Ngbh\\
  #OSP_3_CR#: OSHOP_3_CR_Ngbh\\
  #OSP_CR_API#: OSHOP_cr_API_Ngbh\\
  #OSP_BL_API#: OSHOP_cr_bl_API_Ngbh\\
  #OSP_CR_SHIFT#: OSHOP_cr_shift_Ngbh\\
  #OSP_BL_SHIFT#: OSHOP_cr_bl_shift_Ngbh\\
  #OSP_CR_TST#: OSHOP_cr_TST_Ngbh\\
  #JSP_API#: JSHOP_API_Ngbh\\
  #JSP_SHIFT#: JSHOP_shift_Ngbh\\
  #JSP_3_API#: JSHOP_3_API_Ngbh\\
  #JSP_CR_API#: JSHOP_cr_API_Ngbh\\
  #JSP_BL_API#: JSHOP_cr_bl_API_Ngbh\\
  #JSP_CR_SHIFT#: JSHOP_cr_shift_Ngbh\\
  #JSP_BL_SHIFT#: JSHOP_cr_bl_shift_Ngbh\\
  #JSP_3_CR#: JSHOP_3_CR_Ngbh\\
\\
The output file has the same structure like the input file. These file now 
contains the calculated schedules.\\
\\
When the neighborhood iteration is called by LiSA, then the input file will be
 created automaticly by LiSA. 
*/
//@{
//@}
/** @name Example for an Input File
An example for an open shop problem. Objective function is #Cmax#.\\
\\
#<PROBLEMTYPE>#\\
#Lisa_ProblemType= { O /  / Cmax }#\\
#<\PROBLEMTYPE>#\\
\\
#<CONTROLPARAMETERS>#\\
#long   NUMB_PROBLEMS 1#\\
#long   NUMB_PLANS 1#\\
#string NGBH OSP_CR_API#\\
#string METHOD SA#\\
#long   PROB 30#\\
#long   RESTARTS 30#\\
#long   TABULENGTH 5#\\
#long   NUMB_NGHB 10#\\
#string TYPE RAND#\\
#long   STEPS 200#\\
#<\CONTROLPARAMETERS>#\\
\\
#<VALUES>#\\
#n= 4#\\
#m= 4#\\
#PT= {#\\
# {  3  6  0  7 }#\\
# {  9  0  5  4 }#\\
# {  0  1  5  0 }#\\
# {  5  0  5  3 }#\\
#}#\\
#SIJ= {#\\
# { 1 1 0 1 }#\\
# { 1 0 1 1 }#\\
# { 0 1 1 0 }#\\
# { 1 0 1 1 }#\\
#}#\\
#<\VALUES>#\\
\\
#<SCHEDULE>#\\
#m= 4#\\
#n= 4#\\
#LR= {#\\
# { 2 4 0 5 }#\\
# { 3 0 1 6 }#\\
# { 0 6 4 0 }#\\
# { 1 0 2 8 }#\\
#}#\\
#<\SCHEDULE>#\\
*/
//@{
//@}
/** @name To Insert a New Neighborhood or a New Schedule
First: To insert a new Neighborhood\\
-----------------------\\
\\
Every Neighborhood must be an inherit of the class #Lisa_Neighborhood#.\\
\\
To insert a new Neighborhood you must make some variances in the following 
files : \\
\\
in #source/external/neighbor/nb_iter.hpp# :\\
- insert  \##include "file.hpp"#\\
- insert #const int NAME = CONSTANT#\\
- insert a definition of a name of an instance of the new neighborhood class\\
\\
in #source/external/neighbor/nb_iter.cpp# :\\
- in one of the procedures #one_mach_iter#, #osp_iter# or #jsp_iter# (or in additional new procedures) according to the problem-structure of the neighborhood :\\
- - insert the initialization of the instance of the neighborhood class\\
- - insert the return of the schedule and the kill of the class instance\\
- in the procedure #main# :\\
- - insert a request of #NGBH_St#\\
- - insert the assignment of #art_of_problem#\\
\\
in #tcl/language/german;german_u;english/neighbor.tcl# :\\
- insert a definition of a name for the neighborhood\\
\\
in #tcl/dia_ngbh.tcl# :\\
- insert an additional menu item by #..NGBH..#\\
\\
in #tcl/ngbhptst.tcl# :\\
- insert an additional menu item by #..NGBH..#\\
\\
in #source/external/neighbor/Make.List# :\\
- insert the neighborhood\\
\\
Second: To insert a new schedule\\
----------------------\\
\\
You should define a problem class for your schedule class. Both classes should have the same structure like the other classes.\\
For usage the new class in neighborhoodsearch you must make the following modifications:\\
\\
in #source/external/neighbor/nb_iter.hpp# :\\
- insert  \##include "file.hpp"#\\
- insert #const int NAME = CONSTANT#\\
- insert a definition of names for instance of the new problem class and schedule class\\
\\  
in #source/external/neighbor/nb_iter.cpp# :\\
- you must insert an analogical procedure for the new schedule-typ like the procedures #one_mach_iter#, #osp_iter# or #jsp_iter#\\
- in procedure #main# you must insert a branching into the new procedure dependent on #art_of_problem#\\
in #source/external/neighbor/Make.List# :\\
- insert the schedule class\\
*/
//@{
//@}











