# LiSA External Module Makefile
# Copyright Lars Dornheim 1998, 2002
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

# LiSA part: pr

PROGRAMNAME=pr

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../../..
BINPATH=$(TOPPROGRAMPATH)/bin
ALGPATH=$(TOPPROGRAMPATH)/data/alg_desc
DOCPATH=$(TOPPROGRAMPATH)/doc
OBJPATH=$(TOPPROGRAMPATH)/obj

# ------------------------------------------------------------------------------

-include $(TOPPROGRAMPATH)/Make.Config

# ------------------------------------------------------------------------------

CXXINCL=$(INCL)
CXXFLAGS=$(CXXINCL) -Wall
LDFLAGS=$(STANDARDLIBS) $(LIBS) -Wall

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

install: bindir algdir docdir
	cp $(OBJPATH)/$(PROGRAMNAME) $(BINPATH)/$(PROGRAMNAME)
	cp $(PROGRAMNAME)1_english.alg $(ALGPATH)/language/english/$(PROGRAMNAME)1.alg
	cp $(PROGRAMNAME)2_english.alg $(ALGPATH)/language/english/$(PROGRAMNAME)2.alg
	cp $(PROGRAMNAME)3_english.alg $(ALGPATH)/language/english/$(PROGRAMNAME)3.alg
	cp $(PROGRAMNAME)1_german.alg $(ALGPATH)/language/german/$(PROGRAMNAME)1.alg
	cp $(PROGRAMNAME)2_german.alg $(ALGPATH)/language/german/$(PROGRAMNAME)2.alg
	cp $(PROGRAMNAME)3_german.alg $(ALGPATH)/language/german/$(PROGRAMNAME)3.alg
	cp $(PROGRAMNAME)_english.html $(DOCPATH)/lisa/english/external/$(PROGRAMNAME).html
	cp $(PROGRAMNAME)_german.html $(DOCPATH)/lisa/german/external/$(PROGRAMNAME).html
    
# ------------------------------------------------------------------------------

bindir:
	@mkdir -p $(BINPATH)

# ------------------------------------------------------------------------------

algdir:
	@mkdir -p $(ALGPATH)/language/english
	@mkdir -p $(ALGPATH)/language/german

# ------------------------------------------------------------------------------

docdir:
	@mkdir -p $(DOCPATH)/lisa/english/external
	@mkdir -p $(DOCPATH)/lisa/german/external

# ------------------------------------------------------------------------------

uninstall:
	rm -f $(BINPATH)/$(PROGRAMNAME)
	rm -f $(ALGPATH)/language/*/$(PROGRAMNAME).alg
	rm -f $(DOCPATH)/lisa/*/external/$(PROGRAMNAME).html

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

# ------------------------------------------------------------------------------

develdoc: develdocdir
#	rm -f $(DOCPATH)/devel/$(PROGRAMNAME)/* $(DOCPATH)/devel/$(PROGRAMNAME)/.*html
#	doc++ -B /dev/null -d $(DOCPATH)/devel/$(PROGRAMNAME) -f -m $(DOCSOURCES)

# ------------------------------------------------------------------------------

develdocdir:
	@mkdir -p $(DOCPATH)/devel/$(PROGRAMNAME)

