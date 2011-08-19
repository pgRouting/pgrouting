CREATE OR REPLACE FUNCTION update_stop_time_graph(gtfs_schema TEXT)
RETURNS VOID AS
$$
DECLARE
t_id        TEXT;
headway     INTEGER;
src_id      INTEGER;
src_time    INTERVAL;
dest_id     INTEGER;
dest_time   INTERVAL;
BEGIN
DROP TABLE IF EXISTS stop_id_map;
DROP TABLE IF EXISTS stop_time_graph;

EXECUTE 'SET LOCAL search_path to ' || quote_ident(gtfs_schema);

-- Creating Mapping table between integer and text stop_ids
-- FIXME: Replace with an additional column in stops table
-- like in scheduled_route.
CREATE TABLE stop_id_map(
  stop_id_int4    SERIAL PRIMARY KEY,
  stop_id_text    TEXT UNIQUE NOT NULL REFERENCES stops(stop_id)
);

INSERT INTO stop_id_map(stop_id_text)
  SELECT stop_id FROM stops;

CREATE TABLE stop_time_graph(
  id              SERIAL PRIMARY KEY,
  source          INTEGER REFERENCES stop_id_map(stop_id_int4),
  target          INTEGER REFERENCES stop_id_map(stop_id_int4),
  waiting_time    INTEGER,
  travel_time     INTEGER
);

FOR t_id, headway IN
SELECT trip_id, headway_secs FROM frequencies
LOOP
  FOR src_id, src_time IN
  SELECT stop_id_int4, departure_time
    FROM stop_times, stop_id_map
    WHERE stop_id = stop_id_text AND trip_id = t_id
  LOOP
    FOR dest_id, dest_time IN
    SELECT stop_id_int4, arrival_time
    FROM stop_times, stop_id_map
    WHERE stop_id = stop_id_text
      AND trip_id = t_id
      AND arrival_time > src_time
    LOOP
      INSERT INTO stop_time_graph(
        source,
        target,
        waiting_time,
        travel_time)
        VALUES (
          src_id,
          dest_id,
          headway::INTEGER / 2,
          extract(epoch from (dest_time - src_time))::INTEGER);
        END LOOP;
    END LOOP;
END LOOP;
END
$$ LANGUAGE 'plpgsql' STRICT;

CREATE TYPE nonsc_path_result AS (changeover_id TEXT, cost DOUBLE PRECISION);

CREATE OR REPLACE FUNCTION non_scheduled_route(gtfs_schema TEXT, source TEXT, target TEXT)
RETURNS SETOF nonsc_path_result
AS
$$
BEGIN
EXECUTE 'SET LOCAL search_path to public,' || quote_ident(gtfs_schema);
RETURN QUERY SELECT stop_id_text as changeover_id, cost
  FROM
    shortest_path(
      'SELECT
        id,
        source::int4,
        target::int4,
        (waiting_time + travel_time)::float8 AS cost
      FROM ' || gtfs_schema || '.stop_time_graph',
      (select stop_id_int4 from stop_id_map where stop_id_text = source),
      (select stop_id_int4 from stop_id_map where stop_id_text = target),
      true,
      false
    ),
    stop_id_map
  WHERE vertex_id = stop_id_int4;
END
$$ LANGUAGE 'plpgsql' STRICT;
