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
long K_BRANCHES 5 "Beamwidth"
string INS_ORDER ( MACHINEWISE DIAGONAL1 DIAGONAL2 RANDOM LPT ) "Insertion order"
string INS_METHOD ( INSERT1 INSERT2 ) "Choice method"
string CRITERION ( CMAX CLAST ) "Choice criterion"
</PARAMETERS>