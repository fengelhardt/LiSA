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
Lisa_ProblemType= { F / / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { O / / Cmax }
</PROBLEMTYPE>
</HEURISTIC>

<PARAMETERS>
long K_BRANCHES 2 "Number of Branches"
string TREE_MODE ( INSERT1 INSERT2 ) "Treemode"
string INS_ORDER ( RANDOM LPT LINE_BY_LINE DIAGONAL) "Insertionorder"
string CRITERION ( CMAX CLAST ) "Choice criterion"
</PARAMETERS>
