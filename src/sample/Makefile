# LiSA Sample Modules Main Makefile
# Copyright Lars Dornheim 1998, 2002
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../..
BINPATH=$(TOPPROGRAMPATH)/bin
DOCPATH=$(TOPPROGRAMPATH)/doc
OBJPATH=$(TOPPROGRAMPATH)/obj
SOURCEPATH=$(TOPPROGRAMPATH)/src
MODULENAME=sample
MODULEPATH=$(SOURCEPATH)/$(MODULENAME)

SKIP_MODULES_REGEX=/CVS$$

# ------------------------------------------------------------------------------

general: all

# ------------------------------------------------------------------------------

all:
	@for MODULE in `find $(MODULEPATH) -type d -mindepth 1 -maxdepth 1 | egrep -v "$(SKIP_MODULES_REGEX)"`; do cd $${MODULE}; $(MAKE) all; cd ..; done

# ------------------------------------------------------------------------------

compile:
	@for MODULE in `find $(MODULEPATH) -type d -mindepth 1 -maxdepth 1 | egrep -v "$(SKIP_MODULES_REGEX)"`; do cd $${MODULE}; $(MAKE) compile; cd ..; done

# ------------------------------------------------------------------------------

install:
	@for MODULE in `find $(MODULEPATH) -type d -mindepth 1 -maxdepth 1 | egrep -v "$(SKIP_MODULES_REGEX)"`; do cd $${MODULE}; $(MAKE) install; cd ..; done

# ------------------------------------------------------------------------------

uninstall:
	@for MODULE in `find $(MODULEPATH) -type d -mindepth 1 -maxdepth 1 | egrep -v "$(SKIP_MODULES_REGEX)"`; do cd $${MODULE}; $(MAKE) uninstall; cd ..; done
	rm -fr $(DOCPATH)/lisa/*/$(MODULENAME)

# ------------------------------------------------------------------------------

distclean:
	@for MODULE in `find $(MODULEPATH) -type d -mindepth 1 -maxdepth 1 | egrep -v "$(SKIP_MODULES_REGEX)"`; do cd $${MODULE}; $(MAKE) distclean; cd ..; done

# ------------------------------------------------------------------------------

clean:
	rm -fr $(OBJPATH)

# ------------------------------------------------------------------------------

depend:
	@for MODULE in `find $(MODULEPATH) -type d -mindepth 1 -maxdepth 1 | egrep -v "$(SKIP_MODULES_REGEX)"`; do cd $${MODULE}; $(MAKE) depend; cd ..; done

