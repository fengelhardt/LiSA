<GENERAL>
Name= Neighborhood Search
Type= iterative
Call= nb_iter
Code= external
Help= external/nb_iter.html
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
string NGBH ( API SHIFT CR_API BL_API CR_SHIFT BL_SHIFT 3_CR ) "Neighborhood"
string METHOD ( TS TA II SA ) "Search Method"
string TYPE ( ENUM RAND ) "Create Neighbor"
long STEPS 5000 "Created Solutions"
long NUMB_STUCKS 10000 "Stuck Since (Abort Criterion)";
double ABORT_BOUND 0 "Objective at Most (Abort Criterion)"
long PROB 12 "Start Parameter (Only for SA / TA)"
long MAX_STUCK 30 "Increase After (Only for SA / TA)"
long TABULENGTH 40 "Tabu List Length (Only for TS)"
long NUMB_NGHB 40 "Number of Neighbors (Only for TS)"
</PARAMETERS>

