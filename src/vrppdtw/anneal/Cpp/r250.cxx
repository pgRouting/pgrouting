/* r250.cpp	the r250 uniform random number algorithm, modified for 32 bits

                      (c) Copyright 1994, Everett F. Carter Jr.
                      Permission is granted by the author to use
	              this software for any application provided this
		      copyright notice is preserved.

		see:


		Kirkpatrick, S., and E. Stoll, 1981; A Very Fast Shift-Register
	        Sequence Random Number Generator, Journal of Computational Physics,
		V. 40.

		Maier, W.L., 1991; A Fast Pseudo Random Number Generator,
		Dr. Dobb's Journal, May, pp. 152 - 157


	Caution:  For 64 bit machines you will need to modify MSB, ALL_BITS, etc.


*/

static const char rcsid[] = "@(#)r250.c++	1.7 14:37:51 7/25/94   EFC";

#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include <r250.hpp>
#include <registrar.hpp>

// set the following if you trust rand()
// #define TRUST_RAND

#ifndef TRUST_RAND
// #include <randsmpl.hpp>
#include <rndlcg.hpp>
#endif

#if defined( linux ) && defined( BITS )
#undef BITS
#endif


#define BITS 31

#if WORD_BIT == 32
#ifndef BITS
#define BITS	32
#endif
#else
#ifndef BITS
#define BITS    16
#endif
#endif


#if BITS == 31                   
#define MSB          0x40000000L
#define ALL_BITS     0x7fffffffL
#define HALF_RANGE   0x20000000L
#define STEP         7
#endif

#if BITS == 32                   
#define MSB          0x80000000L
#define ALL_BITS     0xffffffffL
#define HALF_RANGE   0x40000000L
#define STEP         7
#endif

#if BITS == 16
#define MSB         0x8000
#define ALL_BITS    0xffff
#define HALF_RANGE  0x4000
#define STEP        11
#endif


unsigned long int* R250::r250_buffer = NULL;
int R250::r250_index = 0;
int R250::instances = 0;

R250::R250()
{
        unsigned int ser_no = registrar.serial_number();

	instances++;
	seed(12331+ser_no);
}

void R250::init(const long int sd)
{
	int j, k;
	unsigned long int mask, msb;

	if ( r250_buffer == NULL )	// only do init once
		r250_buffer = new unsigned long int[250];

#ifdef TRUST_RAND        

#if BITS == 32 || BITS == 31       
	srand48( sd );
#else
	srand( sd );
#endif	


#else
	//RandomSMPL ran;
	//ran.seed( sd );
	RandLCG ran( sd );
#endif
	
	r250_index = 0;
	for (j = 0; j < 250; j++)	/* fill r250 buffer with 15 bit values */
#ifdef TRUST_RAND
#if BITS == 32 || BITS == 31
		r250_buffer[j] = (unsigned long int)lrand48();
#else
		r250_buffer[j] = rand();
#endif
#else
		// r250_buffer[j] = (unsigned long int)(ALL_BITS * ran.ranf());
		r250_buffer[j] = ran.rani();
#endif

	for (j = 0; j < 250; j++)	/* set some MSBs to 1 */
#ifdef TRUST_RAND
		if ( rand() > HALF_RANGE )
			r250_buffer[j] |= MSB;
#else
		if ( ran.rani() > HALF_RANGE )
			r250_buffer[j] |= MSB;
#endif


	msb = MSB;		/* turn on diagonal bit */
	mask = ALL_BITS;	/* turn off the leftmost bits */

	for (j = 0; j < BITS; j++)
	{
		k = STEP * j + 3;	/* select a word to operate on */
		r250_buffer[k] &= mask; /* turn off bits left of the diagonal */
		r250_buffer[k] |= msb;	/* turn on the diagonal bit */
		mask >>= 1;
		msb  >>= 1;
	}

}

long R250::seed(const long sd)
{
	if ( sd > 0 )
        {
        	seed_val = sd;
		init( seed_val );
        }

	return seed_val;
        
}

unsigned long int R250::rani()	/* returns a random unsigned integer */
{
	register int	j;
	register unsigned long int new_rand;

	if ( r250_index >= 147 )
		j = r250_index - 147;	/* wrap pointer around */
	else
		j = r250_index + 103;

	new_rand = r250_buffer[ r250_index ] ^ r250_buffer[ j ];
	r250_buffer[ r250_index ] = new_rand;

	if ( r250_index >= 249 )	/* increment pointer for next time */
		r250_index = 0;
	else
		r250_index++;

	return new_rand;

}


Real R250::ranf()		/* returns a random double in range 0..1 */
{
	register int	j;
	register unsigned long int new_rand;

	if ( r250_index >= 147 )
		j = r250_index - 147;	/* wrap pointer around */
	else
		j = r250_index + 103;

	new_rand = r250_buffer[ r250_index ] ^ r250_buffer[ j ];
	r250_buffer[ r250_index ] = new_rand;

	if ( r250_index >= 249 )	/* increment pointer for next time */
		r250_index = 0;
	else
		r250_index++;

	return new_rand / (Real)( (unsigned long int)ALL_BITS );


}







