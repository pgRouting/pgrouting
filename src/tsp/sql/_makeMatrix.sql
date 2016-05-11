-- vertices_sql has the form
-- SELECT id, the_geom FROM .....

CREATE OR REPLACE FUNCTION _pgr_makeMatrix(matrix_sql TEXT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost DOUBLE PRECISION)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
    sql text;
    rec record;
    has_the_geom BOOLEAN DEFAULT false;
    has_id BOOLEAN DEFAULT false;
    has_x BOOLEAN DEFAULT false;
    has_y BOOLEAN DEFAULT false;
    
BEGIN

    sql = replace(vertices_sql,',',' , ');
    FOR rec IN 
        WITH
        words AS
        (SELECT word FROM regexp_split_to_table(lower(sql), E'\\s+') AS word)
        SELECT * FROM words WHERE word IN ('the_geom', 'id', 'x', 'y')
    LOOP
        IF rec.word = 'start_vid' THEN has_start_vid = TRUE; END IF;
        IF rec.word = 'end_vid' THEN has_end_vid = TRUE; END IF;
        IF rec.word = 'agg_cost' THEN has_agg_cost = TRUE; END IF;
    END LOOP;

    IF NOT (has_start_vid && has_end_vid && has_agg_cost) THEN
        RAISE EXCEPTION 'An expected column was not found in the query'
        USING HINT = 'Please verify columns: (start_vid, end_vid, agg_cost)';
    END IF;

    CREATE TEMP TABLE ___temp AS 



    IF has_the_geom THEN
        sql := 'WITH
             vertices AS (' || quote_vertices_sql || '),
             distances AS (SELECT DISTINCT a.id AS start_id, b.id as end_id, ST_Distance(a.the_geom, b.the_geom) as distance
                FROM  vertices AS a, vertices AS b
                WHERE a.id != b.id
                ORDER BY a.id, b.id)
            SELECT * from distances';
    ELSE 
        sql := 'WITH
             vertices AS (' || vertices_sql || '),
             distances AS (SELECT DISTINCT a.id AS start_id, b.id as end_id, ST_Distance(ST_MakePoint(a.x,a.y), ST_MakePoint(b.x,b.y)) as distance
                FROM  vertices AS a, vertices AS b
                WHERE a.id != b.id
                ORDER BY a.id, b.id)
            SELECT * from distances';
    END IF;

    BEGIN
        RETURN query EXECUTE sql;

        EXCEPTION WHEN OTHERS THEN
            RAISE EXCEPTION 'Column missing: Expected (id, the_geom) or (id, x, y) columns'
            USING HINT = 'Please verify the query returns the expected columns & types:
' || vertices_sql;
    END;

END
$BODY$
language plpgsql stable cost 10;

CREATE OR REPLACE FUNCTION _pgr_makeMatrix(
    matrix_sql regclass,
    OUT dmatrix double precision[],
    OUT ids integer[]
)
AS
$BODY$
DECLARE
    sql text;
    count_sql text;
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

    -- get the count
    count_sql = format('SELECT count(DISTINCT start_vid) AS count FROM %I', matrix_sql);
    EXECUTE count_sql INTO rec;
    n := rec.count;
    raise notice '%', n;

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
      ids AS (SELECT DISTINCT start_vid FROM %I),
      distinct_ids AS (SELECT DISTINCT row_number() over () AS id, start_vid FROM ids),
      renumbered1 AS (SELECT id AS new_start_vid,  end_vid, agg_cost FROM distinct_ids JOIN %I USING (start_vid)),
      renumbered2 AS (SELECT new_start_vid,  id AS new_end_vid, agg_cost FROM distinct_ids JOIN renumbered1 ON (start_vid = end_vid))
      SELECT new_start_vid AS start_vid, new_end_vid AS end_vid, agg_cost from renumbered2 ORDER BY new_start_vid', matrix_sql,matrix_sql);

    FOR rec IN EXECUTE sql LOOP
        raise notice '%,%', rec.start_vid, rec.end_vid;
        dmatrix[rec.start_vid][rec.end_vid] := rec.agg_cost;
    END LOOP;

END
$BODY$
language plpgsql stable cost 10;
