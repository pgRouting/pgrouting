CREATE DOMAIN wgs84_lat AS double precision CHECK(VALUE >= -180 AND VALUE <= 180);

CREATE DOMAIN wgs84_lon AS double precision CHECK(VALUE >= -90 AND VALUE <= 90);

CREATE DOMAIN gtfstime AS text CHECK(VALUE ~ '^[0-9]?[0-9]:[0-5][0-9]:[0-5][0-9]$');

CREATE FUNCTION gtfstime_to_secs(arg0 gtfstime) RETURNS INTEGER AS
$$
DECLARE
h INTEGER;
m INTEGER;
s INTEGER;
BEGIN
h := split_part(arg0, ':', 1)::INTEGER;
m := split_part(arg0, ':', 2)::INTEGER;
s := split_part(arg0, ':', 3)::INTEGER;
RETURN 3600 * h + 60 * m + s;
END
$$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;

CREATE TYPE gtfs_path_result AS (stop_id TEXT, route_id TEXT);

CREATE TYPE nonsc_path_result AS (changeover_id TEXT, cost DOUBLE PRECISION);

-----------------------------------------------------------------------
-- Core function for Single Mode Public Transit routing.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION sm_public_transit_route(gtfs_schema TEXT, source_stop_id TEXT, target_stop_id TEXT, max_changeovers INTEGER, transit_type INTEGER)
        RETURNS SETOF gtfs_path_result
        AS '$libdir/libtransit_routing'
        LANGUAGE 'C' IMMUTABLE;
