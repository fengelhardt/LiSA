# LiSA Makefile
# Copyright Lars Dornheim 1998
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

# LiSA part: main

PROGRAMNAME=main
INSTALLNAME=lisa

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../..
BINPATH=$(TOPPROGRAMPATH)/bin
DOCPATH=$(TOPPROGRAMPATH)/doc/develop
OBJPATH=$(TOPPROGRAMPATH)/compiled
SOURCEPATH=$(TOPPROGRAMPATH)/src
ALGPATH=$(TOPPROGRAMPATH)/data/alg_desc/language

# ------------------------------------------------------------------------------

-include $(TOPPROGRAMPATH)/Make.Config

# ------------------------------------------------------------------------------

CXXINCL=$(TCLTKINCL) $(XINCL) $(INCL)
CXXFLAGS=$(CXXINCL) -Wall
LDFLAGS=$(TCLTKLIBS) $(XLIBS) $(STANDARDLIBS) $(LIBS) -Wall

DEPENDCXX=g++
DEPENDCXXFLAGS=$(CXXINCL) -Wall

# ------------------------------------------------------------------------------

-include Make.List

-include Make.Objects

# ------------------------------------------------------------------------------

all: compile install

# ------------------------------------------------------------------------------

compile: objdir $(OBJPATH)/$(PROGRAMNAME)

$(OBJPATH)/$(PROGRAMNAME): $(OBJECTS)
	$(LD) -o $(OBJPATH)/$(PROGRAMNAME) $(OBJECTS) $(LDFLAGS)

# ------------------------------------------------------------------------------

objdir:
	@mkdir -p $(OBJPATH)

# ------------------------------------------------------------------------------

-include Make.Depend

# ------------------------------------------------------------------------------

install: bindir
	cp $(OBJPATH)/$(PROGRAMNAME) $(BINPATH)/$(PROGRAMNAME)
	cp $(INSTALLNAME) $(BINPATH)/$(INSTALLNAME)
	chmod 755 $(BINPATH)/$(INSTALLNAME)
	cp $(SOURCEPATH)/algo/disp1_engl.alg $(ALGPATH)/english/disp1.alg
	cp $(SOURCEPATH)/algo/disp2_engl.alg $(ALGPATH)/english/disp2.alg
	cp $(SOURCEPATH)/algo/disp3_engl.alg $(ALGPATH)/english/disp3.alg
	cp $(SOURCEPATH)/algo/disp1_ger.alg $(ALGPATH)/german/disp1.alg
	cp $(SOURCEPATH)/algo/disp2_ger.alg $(ALGPATH)/german/disp2.alg
	cp $(SOURCEPATH)/algo/disp3_ger.alg $(ALGPATH)/german/disp3.alg
	cp $(SOURCEPATH)/algo/disp1_ger.alg $(ALGPATH)/german_u/disp1.alg
	cp $(SOURCEPATH)/algo/disp2_ger.alg $(ALGPATH)/german_u/disp2.alg
	cp $(SOURCEPATH)/algo/disp3_ger.alg $(ALGPATH)/german_u/disp3.alg

# ------------------------------------------------------------------------------

bindir:
	@mkdir -p $(BINPATH)

# ------------------------------------------------------------------------------

uninstall:
	rm -f $(BINPATH)/$(PROGRAMNAME)
	rm -f $(BINPATH)/$(INSTALLNAME)
	rm -f $(ALGPATH)/english/*.alg
	rm -f $(ALGPATH)/german/*.alg
	rm -f $(ALGPATH)/german_u/*.alg

# ------------------------------------------------------------------------------

distclean: clean
	rm -f $(INSTALLNAME)

# ------------------------------------------------------------------------------

clean:
	rm -fr $(OBJPATH)
	rm -f  $(TOPPROGRAMPATH)/tcl/external/*.tcl

# ------------------------------------------------------------------------------

depend:
	$(DEPENDCXX) -MM $(DEPENDCXXFLAGS) $(CXXSOURCES) \
	| gawk 'BEGIN {line=""; }; {if (substr($$0, length($$0), 1)=="\\") line=line substr($$0, 1, length($$0)-1); else {line=line $$0; print line; line=""; }; };' - \
	| sed -e "s/\./_point_/g" \
	| gawk 'BEGIN {print "OBJECTS=\\"; objectfile=""; }; {if (objectfile!="") print "\t$(OBJPATH)/" objectfile " \\"; objectfile=$$0; sub("_point_o.*", "_point_o", objectfile); }; END {print "\t$(OBJPATH)/" objectfile; };' - \
	| sed -e "s/_point_/./g" \
	> Make.Objects
	$(DEPENDCXX) -MM $(DEPENDCXXFLAGS) $(CXXSOURCES) \
	| gawk 'BEGIN {line=""; }; {if (substr($$0, length($$0), 1)=="\\") line=line substr($$0, 1, length($$0)-1); else {line=line $$0; print line; line=""; }; };' - \
	| sed -e "s/\./_point_/g" \
	| gawk '{objectfile=$$0; sub("_point_o.*", "_point_o", objectfile); sourcefile=$$0; sub("_point_cpp.*", "_point_cpp", sourcefile); sub(".*: ", "", sourcefile); print "$(OBJPATH)/" $$0 "; $$(CXX) -c $$(CXXFLAGS) -o $(OBJPATH)/" objectfile " " sourcefile; };' - \
	| sed -e "s/_point_/./g" \
	> Make.Depend

# ------------------------------------------------------------------------------

doc:    
	doc++ -l -B /dev/null -d $(DOCPATH) -F $(DOCSOURCES)

