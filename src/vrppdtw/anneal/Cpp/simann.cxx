// simanneal.c++   Implementation of a General Purpose Simulated Annealing Class
//                      (c) Copyright 1994, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.

/* Uses Cauchy training		*/

static const char rcsid[] = "@(#)simann.c++	1.4 15:02:21 7/25/94   EFC";

#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "simann.hpp"

#ifndef HUGE
#define HUGE	HUGE_VAL
#endif

#ifndef PI2
#define PI2		(PI/2.0)
#endif

SimAnneal::SimAnneal(CostFunction f,const int d) : func(f),dimension(d),ddwell(20),
	rrange( PI2 ), t0(0.0), K(1.0), rho(0.5), dt(0.1), tscale(0.1), maxit(400),
	c_jump(100.0)
{

	x     = new float[dimension];
	xnew  = new float[dimension];
	xbest = new float[dimension];
	y = ybest = HUGE;

        if ( x == NULL || xnew == NULL || xbest == NULL )
        			err = -1;
        else
        			err = 0;
}

int SimAnneal::set_up(CostFunction f, const int d)
{

	dimension = d;

	func = f;

	x     = new float[dimension];
	xnew  = new float[dimension];
	xbest = new float[dimension];
	y = ybest = HUGE;

        if ( x == NULL || xnew == NULL || xbest == NULL )
        			err = -1;
        else
        			err = 0;

	return err;

}

/* increase the temperature until the system "melts" */
float SimAnneal::melt(const int iters)	
{
	int i, j, ok = 0;
	float xc, ynew, t, cold, c = 0.0;

	int n = iters;
	if ( n < 1 )
		n = maxit;

	t = t0;

	for (i = 0; i < n; i++)
	{
		if (i > 0 && c > 0.0)
		{
			cold = c;
			ok = 1;
		}
		
		t += dt;

		for (j = 0; j < dimension; j++)
		{
			xc = rho * t * tan ( uniform.number( -rrange, rrange ) ); 
			x[j] += xc;
		}

		equilibrate( t, ddwell);
		
		/* "energy" */
		ynew = func( x );
		c = ynew - y;

		if ( c < 0.0 && ynew < ybest)
		{
			for (j = 0; j < dimension; j++)
				xbest[j] = x[j];
			ybest = ynew;
		}

		y = ynew;

		if ( ok && c > (c_jump * cold) )	/* phase transition */
			break;

	}

	return t0 = t;
	
}

/* iterate a few times at the present temperature */
/* to get to thermal equilibrium */
int SimAnneal::equilibrate(const float t,const int n)
{
	int i, j, equil = 0;
	float xc, ynew, c, delta;
	float *xtmp;
//	float p;

	delta = 1.0;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < dimension; j++)
		{
			xc = rho * t * tan ( uniform.number( -rrange, rrange ) ); 
			xnew[j] = x[j] + xc;
		}
		/* "energy" */
		ynew = func( xnew );
		c = ynew - y;
		
		if (c < 0.0)		/* keep xnew if energy is reduced */
		{
			xtmp = x;
			x = xnew;
			xnew = xtmp;

			y = ynew;
			if ( y < ybest )
			{
				for (j = 0; j < dimension; j++)
					xbest[j] = x[j];
				ybest = y;
			}

			delta = fabs( c );
			delta = ( y != 0.0 ) ? delta / y : ( ynew != 0.0 ) ?
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
			p = exp( - (ynew - y) / (K * t) );

			if ( p > uniform.number(0.0, 1.0) )
			{
				xtmp = x;
				x = xnew;
				xnew = xtmp;
				y = ynew;
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

/* cool the system with annealing */
float SimAnneal::anneal(const int iters)
{
	int i, j;
	float xc, p, ynew, t, c, dt, told;
	float *xtmp;


	int n = iters;
	if ( n < 1 )
		n = maxit;

	equilibrate( t0, 10 * ddwell );

	told = t0;
	for (i = 0; i < n; i++)
	{
		t = t0 /(1.0 + i * tscale);
		dt = t - told;
		told = t;

		equilibrate(t, ddwell);

		for (j = 0; j < dimension; j++)
	        {
			xc = rho * t * tan ( uniform.number( -rrange, rrange ) ); 
			xnew[j] = x[j] + xc;
		}
		/* "energy" */
		ynew = func( xnew );
		c = ynew - y;
		
		if (ynew <= y)		/* keep xnew if energy is reduced */
		{
			xtmp = x;
			x = xnew;
			xnew = xtmp;
			y = ynew;

			if ( y < ybest )
			{
				for (j = 0; j < dimension; j++)
					xbest[j] = x[j];
				ybest = y;
			}
			continue;
		}
		else
		{
		/* keep xnew with probability, p, if ynew is increased */
			p = exp( - (ynew - y) / (K * t) );

			if ( p > uniform.number(0.0, 1.0) )
			{
				xtmp = x;
				x = xnew;
				xnew = xtmp;
				y = ynew;
			}
		}

	}

	equilibrate( t, 10 * ddwell );
	
	t0 = t;

	return t;
}

void SimAnneal::initial(float* xi)
{
	for (int k = 0; k < dimension; k++)
		x[k] = xi[k];
}

void SimAnneal::current(float* xc)
{
	for (int k = 0; k < dimension; k++)
		xc[k] = x[k];
}

void SimAnneal::optimum(float* xb)
{
	for (int k = 0; k < dimension; k++)
		xb[k] = xbest[k];
}








