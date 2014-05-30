-- /* satest1.c */
-- /* Simple experiments with simulated annealing */
-- /* find the minumum of a compilcated function */
-- /* Uses Cauchy training		*/

with TEXT_IO, SimAnneal, BArray, Random, Ada.Numerics.generic_elementary_functions;
use TEXT_IO, BArray;

procedure sat1 is

	function func(x : in BasicArrayHandle) return float;
        
	package io_float is new float_io(float);
        package io_integer is new integer_io(integer);
        package functs_float is new Ada.Numerics.generic_elementary_functions(float);

	package my_anneal is new SimAnneal( func );

        use io_float, io_integer, functs_float, my_anneal;
        
	Initialization_Error	: exception;
        
	maxit : constant := 400;
        lambda : constant float := 0.1;		-- derivative factor

        y, t0, t, xbest, ybest, x_init	: float;
        fstatus				: float;
        x				: BasicArrayHandle;


	-- the function to minimize
	function f(x : float) return float is
	fret : float;
	begin
		fret := (x + 0.2 ) * x + cos( 14.5 * x - 0.3 );
		return fret;
	end f;

	function fprime(x : float) return float is
	f : float;
        begin
		f := 0.2 + 2.0 * x - 14.5 * sin( 14.5 * x - 0.3 );
		return f;
	end fprime;


	-- the cost function
	function func(x : in BasicArrayHandle) return float is
        retval : float;
	begin
		retval := f(x(0)) + lambda * abs( fprime(x(0)) );
                return retval;
	end func;

	-- estimate minimum near x */
	function newton(x : float) return float is
                fpp, local_x  : float;
                begin
                local_x := x;
		for iter in 0..50 loop
			fpp := 2.0 - 210.25 * cos( 14.5 * local_x - 0.3 );
			if fpp = 0.0 then
				exit;
                        end if;
			local_x := local_x - fprime(local_x) / fpp;
		end loop;

		return local_x;
		end newton;


	begin


	x := new BasicArray(0..0);
        
	x(0) := random.uniform(-1.0, 1.0);
	y := func( x );

	xbest := x(0);	ybest := y;
        
	if  SAInit(1) = 0  then
        	raise Initialization_Error;
	end if;

	put("Boltzman constant: "); put( SABoltzmann );
        put("   learning rate: "); put( SALearning_Rate ); new_line;

	SAInitial(x);		-- set initial condition

	fstatus := SAMelt;		-- melt the system

	-- make it a bit warmer than melting temperature
	t0 := 1.2 * SAtemperature;

	fstatus := SAtemperature( t0 );
	
	x_init := x(0);

	t := SAanneal( MAXIT );
	SAoptimum( x );

	put( "Initial temperature: "); put(t0);
        put( "   final temperature: "); put( t ); new_line;
	put( "Estimated minumum at: "); put( x(0) ); new_line;
	put("   f("); put(x(0)); put(") = "); put( f(x(0)) );
        put("   f'("); put( x(0) ); put(") = "); put( fprime(x(0)) ); new_line;

	y := newton( x(0) );
	put("newton ("); put(x(0)); put(") = "); put(y); new_line;
        put("f("); put(y); put(") = "); put(f(y));
        put(", f'("); put(y); put(") = "); put( fprime(y) ); new_line;
			

exception
	when Initialization_Error =>
        	put("Error in intializing Simulated Annealing package");
                new_line;
              
end sat1;			





