<GENERAL>
Name= Nachbarschaftssuche
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
string NGBH ( API SHIFT ) "Nachbarschaft"
string METHOD ( TabuSearch ThresholdAccepting IterativeImprovement SimulatedAnnealing ) "Suchmethode"
string TYPE ( ENUM RAND ) "Erzeugemodus"
long STEPS 5000 "Anzahl der Lösungen"
long NUMB_STUCKS 10000 "Abbruch nach Stillständen (Abbruchkriterium)";
double ABORT_BOUND 0 "Abbruchschranke (Abbruchkriterium)"
long PROB 12 "Anfangswahrscheinlichkeit (nur für SA / TA)"
long MAX_STUCK 30 "Maximale Stillstandsanzahl (nur für SA / TA)"
long TABULENGTH 40 "Tabulistenlänge (nur für TS)"
long NUMB_NGHB 40 "Anzahl der Nachbarn (nur für TS)"
</PARAMETERS>

