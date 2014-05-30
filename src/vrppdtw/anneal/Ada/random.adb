-- random.adb		Body of Random number package

--    (c) Copyright 1997 Everett F. Carter Jr.   Permission is granted
--    by the author to use this software for any application provided
--    this copyright notice is preserved.

-- $Author:$
-- $Workfile:$
-- $Revision:$
-- $Date:$

-- ==================================================================

with CALENDAR, R250, Ada.Numerics.generic_elementary_functions;
use CALENDAR;

package body RANDOM is

        type INT_32 is range -2**31 .. 2**31-1;

        DEFAULT_SEED : INTEGER range 0 .. 32767;

        y2           : float;
        use_last     : Boolean := False;
        
    package float_funcs is new Ada.Numerics.generic_elementary_functions( float );
    use float_funcs;

        procedure SEED(sd : integer) is
           begin
             R250.r250_init(sd);
           end;

	function UNIT_RANDOM return FLOAT is
           begin
             return R250.ranf;
           end UNIT_RANDOM;
           
        function RANDOM_INT(a : INTEGER; b: INTEGER) return INTEGER is
           retval : integer;
           begin
	     retval := a + INTEGER( R250.rani mod INTEGER(b - a + 1) );
             return retval;             
           end RANDOM_INT;

	function UNIFORM(a : float; b : float) return FLOAT is
           retval : float;
           begin
             retval := a + (b - a) * R250.ranf;
             return retval;
           end UNIFORM;

-- polar form of the Box-Muller transformation
function Normal(m : in float; s : in float) return float is
    x1, x2, w, y1 : float;
    
    begin

      If Use_Last then
             y1 := y2;
             Use_Last := False;
      else
	     loop
               x1 := Uniform(-1.0, 1.0);
               x2 := Uniform(-1.0, 1.0);

               w  := x1 * x1 + x2 * x2;

               exit when w < 1.0;
	     end loop;

             w := sqrt( ( -2.0 * log( w ) ) / w );

             y1 := x1 * w;
             y2 := x2 * w;
             use_last := True;
	end if;
                     
      return m + y1 * s;
      
    end Normal;

begin
	DEFAULT_SEED := INTEGER(INT_32(SECONDS( CLOCK ) ) mod 32768);

	R250.R250_init( DEFAULT_SEED );
        
end RANDOM;
