-- bitops.ads          Specification of C bitwise operations

--    (c) Copyright 1997 Everett F. Carter Jr.   Permission is granted
--    by the author to use this software for any application provided
--    this copyright notice is preserved.

-- $Author:   skip  $
-- $Workfile:   bitops.ads  $
-- $Revision:   1.0  $
-- $Date:   20 Jul 1997 19:16:56  $

-- ==================================================================


package BITOPS is

	function BIT_OR(a, b : in INTEGER) return INTEGER;

        function BIT_AND(a, b : in INTEGER) return INTEGER;

	function BIT_XOR(a, b : in INTEGER) return INTEGER;

	function BIT_NOT(a : in INTEGER) return INTEGER;

private

    pragma Import(C, BIT_OR, "or_bits" );

    pragma Import(C, BIT_AND, "and_bits" );

    pragma Import(C, BIT_XOR, "xor_bits" );

    pragma Import(C, BIT_NOT, "not_bits" );

end BITOPS;

