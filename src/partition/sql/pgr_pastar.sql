-- put your sql link to the c code here.
CREATE OR REPLACE FUNCTION pgr_pastar(tab text, snid integer, spid integer, tnide integer, tpid integer, has_rcost boolean DEFAULT false)
RETURNS SETOF pgr_costResult
AS '$libdir/librouting_pastar', 'pastar'
LANGUAGE c IMMUTABLE STRICT;

