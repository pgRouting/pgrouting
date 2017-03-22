--------------------------------------------------------------------------------
--              PGR_alphaShape
--------------------------------------------------------------------------------

\echo -- q1
SELECT * FROM pgr_alphaShape(
    'SELECT id::integer, ST_X(the_geom)::float AS x, ST_Y(the_geom)::float AS y
    FROM edge_table_vertices_pgr') ORDER BY x, y;
\echo -- q2

SELECT round(ST_Area(ST_MakePolygon(ST_AddPoint(foo.openline, ST_StartPoint(foo.openline))))::numeric, 2) AS st_area
FROM (
    SELECT ST_MakeLine(points ORDER BY id) AS openline
    FROM (
        SELECT ST_MakePoint(x, y) AS points, row_number() over() AS id
        FROM pgr_alphaShape(
            'SELECT id::integer, ST_X(the_geom)::float AS x, ST_Y(the_geom)::float AS y
            FROM edge_table_vertices_pgr')
        ) AS a
   ) AS foo;

\echo -- q3
