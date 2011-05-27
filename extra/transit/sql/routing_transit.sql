CREATE TYPE gtfs_path_result AS (stop_id TEXT, route_id TEXT);
-----------------------------------------------------------------------
-- Core function for Single Mode Public Transit routing.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION sm_public_transit_route(gtfs_schema TEXT, source_stop_id TEXT, target_stop_id TEXT, max_changeovers INTEGER, transit_type INTEGER)
        RETURNS SETOF gtfs_path_result
        AS '$libdir/libtransit_routing'
        LANGUAGE 'C' IMMUTABLE;
