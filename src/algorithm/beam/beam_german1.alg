<GENERAL>
Name= Beam Search
Type= constructive
Call= beam
Code= external
Help= algorithm/beam.html
</GENERAL>

<EXACT>
</EXACT>

<HEURISTIC>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / Cmax }
</PROBLEMTYPE>

<PROBLEMTYPE>
Lisa_ProblemType= { O / / Cmax }
</PROBLEMTYPE>

<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / Cmax }
</PROBLEMTYPE>

<PROBLEMTYPE>
Lisa_ProblemType= { F / / Cmax }
</PROBLEMTYPE>

<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / Cmax }
</PROBLEMTYPE>
</HEURISTIC>



<PARAMETERS>
long K_BRANCHES 5 "Anzahl zu verfolgender Zweige"
string INS_ORDER ( MACHINEWISE DIAGONAL1 DIAGONAL2 RANDOM LPT ) "Einfuegereihenfolge"
string INS_METHOD ( INSERT1 INSERT2 ) "Auswahlmethode"
string CRITERION ( CMAX CLAST ) "Auswahlkriterium"
</PARAMETERS>