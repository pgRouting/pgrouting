# Makefile for Ada version of Simulated Annealing code
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
#       set HOME apprpropriately
#	set CLIB, CLASSDIR, and SMPL to the current directory (.)
#	set SYSTYPE and BSD if necessary (or just comment them out if not
#					  relevant)
#	uncomment LIB_CODE, and LIB as is appropriate
#
#	under DOS all the .o extensions need to be globally changed to .obj,
#	the -o filename  option in the compile statements need to be removed,
#       undefine the MATHLIB option and change the -l for loading the library
#
HOME = .
MACHINE = LINUX
LIB_DIR = /usr/local/lib
LIBDIR = $(LIB_DIR)
#
CC = gcc
CFLAGS = -I$(HOME)
#
#
ADA = ada
ADA_FLAGS =
BIND = gnatbl
#
#
GCC_EXEC_PREFIX = /usr/local/Ada/lib/gcc-lib/i486-linux/2.7.2.1/
C_INCLUDE_PATH = /usr/local/Ada/lib/gcc-lib/i486-linux/2.7.2.1/include
#
# where to look for the 'ads' files
ADA_INCLUDE_PATH = /usr/local/Ada/adainclude
# where to look for the 'ali' files
ADA_OBJECTS_PATH = /usr/local/Ada/lib/gcc-lib/i486-linux/2.7.2.1/adalib
#
#
#
#
#
# =============PROBABLY NO ARE NECESSARY BEYOND HERE (except for DOS)=============
#
all:	sat1 sat2 satest1 satest2

sat1: barray.ali random.ali r250.ali c_bitops.o sat1.ali
	$(BIND) -o sat1 sat1.ali c_bitops.o

sat2: barray.ali random.ali r250.ali c_bitops.o sat2.ali
	$(BIND) -o sat2 sat2.ali c_bitops.o
#
r250.ali: r250.adb r250.ads bitops.ali

#
ADA_SOURCES = simann.ads simann.adb sat1.ada sat2.ada barray.ads barray.adb \
	      r250.ada random.ads random.adb

#
#
#
#
#
.SUFFIXES :
#	generic object code
#
.SUFFIXES: .adb .ads .ali .c .o

.adb.o:
	$(ADA) $(ADA_FLAGS) -c $*.adb

.ads.o:
	$(ADA) $(ADA_FLAGS) -c $*.ads

.adb.ali:
	$(ADA) $(ADA_FLAGS) -c $*.adb

.ads.ali:
	$(ADA) $(ADA_FLAGS) -c $*.ads

.c.o:
	$(CC) $(CFLAGS) -c $*.c

#

