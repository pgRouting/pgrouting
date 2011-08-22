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
src_time INTERVAL;
dest_id INTEGER;
dest_time INTERVAL;
stg_id INTEGER;
old_diff INTEGER;
new_diff INTEGER;
BEGIN
EXECUTE 'SET LOCAL search_path to ' || quote_ident(gtfs_schema) || ', public';
-- Generating unique integer id for stops
IF NOT column_exists(gtfs_schema, 'stops', 'stop_id_int4') THEN
  ALTER TABLE stops
    ADD COLUMN stop_id_int4 SERIAL UNIQUE NOT NULL;
END IF;
IF NOT column_exists(gtfs_schema, 'stops', 'the_geom') THEN
  PERFORM AddGeometryColumn(gtfs_schema, 'stops', 'the_geom', 4326, 'POINT', 2);
END IF;
UPDATE stops SET the_geom = ST_GeomFromText('POINT(' || stop_lon || ' ' || stop_lat || ')', 4326);
-- Adding column stop_id_int4 to stop_times table to avoid joins
IF NOT column_exists(gtfs_schema, 'stop_times', 'stop_id_int4') THEN
  ALTER TABLE stop_times
    ADD COLUMN stop_id_int4 integer NULL REFERENCES stops(stop_id_int4);
END IF;
UPDATE stop_times st
  SET stop_id_int4 = (SELECT s.stop_id_int4 FROM stops s
    WHERE s.stop_id = st.stop_id);
ALTER TABLE stop_times
  ALTER COLUMN stop_id_int4 SET NOT NULL;

-- Adding column agency_timezone to trips to avoid joins
IF NOT column_exists(gtfs_schema, 'trips', 'agency_timezone') THEN
  ALTER TABLE trips
    ADD COLUMN agency_timezone TEXT NULL;
END IF;
UPDATE trips t
  SET agency_timezone = (SELECT a.agency_timezone
    FROM agency a, routes r
    WHERE r.route_id = t.route_id
      AND r.agency_id = a.agency_id);
ALTER TABLE trips
  ALTER COLUMN agency_timezone SET NOT NULL;

-- Adding calculated field 'bitmap' for calendar table
IF NOT column_exists(gtfs_schema, 'calendar', 'bitmap') THEN
  ALTER TABLE calendar
    ADD COLUMN bitmap bit(7) NULL;
  --TODO: Create a trigger to update calendar bitmap upon insert/update
END IF;
UPDATE calendar
  SET bitmap = (((((((saturday::integer << 1) + friday::integer << 1) +
    thursday::integer << 1) + wednesday::integer << 1) +
    tuesday::integer << 1) + monday::integer <<1) +
    sunday::integer)::bit(7);
ALTER TABLE calendar
  ALTER COLUMN bitmap SET NOT NULL;

-- Pre-computing shortest time between stops(directly reachable) for astar heuristic
DROP TABLE IF EXISTS shortest_time_graph CASCADE;
CREATE TABLE shortest_time_graph(
    id              SERIAL PRIMARY KEY,
    source          INTEGER REFERENCES stops(stop_id_int4),
    target          INTEGER REFERENCES stops(stop_id_int4),
    travel_time     INTEGER
);

INSERT INTO shortest_time_graph(source,target,travel_time)
  SELECT src.stop_id_int4, dest.stop_id_int4,
    extract(epoch from min(dest.arrival_time - src.departure_time))
  FROM stop_times src, stop_times dest
  WHERE src.trip_id = dest.trip_id
    AND src.stop_sequence < dest.stop_sequence
  GROUP BY src.stop_id_int4, dest.stop_id_int4;

-- Finding transitive closure of shortest time between stops(indirectly reachable also)
DROP TABLE IF EXISTS shortest_time_closure;
CREATE TABLE shortest_time_closure(
    id              SERIAL PRIMARY KEY,
    source          INTEGER REFERENCES stops(stop_id_int4),
    target          INTEGER REFERENCES stops(stop_id_int4),
    travel_time     INTEGER
);
INSERT INTO shortest_time_closure(source,target,travel_time)
  SELECT source_id, target_id, cost::integer from apsp_johnson(
    'select source, target, travel_time::float as cost from ' || gtfs_schema || '.shortest_time_graph')
    WHERE source_id <> target_id;
END
$$
LANGUAGE 'plpgsql' STRICT;

DROP TYPE IF EXISTS next_link CASCADE;
CREATE TYPE next_link AS (trip_id TEXT, destination_stop_id INTEGER, waiting_cost INTEGER, travel_cost INTEGER);

CREATE OR REPLACE FUNCTION find_trip_waiting_time(
  gtfs_schema TEXT,
  t_id TEXT,
  arrival_time TIMESTAMP WITH TIME ZONE,
  trip_date TIMESTAMP WITH TIME ZONE,
  src_dep_time INTERVAL)
RETURNS INTERVAL
AS
$$
DECLARE
  is_scheduled BOOLEAN;
  waiting_time INTERVAL;
BEGIN
  EXECUTE 'SET LOCAL search_path to ' || quote_ident(gtfs_schema) || ', public';
  SELECT count(*) = 0
    FROM frequencies f
    WHERE f.trip_id = t_id
    INTO is_scheduled;
  IF is_scheduled THEN
    -- RAISE NOTICE 'trip % is scheduled.', t_id;
    waiting_time := trip_date + src_dep_time - arrival_time;
    -- RAISE NOTICE 'trip_date = %, src_dep_time = %, arrival_time = %', trip_date, src_dep_time, arrival_time;
    IF waiting_time < '0'::interval THEN
      RETURN NULL;
    ELSE
      RETURN waiting_time;
    END IF;
  END IF;
  -- RAISE NOTICE 'trip % is frequency governed.', t_id;
  SELECT f.headway_secs * '1 second'::interval
    FROM frequencies f
    WHERE f.trip_id = t_id
      AND src_dep_time BETWEEN f.start_time AND f.end_time
    ORDER BY f.headway_secs
    LIMIT 1
    INTO waiting_time;
  -- RAISE NOTICE 'waiting_time = %', waiting_time;
  RETURN waiting_time;
END
$$
LANGUAGE 'plpgsql' STRICT;

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
    link next_link;
    t_id TEXT;
    t_wait INTERVAL;
    arrival_timestamp TIMESTAMP WITH TIME ZONE;
    t_date TIMESTAMP WITH TIME ZONE;
    src_dep_time INTERVAL;
    src_seq INTEGER;
    BEGIN
    EXECUTE 'SET LOCAL search_path to ' || quote_ident(gtfs_schema) || ',public';
    arrival_timestamp := timestamp with time zone 'epoch' +
                         arrival_time * '1 second'::interval;
    FOR t_id, t_date, src_dep_time, src_seq IN
      SELECT st.trip_id, date_trunc('day', (arrival_timestamp + max_wait_time - st.departure_time) at time zone t.agency_timezone), st.departure_time, st.stop_sequence
        FROM stop_times st, trips t, calendar c
        WHERE st.trip_id = t.trip_id
          AND c.service_id = t.service_id
          AND (c.bitmap & (1<<extract(dow from (arrival_timestamp + max_wait_time - st.departure_time) at time zone t.agency_timezone)::integer)::bit(7)) <> 0::bit(7)
          AND st.stop_id_int4 = source_stop_id
    LOOP
      t_wait := find_trip_waiting_time(gtfs_schema, t_id, arrival_timestamp, t_date, src_dep_time);
      IF t_wait IS NOT NULL AND t_wait < max_wait_time THEN
        -- RAISE NOTICE 't_wait = %', t_wait;
        FOR link IN
          SELECT trip_id, stop_id_int4, extract(epoch from t_wait)::INTEGER,
            extract(epoch from st.arrival_time - src_dep_time)::INTEGER
            FROM stop_times st
            WHERE st.trip_id = t_id
              AND st.stop_sequence > src_seq
        LOOP
          RETURN NEXT link;
        END LOOP;
      END IF;
    END LOOP;
    END
    $$
    LANGUAGE 'plpgsql' STRICT;

CREATE TYPE gtfs_path_result AS (stop_id integer, trip_id TEXT, waiting_time INTEGER, travel_time INTEGER);

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
