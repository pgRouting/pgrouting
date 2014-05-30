-- Barray.ads	Specification of a generic 1-D floating point array
-- 
-- $Header:   C:\usr\skip\ada\vcs\barray.asv   1.1   04 Sep 1994 02:17:12   skip_carter  $


package BARRAY is

subtype RealNumber is Float;
type BasicArray is array(INTEGER range<>) of RealNumber;
type BasicArrayHandle is access BasicArray;

-- append a new datum to the end of the array
procedure append(xh : in out BasicArrayHandle; nvalue : RealNumber);

-- nondestructively reallocate space
procedure resize(xh : in out BasicArrayHandle; newsize : in integer := -1);

-- nondestructively reallocate space, changing the first index
procedure resize(xh : in out BasicArrayHandle; newsize : in integer;
		 first : in integer);

-- output array with the specified number of columns per line
-- need to generalize this so that a file is possible (defaulting to stdout)
procedure put(x : in BasicArray; width : in integer := 1);

procedure put(x : in BasicArrayHandle; width : in integer := 1);

end BARRAY;


