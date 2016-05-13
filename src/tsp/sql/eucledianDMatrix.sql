-- vertices_sql has the form
-- SELECT id, the_geom FROM .....

CREATE OR REPLACE FUNCTION pgr_eucledianDMatrix(vertices_sql TEXT,
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
        IF rec.word = 'the_geom' THEN has_the_geom = TRUE; END IF;
        IF rec.word = 'id' THEN has_id = TRUE; END IF;
        IF rec.word = 'x' THEN has_x = TRUE; END IF;
        IF rec.word = 'y' THEN has_y = TRUE; END IF;
    END LOOP;

    IF has_id = false THEN
        RAISE EXCEPTION 'An expected column was not found in the query'
        USING HINT = 'Please verify columns: (id, the_geom) or (id, x, y) columns';
    END IF;

    IF NOT has_the_geom THEN
        IF (NOT has_x OR NOT has_y) THEN
            RAISE EXCEPTION 'An expected column was not found in the query'
            USING HINT = 'Please verify columns: (id, the_geom) or (id, x, y) columns';
        END IF;
    END IF;


    IF has_the_geom THEN
        sql := 'WITH
             vertices AS (' || quote_vertices_sql || '),
             distances AS (SELECT DISTINCT a.id::BIGINT AS start_id, b.id::BIGINT as end_id, ST_Distance(a.the_geom, b.the_geom) as distance
                FROM  vertices AS a, vertices AS b
                WHERE a.id != b.id
                ORDER BY start_id, end_id)
            SELECT * from distances';
    ELSE 
        sql := 'WITH
             vertices AS (' || vertices_sql || '),
             distances AS (SELECT DISTINCT a.id::BIGINT AS start_id, b.id::BIGINT as end_id, ST_Distance(ST_MakePoint(a.x,a.y), ST_MakePoint(b.x,b.y)) as distance
                FROM  vertices AS a, vertices AS b
                WHERE a.id != b.id
                ORDER BY start_id, end_id)
            SELECT * from distances';
    END IF;

    BEGIN
        RETURN query EXECUTE sql;

        -- TODO
        /*
        EXCEPTION WHEN OTHERS THEN
            RAISE EXCEPTION 'Column missing: Expected (id, the_geom) or (id, x, y) columns 1'
            USING HINT = 'Please verify the query returns the expected columns & types:
' || sql;
*/
    END;

END
$BODY$
language plpgsql stable cost 10;

CREATE OR REPLACE FUNCTION pgr_eucledianDMatrix(
    vertices_sql regclass,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost DOUBLE PRECISION)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
    sql text;
    which INTEGER := 0;
    rec record;
    has_the_geom BOOLEAN DEFAULT false;
    has_id BOOLEAN DEFAULT false;
    has_x BOOLEAN DEFAULT false;
    has_y BOOLEAN DEFAULT false;
    
BEGIN
    sql = format('SELECT id, the_geom from %I ', vertices_sql);
    BEGIN
        EXECUTE sql || 'LIMIT 1';
        which := 1;
        EXCEPTION WHEN OTHERS THEN
        which := 0;
    END;

    IF which = 0 THEN
        sql = format('SELECT id, x, y from %I ', vertices_sql);
        BEGIN
            EXECUTE sql || 'LIMIT 1';
            which := 2;
            EXCEPTION WHEN OTHERS THEN
                RAISE EXCEPTION 'An expected column was not found in the query 1'
                USING HINT = 'Please verify columns: (id, the_geom) or (id, x, y) columns';
        END;
    END IF;


    IF which = 1 THEN
        sql := 'WITH
        vertices AS (' || sql || '),
        distances AS (SELECT DISTINCT a.id AS start_id, b.id as end_id, ST_Distance(a.the_geom, b.the_geom) as distance
            FROM  vertices AS a, vertices AS b
            WHERE a.id != b.id
            ORDER BY a.id, b.id)
        SELECT * from distances';
    ELSE 
        sql := 'WITH
        vertices AS (' || sql || '),
        distances AS (SELECT DISTINCT a.id AS start_id, b.id as end_id, ST_Distance(ST_MakePoint(a.x,a.y), ST_MakePoint(b.x,b.y)) as distance
            FROM  vertices AS a, vertices AS b
            WHERE a.id != b.id
            ORDER BY a.id, b.id)
        SELECT * from distances';
    END IF;

    BEGIN
        RETURN query EXECUTE sql;

        EXCEPTION WHEN OTHERS THEN
            RAISE EXCEPTION 'Column missing: Expected (id, the_geom) or (id, x, y) columns 2'
            USING HINT = 'Please verify the query returns the expected columns & types:
            ' || vertices_sql;
    END;
END
$BODY$
language plpgsql stable cost 10;
