<GENERAL>
Name= Aktiver Plan
Type= constructive
Code= external
Call= dispatch
Help= algorithm/dispatch.html
</GENERAL>

<HEURISTIC>
</HEURISTIC>

<EXACT>
<PROBLEMTYPE>
Lisa_ProblemType= { O / r_i, p_ij=1  / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE>
Lisa_ProblemType= { O / p_ij=1  / Cmax }
</PROBLEMTYPE>
</EXACT>

<PARAMETERS>
string RULE ( RAND FCFS ) "Prioritätsregel"
</PARAMETERS>

<FIXED>
string ACTIVE TRUE
</FIXED>











