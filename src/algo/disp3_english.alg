<GENERAL>
Name= Active Schedule
Type= constructive
Code= internal
Call= dispatch
Help= dispatch.html
</GENERAL>

<HEURISTIC>
</HEURISTIC>

<EXACT>
<PROBLEMTYPE>
Lisa_ProblemType= { O / r_i, p_ij=1  / Cmax }
</PROBLEMTYPE>
</EXACT>

<PARAMETERS>
string RULE ( RAND FCFS ) "Priority rule"
</PARAMETERS>

<FIXED>
string ACTIVE TRUE
</FIXED>











