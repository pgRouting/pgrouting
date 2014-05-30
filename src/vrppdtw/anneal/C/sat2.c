/* sat2.c
  Simple experiments with simulated annealing

  find the nonlinear least squares optimum solution
 
 usage: sat2 < input.data

  
 */


static char rcsid[] = "@(#)sat2.c	1.1 09:47:21 10/30/92   EFC";

#include <stdio.h>
#include <math.h>
#include <float.h>
/* #include <stdlib.h> */

#include "sa.h"

#ifdef USE_TIMER
#include <cputime.h>
#endif

/* #include <rands.h> */
#include <r250.h>

#ifdef _R250_H_
#define uniform(a,b)    ( a + (b - a) * dr250() )
#endif

/* #define MELT_ONLY  */
/* #define EQUIL_ONLY	*/

#define MAXIT 400

#ifndef BUF_SIZE
#define BUF_SIZE		256
#endif

float lambda = 0.0;		/* derivative weight factor */

int n = 0;
float *x, *y;

float energy(float*);		/* the cost function */

float func(float,float,float), fprime_a(float,float,float);
float fprime_b(float,float,float);

void main(int argc, char **argv)
{
	int i, nalloc;
	float z, t0, t;
	float a[2];
#ifdef MS_DOS
	long start_time, stop_time;
	double *time_ptr;
#endif
        
        /* read the data from stdin */
	nalloc = BUF_SIZE;
	x = (float *)malloc( nalloc * sizeof( float ) );
	y = (float *)malloc( nalloc * sizeof( float ) );

	/* read the data  */
	while ( scanf("%f %f",&x[n], &y[n]) == 2)
	{
		if (++n == nalloc)
		{
			nalloc += BUF_SIZE;
			x = (float *)realloc( x, nalloc * sizeof( float ) );
			y = (float *)realloc( y, nalloc * sizeof( float ) );
		}
	}


	fprintf(stderr,"%d points read for analysis\n", n);

#ifdef USE_TIMER
#ifdef MS_DOS
	time_ptr = (double *)malloc( sizeof( double ) );
	initializetimer();

	start_time = readtimer();
#else
	report_cpu( stderr, NULL );
#endif
#endif

	if ( SAInit(energy,2) == 0 )
        {
        	fprintf(stderr,"trouble initializing in SAInit\n");
                exit(1);
        }

        for (i = 0; i < 2; i++)
		a[i] = uniform(0.0, 10.0);

	z = energy( a );
	

	SAinitial( a );
        SABoltzmann( 0.5 );

/*	SAjump( 150.0 ); */
        
	fprintf(stderr,"Boltzman constant: %f\tlearning rate:%f\n",
        		SABoltzmann( NO_VALUE ), SAlearning_rate( NO_VALUE ) );


	fprintf(stderr,"Initial values: ");
        for (i = 0; i < 2; i++)
        	fprintf(stderr,"%g\t", a[i]);
        fprintf(stderr,"\t(energy = %g)\n", z);

	SAmelt( NO_VALUE );		/* melt the system */

	SAcurrent( a );
        z = energy( a );

	fprintf(stderr,"melt values: ");
        for (i = 0; i < 2; i++)
        	fprintf(stderr,"%g\t", a[i]);
        fprintf(stderr,"\t(energy = %g)\n", z);

#ifdef MELT_ONLY
	t0 = SAtemperature( NO_VALUE );
#else
	/* make it a bit warmer than melting temperature */
	t0 = 1.2 * SAtemperature( NO_VALUE );

	SAtemperature( t0 );

#ifndef EQUIL_ONLY	
	t = SAanneal( MAXIT );
	SAcurrent( a );
#endif

#endif
	z = energy( a );
        
	fprintf(stderr,"Initial temperature: %f\tfinal temperature: %f\n", t0, t);
	fprintf(stderr,"Estimated minimum at: ");
        for (i = 0; i < 2; i++)
        	fprintf(stderr,"%g\t", a[i]);
        fprintf(stderr,"\t(energy = %g)\n", z);

	SAoptimum( a );
	z = energy( a );

	fprintf(stderr,"Best minimum at: ");
        for (i = 0; i < 2; i++)
        	fprintf(stderr,"%g\t", a[i]);
        fprintf(stderr,"\t(energy = %g)\n", z);


#ifdef USE_TIMER
#ifdef MS_DOS
	stop_time =  readtimer();
	elapsedtime(start_time, stop_time, time_ptr );

	fprintf(stderr,"elapsed time = %f (ms)\n", *time_ptr );

	restoretimer();
#else
	report_cpu( stderr, NULL );
#endif

#endif

}

float func(float x,float a,float b)
{
	float f;

	f = (1.0 - x * x / (a * a) ) * exp( - x * x / (b * b) );

	return f;
}

float fprime_a(float x,float a,float b)
{
	float f;

	f = 2 * x * x * exp( - x * x / (b * b) ) / ( a * a * a );

	return f;
}

float fprime_b(float x,float a,float b)
{
	float f;

	f = func(x,a,b) * 2.0 * x * x / (b * b * b);

	return f;
}

/* evaluate the sum squared error, the "energy" */
float energy(float *a)
{
	int i;
	float sum = 0.0, val;

	for (i = 0; i < n; i++)
	{
		val = y[i] - func( x[i], a[0], a[1] );
		sum += val * val + 
			lambda * ( fabs( fprime_a(x[i], a[0], a[1]) ) +
				   fabs( fprime_b(x[i], a[0], a[1]) ) );
	}

	return sum;

}






