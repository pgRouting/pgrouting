CREATE OR REPLACE FUNCTION update_stop_time_graph(nonsc_schema TEXT)
RETURNS VOID AS
$$
DECLARE
t_id        TEXT;
headway     INTEGER;
src_id      TEXT;
src_time    INTEGER;
dest_id     TEXT;
dest_time   INTEGER;
BEGIN
DROP TABLE IF EXISTS stop_time_graph;
CREATE TABLE stop_time_graph(
id              SERIAL PRIMARY KEY,
source          INTEGER,
target          INTEGER,
waiting_time    INTEGER,
travel_time     INTEGER
);

FOR t_id, headway in EXECUTE 'SELECT trip_id, headway_secs FROM ' || nonsc_schema || '.trip' LOOP
    FOR src_id, src_time IN EXECUTE 'SELECT stop_id, stop_time FROM ' || nonsc_schema || '.stoptime WHERE trip_id = ''' || t_id || '''' LOOP
        FOR dest_id, dest_time IN EXECUTE 'SELECT stop_id, stop_time FROM ' || nonsc_schema || '.stoptime WHERE trip_id = ''' || t_id || ''' AND stop_time > ' || src_time LOOP
            INSERT INTO stop_time_graph(source, target, waiting_time, travel_time) VALUES
            (
                src_id::INTEGER,
                dest_id::INTEGER,
                headway::INTEGER / 2,
                dest_time::INTEGER - src_time::INTEGER
            );
        END LOOP;
    END LOOP;
END LOOP;
END
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION non_scheduled_route(source INTEGER, target INTEGER)
RETURNS SETOF path_result
AS
$$
BEGIN
RETURN QUERY SELECT * FROM shortest_path('SELECT id, source::int4, target::int4, (waiting_time + travel_time)::float8 AS cost FROM stop_time_graph', source, target, true, false);
END
$$ LANGUAGE 'plpgsql';
