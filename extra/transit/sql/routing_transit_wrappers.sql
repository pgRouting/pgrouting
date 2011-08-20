CREATE TYPE gtfs_path_result2 AS (stop_id TEXT, trip_id TEXT, waiting_time INTERVAL, travel_time INTERVAL);

SELECT prepare_scheduled('gtfs'); -- Wrapper functions are applicable only for 'gtfs' schema

CREATE OR REPLACE FUNCTION gtfs_route(src_id TEXT, dest_id TEXT, query_time TIMESTAMP WITH TIME ZONE)
       RETURNS SETOF GTFS_PATH_RESULT2 AS
$$
SELECT s.stop_id, trip_id, waiting_time * '1 second'::INTERVAL, travel_time * '1 second'::INTERVAL
FROM scheduled_route('gtfs',
  (SELECT stop_id_int4 FROM gtfs.stops WHERE stop_id = $1),
    (SELECT stop_id_int4 FROM gtfs.stops WHERE stop_id = $2),
      extract(epoch from $3)::INTEGER
      ) sr,
      gtfs.stops s
      WHERE sr.stop_id = s.stop_id_int4;
$$
LANGUAGE 'sql' IMMUTABLE STRICT;

