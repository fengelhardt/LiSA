

# LiSA Algorithm Module Makefile
# Copyright Lars Dornheim 1998, 2002
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

# LiSA part: pr

PROGRAMNAME=beam

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../../..
BINPATH=$(TOPPROGRAMPATH)/bin
ALGPATH=$(TOPPROGRAMPATH)/data/alg_desc
DOCPATH=$(TOPPROGRAMPATH)/doc
OBJPATH=$(TOPPROGRAMPATH)/obj

ALGEXT = xml

# ------------------------------------------------------------------------------

-include $(TOPPROGRAMPATH)/Make.Config

# ------------------------------------------------------------------------------

CXXINCL=$(INCL)
CXXFLAGS=$(CXXINCL) -Wall 
LDFLAGS=$(STANDARDLIBS) $(LIBS) -Wall

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
	mkdir -p $(ALGPATH)/language/english
	cp $(PROGRAMNAME)_english1.$(ALGEXT) $(ALGPATH)/language/english/$(PROGRAMNAME)1.$(ALGEXT)
	cp $(PROGRAMNAME)_english2.$(ALGEXT) $(ALGPATH)/language/english/$(PROGRAMNAME)2.$(ALGEXT)
	mkdir -p $(ALGPATH)/language/german
	cp $(PROGRAMNAME)_german1.$(ALGEXT) $(ALGPATH)/language/german/$(PROGRAMNAME)1.$(ALGEXT)
	cp $(PROGRAMNAME)_german2.$(ALGEXT) $(ALGPATH)/language/german/$(PROGRAMNAME)2.$(ALGEXT)
	mkdir -p $(DOCPATH)/lisa/english/algorithm
	cp $(PROGRAMNAME)_english.html $(DOCPATH)/lisa/english/algorithm/$(PROGRAMNAME).html
	mkdir -p $(DOCPATH)/lisa/german/algorithm
	cp $(PROGRAMNAME)_german.html $(DOCPATH)/lisa/german/algorithm/$(PROGRAMNAME).html

# ------------------------------------------------------------------------------

uninstall:
	rm -f $(BINPATH)/$(PROGRAMNAME)
	rm -f $(ALGPATH)/language/*/$(PROGRAMNAME)*.$(ALGEXT)
	rm -f $(DOCPATH)/lisa/*/algorithm/$(PROGRAMNAME).html

# ------------------------------------------------------------------------------

distclean: clean

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

