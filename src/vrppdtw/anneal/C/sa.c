/* a collection of C routines for general purpose Simulated Annealing
   intended to be the C equivalent of the C++ Simulated Annealing object
   SimAnneal

   Uses Cauchy training

*/


static char rcsid[] = "@(#)sa.c	1.2 15:54:31 3/30/93   EFC";

#include <stdio.h>
#include <math.h>
#include <float.h>

/* #include <stdlib.h>		/* for malloc */

/* #include <rands.h> */
#include <r250.h>

#include "sa.h"


/* #define DEBUG */

#ifdef _R250_H_
#define uniform(a,b)    ( a + (b - a) * dr250() )
#endif

#ifndef HUGE
#define HUGE	HUGE_VAL
#endif

#ifndef PI
#define PI		3.1415626536
#endif

#ifndef PI2
#define PI2		(PI/2.0)
#endif

typedef struct
{
	CostFunction func;
	int dimension, maxit, dwell;
	float *x, *xnew, *xbest;
	float dt, c_jump, K, rho, t0, tscale, range;
	double y, ybest;
} SimAnneal;

static SimAnneal s;

/* iterate a few times at the present temperature */
/* to get to thermal equilibrium */
#ifdef NO_PROTO
static int equilibrate(t, n)
float t;
int n;
#else
static int equilibrate(float t,int n)
#endif
{
	int i, j, equil = 0;
	float xc, ynew, c, delta, p;
	float *xtmp;

	delta = 1.0;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < s.dimension; j++)
		{
			xc = s.rho * t * tan ( uniform( -s.range, s.range ) ); 
			s.xnew[j] = s.x[j] + xc;
		}
		/* "energy" */
		ynew = s.func( s.xnew );
		c = ynew - s.y;
		
		if (c < 0.0)		/* keep xnew if energy is reduced */
		{
			xtmp = s.x;
			s.x = s.xnew;
			s.xnew = xtmp;

			s.y = ynew;
			if ( s.y < s.ybest )
			{
				for (j = 0; j < s.dimension; j++)
					s.xbest[j] = s.x[j];
				s.ybest = s.y;
			}

			delta = fabs( c );
			delta = ( s.y != 0.0 ) ? delta / s.y : ( ynew != 0.0 ) ?
					delta / ynew : delta;

			/* equilibrium is defined as a 10% or smaller change
			   in 10 iterations */
			if ( delta < 0.10 )
				equil++;
			else
				equil = 0;


		}
		else
		{
		/* keep xnew with probability, p, if ynew is increased */
/*
			p = exp( - (ynew - s.y) / (s.K * t) );

			if ( p > number(0.0, 1.0) )
			{
				xtmp = s.x;
				s.x = s.xnew;
				s.xnew = xtmp;
				s.y = ynew;
				equil = 0;
			}
			else
*/

				equil++;
		}
		
		if (equil > 9)
			break;



	}

	return i + 1;
}


/* initialize internal variables and define the cost function */
#ifdef NO_PROTO
int SAInit(f, d)
CostFunction f;
int d;
#else
int SAInit(CostFunction f, int d)
#endif
{
	int space;
        
	s.func = f;
        s.dimension = d;
        s.t0 = 0.0;
        s.K  = 1.0;
        s.rho = 0.5;
        s.dt  = 0.1;
        s.tscale = 0.1;
        s.maxit = 400;
        s.c_jump = 100.0;
	s.range = PI2;
	s.dwell = 20;

	space = s.dimension * sizeof(float);
        
        if ( (s.x = (float *)malloc( space ))  == NULL )
        		return 0;
        if ( (s.xnew = (float *)malloc( space )) == NULL )
        		return 0;
        if ( (s.xbest = (float *)malloc( space )) == NULL )
        		return 0;

        s.y = s.ybest = HUGE;

#ifdef _R250_H_
	r250_init( 12331 );
#endif

   	return 1;
        
}

void SAfree()
{
	free( s.x );
        free( s.xnew );
        free( s.xbest );
        s.dimension = 0;
}

#ifdef NO_PROTO
int SAiterations(m)
int m;
#else
int SAiterations(int m)
#endif
{
	if ( m > 0 )
        	s.maxit = m;

       return s.maxit;
}

#ifdef NO_PROTO
int SAdwell(m)
int m;
#else
int SAdwell(int m)
#endif
{
        if ( m > 0 )
                s.dwell = m;

       return s.dwell;
}



#ifdef NO_PROTO
float SABoltzmann(k)
float k;
#else
float SABoltzmann(float k)
#endif
{
	if ( k > 0.0 )
        	s.K = k;

       return s.K;
}

#ifdef NO_PROTO
float SAtemperature(t)
float t;
#else
float SAtemperature(float t)
#endif
{
	if ( t > 0.0 )
        	s.t0 = t;

       return s.t0;
}

#ifdef NO_PROTO
float SAlearning_rate(r)
float r;
#else
float SAlearning_rate(float r)
#endif
{
	if ( r > 0.0 )
        	s.rho = r;

       return s.rho;
}

#ifdef NO_PROTO
float SAjump(j)
float j;
#else
float SAjump(float j)
#endif
{
	if ( j > 0.0 )
        	s.c_jump = j;

       return s.c_jump;
}

#ifdef NO_PROTO
float SArange(r)
float r;
#else
float SArange(float r)
#endif
{
        if ( r > 0.0 )
                s.range = r;

       return s.range;
}


#ifdef NO_PROTO
void SAinitial(xi)
float* xi;
#else
void SAinitial(float* xi)
#endif
{
	int k;
	for (k = 0; k < s.dimension; k++)
		s.x[k] = xi[k];
}

#ifdef NO_PROTO
void SAcurrent(xc)
float* xc;
#else
void SAcurrent(float* xc)
#endif
{
	int k;
        
	for (k = 0; k < s.dimension; k++)
		xc[k] = s.x[k];
}

#ifdef NO_PROTO
void SAoptimum(xb)
float* xb;
#else
void SAoptimum(float* xb)
#endif
{
	int k;
        
	for (k = 0; k < s.dimension; k++)
		xb[k] = s.xbest[k];
}




/* increase the temperature until the system "melts" */
#ifdef NO_PROTO
float SAmelt(iters)
int iters;
#else
float SAmelt(int iters)	
#endif
{
	int i, j, ok = 0;
	float xc, ynew, t, cold, c = 0.0;

	int n = iters;
	if ( n < 1 )
		n = s.maxit;

	t = s.t0;

	for (i = 0; i < n; i++)
	{
		if (i > 0 && c > 0.0)
		{
			cold = c;
			ok = 1;
		}
		
		t += s.dt;

		for (j = 0; j < s.dimension; j++)
		{
			xc = s.rho * t * tan ( uniform( -s.range, s.range ) ); 
			s.x[j] += xc;
		}

		equilibrate( t, s.dwell);
		
		/* "energy" */
		ynew = s.func( s.x );
		c = ynew - s.y;

		if ( c < 0.0 && ynew < s.ybest)
		{
			for (j = 0; j < s.dimension; j++)
				s.xbest[j] = s.x[j];
			s.ybest = ynew;
		}

		s.y = ynew;

		if ( ok && c > (s.c_jump * cold) )	/* phase transition */
			break;

	}

	return s.t0 = t;
	
}

/* cool the system with annealing */
#ifdef NO_PROTO
float SAanneal(iters)
#else
float SAanneal(int iters)
#endif
{
	int i, j;
	float xc, p, ynew, t, c, dt, told;
	float *xtmp;


	int n = iters;
	if ( n < 1 )
		n = s.maxit;

	equilibrate( s.t0, 10 * s.dwell );

	told = s.t0;
	for (i = 0; i < n; i++)
	{
		t = s.t0 /(1.0 + i * s.tscale);
		dt = t - told;
		told = t;

		equilibrate(t, s.dwell);

		for (j = 0; j < s.dimension; j++)
	        {
			xc = s.rho * t * tan ( uniform( -s.range, s.range ) ); 
			s.xnew[j] = s.x[j] + xc;
		}
		/* "energy" */
		ynew = s.func( s.xnew );
		c = ynew - s.y;
		
		if (ynew <= s.y)	/* keep xnew if energy is reduced */
		{
			xtmp = s.x;
			s.x = s.xnew;
			s.xnew = xtmp;
			s.y = ynew;

			if ( s.y < s.ybest )
			{
				for (j = 0; j < s.dimension; j++)
					s.xbest[j] = s.x[j];
				s.ybest = s.y;
			}
			continue;
		}
		else
		{
		/* keep xnew with probability, p, if ynew is increased */
			p = exp( - (ynew - s.y) / (s.K * t) );

			if ( p > uniform(0.0, 1.0) )
			{
				xtmp = s.x;
				s.x = s.xnew;
				s.xnew = xtmp;
				s.y = ynew;
			}
		}

	}

	equilibrate( t, 10 * s.dwell );
	
	return s.t0 = t;
}


