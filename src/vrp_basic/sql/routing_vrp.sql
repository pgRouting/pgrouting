-----------------------------------------------------------------------
-- Core function for vrp with sigle depot computation
-- See README for description
-----------------------------------------------------------------------
--
--

create or replace function pgr_vrpOneDepot(
	order_sql text,
	vehicle_sql text,
	cost_sql text,
	depot_id integer,
	 
	OUT oid integer, 
	OUT opos integer, 
	OUT vid integer, 
	OUT tarrival integer, 
	OUT tdepart integer)
returns setof record as
'$libdir/librouting_vrp', 'vrp'
LANGUAGE c IMMUTABLE STRICT;


