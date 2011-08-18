CREATE OR REPLACE FUNCTION update_stop_time_graph(gtfs_schema TEXT)
RETURNS VOID AS
$$
DECLARE
t_id        TEXT;
headway     INTEGER;
src_id      TEXT;
src_time    GTFSTime;
dest_id     TEXT;
dest_time   GTFSTime;
BEGIN
DROP TABLE IF EXISTS stop_id_map;
DROP TABLE IF EXISTS stop_time_graph;

-- Creating Mapping table between integer and text stop_ids

EXECUTE 'CREATE TABLE ' || gtfs_schema || '.stop_id_map(
    stop_id_int4    SERIAL PRIMARY KEY,
    stop_id_text    TEXT UNIQUE NOT NULL REFERENCES ' || gtfs_schema || '.stops(stop_id)
)';

EXECUTE 'INSERT INTO ' || gtfs_schema || '.stop_id_map(stop_id_text)
    SELECT stop_id FROM ' || gtfs_schema || '.stops';

EXECUTE 'CREATE TABLE ' || gtfs_schema || '.stop_time_graph(
    id              SERIAL PRIMARY KEY,
    source          INTEGER REFERENCES ' || gtfs_schema || '.stop_id_map(stop_id_int4),
    target          INTEGER REFERENCES ' || gtfs_schema || '.stop_id_map(stop_id_int4),
    waiting_time    INTEGER,
    travel_time     INTEGER
)';

FOR t_id, headway IN
EXECUTE 'SELECT trip_id, headway_secs FROM ' ||
         gtfs_schema || '.frequencies'
LOOP
    FOR src_id, src_time IN
    EXECUTE 'SELECT stop_id_int4, departure_time FROM ' ||
             gtfs_schema || '.stop_times JOIN ' ||
             gtfs_schema || '.stop_id_map
             ON stop_id = stop_id_text
             WHERE trip_id = ''' || t_id || ''''
    LOOP
        FOR dest_id, dest_time IN
        EXECUTE 'SELECT stop_id_int4, arrival_time FROM ' ||
                 gtfs_schema || '.stop_times JOIN ' ||
                 gtfs_schema || '.stop_id_map
                 ON stop_id = stop_id_text
                 WHERE trip_id = ''' || t_id || ''' AND
                       arrival_time > ''' || src_time || ''''
        LOOP
            EXECUTE 'INSERT INTO ' || gtfs_schema || '.stop_time_graph
            (source, target, waiting_time, travel_time) VALUES
            (' ||
                src_id::INTEGER || ',' ||
                dest_id::INTEGER || ',' ||
                headway::INTEGER / 2 || ',' ||
                extract(epoch from (dest_time::INTERVAL - src_time::INTERVAL))::INTEGER ||
            ')';
        END LOOP;
    END LOOP;
END LOOP;
END
$$ LANGUAGE 'plpgsql';

CREATE TYPE nonsc_path_result AS (changeover_id TEXT, cost DOUBLE PRECISION);

CREATE OR REPLACE FUNCTION non_scheduled_route(gtfs_schema TEXT, source TEXT, target TEXT)
RETURNS SETOF nonsc_path_result
AS
$$
BEGIN
RETURN QUERY EXECUTE
    'SELECT stop_id_text as changeover_id, cost FROM
     shortest_path(
        ''SELECT
            id,
            source::int4,
            target::int4,
            (waiting_time + travel_time)::float8 AS cost
          FROM ' || gtfs_schema || '.stop_time_graph'',
        (select stop_id_int4 from ' || gtfs_schema || '.stop_id_map where stop_id_text = ''' || source || '''),
        (select stop_id_int4 from ' || gtfs_schema || '.stop_id_map where stop_id_text = ''' || target || '''),
        true,
        false
     ) JOIN ' || gtfs_schema || '.stop_id_map ON vertex_id = stop_id_int4';
END
$$ LANGUAGE 'plpgsql';
