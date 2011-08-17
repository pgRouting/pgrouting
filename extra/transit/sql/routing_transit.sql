CREATE DOMAIN wgs84_lat AS double precision CHECK(VALUE >= -90 AND VALUE <= 90);

CREATE DOMAIN wgs84_lon AS double precision CHECK(VALUE >= -180 AND VALUE <= 180);

CREATE DOMAIN gtfstime AS text CHECK(VALUE ~ '^[0-9]?[0-9]:[0-5][0-9]:[0-5][0-9]$');

-- FIXME: Use builtin type Interval to manipulate gtfstime
CREATE OR REPLACE FUNCTION gtfstime_to_secs(hms gtfstime) RETURNS INTEGER AS
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

-- FIXME: Use builtin type Interval to manipulate gtfstime
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

CREATE OR REPLACE FUNCTION get_midnight_secs(magic_time TIMESTAMP WITH TIME ZONE)
RETURNS INTEGER
AS
$$
DECLARE
h INTEGER;
m INTEGER;
s INTEGER;
BEGIN
h = extract(hour from magic_time);
m = extract(minute from magic_time);
s = extract(second from magic_time)::integer;
RETURN h * 3600 + m * 60 + s;
END
$$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;

-- check if a column exists
CREATE OR REPLACE FUNCTION column_exists(schema_name text, table_name text, column_name text)
RETURNS BOOLEAN AS
$$
DECLARE
q text;
onerow record;
BEGIN
q = 'SELECT attname FROM pg_attribute
    WHERE attrelid = (
      SELECT oid FROM pg_class
      WHERE relnamespace = (
        SELECT oid FROM pg_namespace
        WHERE nspname = ' || quote_literal(schema_name) || '
      ) AND relname = ' || quote_literal(table_name) ||'
    )AND attname = ' || quote_literal(column_name);
FOR onerow IN EXECUTE q
LOOP
  RETURN true;
END LOOP;
RETURN false;
END
$$
LANGUAGE 'plpgsql' VOLATILE;

CREATE OR REPLACE FUNCTION prepare_scheduled(gtfs_schema TEXT)
RETURNS VOID
AS
$$
DECLARE
t_id TEXT;
src_id INTEGER;
src_time gtfstime;
dest_id INTEGER;
dest_time gtfstime;
stg_id INTEGER;
old_diff INTEGER;
new_diff INTEGER;
BEGIN
-- Generating unique integer id for stops
IF NOT column_exists(gtfs_schema, 'stops', 'stop_id_int4') THEN
  EXECUTE 'ALTER TABLE ' || gtfs_schema || '.stops
    ADD COLUMN stop_id_int4 SERIAL UNIQUE NOT NULL';
END IF;
-- Adding column stop_id_int4 to stop_times table to avoid joins
IF NOT column_exists(gtfs_schema, 'stop_times', 'stop_id_int4') THEN
  EXECUTE 'ALTER TABLE ' || gtfs_schema || '.stop_times
    ADD COLUMN stop_id_int4 integer NULL REFERENCES ' || gtfs_schema || '.stops(stop_id_int4)';
END IF;
EXECUTE '
UPDATE ' || gtfs_schema || '.stop_times st
SET stop_id_int4 = (SELECT s.stop_id_int4 FROM ' ||
    gtfs_schema || '.stops s
    WHERE s.stop_id = st.stop_id);
ALTER TABLE ' || gtfs_schema || '.stop_times
ALTER COLUMN stop_id_int4 SET NOT NULL';

-- Adding calculated field 'bitmap' for calendar table
IF NOT column_exists(gtfs_schema, 'calendar', 'bitmap') THEN
  EXECUTE 'ALTER TABLE ' || gtfs_schema || '.calendar
      ADD COLUMN bitmap bit(7) NULL';
  --TODO: Create a trigger to update calendar bitmap upon insert/update
END IF;
EXECUTE '
UPDATE ' || gtfs_schema || '.calendar
SET bitmap = (((((((saturday << 1) + friday << 1) + thursday << 1) +
  wednesday << 1) + tuesday << 1) + monday <<1) + sunday)::bit(7);
ALTER TABLE ' || gtfs_schema || '.calendar
ALTER COLUMN bitmap SET NOT NULL';

-- Pre-computing shortest time between stops(directly reachable) for astar heuristic
EXECUTE 'DROP TABLE IF EXISTS ' || gtfs_schema || '.shortest_time_graph CASCADE';
EXECUTE 'CREATE TABLE ' || gtfs_schema || '.shortest_time_graph(
    id              SERIAL PRIMARY KEY,
    source          INTEGER REFERENCES ' || gtfs_schema || '.stops(stop_id_int4),
    target          INTEGER REFERENCES ' || gtfs_schema || '.stops(stop_id_int4),
    travel_time     INTEGER
)';
FOR t_id IN
EXECUTE 'SELECT trip_id FROM ' || gtfs_schema || '.trips'
LOOP
  FOR src_id, src_time IN
  EXECUTE 'SELECT stop_id_int4, departure_time FROM ' ||
           gtfs_schema || '.stop_times
           WHERE trip_id = ' || quote_literal(t_id)
  LOOP
    FOR dest_id, dest_time IN
    EXECUTE 'SELECT stop_id_int4, arrival_time FROM ' ||
           gtfs_schema || '.stop_times
           WHERE trip_id = ' || quote_literal(t_id) || ' AND
              arrival_time > ' || quote_literal(src_time)
    LOOP
      new_diff := gtfstime_to_secs(dest_time) - gtfstime_to_secs(src_time);
      EXECUTE 'SELECT id, travel_time from ' || gtfs_schema || '.shortest_time_graph
        WHERE source = ' || src_id || ' AND target = ' || dest_id
        INTO stg_id, old_diff;
      IF stg_id IS NULL THEN
        EXECUTE 'INSERT INTO ' || gtfs_schema || '.shortest_time_graph(
          source, target, travel_time) VALUES
          (' ||
            src_id || ',' ||
            dest_id || ',' ||
            new_diff ||
          ')';
      ELSIF old_diff > new_diff THEN
        EXECUTE 'UPDATE ' || gtfs_schema || '.shortest_time_graph
          SET travel_time = ' || new_diff ||
          ' WHERE id = ' || stg_id;
      END IF;
    END LOOP;
  END LOOP;
END LOOP;

-- Finding transitive closure of shortest time between stops(indirectly reachable also)
EXECUTE 'DROP TABLE IF EXISTS ' || gtfs_schema || '.shortest_time_closure';
EXECUTE 'CREATE TABLE ' || gtfs_schema || '.shortest_time_closure(
    id              SERIAL PRIMARY KEY,
    source          INTEGER REFERENCES ' || gtfs_schema || '.stops(stop_id_int4),
    target          INTEGER REFERENCES ' || gtfs_schema || '.stops(stop_id_int4),
    travel_time     INTEGER
)';
EXECUTE 'INSERT INTO ' || gtfs_schema || '.shortest_time_closure(source,target,travel_time) SELECT source_id, target_id, cost::integer from apsp_johnson(' || quote_literal('select source, target, travel_time::float as cost from ' || gtfs_schema || '.shortest_time_graph') || ') where source_id <> target_id';

END
$$
LANGUAGE 'plpgsql' STRICT;

CREATE OR REPLACE FUNCTION check_service(gtfs_schema TEXT, service_id TEXT, service_timestamp TIMESTAMP, max_wait_time INTERVAL)
RETURNS BOOLEAN
AS
$$
DECLARE
  service_available BOOLEAN;
BEGIN
EXECUTE 'SELECT bitmap & (1<<extract(dow from timestamp ' ||
    quote_literal(service_timestamp) || ')::integer)::bit(7) <> 0::bit(7)
    FROM ' || gtfs_schema || '.calendar
    WHERE service_id = ' || quote_literal(service_id)
  INTO service_available;
RETURN service_timestamp::time < max_wait_time AND service_available;
END
$$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;

DROP TYPE IF EXISTS next_link CASCADE;
CREATE TYPE next_link AS (trip_id TEXT, destination_stop_id INTEGER, waiting_cost INTEGER, travel_cost INTEGER);

CREATE OR REPLACE FUNCTION fetch_next_links(
        gtfs_schema TEXT,
        source_stop_id INTEGER,
        arrival_time INTEGER,
        max_wait_time INTERVAL
    )
    RETURNS SETOF next_link
    AS
    $$
    DECLARE
    r next_link;
    BEGIN
    for r in EXECUTE '
      SELECT st1.trip_id, st2.stop_id_int4,' || get_midnight_secs(timestamp 'epoch' + max_wait_time) || ' - get_midnight_secs((' || quote_literal(
          timestamp with time zone 'epoch' +
          arrival_time * '1 second'::interval +
          max_wait_time) ||
          '::timestamp with time zone - st1.arrival_time::interval) at time zone a.agency_timezone), gtfstime_to_secs(st2.arrival_time) - gtfstime_to_secs(st1.arrival_time)
      FROM ' ||
        gtfs_schema || '.stop_times st1, ' ||
        gtfs_schema || '.stop_times st2, ' ||
        gtfs_schema || '.trips t, ' ||
        gtfs_schema || '.routes r, ' ||
        gtfs_schema || '.agency a
      WHERE st1.stop_id_int4 = ' || source_stop_id ||
      ' AND st2.trip_id = st1.trip_id
        AND st2.stop_sequence > st1.stop_sequence
        AND t.trip_id = st1.trip_id
        AND t.route_id = r.route_id
        AND r.agency_id = a.agency_id
        AND check_service(' || quote_literal(gtfs_schema) ||
          ', t.service_id, (' || quote_literal(
          timestamp with time zone 'epoch' +
          arrival_time * '1 second'::interval +
          max_wait_time) ||
          '::timestamp with time zone - st1.arrival_time::interval) at time zone a.agency_timezone, ' ||
          quote_literal(max_wait_time) || ')'
    LOOP
      RETURN next r;
    END LOOP;
    END
    $$
    LANGUAGE 'plpgsql' IMMUTABLE STRICT;

CREATE TYPE gtfs_path_result AS (stop_id integer, trip_id TEXT, waiting_time INTEGER, travel_time INTEGER);

CREATE TYPE nonsc_path_result AS (changeover_id TEXT, cost DOUBLE PRECISION);

-----------------------------------------------------------------------
-- Core function for Single Mode Public Transit routing.
-----------------------------------------------------------------------

CREATE OR REPLACE FUNCTION scheduled_route(
        gtfs_schema TEXT,
        source_stop_id INTEGER,
        target_stop_id INTEGER,
        query_time INTEGER
    )
    RETURNS SETOF gtfs_path_result
    AS '$libdir/libtransit_routing'
    LANGUAGE 'C';
