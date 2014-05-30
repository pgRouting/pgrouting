-- random.ads	Specification of Random Number package
--		this package does not generate the random number
--              directly but calls R250, it then returns the value
--              appropriately transformed.


--    (c) Copyright 1997 Everett F. Carter Jr.   Permission is granted
--    by the author to use this software for any application provided
--    this copyright notice is preserved.

-- $Author:$
-- $Workfile:$
-- $Revision:$
-- $Date:$

-- ==================================================================



package RANDOM is
	procedure SEED(sd : integer);
        	-- establish the seed for the sequence of random numbers
        function UNIT_RANDOM return FLOAT;
        	-- returns a random FLOAT >= 0.0 and < 1.0
        function RANDOM_INT(A : INTEGER; B : INTEGER)  return INTEGER;
		-- returns a random integer in the range A..B
        function UNIFORM(a : float; b : float) return FLOAT;
        	-- returns a random FLOAT >= a and <= b
                
	function Normal(m : in float; s : in float) return float;
                -- normal random number with mean m, sdv s
                
end RANDOM;









