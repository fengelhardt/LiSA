# LiSA Makefile
# Copyright Lars Dornheim 1998
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

# LiSA part: sample

PROGRAMNAME=beam
INSTALLNAME=beam

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../../..
BINPATH=$(TOPPROGRAMPATH)/bin
DOCPATH=$(TOPPROGRAMPATH)/doc/develop/beam
OBJPATH=$(TOPPROGRAMPATH)/compiled
SOURCEPATH=$(TOPPROGRAMPATH)/src
ALGPATH=$(TOPPROGRAMPATH)/data/alg_desc/language

# ------------------------------------------------------------------------------

-include $(TOPPROGRAMPATH)/Make.Config

# ------------------------------------------------------------------------------

ABSOLUTE_BINPATH=$(ABSOLUTE_PROGRAMPATH)/bin

# ------------------------------------------------------------------------------

CXXINCL=$(INCL)
CXXFLAGS=$(CXXINCL) -Wall -g 
LDFLAGS=$(STANDARDLIBS) $(LIBS) -Wall

DEPENDCXX=g++
DEPENDCXXFLAGS=$(CXXINCL) -Wall

# ------------------------------------------------------------------------------

-include Make.List

-include Make.Objects

# ------------------------------------------------------------------------------

all: compile

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
	cp $(PROGRAMNAME)_english.alg  $(ALGPATH)/english/$(PROGRAMNAME).alg
	cp $(PROGRAMNAME)_german.alg  $(ALGPATH)/german/$(PROGRAMNAME).alg
	cp $(PROGRAMNAME)_german_u.alg  $(ALGPATH)/german_u/$(PROGRAMNAME).alg
#	rm -f $(BINPATH)/$(INSTALLNAME)
#	ln -s $(ABSOLUTE_BINPATH)/$(PROGRAMNAME) $(BINPATH)/$(INSTALLNAME)
#	rm -f $(bindir)/$(INSTALLNAME)
#	ln -s $(ABSOLUTE_BINPATH)/$(PROGRAMNAME) $(bindir)/$(INSTALLNAME)

# ------------------------------------------------------------------------------

bindir:
	@mkdir -p $(BINPATH)

# ------------------------------------------------------------------------------

uninstall:
	rm -f $(BINPATH)/$(PROGRAMNAME)
	rm -f $(BINPATH)/$(INSTALLNAME)
	rm -f $(bindir)/$(INSTALLNAME)
	rm -f $(ALGPATH)/english/$(PROGRAMNAME).alg
	rm -f $(ALGPATH)/german/$(PROGRAMNAME).alg
	rm -f $(ALGPATH)/german_u/$(PROGRAMNAME).alg

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

doc: docdir
	rm -f $(DOCPATH)/* $(DOCPATH)/.*html
	doc++ -B /dev/null -d $(DOCPATH) -f -m $(DOCSOURCES)

# ------------------------------------------------------------------------------

docdir:
	@mkdir -p $(DOCPATH)




