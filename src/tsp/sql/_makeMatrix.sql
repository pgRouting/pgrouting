-- vertices_sql has the form
-- SELECT id, the_geom FROM .....

CREATE OR REPLACE FUNCTION _pgr_makeMatrix(
    matrix_sql regclass,
    OUT dmatrix double precision[],
    OUT ids BIGINT[]
)
AS
$BODY$
DECLARE
    sql text;
    count_sql text;
    ids_sql text;
    rec record;
    dummy_row double precision[];
    n INTEGER; -- size of matrix
    has_the_geom BOOLEAN DEFAULT false;
    
BEGIN
    dmatrix := array[]::double precision[];
    ids := array[]::integer[];
    sql = format('SELECT start_vid, end_vid, agg_cost FROM %I ', matrix_sql);
    BEGIN
        EXECUTE sql || 'LIMIT 1';
        EXCEPTION WHEN OTHERS THEN
            RAISE EXCEPTION 'An expected column was not found in the query 1'
            USING HINT = 'Please verify columns: (start_vid, end_vid, agg_cost)';
    END;

    count_sql = format('SELECT count(DISTINCT start_vid) AS count FROM %I', matrix_sql);
    EXECUTE count_sql INTO rec;
    n := rec.count;

    ids_sql = format('SELECT DISTINCT start_vid FROM %I ORDER BY start_vid', matrix_sql);
    FOR rec IN EXECUTE ids_sql LOOP
        ids := ids || rec.start_vid;
    END LOOP;

    FOR i in 1 .. n LOOP
        dummy_row := dummy_row || '+Infinity'::float8;
    END LOOP;

    FOR i in 1 .. n LOOP
        dmatrix := dmatrix || ARRAY[dummy_row];
    END LOOP;

    FOR i IN 1 .. n LOOP
        dmatrix[i][i] :=  0.0;
    END LOOP;

    sql = format('
    WITH 
      ids AS (SELECT DISTINCT start_vid FROM %I ORDER by start_vid),
      distinct_ids AS (SELECT DISTINCT row_number() over () AS id, start_vid FROM ids),
      renumbered1 AS (SELECT id AS new_start_vid,  end_vid, agg_cost FROM distinct_ids JOIN %I USING (start_vid) ORDER BY new_start_vid, end_vid),
      renumbered2 AS (SELECT new_start_vid,  id AS new_end_vid, agg_cost FROM distinct_ids JOIN renumbered1 ON (start_vid = end_vid))
      SELECT new_start_vid AS start_vid, new_end_vid AS end_vid, agg_cost from renumbered2 ORDER BY new_start_vid', matrix_sql,matrix_sql);

    FOR rec IN EXECUTE sql LOOP
        dmatrix[rec.start_vid][rec.end_vid] := rec.agg_cost;
    END LOOP;

END
$BODY$
language plpgsql stable cost 10;
