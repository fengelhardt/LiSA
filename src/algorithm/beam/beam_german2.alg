<GENERAL>
Name= Beam Search
Type= constructive
Call= beam
Code= external
Help= algorithm/beam.html
</GENERAL>

<EXACT>
<PROBLEMTYPE>
Lisa_ProblemType= { 1 / / Cmax }
</PROBLEMTYPE>
</EXACT>

<HEURISTIC>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 /  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { 1 / r_i / SumWiTi }
</PROBLEMTYPE>

<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O /  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F /  / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { F / r_i / SumWiTi }
</PROBLEMTYPE>
</HEURISTIC>


<PARAMETERS>
long K_BRANCHES 5 "Anzahl zu verfolgender Zweige"
string INS_ORDER ( MACHINEWISE DIAGONAL1 DIAGONAL2 RANDOM LPT ) "Einfuegereihenfolge"
string INS_METHOD ( INSERT1 INSERT2 ) "Auswahlmethode"
</PARAMETERS>