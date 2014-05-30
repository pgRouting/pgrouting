// randgen.c++
//                      (c) Copyright 1994, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.

static const char rcsid[] = "@(#)randgen.c++	1.5 14:38:16 7/25/94   EFC";

#include <randgen.hpp>

ostream* RandomGenerator::ostr = &cerr;


// everything else is either inline or pure virtual
