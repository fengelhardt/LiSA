<GENERAL>
Name= Neighborhood Search
Type= iterative
Call= nb_iter
Code= external
Help= external/nb_iter.html
TKfile= nb_iter.tcl
TKfile_test= nb_iter_t.tcl
</GENERAL>

<EXACT>
</EXACT>

<HEURISTIC>
<PROBLEMTYPE>
Lisa_ProblemType= { 1 / r_i  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { O / r_i  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { J / r_i  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { 1 / r_i  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { O / r_i  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { J / r_i  / SumWiUi }
</PROBLEMTYPE>
</HEURISTIC>

<PARAMETERS>
long PROB 12 "Start Parameter"
long MAX_STUCK 30 "Increase After"
long TABULENGTH 40 "Tabu List Length"
long NUMB_NGHB 40 "Number of Neighboors"
long STEPS 5000 "Created Solutions"
long NUMB_STUCKS 10000 "Stuck Since";
double ABORT_BOUND 0 "Objective at Most"
string NGBH ( API SHIFT CR_API BL_API CR_SHIFT BL_SHIFT 3_CR ) "Neighborhood"
string METHOD ( TS TA II SA ) "Search Method"
string TYPE ( ENUM RAND ) "Create Neighbor"
</PARAMETERS>





