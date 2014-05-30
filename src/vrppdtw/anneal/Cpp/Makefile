# Makefile for the C++ version of the Simulated Annealing code
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
MACHINE = Linux
UTIL = ../util
#
# AT&T cfront
#CC=cc
#CPP = CC
#CLASSLIB = classes
#
# Gnu C++ (V2.4.2)
#
CC=gcc
CPP=g++
#EXTRA_LIBS = -lg++
#
#
CFLAGS= -O -I. -I$(UTIL)
CPFLAGS = $(CFLAGS) -D$(MACHINE)
#
# The suffix for C++ code
C++ = cxx
O = o
#
#
#
#
ARGFILES = getargs.o stoi.o
RANDS = r250c.o randlcg.o
#
MATHLIB = -lm
#
# if library code is collected into an actual library
#LIB_CODE = $(CLASSDIR)/lib$(CLASSLIB).a
#LIB      = -L$(CLASSDIR) -l$(CLASSLIB)
#
# if library code is separate object files
LIB_CODE = cputime.o r250.o random.o rndlcg.o registrar.o standio.o barray.o \
           randgen.o erstream.o
LIB       = $(LIB_CODE)
#
#
# =============PROBABLY NO ARE NECESSARY BEYOND HERE (except for DOS)=============
#
all:	satest1 satest2

satest1: satest1.$(C++) simann.o $(ARGFILES) $(LIB_CODE)
	$(CPP) $(CPFLAGS) -o satest1 satest1.$(C++) simann.o $(ARGFILES) $(LIB) \
	$(EXTRA_LIBS) $(MATHLIB)

satest2: satest2.$(C++) simann.o $(ARGFILES) $(LIB_CODE)
	$(CPP) $(CPFLAGS) -o satest2 satest2.$(C++) simann.o $(ARGFILES) $(LIB) \
	$(EXTRA_LIBS) $(MATHLIB)

logfit: logfit.$(C++) simann.$(O) $(LIB_CODE)
	$(CPP) $(CPFLAGS) -o logfit logfit.$(C++) simann.$(O) $(LIB) \
	$(EXTRA_LIBS) $(MATHLIB)
#
#
#
#
#
cputime.o: cputime.cxx cputime.hpp
	$(CPP) $(CPFLAGS) -c cputime.cxx

getargs.o: $(UTIL)/getargs.c
	$(CC) $(CFLAGS) -c $(UTIL)/getargs.c

stoi.o: $(UTIL)/stoi.c
	$(CC) $(CFLAGS) -c $(UTIL)/stoi.c

#
#
.SUFFIXES :
.SUFFIXES : .c++ .cpp .cxx .c .h .o

.c++.o:
	$(CPP) $(CPFLAGS) -c $*.c++

.cpp.o:
	$(CPP) $(CPFLAGS) -c $*.cpp

.cxx.o:
	$(CPP) $(CPFLAGS) -c $*.cxx

.c.o:
	$(CC) $(CFLAGS) -c $*.c

#

