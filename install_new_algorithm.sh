#!/bin/sh
cd $HOME/LiSA/src/external
mkdir $1
cd $1

echo "# LiSA Makefile
# Copyright Lars Dornheim 1998
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

# LiSA part: $1

PROGRAMNAME=$1
INSTALLNAME=$1

# ------------------------------------------------------------------------------
" > Makefile

echo "TOPPROGRAMPATH=../../..
BINPATH=\$(TOPPROGRAMPATH)/bin
DOCPATH=\$(TOPPROGRAMPATH)/doc/develop/$1
OBJPATH=\$(TOPPROGRAMPATH)/compiled
SOURCEPATH=\$(TOPPROGRAMPATH)/src
" >>Makefile

echo "
# ------------------------------------------------------------------------------

-include \$(SOURCEPATH)/Make.Config

# ------------------------------------------------------------------------------

ABSOLUTE_BINPATH=\$(ABSOLUTE_PROGRAMPATH)/bin

# ------------------------------------------------------------------------------

CXXINCL=\$(INCL)
CXXFLAGS=\$(CXXINCL) -Wall
LDFLAGS=\$(STANDARDLIBS) \$(LIBS) -Wall

DEPENDCXX=g++
DEPENDCXXFLAGS=\$(CXXINCL) -Wall

# ------------------------------------------------------------------------------

-include Make.List

-include Make.Objects

# ------------------------------------------------------------------------------

all: compile install

# ------------------------------------------------------------------------------

compile: objdir \$(OBJPATH)/\$(PROGRAMNAME)

\$(OBJPATH)/\$(PROGRAMNAME): \$(OBJECTS)
	\$(LD) -o \$(OBJPATH)/\$(PROGRAMNAME) \$(OBJECTS) \$(LDFLAGS)

# ------------------------------------------------------------------------------

objdir:
	@mkdir -p \$(OBJPATH)
" >> Makefile

echo "
# ------------------------------------------------------------------------------

-include Make.Depend

# ------------------------------------------------------------------------------

install: bindir
	cp \$(OBJPATH)/\$(PROGRAMNAME) \$(BINPATH)/\$(PROGRAMNAME)
	cp $1_engl.alg \$(TOPPROGRAMPATH)/data/alg_desc/language/english/$1.alg
	cp $1_ger.alg \$(TOPPROGRAMPATH)/data/alg_desc/language/german/$1.alg
	cp $1_geru.alg \$(TOPPROGRAMPATH)/data/alg_desc/language/german_u/$1.alg
	cp $1_engl.html \$(TOPPROGRAMPATH)/doc/language/english/$1.html
	cp $1_ger.html \$(TOPPROGRAMPATH)/doc/language/german/$1.html
    
# ------------------------------------------------------------------------------

bindir:
	@mkdir -p \$(BINPATH)

# ------------------------------------------------------------------------------

uninstall:
	rm -f \$(BINPATH)/\$(PROGRAMNAME)
	rm -f \$(BINPATH)/\$(INSTALLNAME)
	rm -f \$(bindir)/\$(INSTALLNAME)

# ------------------------------------------------------------------------------

distclean: clean

# ------------------------------------------------------------------------------

clean:
	rm -fr \$(OBJPATH)

# ------------------------------------------------------------------------------
" >> Makefile

echo "depend:
	\$(DEPENDCXX) -MM \$(DEPENDCXXFLAGS) \$(CXXSOURCES) \\
	| gawk 'BEGIN {line=\"\"; }; {if (substr(\$\$0, length(\$\$0), 1)==\"\\\\\") line=line substr(\$\$0, 1, length(\$\$0)-1); else {line=line \$\$0; print line; line=\"\"; }; };' - \\
	| sed -e \"s/\\./_point_/g\" \\
	| gawk 'BEGIN {print \"OBJECTS=\\\\\"; objectfile=\"\"; }; {if (objectfile!=\"\") print \"\\t\$(OBJPATH)/\" objectfile \" \\\\\"; objectfile=\$\$0; sub(\"_point_o.*\", \"_point_o\", objectfile); }; END {print \"\\t\$(OBJPATH)/\" objectfile; };' - \\
	| sed -e \"s/_point_/./g\" \\
	> Make.Objects
	\$(DEPENDCXX) -MM \$(DEPENDCXXFLAGS) \$(CXXSOURCES) \\
	| gawk 'BEGIN {line=\"\"; }; {if (substr(\$\$0, length(\$\$0), 1)==\"\\\\\") line=line substr(\$\$0, 1, length(\$\$0)-1); else {line=line \$\$0; print line; line=\"\"; }; };' - \\
	| sed -e \"s/\\./_point_/g\" \\
	| gawk '{objectfile=\$\$0; sub(\"_point_o.*\", \"_point_o\", objectfile); sourcefile=\$\$0; sub(\"_point_cpp.*\", \"_point_cpp\", sourcefile); sub(\".*: \", \"\", sourcefile); print \"\$(OBJPATH)/\" \$\$0 \"; \$\$(CXX) -c \$\$(CXXFLAGS) -o \$(OBJPATH)/\" objectfile \" \" sourcefile; };' - \\
	| sed -e \"s/_point_/./g\" \\
	> Make.Depend

# ------------------------------------------------------------------------------
" >> Makefile

echo "doc: docdir
	rm -f \$(DOCPATH)/* \$(DOCPATH)/.*html
	doc++ -B /dev/null -d \$(DOCPATH) -f -m \$(DOCSOURCES)

# ------------------------------------------------------------------------------

docdir:
	@mkdir -p \$(DOCPATH)
" >> Makefile


echo "CXXSOURCES=\\
	$1.cpp \\
	../../basics/global.cpp \\
	../../basics/except.cpp \\
	../../basics/matrix.cpp \\
	../../basics/pair.cpp \\
	../../basics/list.cpp \\
	../../lisa_dt/ctrlpara.cpp\\
	../../basics/lisa_str.cpp \\
	../../lisa_dt/ptype.cpp \\
	../../lisa_dt/mo.cpp \\
	../../lisa_dt/lvalues.cpp \\
	../../lisa_dt/schedule.cpp \\

DOCSOURCES=\\
     $1.hpp

" > Make.List

echo "/*
 * ************** $1.cpp *******************************
 * 
 * Sample how to implement an algorithm for LiSA
 *
 * Owner: LiSA
 *
 * 30.04.2001
*/

#include<iostream.h>
#include\"../../basics/global.hpp\"
#include\"../../lisa_dt/ctrlpara.hpp\"
#include\"../../lisa_dt/schedule.hpp\"
#include\"../../lisa_dt/ptype.hpp\"
#include\"../../lisa_dt/lvalues.hpp\"
#include<fstream.h>
#include\"../../basics/except.hpp\"

int main(int argc, char *argv[]) 
{

    // Auskommentieren, falls die Fehlermeldungen weitergesendet werden sollen
    //  G_ExceptionList.set_output_to_cout();   

    Lisa_ProblemType * lpr = new Lisa_ProblemType;
    Lisa_ControlParameters * sp = new Lisa_ControlParameters;
    Lisa_Values * my_werte=new Lisa_Values;
    
    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);

    // read problem description 
    i_strm >> (*lpr);

    // read control parameters: 
    i_strm >> (*sp);

    // read problem instance:
    i_strm >> (*my_werte);
    
    // LiSA  schedule object for storing results
    Lisa_Schedule * out_schedule = new Lisa_Schedule(my_werte->get_n(),
						     my_werte->get_m());
    out_schedule->make_LR();
    
    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************
    
    // So koennen Parameter im LiSA Hauptfenster ausgegeben werden:
    // cout << \"WARNING: Der Problemtype lautet:\" << lpr->output_problem()<< endl;
    // cout << \"WARNING: Die Obere Schranke lautet:\" << sp->get_double(\"UPPER_BOUND\")<< endl;
    // cout << \"WARNING: P(1,2)=\" << (*my_werte->PT)[1][2]<< endl;
    // cout << \"WARNING: Es sind \" << sp->get_int(\"NB_STEPS\") << \" Schritte zu tun \" << endl;
    // cout << \"WARNING: Iss\" << sp->get_string(\"NAME\")<< endl;

    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    
    // write results to output file:
    o_strm << *out_schedule;
    delete out_schedule;
    delete my_werte;
    delete lpr;
}
" > $1.cpp

echo "<GENERAL>
Name= $1-module
Type= constructive
Call= $1
Code= external
Help= $1.html
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
string NAME ( was das soll ) \"name\"
double UPPER_BOUND 1000000 \"Obere Schranke\"
long NB_STEPS 1 \"Anzahl der Schritte\"
</PARAMETERS>
" > $1_engl.alg

cp $1_engl.alg $1_ger.alg
cp $1_engl.alg $1_geru.alg

echo "
<HTML>
<HEAD> <TITLE>Hilfe zum $1-module - Fenster</TITLE>
</HEAD>
<BODY BACKGROUND=\"../../../img/lisa.gif\" BGCOLOR=\"#e3e3e3\">
<H2> $1 Algorithmus </H2>

Benutzen Sie diesen Algorithmus, um ...

<P>
<H2>So kommen Sie hierher:</H2> 
Stellen Sie sicher, da&szlig; ein 
<A HREF=\"problem.htm\"> Problemtyp </A> und alle 
<A HREF=\"values.htm\"> Problemparameter </A> 
definiert sind. W&auml;hlen Sie dann den Men&uuml;punkt 
<I> Algorithmen|Heuristische Verfahren|$1-module</I> und
dr&uuml;cken nach Anpassen der Einstellungen <I>OK</I>.
<P>
<H2>Einstellungen:</H2>
<DL>
<DT> name: <DD>
   ...
<DT>Obere Schranke:<DD>
   Wenn Sie bereits einen Plan kennen, geben Sie dessen Zielfunktionswert hier
   an. 
<DT>Anzahl der Schritte:<DD>
    ...
</DL>
<H2>Probleme:</H2>
<UL>
<LI> Problem 1
<LI> Problem 2
</UL>
<P>
<HR>
<A HREF=\"main.htm\"> Inhaltsverzeichnis </A>
<HR>
30.04.2001 LiSA
</BODY>
</HTML>
" > $1_engl.html

cp $1_engl.html $1_ger.html


make depend
make all
