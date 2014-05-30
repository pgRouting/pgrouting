

		Simulated Annealing Package


         	When one trys to estimate such things as the minimum, or the
         least-squares optimal parameters of a nonlinear system, the
         existence of multiple local minima can cause problems with the
         determination of the global optimum.   Techniques such as Newton's
         method, the Simplex method and Least-squares Linear Taylor
         Differential correction technique can be useful provided that
         one is lucky enough to start sufficiently close to the global minimum.
         All these methods suffer from the inability to distinguish a local
         minimum from a global one because they follow the local gradients
         towards the minimum.  Simulated annealing is a technique for solving
         such problems. It works by following local gradients, but occasionally
         it will move against the gradient in order to escape a local extremum.


		The attached code implements a straightforward Simulated
	Annealing package.  There are three versions of the package, one in
	C, one in C++ and one in Ada.  You will find each version in its own
	subdirectory.

		There are two sample driver programs for each package
	(the first example program for the C and C++ versions can be compiled
	 for two different examples).  The first example is finding the
	global minimum of a function that has multiple local minima
	(the second version of this example puts constraints on the range
	that the search can be done over).   The second example finds the
	optimal fit to the function:

		y = (1-(x/a)**2) exp( - (x/b)**2 )

	Since this function is nonlinear, there is no closed form analytic least
	squares solution.  There are two input data sets that can be used
	(data3n.dat and data3rn.dat each with different noise levels).  The
	actual parameters are a = 70.0, and b = 50.0 (note that because of the
	squares of the parameters, the positive and negative values of these
	parameters are equivalent).

		The C and C++ code is initializes with a pointer to the
	cost function and the number of parameters it contains.  The Ada
	code implements the library as a generic package and so the cost
	function is bound to the library at compile time.   The cost
	function should take as parameters a vector of the parameters and
	return a measure of how good the current estimate of the parameters
	is (the smaller the better).  Both versions of the first example are
	one dimensional, the second example is two dimensional.


	The routines:

		SAInit()  - (C, Ada)
				Informs the library of the cost function to
			      apply and its dimension. (The C++ equivalent is
			      the constructor for the object).  The Ada version
			      just accepts the number of dimensions of the system
			      as a parameter since the function to use is bound
			      to the library at compile time.
			      Returns a 0 if the initialization fails.


		SAfree()  - (C) 
				Releases internally allocated space for the
			       library. (The C++ equivalent is the destructor for
			       the object, there is no Ada equivalent).

		SAinitial() - (C, Ada)
		initial()   - (C++)
				Sets the initial guess values of the parameters
				to be optimized.

		SAcurrent()  - (C, Ada)
		current()    - (C++)
				Gets the current values of the parameters, after
				the annealing is performed, this the near optimum
				estimate of the parameters.

		SAoptimum()  - (C, Ada)
		optimum()    - (C++)
				Gets the best values of the parameters found so
				far.  Because during the final annealing process
				the iterations may have stopped during an upgradient
				exploration, the current values of the parameters
				may not be the best ones found (the more iterations
				that are performed, the less likely that this will
				be the case).  As a hedge against this possibility,
				the best values found are stored separately.  This
				routine returns this value.

		SAMelt()     - (C, Ada)
		melt()	     - (C++)
				This routine "heats" the system starting at the
				current temperature, until a phase change is
				detected.  The phase change is determined by the
				occurence by a large jump in the specific heat
				for the system.  This routine returns the melting
				temperature for diagnostic purposes.

		SAanneal()   - (C, Ada)
				This routine applies a Cauchy cooling schedule
				to the system in order to perform the annealing
				process.  This routine returns the final temperature
				of the system for diagnostic purposes.

		The following routines are all QUERY/SET functions,  they all
		provide a means to query and/or set certain internal parameters.
		In order to SET a value, just pass the desired quantity to the
		function.  To QUERY a value, in C pass the predefined quantity
		NO_VALUE, in C++ and Ada just call the function with no parameters.


		SABoltzmann()  - (C, Ada)
		Boltzmann()    - (C++)
				Set/Query the "Boltzmann" constant for this
				problem.  This constant is used as a scaling
				factor in the calculation of the probabilities
				for accepting upgradient changes in the parameters.

		SAlearning_rate() - (C, Ada)
		learning_rate()   - (C++)
				Set/Query the "learning rate".  This parameter
				determines how heavily to weight the temperature
				when calculating the parameter jump sizes.

		SAtemperature()  - (C, Ada)
		temperature()	 - (C++)
				Set/Query the system "temperature".  Ordinarily
				this is not set by the application, although I
				have often found it worthwhile to "heat" the
				system a little above the melting point before
				begining the annealing process.

		SAjump()        - (C, Ada)
		jump()          - (C++)
				Set/Query the critical value of the jump in
				the specific heat that is required for the system
				to be considered to be melted.  This value is
				the minimum Specific_Heat/Old_Specific_Heat that
				is to be used.

		SAiterations()  - (C, Ada)
		iterations()    - (C++)
				Set/Query the number of iterations to be used
				in the annealing process.  This can be used to
				set or query the default value that will be used
				if it is not specified in the SAanneal() ( or
				anneal() ) routine.

	Extensions to the code:

		The code is written to be as general as possible, trading off
	optimization in favor of generality.  There are almost certainly ways
	to speed up the code if the structure of a specific problem is accounted
	for.
		A possible extension that would be easy to do, is to allow more
	than one simulated annealing process to exist within a single program.
	This can be readily done in the C and Ada code because all the data is
	maintained within a hidden structure.  All that would need to be done
	is to have SAInit() create each structure dynamically and to return a
	handle to that structure instead of a 1/0.  All the other routines would
	then need to be modified to have an additional parameter that is the
	handle for the annealing object.  The C++ code supports multiple objects
	directly without the need for any modification.


	The Random number generator:

		I have provided a 31 or 16 bit implementation of the random number
	generator known as R250.  This generator not only has fairly good
	statistical properties, but it is also fast.  It compares about 15%
	faster than other random number generators that I have tried.  Its period
	is 2^249, so the 16 bit version is actually practical to use, the
	default compiler settings are for 31 bits.



	Portability notes:

		This code should port to different platforms readily.  I
	have run it on MS-DOS with Zortech C++ V3.0 and Borland C++ V3.0.
	On Unix I have run it with the AT&T V2.1 C++ compiler on MIPS and
	Silicon Graphics workstations and on the Linux operating system.
	The system dependencies that I am aware of can be handled by the
	macros within the Makefile.

	The CPU timer function may need to be rewritten for some systems.
	I have NOT provided an MS-DOS version. (I have a millisecond resolution
	CPU timer for MS-DOS and can provide it to those that request it).

	The Makefile was written for Unix systems.  Some changes will be
	necessary for using in on MS-DOS.  The site dependent stuff is
	all at the top of the Makefile, I have tried to make the changes
	obvious (but since this package has not been beta tested, I
	would appreciate any feedback).

	The C library is written to directly provide for either an ANSI-C
	compiler or the older style K&R compiler.  If you have the older
	compiler define the macro NO_PROTO (i.e. compile with -DNO_PROTO
	in the compile command).  The example programs are written for
	ANSI-C, but following the example of the include file SA.H, you
	should be able to hand edit the function prototypes so that it
	will compile under a K&R compiler.



	Intercomparison of packages:

		To within roundoff differences, these packages will give
	the same result regardless of the language being used.  However
	in order to make the comparison, the initial guess of the solution
	should not be done using the random number generator.  This is
	because the differences in how the global random number routine
	is handled in the procedural C code and the object oriented C++ code.




 Everett (Skip) Carter          Phone: 408-641-0645 FAX:  408-641-0647
 Taygeta Scientific Inc.        INTERNET: skip@taygeta.com
 1340 Munras Ave., Suite 314    UUCP:     ...!uunet!taygeta!skip
 Monterey, CA. 93940            WWW: http://www.taygeta.com/skip.html

