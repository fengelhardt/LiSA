<GENERAL>
Name= Neighbourhood Search
Type= iterative
Call= nb_iter
Code= external
Help= algorithm/nb_iter.html
</GENERAL>

<EXACT>
</EXACT>

<HEURISTIC>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumWiTi }
</PROBLEMTYPE>
</HEURISTIC>

<PARAMETERS>
string NGBH ( API SHIFT ) "Neighbourhood"
string METHOD ( TabuSearch ThresholdAccepting IterativeImprovement SimulatedAnnealing ) "Search Method"
string TYPE ( ENUM RAND ) "Create Neighbour"
long STEPS 5000 "Created Solutions"
long NUMB_STUCKS 10000 "Stuck Since (Abort Criterion)";
double ABORT_BOUND 0 "Objective at Most (Abort Criterion)"
long PROB 12 "Start Parameter (Only for SA / TA)"
long MAX_STUCK 30 "Increase After (Only for SA / TA)"
long TABULENGTH 40 "Tabu List Length (Only for TS)"
long NUMB_NGHB 40 "Number of Neighbours (Only for TS)"
</PARAMETERS>

