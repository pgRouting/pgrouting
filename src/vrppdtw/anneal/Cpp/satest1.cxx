/* satest1.c++

   Simple experiments with simulated annealing

   find the minumum of a function, there are two examples here
   one uses a simple cost function, the other imposes range constraints
   by making illegal values very costly

*/

static const char rcsid[] = "@(#)satest1.c++	1.1 09:50:13 10/30/92   EFC";

#include <stdlib.h>
#include <iostream.h>
#include <math.h>

#include "simann.hpp"

#include <random.hpp>

#ifndef HUGE
#define HUGE     HUGE_VAL
#endif

// Define ONE of the following
#define EXAMPLE_1
// #define EXAMPLE_2

#define MAXIT 400

// float lambda = 0.1;		/* derivative weight factor */
float lambda = 0.0;


float func(float*), f(float), fprime(float), newton(float);

int main(int , char** )
{
	float y, t0, t;
	float x[1], x_init;

	SimAnneal sa(func,1);

	if ( !sa )
        {
        	cerr << "problem initializing SimAnneal object\n";
                exit(1);
        }

	RUniform uniform(1);

	x_init = x[0] = uniform.number(-1.0, 1.0);

	sa.initial(x);		// set initial condition

	sa.melt();		// melt the system

	/* make it a bit warmer than melting temperature */
	t0 = 1.2 * sa.temperature();

	sa.temperature( t0 );
	
	t = sa.anneal( MAXIT );
	sa.optimum( x );

	cout << "Boltzman constant: " << sa.Boltzmann() << "\tlearning rate: "
				      << sa.learning_rate() << '\n';
	cout << "initial x: " << x_init << '\n';
	cout << "initial temperature: " << t0 << '\t';
	cout << "final temperature: " << t << '\n';

	cout << "Estimated minumum at: " << x[0] << '\n';
	cout << "\tf(" << x[0] << ") = " << f(x[0]);
	cout << "\tf'(" << x[0] << ") = " << fprime(x[0]) << '\n';

	y = newton( x[0] );
	cout << "newton (" << x[0] << ") = " << y << "\tfprime(" << y <<
		") = " << fprime(y) << endl;

	return 0;		
}


#ifdef EXAMPLE_1

float func(float x[])		// the cost function
{
	return f(x[0]) + lambda * fabs( fprime(x[0]) );
}

float f(float x)		// the function to minimize
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

float func(float x[])		// the cost function with constraints
{
	if ( x[0] < -2.0 || x[0] > 2.0)
        	return HUGE;
                
	return f(x[0]) + lambda * fabs( fprime(x[0]) );
}

float f(float x)		// the function to minimize
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




