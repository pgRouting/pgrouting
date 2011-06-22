CREATE DOMAIN wgs84_lat AS double precision CHECK(VALUE >= -180 AND VALUE <= 180);

CREATE DOMAIN wgs84_lon AS double precision CHECK(VALUE >= -90 AND VALUE <= 90);

CREATE DOMAIN gtfstime AS text CHECK(VALUE ~ '^[0-9]?[0-9]:[0-5][0-9]:[0-5][0-9]$');

CREATE FUNCTION gtfstime_to_secs(hms gtfstime) RETURNS INTEGER AS
$$
DECLARE
h INTEGER;
m INTEGER;
s INTEGER;
BEGIN
h = split_part(hms, ':', 1)::INTEGER;
m = split_part(hms, ':', 2)::INTEGER;
s = split_part(hms, ':', 3)::INTEGER;
RETURN 3600 * h + 60 * m + s;
END
$$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;

CREATE FUNCTION secs_to_gtfstime(secs integer) RETURNS gtfstime AS
$$
DECLARE
h INTEGER;
m INTEGER;
s INTEGER;
hms TEXT;
BEGIN
h = secs / 3600;
m = (secs - h * 3600) / 60;
s = secs - h * 3600 - m * 60;
hms = '';
if h < 10 then
    hms = hms || '0';
end if;
hms = hms || h || ':';
if m < 10 then
    hms = hms ||'0';
end if;
hms = hms || m || ':';
if s < 10 then
    hms = hms || '0';
end if;
hms = hms || s;
RETURN hms;
END
$$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;

CREATE TYPE gtfs_path_result AS (stop_id TEXT, trip_id TEXT);

CREATE TYPE nonsc_path_result AS (changeover_id TEXT, cost DOUBLE PRECISION);

-----------------------------------------------------------------------
-- Core function for Single Mode Public Transit routing.
-----------------------------------------------------------------------

CREATE OR REPLACE FUNCTION scheduled_route(
        gtfs_schema TEXT,
        source_stop_id TEXT,
        target_stop_id TEXT,
        query_time TIMESTAMP WITH TIME ZONE
    )
    RETURNS SETOF gtfs_path_result
    AS '$libdir/libtransit_routing'
    LANGUAGE 'C' IMMUTABLE;
