

CREATE OR REPLACE FUNCTION _unnest_matrix(matrix float8[][], OUT start_vid integer, OUT end_vid integer, out agg_cost float8)
RETURNS SETOF record AS 

$body$
DECLARE

m float8[];

BEGIN
    start_vid = 1;
    foreach m slice 1 in  ARRAY matrix
    LOOP
        end_vid = 1;
        foreach agg_cost in  ARRAY m
        LOOP
            RETURN next;
            end_vid = end_vid + 1;
        END LOOP;
        start_vid = start_vid + 1;
    END LOOP;
END;
$body$
language plpgsql volatile cost 500 ROWS 50;



CREATE OR REPLACE FUNCTION pgr_tsp(
    matrix float8[][],
    startpt INTEGER,
    endpt INTEGER DEFAULT -1,
    OUT seq INTEGER,
    OUT id INTEGER)
RETURNS SETOF record AS
$body$
DECLARE
table_sql TEXT;
debuglevel TEXT;
BEGIN
    -- checking the fixed columns and data types of the integers
    EXECUTE 'SHOW client_min_messages' INTO debuglevel;

    EXECUTE 'SET client_min_messages TO NOTICE';
    RAISE NOTICE 'Deprecated Signature pgr_tsp(float8[][], integer, integer)';
    EXECUTE 'set client_min_messages  to '|| debuglevel;

    CREATE TEMP TABLE ___tmp2 ON COMMIT DROP AS SELECT * FROM _unnest_matrix( matrix );


    IF endpt = -1 THEN endpt := startpt;
    END IF;

    
    RETURN QUERY
    WITH 
    result AS (
        SELECT * FROM pgr_TSP(
        $$SELECT * FROM ___tmp2 $$,
        startpt, endpt,

        tries_per_temperature :=  500 :: INTEGER,
        max_changes_per_temperature := 30 :: INTEGER,
        max_consecutive_non_changes := 500 :: INTEGER,

        randomize:=false)
    )
    SELECT (row_number() over(ORDER BY result.seq) - 1)::INTEGER AS seq, (result.node)::INTEGER AS id

    FROM result WHERE NOT(result.node = startpt AND result.seq != 1);

    DROP TABLE ___tmp2;
END;
$body$
language plpgsql volatile cost 500 ROWS 50;
