# LiSA Global Makefile
# Copyright Lars Dornheim 1998
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../..
BINPATH=$(TOPPROGRAMPATH)/bin
OBJPATH=$(TOPPROGRAMPATH)/compiled
DOCPATH=$(TOPPROGRAMPATH)/doc/develop/html
SOURCEPATH=$(TOPPROGRAMPATH)/src

# ------------------------------------------------------------------------------

general: all

# ------------------------------------------------------------------------------

all:
	cd $(SOURCEPATH)/external/bb; $(MAKE) all
	cd $(SOURCEPATH)/external/neighbor; $(MAKE) all
	cd $(SOURCEPATH)/external/neighbor/gen_ptst; $(MAKE) all
	cd $(SOURCEPATH)/external/travel_neighbour $(MAKE) all
	cd $(SOURCEPATH)/external/bb_jsp; $(MAKE) all
	cd $(SOURCEPATH)/external/match; $(MAKE) all
	cd $(SOURCEPATH)/external/fsheur; $(MAKE) all
	cd $(SOURCEPATH)/external/beam; $(MAKE) all
	cd $(SOURCEPATH)/external/bottle; $(MAKE) all
	cd $(SOURCEPATH)/external/single; $(MAKE) all
	cd $(SOURCEPATH)/external/open2; $(MAKE) all
	cd $(SOURCEPATH)/external/job2; $(MAKE) all
	cd $(SOURCEPATH)/external/flow2; $(MAKE) all
	cd $(SOURCEPATH)/external/pr; $(MAKE) all
# ------------------------------------------------------------------------------

compile:
	cd $(SOURCEPATH)/external/bb; $(MAKE) compile
	cd $(SOURCEPATH)/external/neighbor; $(MAKE) compile
	cd $(SOURCEPATH)/external/neighbor/gen_ptst; $(MAKE) compile
	cd $(SOURCEPATH)/external/travel_neighbour $(MAKE) compile
	cd $(SOURCEPATH)/external/bb_jsp; $(MAKE) compile
	cd $(SOURCEPATH)/external/match; $(MAKE) compile
	cd $(SOURCEPATH)/external/fsheur; $(MAKE) compile
	cd $(SOURCEPATH)/external/beam; $(MAKE) compile
	cd $(SOURCEPATH)/external/bottle; $(MAKE) compile
	cd $(SOURCEPATH)/external/single; $(MAKE) compile
	cd $(SOURCEPATH)/external/open2; $(MAKE) compile
	cd $(SOURCEPATH)/external/job2; $(MAKE) compile
	cd $(SOURCEPATH)/external/flow2; $(MAKE) compile
	cd $(SOURCEPATH)/external/pr; $(MAKE) compile
# ------------------------------------------------------------------------------

install:
	cd $(SOURCEPATH)/external/bb; $(MAKE) install
	cd $(SOURCEPATH)/external/neighbor; $(MAKE) install
	cd $(SOURCEPATH)/external/neighbor/gen_ptst; $(MAKE) install
	cd $(SOURCEPATH)/external/travel_neighbour $(MAKE) install
	cd $(SOURCEPATH)/external/bb_jsp; $(MAKE) install
	cd $(SOURCEPATH)/external/match; $(MAKE) install
	cd $(SOURCEPATH)/external/fsheur; $(MAKE) install
	cd $(SOURCEPATH)/external/beam; $(MAKE) install
	cd $(SOURCEPATH)/external/bottle; $(MAKE) install
	cd $(SOURCEPATH)/external/single; $(MAKE) install
	cd $(SOURCEPATH)/external/open2; $(MAKE) install
	cd $(SOURCEPATH)/external/job2; $(MAKE) install
	cd $(SOURCEPATH)/external/flow2; $(MAKE) install
	cd $(SOURCEPATH)/external/pr; $(MAKE) install
# ------------------------------------------------------------------------------

uninstall:
	cd $(SOURCEPATH)/external/bb; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/neighbor; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/neighbor/gen_ptst; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/travel_neighbour $(MAKE) uninstall	
	cd $(SOURCEPATH)/external/bb_jsp; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/match; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/fsheur; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/beam; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/bottle; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/single; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/open2; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/job2; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/flow2; $(MAKE) uninstall
	cd $(SOURCEPATH)/external/pr; $(MAKE) uninstall
	rm -fr $(BINPATH)
# ------------------------------------------------------------------------------

distclean:
	cd $(SOURCEPATH)/external/bb; $(MAKE) distclean
	cd $(SOURCEPATH)/external/neighbor; $(MAKE) distclean
	cd $(SOURCEPATH)/external/neighbor/gen_ptst; $(MAKE) distclean
	cd $(SOURCEPATH)/external/travel_neighbour $(MAKE) distclean
	cd $(SOURCEPATH)/external/bb_jsp; $(MAKE) distclean
	cd $(SOURCEPATH)/external/match; $(MAKE) distclean
	cd $(SOURCEPATH)/external/fsheur; $(MAKE) distclean
	cd $(SOURCEPATH)/external/beam; $(MAKE) distclean
	cd $(SOURCEPATH)/external/bottle; $(MAKE) distclean
	cd $(SOURCEPATH)/external/single; $(MAKE) distclean
	cd $(SOURCEPATH)/external/open2; $(MAKE) distclean
	cd $(SOURCEPATH)/external/job2; $(MAKE) distclean
	cd $(SOURCEPATH)/external/flow2; $(MAKE) distclean
	cd $(SOURCEPATH)/external/pr; $(MAKE) distclean
# ------------------------------------------------------------------------------

clean: logo
	rm -fr $(OBJPATH)

# ------------------------------------------------------------------------------

depend:
	cd $(SOURCEPATH)/external/bb; $(MAKE) depend
	cd $(SOURCEPATH)/external/neighbor; $(MAKE) depend
	cd $(SOURCEPATH)/external/neighbor/gen_ptst; $(MAKE) depend
	cd $(SOURCEPATH)/external/travel_neighbour $(MAKE) depend
	cd $(SOURCEPATH)/external/bb_jsp; $(MAKE) depend
	cd $(SOURCEPATH)/external/match; $(MAKE) depend
	cd $(SOURCEPATH)/external/fsheur; $(MAKE) depend
	cd $(SOURCEPATH)/external/beam; $(MAKE) depend
	cd $(SOURCEPATH)/external/bottle; $(MAKE) depend
	cd $(SOURCEPATH)/external/single; $(MAKE) depend
	cd $(SOURCEPATH)/external/open2; $(MAKE) depend
	cd $(SOURCEPATH)/external/job2; $(MAKE) depend
	cd $(SOURCEPATH)/external/flow2; $(MAKE) depend
	cd $(SOURCEPATH)/external/pr; $(MAKE) depend
# ------------------------------------------------------------------------------

doc: 	
#	cd $(SOURCEPATH)/external/bb; $(MAKE) doc
#	cd $(SOURCEPATH)/external/eval; $(MAKE) doc
#	cd $(SOURCEPATH)/external/neighbor; $(MAKE) doc
#	cd $(SOURCEPATH)/external/neighbor/paratest; $(MAKE) doc
#	cd $(SOURCEPATH)/external/neighbor/gen_ptst; $(MAKE) doc
#	cd $(SOURCEPATH)/external/bb_jsp; $(MAKE) doc
#	cd $(SOURCEPATH)/external/match; $(MAKE) doc
#	cd $(SOURCEPATH)/external/fsheur; $(MAKE) doc
#	cd $(SOURCEPATH)/external/beam; $(MAKE) doc
#	cd $(SOURCEPATH)/external/bottle; $(MAKE) doc
#	cd $(SOURCEPATH)/external/single; $(MAKE) doc
#	cd $(SOURCEPATH)/external/open2; $(MAKE) doc
#	cd $(SOURCEPATH)/external/job2; $(MAKE) doc
#	cd $(SOURCEPATH)/external/flow2; $(MAKE) doc
#	cd $(SOURCEPATH)/external/pr; $(MAKE) doc
