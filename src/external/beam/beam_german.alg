<GENERAL>
Name= Beam Search
Type= constructive
Call= beam
Code= external
Help= main.htm
</GENERAL>

<EXACT>
</EXACT>

<HEURISTIC>
<PROBLEMTYPE>
Lisa_ProblemType = { O / / Cmax }
</PROBLEMTYPE>
</HEURISTIC>

<PARAMETERS>
long K_BRANCHES 2 "Anzahl zu verfolgender Zweige"
string TREE_MODE ( INSERT1 INSERT2 ) "Auswahlrestriktion"
string INS_ORDER ( RANDOM LPT ) "Einfügereihenfolge"
</PARAMETERS>
