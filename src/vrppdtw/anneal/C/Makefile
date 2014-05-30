# Makefile for C version of Simulated Annealing code
#
#
# To port to your machine:
#	set CFLAGS for your C compiler
#	set CC to your C compiler
#	set CPFLAGS for your C++ compiler
#	set CPP to your C++ compiler
#	set C++ to whatever extension your C++ compiler accepts (the files
#		in the archive have .cxx) and change those files to have that
#		extension.
#
#	under DOS all the .o extensions need to be globally changed to .obj,
#	the -o filename  option in the compile statements need to be removed,
#       undefine the MATHLIB option and change the -l for loading the library
#
MACHINE = MIPS
#
# Gnu C/C++
#
CC=gcc
CPP=gcc
#
#
UTIL = ../util
#
CFLAGS= -O -I. -I$(UTIL) -D$(MACHINE)
#
O = o
#
#
#
ARGFILES = getargs.o stoi.o
RANDS = r250.o randlcg.o
#
MATHLIB = -lm
#
#
#
# =============PROBABLY NO ARE NECESSARY BEYOND HERE (except for DOS)=============
#
all:	sat1 sat2

sat1:	sat1.c sa.o $(RANDS)
	$(CC) $(CFLAGS) -o sat1 sat1.c sa.o $(RANDS) $(MATHLIB)

#
#
cputime.o: $(UTIL)/cputime.c
	$(CC) $(CFLAGS) -c $(UTIL)/cputime.c

getargs.o: $(UTIL)/getargs.c
	$(CC) $(CFLAGS) -c $(UTIL)/getargs.c

stoi.o: $(UTIL)/stoi.c
	$(CC) $(CFLAGS) -c $(UTIL)/stoi.c

#
#
.SUFFIXES :
.SUFFIXES : .c .h .o

.c.o:
	$(CC) $(CFLAGS) -c $*.c

#

