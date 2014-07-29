-----------------------------------------------------------------------
-- Core function for vrp with sigle depot computation
-- See README for description
-----------------------------------------------------------------------
--
--
create or replace function pgr_gsoc_vrppdtw(
                sql text,
                id integer,
                x integer,
                y integer,
                demand integer,
                etime integer,
                ltime integer,
                stime integer,
                pindex integer,
                dindex integer)
returns setof record as
'$libdir/librouting_vrpdtw', 'vrpdtw'
LANGUAGE c VOLATILE STRICT;


