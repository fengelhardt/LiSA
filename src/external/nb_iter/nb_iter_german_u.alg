<GENERAL>
Name= Neighborhood_Search
Type= iterative
Call= nb_iter
Code= external
Help= german_u/neighbor.html
TKfile= neighbor.tcl
TKfile_test= ngbhptst.tcl
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
long PROB 12 "Anfangswahrscheinlichkeit"
long MAX_STUCK 30 "Maximale Anzahl der Stillstände"
long TABULENGTH 40 "Tabulistenlänge"
long NUMB_NGHB 40 "Anzahl der Nachbarn"
long STEPS 5000 "Anzahl der Lösungen"
long NUMB_STUCKS 10000 "Abbruch nach Stillständen";
double ABORT_BOUND 0 "Abbruchschranke"
string NGBH ( API SHIFT CR_API BL_API CR_SHIFT BL_SHIFT 3_CR ) "Nachbarschaft"
string METHOD ( TS TA II SA ) "Suchmethode"
string TYPE ( ENUM RAND ) "Erzeugemodus"
</PARAMETERS>





