
-- duplicated points are removed
PREPARE q1 AS
SELECT * FROM pgr_alphaShape1(
    'SELECT ST_X(the_geom) AS x, ST_Y(the_geom) AS y
    FROM edge_table_vertices_pgr
    UNION ALL
    SELECT ST_X(the_geom) AS x, ST_Y(the_geom) AS y
    FROM edge_table_vertices_pgr

    ORDER BY x, y
    ') ORDER BY x, y;

PREPARE q2 AS
SELECT * FROM pgr_alphaShape1(
    'SELECT ST_X(the_geom) AS x, ST_Y(the_geom) AS y
    FROM edge_table_vertices_pgr

    ORDER BY x, y
    ') ORDER BY x, y;

-- set_eq(q1, q2)

EXECUTE q1;
EXECUTE q2;

