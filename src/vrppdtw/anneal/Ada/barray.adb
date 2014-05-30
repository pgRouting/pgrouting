-- Barray.adb	Implementation of a generic 1-D floating point array
-- 
-- $Header:   C:\usr\skip\ada\vcs\barray.abv   1.1   04 Sep 1994 02:17:34   skip_carter  $

with text_io; use text_io;

package body BARRAY is

	package io_Real is new float_io(RealNumber);
        use io_Real;

-- nondestructively reallocate space
procedure resize(xh : in out BasicArrayHandle; newsize : in integer := -1) is

	newlast,keep	: integer;
        xnew		: BasicArrayHandle;
        
begin
	if newsize < 0 then
        	newlast := xh'last + 1;	     -- default is to allocate 1 more
        else
        	newlast := xh'first + newsize - 1;
        end if;

        xnew := new BasicArray(xh'first..newlast);

	keep := xh'last;
        if keep > newlast then
        	keep := newlast;	-- keep the smallest index
	end if;
        
        for i in xh'first..keep loop
        	xnew(i) := xh(i);	-- copy old data over
        end loop;

	-- free( xh );		-- need to instantiate unchecked_deallocation
				-- in order to use free();
                                
        xh := xnew;
        
end resize;

-- resize with new first index
procedure resize(xh : in out BasicArrayHandle; newsize : in integer; first : in integer) is

	newlast,keep,k	: integer;
        xnew		: BasicArrayHandle;
        
begin
        newlast := first + newsize - 1;
	
        xnew := new BasicArray(first..newlast);

	keep := xh'length;
        if keep > newsize then
        	keep := newsize;	-- keep the smallest index
	end if;

        k := xnew'first;
        keep := xh'first+keep-1;
        for i in xh'first..keep loop
        	xnew(k) := xh(i);	-- copy old data over
                k := k + 1;
        end loop;

	-- free( xh );		-- need to instantiate unchecked_deallocation
				-- in order to use free();
                                
        xh := xnew;
        
end resize;

procedure append( xh : in out BasicArrayHandle; nvalue : RealNumber) is
   loc : integer;
begin
	resize( xh );
	loc := xh'last;
        xh( loc ) := nvalue;
end append;

procedure put(x : in BasicArray; width : in integer := 1) is
    counter : integer := 1;
begin
	for i in x'range loop
		put( x(i) );
                if counter MOD width = 0 then
                		new_line;
                else
                	put( "   " );
                end if;
                counter := counter + 1;
        end loop;
        
end put;

procedure put(x : in BasicArrayHandle; width : in integer := 1) is
    counter : integer := 1;
begin
	for i in x'range loop
		put( x(i) );
                if counter MOD width = 0 then
                		new_line;
                else
                	put( "   " );
                end if;
                counter := counter + 1;
        end loop;
        
end put;

end BARRAY;

