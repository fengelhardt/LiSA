<GENERAL>
Name= Nachbarschaftssuche
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
long PROB 12 "Anfangswahrscheinlichkeit"
long MAX_STUCK 30 "Maximale Anzahl der Stillstaende"
long TABULENGTH 40 "Tabulistenlaenge"
long NUMB_NGHB 40 "Anzahl der Nachbarn"
long STEPS 5000 "Anzahl der Loesungen"
long NUMB_STUCKS 10000 "Abbruch nach Stillstaenden";
double ABORT_BOUND 0 "Abbruchschranke"
string NGBH ( API SHIFT CR_API BL_API CR_SHIFT BL_SHIFT 3_CR ) "Nachbarschaft"
string METHOD ( TS TA II SA ) "Suchmethode"
string TYPE ( ENUM RAND ) "Erzeugemodus"
</PARAMETERS>





