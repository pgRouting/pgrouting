/* sat1.c

   Simple experiments with simulated annealing

   find the minumum of a function, there are two examples here
   one uses a simple cost function, the other imposes range constraints
   by making illegal values very costly

*/

static char rcsid[] = "@(#)sat1.c	1.1 09:47:16 10/30/92   EFC";

#include <stdio.h>
#include <math.h>
#include <float.h>

#include "sa.h"

#ifndef HUGE
#define HUGE     HUGE_VAL
#endif

/* use ONE or the other */
/* #include <rands.h> */
#include <r250.h>

#ifdef _R250_H_
#define uniform(a,b)    ( a + (b - a) * dr250() )
#endif


/* Define ONE of the following */
/* #define EXAMPLE_1 */
#define EXAMPLE_2

/* #define MELT_ONLY  */
/* #define EQUIL_ONLY	*/

#define MAXIT 400

float lambda = 0.1;		/* derivative weight factor */

float func(float*), f(float), fprime(float), newton(float);

void main(int argc, char **argv)
{
	int i;
	float y, t0, t, xbest, ybest;
	float x[1], x_init;

	
	if ( SAInit(func,1) == 0 )
        {
        	fprintf(stderr,"trouble initializing in SAInit\n");
                exit(1);
        }

	xbest = x[0] = uniform(-1.0, 1.0);
	ybest = y = func( x );

	fprintf(stderr,"Boltzman constant: %f\tlearning rate:%f\n",
        		SABoltzmann( NO_VALUE ), SAlearning_rate( NO_VALUE ) );

	SAinitial(x);		/* set initial condition */

	SAmelt( NO_VALUE );		/* melt the system  */

#ifndef MELT_ONLY

	/* make it a bit warmer than melting temperature */
	t0 = 1.2 * SAtemperature( NO_VALUE );

	SAtemperature( t0 );
	
	x_init = x[0];

#ifndef EQUIL_ONLY	
	t = SAanneal( MAXIT );
	SAoptimum( x );
#endif

#endif

	fprintf(stderr,"Initial temperature: %f\tfinal temperature: %f\n", t0, t);
	fprintf(stderr,"Estimated minumum at: %f\n", x[0]);
        fprintf(stderr,"\tf(%f) = %f\tf'(%f) = %g\n", x[0], f(x[0]), x[0],
		fprime(x[0]) );

	y = newton( x[0] );
  fprintf(stderr,"newton (%f) = %g\nf(%f) = %g, fprime(%f) = %g\nnewton(%f) = %g\n",
			x[0], y, y, f(y), y, fprime( y ), x[0], newton( x[0] ) );
			

			
}

#ifdef EXAMPLE_1

float func(float* x)		/* the cost function */
{
	return f(x[0]) + lambda * fabs( fprime(x[0]) );
}

float f(float x)		/* the function to minimize */
{
	float fret;

	fret = (x + 0.2 ) * x + cos( 14.5 * x - 0.3 );

	return fret;
}

float fprime(float x)
{
	float f;

	f = 0.2 + 2 * x - 14.5 * sin( 14.5 * x - 0.3 );

	return f;
}


float newton(float x)		/* estimate minimum near x */
{
	int iter;
	float fpp;

	for (iter = 0; iter < 50; iter++)
	{
		fpp = 2.0 - 210.25 * cos( 14.5 * x - 0.3 );
		if (fpp == 0.0)
			break;
		x -= fprime(x) / fpp;
	}

	return x;
}

#endif

#ifdef EXAMPLE_2

float func(float* x)		/* the cost function with constraints */
{
	if ( x[0] < -2.0 || x[0] > 2.0)
        	return HUGE;
                
	return f(x[0]) + lambda * fabs( fprime(x[0]) );
}

float f(float x)		/* the function to minimize */
{
	float fret;

	fret = ( (3.0 * x + 5.0 ) * x - 2.0) * x + 3.0;

	return fret;
}

float fprime(float x)
{
	float f;

	f = (9 * x + 10 ) * x - 2;

	return f;
}


float newton(float x)		/* estimate minimum near x */
{
	int iter;
	float fpp;

	for (iter = 0; iter < 50; iter++)
	{
		fpp = 18 * x + 10;
		if (fpp == 0.0)
			break;
		x -= fprime(x) / fpp;
	}

	return x;
}

#endif
