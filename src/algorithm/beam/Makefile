

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

LANGS=english german

# ------------------------------------------------------------------------------

install:
	mkdir -p $(BINPATH)
	cp $(OBJPATH)/$(PROGRAMNAME) $(BINPATH)/$(PROGRAMNAME)
	for l in ${LANGS}; do \
		( mkdir -p $(ALGPATH)/language/$${l} && cp $(PROGRAMNAME)_$${l}*.$(ALGEXT) $(ALGPATH)/language/$${l} && \
		  mkdir -p $(DOCPATH)/lisa/$${l}/algorithm && cp $(PROGRAMNAME)_$${l}*.html $(DOCPATH)/lisa/$${l}/algorithm/$(PROGRAMNAME).html; ); done
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

