-- r250.ads	The R250 32 bit positive (e.q. 31 bit) random number
--		 generator specification.

-- This code is based upon the algorithm described in:
-- Kirkpatrick, S., and E. Stoll, 1981; A Very Fast Shift-Register
-- Sequence Random Number Generator, Journal of Computational Physics,
-- V. 40

--    (c) Copyright 1997 Everett F. Carter Jr.   Permission is granted
--    by the author to use this software for any application provided
--    this copyright notice is preserved.

-- $Author:   skip  $
-- $Workfile:   r250.ads  $
-- $Revision:   1.0  $
-- $Date:   24 Jul 1997 11:00:18  $

-- ==================================================================


package R250 is

	procedure r250_init(seed : integer);
        function  rani return integer;
        function  ranf return float;

end R250;

