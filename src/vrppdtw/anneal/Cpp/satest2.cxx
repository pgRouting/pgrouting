/* satest2.c++
   Simple experiments with simulated annealing

   find the nonlinear least squares optimum solution

   usage:   satest2 input.file
   
*/



static const char rcsid[] = "@(#)satest2.c++	1.1 09:50:25 10/30/92   EFC";

#define PROGRAM "satest2"

#include <iostream.h>
#include <stdlib.h>
#include <math.h>

#include "simann.hpp"

#include <standio.hpp>
#include <barray.hpp>
#include <cputime.hpp>
#include <random.hpp>

// #define MELT_ONLY
// #define EQUIL_ONLY

#define MAXIT 400

float lambda = 0.0;		/* derivative weight factor */

BasicArray x, y;
int n;

float energy(float*);		// the cost function

float func(float,float,float), fprime_a(float,float,float);
float fprime_b(float,float,float);

void main(int argc, char **argv)
{
	int i;
	float z, t0, t;
	float a[2];

        
	// first read in the dataset
	Stdin fin(&argc, argv);
        
	if ( !fin )
        {
		cerr << PROGRAM << " unable to open input file " << argv[1] << endl;
                exit(1);
        }

	// read x, y pairs in
	i = 0;
	while ( !fin.eof() )
	{
		fin >> z;
		if ( fin.eof() || fin.fail() )
				break;
		x.resize( ++i );
		y.resize( i );
		x[i-1] = z;
		fin >> z;
		y[i-1] = z;
	}
	n = i;

	cerr << n << " points read for analysis\n";

	CPUTime *cpu = new CPUTime;

	SimAnneal sa(energy,2);

	if ( !sa )
        {
        	cerr << "problem initializing SimAnneal object\n";
                exit(1);
        }

       	RUniform uniform;


	// now start up the estimation
	for (i = 0; i < 2; i++)
	{
		a[i] = uniform.number(0.0, 10.0);
	}

	z = energy( a );

	
	sa.initial( a );		// set initial condition

	sa.Boltzmann( 0.5 );
	// sa.jump( 150.0 );


	cout << "Boltzman constant: " << sa.Boltzmann() << "\tlearning rate: "
				      << sa.learning_rate() << '\n';
	cout << "initial values: ";
	for (i = 0; i < 2; i++)
		cout << a[i] << '\t';
	cout << "    (energy = " << z << ")" << endl;

	sa.melt();		// melt the system

	sa.current( a );
	z = energy( a );

	cout << "melt values   : ";
	for (i = 0; i < 2; i++)
		cout << a[i] << '\t';
	cout << "    (energy = " << z << ")" << endl;

#ifdef MELT_ONLY
	t0 = sa.temperature();
#else
	/* make it a bit warmer than melting temperature */
	t0 = 1.2 * sa.temperature();

	sa.temperature( t0 );

#ifndef EQUIL_ONLY	
	t = sa.anneal( MAXIT );
	sa.current( a );
#endif

#endif

	z = energy( a );

	cout << "initial temperature: " << t0 << '\t';
	cout << "final temperature: " << t << '\n';

	cout << "Estimated minumum at: ";
	for (i = 0; i < 2; i++)
		cout << a[i] << '\t';
	cout << "    (energy = " << z << ")\n";;


	sa.optimum( a );
	z = energy( a );

	cout << "Best minumum at: ";
	for (i = 0; i < 2; i++)
		cout << a[i] << '\t';
	cout << "    (energy = " << z << ")\n";;

	delete cpu;
        
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





