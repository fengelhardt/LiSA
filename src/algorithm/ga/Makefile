

# LiSA Algorithm Module Makefile
# Copyright Lars Dornheim 1998, 2002
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

# LiSA part: pr

PROGRAMNAME=ga

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../../..
BINPATH=$(TOPPROGRAMPATH)/bin
ALGPATH=$(TOPPROGRAMPATH)/data/alg_desc
DOCPATH=$(TOPPROGRAMPATH)/doc
OBJPATH=$(TOPPROGRAMPATH)/obj

ALGEXT=xml
LANGS=english german

# ------------------------------------------------------------------------------

-include $(TOPPROGRAMPATH)/Make.Config

# ------------------------------------------------------------------------------

CXXINCL=$(INCL)
CXXFLAGS=$(CXXINCL) $(COMPILE_FLAGS) -Wall
LDFLAGS=$(STANDARDLIBS) $(LIBS)

DEPENDCXX=$(CXX)
DEPENDCXXFLAGS=$(CXXINCL) -Wall 

# ------------------------------------------------------------------------------

-include Make.List

-include Make.Objects

CLEAN_OBJECTS=$(addprefix $(OBJPATH)/,$(addsuffix .o,$(basename $(ALG_SOURCES))))

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

install: install_xml install_doc
	mkdir -p $(BINPATH)
	cp $(OBJPATH)/$(PROGRAMNAME) $(BINPATH)/$(PROGRAMNAME)
	for l in ${LANGS}; do \
		( mkdir -p $(ALGPATH)/language/$${l} && cp $(PROGRAMNAME)_$${l}*.$(ALGEXT) $(ALGPATH)/language/$${l} && \
		  mkdir -p $(DOCPATH)/lisa/$${l}/algorithm && cp $(PROGRAMNAME)_$${l}*.html $(DOCPATH)/lisa/$${l}/algorithm/$(PROGRAMNAME).html; ); done
# ------------------------------------------------------------------------------

install_xml:
	for l in ${LANGS}; do \
	( mkdir -p $(ALGPATH)/language/$${l} && cp $(PROGRAMNAME)_$${l}*.$(ALGEXT) $(ALGPATH)/language/$${l});done

install_doc:
	for l in ${LANGS}; do \
	( mkdir -p $(DOCPATH)/lisa/$${l}/algorithm && cp $(PROGRAMNAME)_$${l}*.html $(DOCPATH)/lisa/$${l}/algorithm/$(PROGRAMNAME).html; ); done

clean_doc:
	for l in ${LANGS}; do \
	( rm $(DOCPATH)/lisa/$${l}/algorithm/$(PROGRAMNAME)*.html; ); done

clean_xml:
	for l in ${LANGS}; do \
		( rm $(ALGPATH)/language/$${l}/$(PROGRAMNAME)*.$(ALGEXT) ); done

uninstall: clean_xml
	rm -f $(BINPATH)/$(PROGRAMNAME)
	rm -f $(DOCPATH)/lisa/*/algorithm/$(PROGRAMNAME).html

# ------------------------------------------------------------------------------

distclean: clean clean_xml clean_doc

# ------------------------------------------------------------------------------

clean:
	rm -f $(CLEAN_OBJECTS)
	rm -f $(OBJPATH)/$(PROGRAMNAME) $(BINPATH)/$(PROGRAMNAME)

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
