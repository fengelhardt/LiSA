<GENERAL>
Name= job2-module
Type= constructive
Call= job2
Code= external
Help= job2.html
</GENERAL>

<EXACT>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / p_ij=1  / Cmax }
</PROBLEMTYPE>
</EXACT>

<HEURISTIC>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / Cmax }
</PROBLEMTYPE>
</HEURISTIC>

<PARAMETERS>
string NAME ( was das soll ) "name"
double UPPER_BOUND 1000000 "Obere Schranke"
long NB_STEPS 1 "Anzahl der Schritte"
</PARAMETERS>

