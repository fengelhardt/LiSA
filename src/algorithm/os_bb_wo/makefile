
#makefile fuer openshop

CC		=cc -O4 
CFLAGS		= -I.





A		= main.o table.o list.o stack.o block.o branch.o heur1.o \
                  init.o low.o read.o select.o sort.o calc.o fix.o head.o

B 		= main.o table.o list.o stack.o block.o branch.o heur2.o \
                  init.o low.o read.o select.o sort.o calc.o fix.o head.o

C		= main.o table.o list.o stack.o block.o branch.o heur3.o \
                  init.o low.o read.o select.o sort.o calc.o fix.o head.o

D 		= main.o table.o list.o stack.o block.o branch.o heur4.o \
                  init.o low.o read.o select.o sort.o calc.o fix.o head.o

E		= main.o table.o list.o stack.o block.o branch.o heur5.o \
                  init.o low.o read.o select.o sort.o calc.o fix.o head.o

F		= main.o table.o list.o stack.o block.o branch.o heur6.o \
                  init.o low.o read.o select.o sort.o calc.o fix.o head.o

G		= main.o table.o list.o stack.o block.o branch.o heur7.o \
                  init.o low.o read.o select.o sort.o calc.o fix.o head.o

H		= main.o table.o list.o stack.o block.o branch.o heur8.o \
                  init.o low.o read.o select.o sort.o calc.o fix.o head.o



open1:		$A
		$(CC) -o $@ $A

open2:		$B
		$(CC) -o $@ $B

open3:		$C
		$(CC) -o $@ $C

open4:		$D
		$(CC) -o $@ $D

open5:		$E
		$(CC) -o $@ $E

open6:		$F
		$(CC) -o $@ $F

open7:		$G
		$(CC) -o $@ $G

open8:		$H
		$(CC) -o $@ $H


main.o:		main.c data.h table.h list.h stack.h block.h branch.h fix.h \
                head.h heur.h init.h low.h read.h select.h

table.o:	table.c table.h data.h

block.o:	block.c block.h calc.h data.h table.h list.h

branch.o:	branch.c branch.h data.h table.h list.h

calc.o:		calc.c table.h

fix.o:		fix.c fix.h data.h table.h list.h

head.o:		head.c head.h data.h table.h list.h sort.h

heur1.o:	heur1.c heur.h data.h table.h list.h low.h calc.h

heur2.o:	heur2.c heur.h data.h table.h list.h low.h calc.h

heur3.o:	heur3.c heur.h data.h table.h list.h low.h calc.h

heur4.o:	heur4.c heur.h data.h table.h list.h low.h calc.h

heur5.o:	heur5.c heur.h data.h table.h list.h low.h calc.h

heur6.o:	heur6.c heur.h data.h table.h list.h low.h calc.h

heur7.o:	heur7.c heur.h data.h table.h list.h low.h calc.h

heur8.o:	heur8.c heur.h data.h table.h list.h low.h calc.h

init.o:		init.c init.h data.h table.h

list.o:		list.c list.h data.h table.h

low.o:		low.c low.h data.h table.h list.h sort.h

read.o:		read.c read.h data.h table.h list.h

select.o:	select.c select.h data.h table.h list.h head.h sort.h

sort.o:		sort.c sort.h data.h

stack.o:	stack.c stack.h data.h table.h list.h

