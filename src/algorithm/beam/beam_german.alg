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
Lisa_ProblemType= { 1 / / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { O / / Cmax }
</PROBLEMTYPE>
</HEURISTIC>

<PARAMETERS>
long K_BRANCHES 2 "Anzahl zu verfolgender Zweige"
string TREE_MODE ( INSERT1 INSERT2 ) "Auswahlrestriktion"
string INS_ORDER ( RANDOM LPT LINE_BY_LINE DIAGONAL) "Einfügereihenfolge"
</PARAMETERS>
