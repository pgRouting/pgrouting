CREATE DOMAIN wgs84_lat AS double precision CHECK(VALUE >= -90 AND VALUE <= 90);

CREATE DOMAIN wgs84_lon AS double precision CHECK(VALUE >= -180 AND VALUE <= 180);

CREATE DOMAIN gtfstime AS text CHECK(VALUE ~ '^[0-9]?[0-9]:[0-5][0-9]:[0-5][0-9]$');

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

-- Adding column agency_timezone to trips to avoid joins
IF NOT column_exists(gtfs_schema, 'trips', 'agency_timezone') THEN
  EXECUTE 'ALTER TABLE ' || gtfs_schema || '.trips
    ADD COLUMN agency_timezone TEXT NULL';
END IF;
EXECUTE '
UPDATE ' || gtfs_schema || '.trips t
SET agency_timezone = (
  SELECT a.agency_timezone
  FROM ' ||
    gtfs_schema || '.agency a, ' ||
    gtfs_schema || '.routes r
  WHERE r.route_id = t.route_id
    AND r.agency_id = a.agency_id);
ALTER TABLE ' || gtfs_schema || '.trips
ALTER COLUMN agency_timezone SET NOT NULL';

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
      new_diff := extract(epoch from (dest_time::interval - src_time::interval));
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
  EXECUTE 'SELECT count(*) = 0
    FROM ' ||
      gtfs_schema || '.frequencies f
    WHERE f.trip_id = ' || quote_literal(t_id)
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
  EXECUTE 'SELECT f.headway_secs * ''1 second''::interval
    FROM ' ||
      gtfs_schema || '.frequencies f
    WHERE f.trip_id = ' || quote_literal(t_id) || '
      AND ' || quote_literal(src_dep_time) || '::interval BETWEEN f.start_time::interval AND f.end_time::interval
    ORDER BY f.headway_secs
    LIMIT 1'
    INTO waiting_time;
  -- RAISE NOTICE 'waiting_time = %', waiting_time;
  RETURN waiting_time;
END
$$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;

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
    arrival_timestamp := timestamp with time zone 'epoch' +
                         arrival_time * '1 second'::interval;
    FOR t_id, t_date, src_dep_time, src_seq in EXECUTE '
      SELECT st.trip_id, date_trunc(''day'', (timestamp with time zone ' || quote_literal(arrival_timestamp + max_wait_time) || '
      - st.departure_time::interval) at time zone t.agency_timezone), st.departure_time, st.stop_sequence
      FROM ' ||
        gtfs_schema || '.stop_times st, ' ||
        gtfs_schema || '.trips t, ' ||
        gtfs_schema || '.calendar c
      WHERE st.trip_id = t.trip_id
        AND c.service_id = t.service_id
        AND (c.bitmap & (1<<extract(dow from (timestamp with time zone ' ||
          quote_literal(arrival_timestamp) || '
          - st.departure_time::interval) at time zone t.agency_timezone)::integer)::bit(7)) <> 0::bit(7)
        AND st.stop_id_int4 = ' || source_stop_id
    LOOP
      t_wait := find_trip_waiting_time(gtfs_schema, t_id, arrival_timestamp, t_date, src_dep_time);
      IF t_wait IS NOT NULL AND t_wait < max_wait_time THEN
        -- RAISE NOTICE 't_wait = %', t_wait;
        FOR link in EXECUTE '
          SELECT trip_id, stop_id_int4, ' || extract(epoch from t_wait)::integer || ',
          extract(epoch from arrival_time::interval - ' || quote_literal(src_dep_time) || '::interval)::integer
          FROM ' || gtfs_schema || '.stop_times
          WHERE trip_id = ' || quote_literal(t_id) || '
            AND stop_sequence > ' || src_seq
        LOOP
          RETURN NEXT link;
        END LOOP;
      END IF;
    END LOOP;
    END
    $$
    LANGUAGE 'plpgsql' IMMUTABLE STRICT;

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
