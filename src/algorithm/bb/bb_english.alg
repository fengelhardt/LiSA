<GENERAL>
Name= Branch & Bound
Type= constructive
Call= bb
Code= external
Help= algorithm/bb.html
</GENERAL>

<EXACT>
<PROBLEMTYPE>
Lisa_ProblemType= { O / r_i  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { J / r_i  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { O / r_i  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { J / r_i  / SumWiUi }
</PROBLEMTYPE>
</EXACT>

<HEURISTIC>
</HEURISTIC>

<PARAMETERS>
long NB_SOLUTIONS 1 "Number of solutions"
double LOWER_BOUND 0 "Lower bound"
double UPPER_BOUND 1000000 "Upper bound"
string INS_ORDER ( RANDOM LPT ) "Insertion order"
string BOUNDING ( EXTENDED NORMAL ) "Bounding"
</PARAMETERS>




