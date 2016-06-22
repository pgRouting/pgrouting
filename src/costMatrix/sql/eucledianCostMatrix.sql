-- vertices_sql has the form
-- SELECT id, the_geom FROM .....

CREATE OR REPLACE FUNCTION pgr_eucledianCostMatrix(vertices_sql TEXT,
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

    sql := 'CREATE TABLE ___tmp  AS ' || vertices_sql ;
    EXECUTE sql;

    RETURN query SELECT * FROM pgr_eucledianDMatrix('___tmp'::regclass);

    DROP TABLE ___tmp;
    RETURN;

END
$BODY$
language plpgsql volatile cost 10;

CREATE OR REPLACE FUNCTION pgr_eucledianCostMatrix(
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
                RAISE EXCEPTION 'An expected column was not found in the query (3)'
                USING HINT = 'Please verify columns: (id, the_geom) or (id, x, y) columns';
        END;
    END IF;


    IF which = 1 THEN
        sql := 'WITH
        vertices AS (' || sql || '),
        distances AS (SELECT DISTINCT a.id::BIGINT AS start_id, b.id::BIGINT as end_id, ST_Distance(a.the_geom, b.the_geom) as distance
            FROM  vertices AS a, vertices AS b
            WHERE a.id != b.id
            ORDER BY start_id, end_id)
        SELECT * from distances';
    ELSE 
        sql := 'WITH
        vertices AS (' || sql || '),
        distances AS (SELECT DISTINCT a.id::BIGINT AS start_id, b.id::BIGINT as end_id, ST_Distance(ST_MakePoint(a.x,a.y), ST_MakePoint(b.x,b.y)) as distance
            FROM  vertices AS a, vertices AS b
            WHERE a.id != b.id
            ORDER BY start_id, end_id)
        SELECT * from distances';
    END IF;

    BEGIN
        RETURN query EXECUTE sql;
    END;
END
$BODY$
language plpgsql volatile cost 10;
