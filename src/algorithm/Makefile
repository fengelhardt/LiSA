# LiSA Algorithm Modules Main Makefile
# Copyright Lars Dornheim 1998, 2002
# E-Mail: Lars.Dornheim@Student.Uni-Magdeburg.DE
# WWW: http://graf350.urz.uni-magdeburg.de/~dornheim/index.html

# ------------------------------------------------------------------------------

TOPPROGRAMPATH=../..
BINPATH=$(TOPPROGRAMPATH)/bin
DOCPATH=$(TOPPROGRAMPATH)/doc
OBJPATH=$(TOPPROGRAMPATH)/obj
SOURCEPATH=$(TOPPROGRAMPATH)/src
MODULENAME=algorithm
MODULEPATH=$(SOURCEPATH)/$(MODULENAME)

SKIP_MODULES_REGEX=/CVS$$

#MODULES=`find $(MODULEPATH) -type d -mindepth 1 -maxdepth 1 | egrep -v "$(SKIP_MODULES_REGEX)"`
MODULES=`find $(MODULEPATH)/* -type d  -prune | egrep -v "$(SKIP_MODULES_REGEX)"`

include $(SOURCEPATH)/Make.modules

