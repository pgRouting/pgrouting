--------------------------------------------------------------------------------
--              PGR_alphaShape
--------------------------------------------------------------------------------
-- testing with areas
--SELECT * FROM pgr_alphaShape('SELECT id, x, y FROM vertex_table');
--SELECT * FROM pgr_alphaShape('SELECT id::integer, ST_X(the_geom)::float AS x, ST_Y(the_geom)::float AS y FROM edge_table_vertices_pgr');

SELECT round(ST_Area(ST_MakePolygon(ST_AddPoint(foo.openline, ST_StartPoint(foo.openline))))::numeric, 2) AS st_area
FROM (SELECT ST_MakeLine(points ORDER BY id) AS openline FROM
(SELECT ST_MakePoint(x, y) AS points, row_number() over() AS id

FROM pgr_alphaShape('SELECT id, x, y FROM vertex_table')

) AS a) AS foo;

\echo '-----------------------------'

SELECT round(ST_Area(ST_MakePolygon(ST_AddPoint(foo.openline, ST_StartPoint(foo.openline))))::numeric, 2) AS st_area
FROM (SELECT ST_MakeLine(points ORDER BY id) AS openline FROM
(SELECT ST_MakePoint(x, y) AS points, row_number() over() AS id

FROM pgr_alphaShape('SELECT id::integer, ST_X(the_geom)::float AS x, ST_Y(the_geom)::float AS y FROM edge_table_vertices_pgr')

) AS a) AS foo;
