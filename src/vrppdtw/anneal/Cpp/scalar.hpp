// scalar.hpp		Macros to define the scalar type to be used in Arrays
//

// rcsid: $Id$ %W% %U% %G%   EFC

#ifndef _Scalar_HPP_
#define _Scalar_HPP_  %I%

#ifdef COMPLEX
#include <classlib/complex.hpp>
#endif

// REAL_F, REAL_D or COMPLEX needs to be defined either here or on the command line
// #define REAL_F		// for real (float) Vectors and Matrices
// #define REAL_D		// for real (double) Vectors and Matrices
// #define COMPLEX		// for complex Vectors and Matrices

#ifndef scalar_type

#ifdef COMPLEX
#define scalar_type	Complex
#endif

#ifdef REAL_D
#ifndef scalar_type
#define scalar_type double
#endif
#endif

#ifdef REAL_F
#ifndef scalar_type
#define scalar_type float
#endif
#endif

// just to be sure set to floating point if the all the above fall through
#ifndef scalar_type
#define REAL_F
#define scalar_type float
#endif

#endif


#endif







