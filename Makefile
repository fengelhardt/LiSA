# LiSA Global Makefile
# Copyright Lars Dornheim 1998, 2002
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

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
ALGPATH=$(TOPPROGRAMPATH)/data/alg_desc
DOCPATH=$(TOPPROGRAMPATH)/doc
CYGWINPATH=$(TOPPROGRAMPATH)/win
OBJPATH=$(TOPPROGRAMPATH)/obj
SOURCEPATH=$(TOPPROGRAMPATH)/src
NONGUI_MODULES=algorithm utility sample
GUI_MODULES=main
MODULES=$(GUI_MODULES) $(NONGUI_MODULES)

# ------------------------------------------------------------------------------

general: all

# ------------------------------------------------------------------------------

logo:
	@echo
	@echo "1111  2222   333333333333 444444444444"
	@echo "1111  2222   333333333333 444444444444"
	@echo "4444  1111   5555         3333    2222"
	@echo "4444  1111   5555         3333    2222"
	@echo "2222  3333   111111111111 555555555555"
	@echo "2222  3333   111111111111 555555555555"
	@echo "3333                 5555 2222    1111"
	@echo "3333                 5555 2222    1111"
	@echo "555555555555 444444444444 1111    3333"
	@echo "555555555555 444444444444 1111    3333"
	@echo
	@echo "LiSA - Version $(VERSION) - (C) LiSA Group at Magdeburg University 2002"
	@echo
	@echo
	@echo

# ------------------------------------------------------------------------------

about: logo
	@echo "Library of Scheduling Algorithms (LiSA)"
	@echo
	@echo "Version: $(PATCH_LEVEL)"
	@echo
	@echo "Copyright LiSA Group at Magdeburg University 2002"
	@echo
	@echo "Team: Heidemarie Braesel, Lars Dornheim, Martin Harborth, Sandra Kutz,"
	@echo "      Ivo Rössling, Thomas Tautenhahn, Ines Wasmund, Per Willenius,"
	@echo "      Andreas Winkler, Marc Mörig, Christian Schulz, Manuela Vogel "
	@echo
	@echo "E-Mail: lisagroup@lisa.math.uni-magdeburg.de"
	@echo
	@echo "WWW: http://lisa.math.uni-magdeburg.de"
	@echo

# ------------------------------------------------------------------------------

help: logo
	@echo "'make' ............. same as 'make compile'"
	@echo "'make about' ....... shows the about information"
	@echo "'make all' ......... compiles and installs all"
	@echo "'make nongui' ...... compiles and installs everything but the GUI"
	@echo "'make help' ........ shows this help"
	@echo "'make compile' ..... compiles all"
	@echo "'make install' ..... installs all"
	@echo "'make uninstall' ... uninstalls all"
	@echo "'make cygwin' ...... creates windows standalone version on cygwin"
	@echo "'make uncygwin' .... uninstalls windows standalone version"
	@echo "'make distclean' ... deletes the configuration and the compiled objects"
	@echo "'make clean' ....... deletes only the compiled objects"
	@echo "'make depend' ...... creates the dependencies for the compiling"
	@echo "'make version' ..... updates the version information in the source files"
	@echo "'make develdoc' .... creates the source code documentation"
	@echo

# ------------------------------------------------------------------------------

all: logo
	@for MODULE in $(MODULES); do cd $(SOURCEPATH)/$${MODULE}; $(MAKE) all; cd ../..; done

# ------------------------------------------------------------------------------

nongui: logo
	@for MODULE in $(NONGUI_MODULES); do cd $(SOURCEPATH)/$${MODULE}; $(MAKE) all; cd ../..; done

# ------------------------------------------------------------------------------

compile: logo
	@for MODULE in $(MODULES); do cd $(SOURCEPATH)/$${MODULE}; $(MAKE) compile; cd ../..; done

# ------------------------------------------------------------------------------

install: logo
	@for MODULE in $(MODULES); do cd $(SOURCEPATH)/$${MODULE}; $(MAKE) install; cd ../..; done

# ------------------------------------------------------------------------------

uninstall:
	@for MODULE in $(MODULES); do cd $(SOURCEPATH)/$${MODULE}; $(MAKE) uninstall; cd ../..; done
	rm -fr $(BINPATH)
	rm -fr $(ALGPATH)

# ------------------------------------------------------------------------------

cygwin: cygwindir
	rm -fr $(CYGWINPATH)/*
	mkdir $(CYGWINPATH)/LiSA
	cp $(TOPPROGRAMPATH)/README $(CYGWINPATH)/LiSA/README.txt
	cp $(TOPPROGRAMPATH)/LICENSE $(CYGWINPATH)/LiSA/LICENSE.txt
	cp -r $(BINPATH) $(CYGWINPATH)/LiSA/bin
	rm -f $(CYGWINPATH)/LiSA/bin/lisa
	cp -r $(DOCPATH) $(CYGWINPATH)/LiSA/doc
	cp -r $(TOPPROGRAMPATH)/data $(CYGWINPATH)/LiSA
	cp -r $(TOPPROGRAMPATH)/img $(CYGWINPATH)/LiSA
	cp -r $(TOPPROGRAMPATH)/tcl $(CYGWINPATH)/LiSA
	cp /bin/cygwin1.dll $(CYGWINPATH)/LiSA/bin
	cp /bin/cygtcl[0-9]*.dll $(CYGWINPATH)/LiSA/bin
	cp /bin/cygtk[0-9]*.dll $(CYGWINPATH)/LiSA/bin
	mkdir -p $(CYGWINPATH)/LiSA/share/`cd /usr/share;find tcl* -maxdepth 0`
	cp -r /usr/share/tcl*/init.tcl $(CYGWINPATH)/LiSA/share/tcl*
	mkdir -p $(CYGWINPATH)/LiSA/share/`cd /usr/share;find tk* -maxdepth 0`
	cp -r /usr/share/tk*/tclIndex $(CYGWINPATH)/LiSA/share/tk*
	cp -r /usr/share/tk*/*.tcl $(CYGWINPATH)/LiSA/share/tk*
	cd $(CYGWINPATH); zip -9r lisa-$(VERSION)-win.zip LiSA

# ------------------------------------------------------------------------------

cygwindir:
	@mkdir -p $(CYGWINPATH)

# ------------------------------------------------------------------------------

uncygwin:
	rm -fr $(CYGWINPATH)

# ------------------------------------------------------------------------------

distclean: uncygwin uninstall clean
	rm -f $(TOPPROGRAMPATH)/config.*
	rm -f $(TOPPROGRAMPATH)/Make.Config
	@for MODULE in $(MODULES); do cd $(SOURCEPATH)/$${MODULE}; $(MAKE) distclean; cd ../..; done
	rm -fr $(DOCPATH)

# ------------------------------------------------------------------------------

clean:
	rm -fr $(OBJPATH)

# ------------------------------------------------------------------------------

depend:
	@for MODULE in $(MODULES); do cd $(SOURCEPATH)/$${MODULE}; $(MAKE) depend; cd ../..; done

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

develdoc: 	
	@for MODULE in $(MODULES); do cd $(SOURCEPATH)/$${MODULE}; $(MAKE) develdoc; cd ../..; done

