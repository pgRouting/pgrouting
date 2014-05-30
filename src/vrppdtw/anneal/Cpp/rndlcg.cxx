// rndlcd.c++   Implementation of Linear Congruential Method
//
//                      (c) Copyright 1994, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.

static const char rcsid[] = "@(#)rndlcg.c++	1.2 14:38:46 7/25/94   EFC";


#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include <rndlcg.hpp>
#include <registrar.hpp>

#define ALL_BITS     0xffffffff

int RandLCG::instances = 0;

const long int RandLCG::quotient  = LONG_MAX / 16807L;
const long int RandLCG::remainder = LONG_MAX % 16807L;

RandLCG::RandLCG()
{
        unsigned int ser_no = registrar.serial_number();

        instances++;
        seed(12331+ser_no);
}

long RandLCG::seed(const long sd)
{
        if ( sd > 0 )
                seed_val = sd;

        return seed_val;
}

unsigned long int RandLCG::rani()       /* returns a random unsigned integer */
{
	if ( seed_val <= quotient )
		seed_val = (seed_val * 16807L) % LONG_MAX;
	else
	{
		long int high_part = seed_val / quotient;
		long int low_part  = seed_val % quotient;

		long int test = 16807L * low_part - remainder * high_part;

		if ( test > 0 )
			seed_val = test;
		else
			seed_val = test + LONG_MAX;

	}

	return seed_val; 
}

Real RandLCG::ranf()               /* returns a random double in range 0..1 */
{
	unsigned long int new_rand = rani();

	return new_rand / (Real)( (unsigned long int) ALL_BITS );

}




