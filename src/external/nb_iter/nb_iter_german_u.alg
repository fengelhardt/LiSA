<GENERAL>
Name= Nachbarschaftssuche
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
string NGBH ( API SHIFT CR_API BL_API CR_SHIFT BL_SHIFT 3_CR ) "Nachbarschaft"
string METHOD ( TS TA II SA ) "Suchmethode"
string TYPE ( ENUM RAND ) "Erzeugemodus"
long STEPS 5000 "Anzahl der Lösungen"
long NUMB_STUCKS 10000 "Abbruch nach Stillständen (Abbruchkriterium)";
double ABORT_BOUND 0 "Abbruchschranke (Abbruchkriterium)"
long PROB 12 "Anfangswahrscheinlichkeit (nur für SA / TA)"
long MAX_STUCK 30 "Maximale Stillstandsanzahl (nur für SA / TA)"
long TABULENGTH 40 "Tabulistenlänge (nur für TS)"
long NUMB_NGHB 40 "Anzahl der Nachbarn (nur für TS)"
</PARAMETERS>

