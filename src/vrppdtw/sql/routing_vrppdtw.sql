-----------------------------------------------------------------------
-- Core function for vrp with sigle depot computation
-- See README for description
-----------------------------------------------------------------------
--
--
create or replace function pgr_gsoc_vrppdtw(
                sql text,
                vehicle_num integer,
                capacity integer
                )
returns setof record as
'$libdir/librouting_vrppdtw', 'vrppdtw'
LANGUAGE c VOLATILE STRICT;


