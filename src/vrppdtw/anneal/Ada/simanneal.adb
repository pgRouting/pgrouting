--   simann.adb
--   a collection of routines for general purpose Simulated Annealing
--   intended to be the Ada equivalent of the C++ Simulated Annealing object
--   SimAnneal
--                      (c) Copyright 1994, Everett F. Carter Jr.
--                      Permission is granted by the author to use
--			this software for any application provided this
--			copyright notice is preserved.

--   Used Cauchy training

with Ada.Numerics.generic_elementary_functions, random;

package body SimAnneal is

	package functs_float is new Ada.Numerics.generic_elementary_functions(float);

        use functs_float;

type SimAnnealRec is record

	dimension, maxit	: integer;
        dt, c_jump, K, rho, t0 , tscale, y, ybest	: float;
	x, xnew, xbest		: BasicArrayHandle;
	
end record;

       PI	: constant float := 3.1415926536;
       PI2	: constant float := PI/2.0;
       HUGE	: constant float := 1.0e20;
       
	s	: SimAnnealRec;

-- iterate a few times at the present temperature */
-- to get to thermal equilibrium */

	function equilibrate(t : float; n : integer) return integer is

		i	: integer;
                equil   : integer := 0;
                xc, ynew, c	: float;
                del	: float := 1.0;
                xtmp	: BasicArrayHandle;
	begin
          i := 0;
          while i < n loop

		for j in 0..s.dimension-1 loop
			xc := s.rho * t * tan ( random.uniform( -PI2, PI2 ) ); 
			s.xnew(j) := s.x(j) + xc;
		end loop;
                
		-- "energy"
		ynew := func( s.xnew );
		c := ynew - s.y;
		
		if c < 0.0 then		-- keep xnew if energy is reduced
                
			xtmp := s.x;
			s.x := s.xnew;
			s.xnew := xtmp;

			s.y := ynew;
			if  s.y < s.ybest then

				for j in 0..s.dimension-1 loop
					s.xbest(j) := s.x(j);
                                end loop;
				s.ybest := s.y;
			end if;

			del := abs( c );
                        if  s.y /= 0.0 then
                        	del := del / s.y;
                        elsif ynew /= 0.0 then
				del := del / ynew;
                        end if;

			-- equilibrium is defined as a 10% or smaller change
			-- in 10 iterations 
			if  del < 0.10 then
				equil := equil + 1;
			else
				equil := 0;
			end if;

                else
				equil := equil + 1;
		end if;
		
		exit when equil > 9;

		i := i + 1;
                
	end loop;

	return i + 1;
        
end equilibrate;


	-- initialize internal variables
	function SAInit(d : integer) return integer is
                begin
		  s.dimension := d;
        	  s.t0 := 0.0;
        	  s.K  := 1.0;
        	  s.rho := 0.5;
        	  s.dt  := 0.1;
        	  s.tscale := 0.1;
        	  s.maxit := 400;
        	  s.c_jump := 100.0;
                  
		s.x     := new BasicArray(0..d-1);
		s.xnew  := new BasicArray(0..d-1);
		s.xbest := new BasicArray(0..d-1);

        	s.y := HUGE;
        	s.ybest := HUGE;

   		return 1;
        
	end SAInit;

	procedure SAFree is
	begin
        	s.dimension := 0;
        end SAFree;

	function SAIterations(m : integer := -1) return integer is
           begin
	     if m > 0 then
             	s.maxit := m;
             end if;
             return s.maxit;
	   end SAIterations;
           

	function SABoltzmann(k : float := -1.0) return float is
           begin
	     if k > 0.0 then
        	s.K := k;
	     end if;
             return s.K;
	   end SABoltzmann;

	function SATemperature(t : float := -1.0) return float is
           begin
	     if t > 0.0 then
        	s.t0 := t;
	     end if;
       	     return s.t0;
	   end SATemperature;

	function SALearning_Rate(r : float := -1.0) return float is
           begin
	     if r > 0.0 then
        	s.rho := r;
	     end if;
             return s.rho;
	   end SALearning_Rate;

	function SAJump(j : float := -1.0) return float is
           begin
	     if j > 0.0 then
        	s.c_jump := j;
	     end if;
             return s.c_jump;
           end SAJump;

	procedure SAInitial(xi : in BasicArrayHandle) is
	   begin
	     for k in xi'range loop
		s.x(k) := xi(k);
             end loop;
           end SAInitial;   

	procedure SACurrent(xc : BasicArrayHandle) is
           begin
	     for k in s.x'range loop
		xc(k) := s.x(k);
	     end loop;
           end SACurrent;

	procedure SAOptimum(xb : BasicArrayHandle) is
           begin
	     for k in s.xbest'range loop
		xb(k) := s.xbest(k);
	     end loop;
           end SAOptimum;


	-- increase the temperature until the system "melts"

        function SAMelt(iters : integer := -1) return float is
            n		: integer := iters;
            ok		: Boolean := FALSE;
            status	: integer;
            xc, ynew, t, cold : float;
            c		: float := 0.0;
          begin
            if n < 1  then
            	n := s.maxit;
            end if;          

	    t := s.t0;

	    for i in 0..n-1 loop
                 if i > 0 and c > 0.0 then
                          cold := c;
                          ok   := TRUE;
                 end if;	     
		
		t := t + s.dt;

		for j in 0..s.dimension-1 loop
			xc := s.rho * t * tan ( random.uniform( -PI2, PI2 ) ); 
			s.x(j) := s.x(j) + xc;
		end loop;

		status := equilibrate( t, 10);
		
		-- "energy"
		ynew := func( s.x );
		c := ynew - s.y;

		if  c < 0.0 and ynew < s.ybest then
			for j in 0..s.dimension-1 loop
				s.xbest(j) := s.x(j);
                        end loop;
			s.ybest := ynew;
		end if;

		s.y := ynew;

		-- phase transition
                if  ok = TRUE and c > (s.c_jump * cold) then
			exit;
		end if;
                
	end loop;

	s.t0 := t;
	return t;

	end SAMelt;

	-- cool the system with annealing

        function SAAnneal( iters : integer := -1 ) return float is
            n				  : integer := iters;
            status			  : integer;
            xc, p, ynew, t, c, dt, told   : float;
            xtmp			  : BasicArrayHandle;

	begin	

	  if  n < 1 then
		n := s.maxit;
	  end if;
          
	status := equilibrate( s.t0, 100 );

	told := s.t0;
	for i in 0..n-1 loop
		t := s.t0 /(1.0 + float(i) * s.tscale);
		dt := t - told;
		told := t;

		status := equilibrate(t, 20);

		for j in 0..s.dimension-1 loop
			xc := s.rho * t * tan ( random.uniform( -PI2, PI2 ) ); 
			s.xnew(j) := s.x(j) + xc;
		end loop;
                
		-- "energy"
		ynew := func( s.xnew );
		c := ynew - s.y;
		
		-- keep xnew if energy is reduced
                if ynew <= s.y then	
			xtmp := s.x;		-- I hope this pointer
			s.x := s.xnew;		-- shuffling works in Ada
			s.xnew := xtmp;
			s.y := ynew;

			if  s.y < s.ybest then
				for j in 0..s.dimension-1 loop
					s.xbest(j) := s.x(j);
                                end loop;
				s.ybest := s.y;
			end if;

		else
                        p := exp( - (ynew - s.y) / (s.K * t) );

                        if p > random.uniform(0.0, 1.0) then
                                xtmp := s.x;
				s.x := s.xnew;
				s.xnew := xtmp;
				s.y := ynew;
			end if;
		end if;

	end loop;

	status := equilibrate( t, 200);

	s.t0 := t;
        
	return t;

	end SAAnneal;
        

end SimAnneal;
