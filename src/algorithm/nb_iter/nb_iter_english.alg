<GENERAL>
Name= Neighborhood Search
Type= iterative
Call= nb_iter
Code= external
Help= algorithm/nb_iter.html
</GENERAL>

<EXACT>
</EXACT>

<HEURISTIC>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J /  / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J /  / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J /  / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J /  / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J /  / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J /  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J /  / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J /  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumWiTi }
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

