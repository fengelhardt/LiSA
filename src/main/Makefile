# LiSA Main Module Makefile
# Copyright Lars Dornheim 1998, 2002
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

# LiSA part: main

PROGRAMNAME=main
INSTALLNAME=lisa

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../..
BINPATH=$(TOPPROGRAMPATH)/bin
DATAPATH=$(TOPPROGRAMPATH)/data
DOCPATH=$(TOPPROGRAMPATH)/doc
OBJPATH=$(TOPPROGRAMPATH)/obj
SOURCEPATH=$(TOPPROGRAMPATH)/src

# ------------------------------------------------------------------------------

-include $(TOPPROGRAMPATH)/Make.Config

# ------------------------------------------------------------------------------

CXXINCL=$(TCLTKINCL) $(XINCL) $(INCL)
CXXFLAGS=$(CXXINCL) -Wall
LDFLAGS=$(TCLTKLIBS) $(XLIBS) $(STANDARDLIBS) $(LIBS) -Wall

DEPENDCXX=$(CXX)
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

install:
	mkdir -p $(BINPATH)
	cp $(OBJPATH)/$(PROGRAMNAME) $(BINPATH)/$(PROGRAMNAME)
	cp $(INSTALLNAME) $(TOPPROGRAMPATH)/$(INSTALLNAME)
	chmod 755 $(TOPPROGRAMPATH)/$(INSTALLNAME)
	mkdir -p $(DATAPATH)
	cp $(SOURCEPATH)/main/default.xml $(DATAPATH)
	cp $(SOURCEPATH)/xml/LiSA.dtd $(DATAPATH)
	mkdir -p $(DATAPATH)/classify
	cp $(SOURCEPATH)/classify/classify.bib $(DATAPATH)/classify
	cp $(SOURCEPATH)/classify/classify.bst $(DATAPATH)/classify
	cp $(SOURCEPATH)/classify/classify.tex $(DATAPATH)/classify
	cp -r $(SOURCEPATH)/sample/problem $(DATAPATH)/sample
	mkdir -p $(DOCPATH)/lisa
	cp -r $(SOURCEPATH)/doc/* $(DOCPATH)/lisa

# ------------------------------------------------------------------------------

uninstall:
	rm -f $(BINPATH)/$(PROGRAMNAME)
	rm -f $(TOPPROGRAMPATH)/$(INSTALLNAME)
	rm -f $(DATAPATH)/default.xml
	rm -fr $(DATAPATH)/classify
	rm -fr $(DATAPATH)/sample
	rm -f $(DOCPATH)/lisa/*.html

# ------------------------------------------------------------------------------

distclean: clean
	rm -f $(INSTALLNAME)

# ------------------------------------------------------------------------------

clean:
	rm -fr $(OBJPATH)

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

