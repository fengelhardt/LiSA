# LiSA Global Makefile
# Copyright Lars Dornheim 1998
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# LiSA parts: Main, bb, eval, neighbor

# ------------------------------------------------------------------------------

# The release level should be 0 for alpha, 1 for beta, and 2 for final/patch.
# The release serial value is the number that follows the "a", "b", or "p" in
# the patch level; for example, if the patch level is 1.0b2, RELEASE_SERIAL is
# 2. It restarts at 1 whenever the release level is changed, except for the
# final release which is 0 (the first patch will start at 1).

MAJOR_VERSION=2
MINOR_VERSION=2
RELEASE_LEVEL=2
RELEASE_LEVEL_LETTER=a
RELEASE_SERIAL=0

VERSION=$(MAJOR_VERSION).$(MINOR_VERSION)
PATCH_LEVEL=$(VERSION)$(RELEASE_LEVEL_LETTER)$(RELEASE_SERIAL)

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=.
BINPATH=$(TOPPROGRAMPATH)/bin
OBJPATH=$(TOPPROGRAMPATH)/compiled
DOCPATH=$(TOPPROGRAMPATH)/doc/develop/html
SOURCEPATH=$(TOPPROGRAMPATH)/src

# ------------------------------------------------------------------------------

general: all

# ------------------------------------------------------------------------------

logo:
	@echo
	@echo "####  ****   @@@@@@@@@@@@ XXXXXXXXXXXX"
	@echo "####  ****   @@@@@@@@@@@@ XXXXXXXXXXXX"
	@echo "XXXX  ####   %%%%         @@@@    ****"
	@echo "XXXX  ####   %%%%         @@@@    ****"
	@echo "****  @@@@   ############ %%%%%%%%%%%%"
	@echo "****  @@@@   ############ %%%%%%%%%%%%"
	@echo "@@@@                 %%%% ****    ####"
	@echo "@@@@                 %%%% ****    ####"
	@echo "%%%%%%%%%%%% XXXXXXXXXXXX ####    @@@@"
	@echo "%%%%%%%%%%%% XXXXXXXXXXXX ####    @@@@"
	@echo
	@echo "LiSA - Version $(VERSION) - (C) LiSA Group at Magdeburg University 2002"
	@echo
	@echo
	@echo

# ------------------------------------------------------------------------------

about: logo
	@echo "Library of Scheduling Algorithms (LiSA)"
	@echo ""
	@echo "Version: $(PATCH_LEVEL)"
	@echo ""
	@echo "Copyright LiSA Group at Magdeburg University 2001"
	@echo ""
	@echo "Team: Heidemarie Braesel, Lars Dornheim, Martin Harborth, Sandra Kutz,"
	@echo "      Thomas Tautenhahn, Ines Wasmund, Per Willenius, Andreas Winkler"
	@echo "      Marc Mörig, Christian Schulz, Manuela Vogel "
	@echo "E-Mail: lisagroup@lisa.math.uni-magdeburg.de"
	@echo ""
	@echo "WWW: http://lisa.math.uni-magdeburg.de"
	@echo ""

# ------------------------------------------------------------------------------

help: logo
	@echo "'make' ............. same as 'make compile'"
	@echo "'make about' ....... shows the about information"
	@echo "'make all' ......... compiles and installs all"
	@echo "'make help' ........ shows this help"
	@echo "'make compile' ..... compiles all"
	@echo "'make install' ..... installs all"
	@echo "'make uninstall' ... uninstalls all"
	@echo "'make distclean' ... deletes the configuration and the compiled objects"
	@echo "'make clean' ....... deletes only the compiled objects"
	@echo "'make depend' ...... creates the dependencies for the compiling"
	@echo "'make version' ..... updates the version information in the source files"
	@echo "'make doc' ......... creates the source code documentation"
	@echo

# ------------------------------------------------------------------------------

all: logo
	cd $(SOURCEPATH)/main; $(MAKE) all
	cd $(SOURCEPATH)/external; $(MAKE) all
# ------------------------------------------------------------------------------

compile: logo
	cd $(SOURCEPATH)/main; $(MAKE) compile
	cd $(SOURCEPATH)/external; $(MAKE) compile
# ------------------------------------------------------------------------------

install: logo
	cd $(SOURCEPATH)/main; $(MAKE) install
	cd $(SOURCEPATH)/external; $(MAKE) install
# ------------------------------------------------------------------------------

uninstall: logo
	cd $(SOURCEPATH)/main; $(MAKE) uninstall
	cd $(SOURCEPATH)/external; $(MAKE) uninstall
	rm -fr $(BINPATH)
# ------------------------------------------------------------------------------

distclean: uninstall clean
	rm -f $(TOPPROGRAMPATH)/config.*
	rm -f $(TOPPROGRAMPATH)/Make.Config
	cd $(SOURCEPATH)/main; $(MAKE) distclean
	cd $(SOURCEPATH)/external; $(MAKE) distclean
# ------------------------------------------------------------------------------

clean: logo
	rm -fr $(OBJPATH)

# ------------------------------------------------------------------------------

depend:
	cd $(SOURCEPATH)/main; $(MAKE) depend
	cd $(SOURCEPATH)/external; $(MAKE) depend
# ------------------------------------------------------------------------------

version:
	@echo
	@find $(SOURCEPATH) -name "*.[hc]pp" -type f \
	| gawk '{print "\"" $$0 "\""}' \
	| xargs -n 1 $(TOPPROGRAMPATH)/make_version $(PATCH_LEVEL)
	@echo
	@echo "creating $(SOURCEPATH)/general/version.hpp"
	@rm -f $(SOURCEPATH)/general/version.hpp.temp
	@cat $(SOURCEPATH)/general/version.hpp \
	| $(TOPPROGRAMPATH)/make_substitute "LISA_MAJOR_VERSION " $(MAJOR_VERSION) \
	| $(TOPPROGRAMPATH)/make_substitute "LISA_MINOR_VERSION " $(MINOR_VERSION) \
	| $(TOPPROGRAMPATH)/make_substitute "LISA_RELEASE_LEVEL " $(RELEASE_LEVEL) \
	| $(TOPPROGRAMPATH)/make_substitute "LISA_RELEASE_LEVEL_LETTER " "'$(RELEASE_LEVEL_LETTER)'" \
	| $(TOPPROGRAMPATH)/make_substitute "LISA_RELEASE_SERIAL " $(RELEASE_SERIAL) \
	| $(TOPPROGRAMPATH)/make_substitute "LISA_VERSION " '\"$(VERSION)\"' \
	| $(TOPPROGRAMPATH)/make_substitute "LISA_PATCH_LEVEL " '\"$(PATCH_LEVEL)\"' \
	> $(SOURCEPATH)/general/version.hpp.temp
	@mv $(SOURCEPATH)/general/version.hpp.temp $(SOURCEPATH)/general/version.hpp
	@echo

# ------------------------------------------------------------------------------

doc: 	
	cd $(SOURCEPATH)/main; $(MAKE) doc
	cd $(SOURCEPATH)/external; $(MAKE) doc
