<GENERAL>
Name= Irreduzibilitätstest (Research)
Type= iterative
Call= irred
Code= external
Help= algorithm/irred.html
</GENERAL>

<EXACT>
</EXACT>

<HEURISTIC>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { O / r_i / SumWiTi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / Cmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / Lmax }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumWiCi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumWiUi }
</PROBLEMTYPE>
<PROBLEMTYPE> 
Lisa_ProblemType= { J / r_i / SumWiTi }
</PROBLEMTYPE>
</HEURISTIC>

<PARAMETERS>
string TYPE ( SIMILAR ALL_REDUCING ITERATIVE_REDUCING ) "Erzeuge Pläne: "
string RETURN ( ONLY_IRREDUCIBLE ALL ) "Gib folgende Pläne zurück: "
string RNDM ( NO YES ) "Erzeuge Pläne in zufälliger Reihenfolge: "
</PARAMETERS>










