<GENERAL>
Name= Dispatching Rules   
Type= constructive
Code= external
Call= dispatch
Help= algorithm/dispatch.html
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
string ACTIVE ( FALSE  TRUE) "construct active schedule"
string RULE ( RAND FCFS EDD LQUE SPT WSPT ECT WI LPT ) "priority"
</PARAMETERS>

















