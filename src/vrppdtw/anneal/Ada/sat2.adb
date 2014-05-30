--  satest2.ada
-- Simple experiments with simulated annealing
-- find the nonlinear least squares optimum solution
-- Uses Cauchy training	

with TEXT_IO, SimAnneal, BArray, Random, Ada.Numerics.generic_elementary_functions;
use TEXT_IO, BArray;

procedure sat2 is

	n				: integer := 0;
        x, y				: BasicArrayHandle;
        
	function energy(a : in BasicArrayHandle) return float;
        
	package io_float is new float_io(float);
        package io_integer is new integer_io(integer);
        package functs_float is new Ada.Numerics.generic_elementary_functions(float);

	package my_anneal is new SimAnneal( energy );

        use io_float, io_integer, functs_float, my_anneal;
        
	Initialization_Error	: exception;
        
	maxit : constant := 400;
        lambda : constant float := 0.0;		-- derivative factor

	max_line_length : constant := 256;
        
	input_file	: file_type;
        last_1		: natural;	-- index into input_file_name
        input_file_name : string(1..max_line_length);

        z, t0, t			: float;
        fstatus				: float;
	a				: BasicArrayHandle;


	-- the function to minimize
	function func(x : float; a: float; b : float) return float is
	fret : float;
	begin
		fret := (1.0 - x * x / (a * a) ) * exp( - x * x / (b * b) );
		return fret;
	end func;

	function fprime_a(x : float; a : float; b : float) return float is
	f : float;
        begin
		f := 2.0 * x * x * exp( - x * x / (b * b) ) / ( a * a * a );
		return f;
	end fprime_a;

	function fprime_b(x : float; a : float; b : float) return float is
	f : float;
        begin
		f := func(x,a,b) * 2.0 * x * x / (b * b * b);
		return f;
	end fprime_b;


	-- the cost function
	function energy(a : in BasicArrayHandle) return float is
        val : float;
        sum : float := 0.0;
	begin
        	for i in 0..n-1 loop
			val := y(i) - func( x(i), a(0), a(1) );
			sum := sum + val * val + 
				lambda * ( abs( fprime_a(x(i), a(0), a(1)) ) +
				         abs( fprime_b(x(i), a(0), a(1)) ) );
                end loop;
                return sum;
	end energy;


	begin		-- procedure sat2

     put( "give data file name: ");
     get_line( item => input_file_name, last => last_1 );
     open( file => input_file, mode => in_file,
     	   name => input_file_name(1..last_1) );

     x := new BasicArray(0..n);
     y := new BasicArray(0..n);
     
      -- read input data
     while not end_of_file( input_file ) loop

	get(input_file, z );
	resize(x, n + 1);
	x(n) := z;

	get(input_file, z );
	resize(y, n + 1);
	y(n) := z;

	n := n + 1;			-- n is now the number of elements

     end loop;
     
     close( input_file );


        put( n ); put(" points read for analysis" ); new_line;
        
	a := new BasicArray(0..1);

        for i in a'range loop
		a(i) := random.uniform(0.0, 10.0);
        end loop;
        
	z := energy( a );

	if  SAInit(2) = 0  then
        	raise Initialization_Error;
	end if;

	SAInitial(a);		-- set initial condition

	fstatus := SABoltzmann( 0.5 );
        
	put("Boltzman constant: "); put( SABoltzmann );
        put("   learning rate: "); put( SALearning_Rate ); new_line;

	put( "Initial values: " ); put(a,3);
        put( "(energy = "); put( z ); put(')'); new_line;
        
	fstatus := SAMelt;		-- melt the system
	SACurrent( a );
        z := energy( a );

	put( "Melt values: " ); put(a,3);
        put( "(energy = "); put( z ); put(')'); new_line;

	-- make it a bit warmer than melting temperature
	t0 := 1.2 * SAtemperature;

	fstatus := SAtemperature( t0 );
	
	t := SAanneal( MAXIT );		-- do the annealing
	SACurrent( a );
	z := energy( a );
        
	put( "Initial temperature: "); put(t0);
        put( "   final temperature: "); put( t ); new_line;
	put( "Estimated minumum at: "); put( a, 3);
        put( "(energy = "); put( z ); put(')'); new_line;

	SAOptimum( a );
	z := energy( a );

	put( "Best minumum at: "); put( a, 3);
        put( "(energy = "); put( z ); put(')'); new_line;

exception
	when Initialization_Error =>
        	put("Error in intializing Simulated Annealing package");
                new_line;
              
end sat2;			





