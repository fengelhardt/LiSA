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
long NB_SOLUTIONS 1 "Anzahl der Lösungen"
double LOWER_BOUND 0 "Untere Schranke"
double UPPER_BOUND 1000000 "Obere Schranke"
string INS_ORDER ( RANDOM LPT ) "Einfügereihenfolge"
string BOUNDING ( EXTENDED NORMAL ) "Art der unteren Schranke"
</PARAMETERS>




