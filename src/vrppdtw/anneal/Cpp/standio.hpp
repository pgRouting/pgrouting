// standio.hpp		A pair of classes to handle I/O from EITHER a
//			named file OR STDIN/STDOUT

//	tested on MSDOS with Zortech C++ 3.0	       Borland C++ 3.0
//	on Unix  with AT&T 2.1 on MIPS and SGI Indigo workstations

//                     (c) Copyright 1995, Everett F. Carter Jr.
//                     Permission is granted by the author to use
//		       this software for any application provided this
//		       copyright notice is preserved.

// rcsid: $Id$   EFC

#ifndef STANDARD_IO_HPP_
#define STANDARD_IO_HPP_ $Revision:   1.8  $

#include <stdlib.h>
#include <iostream.h>

#ifdef __ZTC__
#include <fstream.hpp>
#ifndef LOCAL_FILEBUF		/* Zortech MUST have this define ON */
#define LOCAL_FILEBUF
#endif
#else
#include <fstream.h>
#endif

#ifdef __GNUC__
#include <streambuf.h>
#endif

class Stdin : public streambuf, public ifstream, virtual public ios
{
	private:
          char *fname;
#ifdef LOCAL_FILEBUF
	  filebuf InputFile;
#endif
	  int do_open_input(char* infile, int mode = ios::in );
          void open_input(int *argc, char **argv, int mode); 
        public:
          Stdin(int *argc, char **argv, int mode = 0);
          Stdin(char* infile = NULL, int mode = 0);
         ~Stdin() { if (fname) close(); }
	  const char* name() const;
                                     
};

class Stdout : public streambuf, public ofstream, virtual public ios
{
	private:
          char *fname;
#ifdef LOCAL_FILEBUF
	  filebuf OutputFile;
#endif
	  int  do_open_output(char* outfile, int mode = ios::out );
	  void open_output(int *argc, char **argv, int mode);
        public:
          Stdout(int *argc, char **argv, int mode = 0);
          Stdout(char* outfile = NULL, int mode = 0);
         ~Stdout() { if ( fname ) { flush(); close(); } }
	  const char* name() const;
};

#endif





