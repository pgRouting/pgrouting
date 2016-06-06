
/*
    Old signature has:
    sql: id INTEGER, x FLOAT, y FLOAT
*/




CREATE OR  REPLACE FUNCTION pgr_tsp(sql text, start_id INTEGER, end_id INTEGER default (-1))
returns setof pgr_costResult as
$body$
DECLARE
table_sql TEXT;
rec RECORD;
debuglevel TEXT;
n BIGINT;

BEGIN
    -- checking the fixed columns and data types of the integers
    EXECUTE 'SHOW client_min_messages' INTO debuglevel;
    EXECUTE 'SET client_min_messages TO ERROR';

    table_sql := 'CREATE TABLE ___tmp  AS ' || sql ;
    EXECUTE table_sql;


    BEGIN
        EXECUTE 'SELECT id, x, y FROM ___tmp' INTO rec;
        EXCEPTION
            WHEN OTHERS THEN
                EXECUTE 'set client_min_messages  to '|| debuglevel;
                RAISE EXCEPTION 'An expected column was not found in the query'
                USING ERRCODE = 'XX000',
                HINT = 'Please verify the column names: id, x, y';
    END;

    EXECUTE
    'SELECT
        pg_typeof(id)::text as id_type,
        pg_typeof(x)::text as x_type,
        pg_typeof(y)::text as y_type FROM ___tmp' INTO rec;


    IF NOT((rec.id_type in ('integer'::text))
        AND (rec.x_type = 'double precision'::text)
        AND (rec.y_type = 'double precision'::text)) THEN
            EXECUTE 'set client_min_messages  to '|| debuglevel;
            RAISE EXCEPTION 'Error, ''id'' must be of type INTEGER, ''x'' ad ''y'' must be of type FLOAT'
            USING ERRCODE = 'XX000';
    END IF;

    EXECUTE 'SELECT count(*) AS n FROM (' || sql || ') AS __a__' INTO rec;
    n = rec.n;

    RETURN query
        SELECT seq - 1 AS seq, node AS id1, node AS id2, cost
        FROM pgr_eucledianTSP(sql, start_id, end_id,

            tries_per_temperature :=  500 * n :: INTEGER,
            max_changes_per_temperature := 60 * n :: INTEGER,
            max_consecutive_non_changes := 500 * n :: INTEGER,

            randomize := false) WHERE seq <= n;
        DROP TABLE ___tmp;
        EXECUTE 'set client_min_messages  to '|| debuglevel;

END;
$body$
language plpgsql volatile cost 500 ROWS 50;
