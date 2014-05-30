-- r250.adb	The R250 32 bit positive (e.q. 31 bit) random number
--		 generator specification.

--    (c) Copyright 1997 Everett F. Carter Jr.   Permission is granted
--    by the author to use this software for any application provided
--    this copyright notice is preserved.

-- $Author:   skip  $
-- $Workfile:   r250.adb  $
-- $Revision:   1.0  $
-- $Date:   24 Jul 1997 11:00:38  $

-- ==================================================================


with Bitops;	use Bitops;		-- get bitwise operators

package body R250 is

        type INT_16 is range -2**15 .. 2**15-1;
        type INT_32 is range -2**31 .. 2**31-1;

	MAX : constant := 2147483647;
                
        MS_BIT   : constant := 16#40000000#;
        ALL_BITS : constant := 16#7fffffff#;
        HALF_RANGE : constant := 16#20000000#;
	STEP     : constant := 7;
        
        buffer  : array(0..249) of INTEGER;
	index	: integer;

        LOCAL_SEED      : INTEGER; 	-- range 1 .. MODULUS - 1;

	function rand return INTEGER is-- only used for initialization
	   MULTIPLIER: constant := 16807;
           Q        : constant := 127773;
           R        : constant := 2836;
           T        : INTEGER;
         begin
           T := MULTIPLIER * (LOCAL_SEED mod Q) - R * (LOCAL_SEED / Q);
           If T > 0 Then
           	LOCAL_SEED := T;
           Else
           	LOCAL_SEED := T + MAX;
	   End if;
                           
           return LOCAL_SEED;

           end rand;
        
	procedure R250_init(seed : integer) is
           k	: integer;
           mask, msb : INTEGER;
           begin
           
             local_seed := seed;
             
	     index := 0;
             for i in 0..249 loop
                 buffer(i) := rand;
             end loop;
	     for i in 0..249 loop
                 if rand > HALF_RANGE then
                        buffer(i) := BIT_OR( buffer(i), MS_BIT);
		 end if;
             end loop;

	     msb  := MS_BIT;
             mask := ALL_BITS;

             for i in 0..30 loop
                 k := STEP * i + 3;
                 buffer(k) := BIT_AND( buffer(k), mask);
                 buffer(k) := BIT_OR( buffer(k),  msb);
                 msb := msb / 2;
                 mask := mask / 2;
            end loop;
	end R250_init;

        function rani return INTEGER is
           j	: integer;
           new_rand : INTEGER;
           begin
             if index >= 147 then
                   j := index - 147;
             else
                   j := index + 103;
	     end if;

             new_rand := BIT_XOR( buffer(index), buffer(j) );
             buffer(index) := new_rand;

             if index >= 249 then
                      index := 0;
             else
             	      index := index + 1;
             end if;

             return integer(new_rand);
        end rani;

        
	function ranf return float is
           j	: integer;
           new_rand : INTEGER;
           retval   : float;
           begin
             if index >= 147 then
                   j := index - 147;
             else
                   j := index + 103;
	     end if;

             new_rand := BIT_XOR( buffer(index), buffer(j) );
             buffer(index) := new_rand;

             if index >= 249 then
                      index := 0;
             else
             	      index := index + 1;
             end if;

	     retval := float( new_rand ) / float( ALL_BITS );

	     return retval;
              
           end ranf;

end R250;
