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
</HEURISTIC>

<PARAMETERS>
string NGBH ( CR_TST API SHIFT CR_API BL_API CR_SHIFT BL_SHIFT 3_CR ) "Nachbarschaft"
string METHOD ( TabuSearch ThresholdAccepting IterativeImprovement SimulatedAnnealing ) "Suchmethode"
string TYPE ( ENUM RAND ) "Erzeugemodus"
long STEPS 5000 "Anzahl der Loesungen"
long NUMB_STUCKS 10000 "Abbruch nach Stillstaenden (Abbruchkriterium)";
double ABORT_BOUND 0 "Abbruchschranke (Abbruchkriterium)"
long PROB 12 "Anfangswahrscheinlichkeit (nur fuer SA / TA)"
long MAX_STUCK 30 "Maximale Stillstandsanzahl (nur fuer SA / TA)"
long TABULENGTH 40 "Tabulistenlaenge (nur fuer TS)"
long NUMB_NGHB 40 "Anzahl der Nachbarn (nur fuer TS)"
</PARAMETERS>

