--------------------------------------------------------------------------------
--              PGR_alphAShape
--------------------------------------------------------------------------------
-- testing with areas
--SELECT * FROM pgr_alphAShape('SELECT id, x, y FROM vertex_table');
--SELECT * FROM pgr_alphAShape('SELECT id::integer, st_x(the_geom)::float as x, st_y(the_geom)::float as y  FROM edge_table_vertices_pgr');

SELECT round(st_area(ST_MakePolygon(ST_AddPoint(foo.openline, ST_StartPoint(foo.openline))))::numeric, 2) as st_area
from (select st_makeline(points order by id)  as openline from
(SELECT st_makepoint(x,y) as points ,row_number() over() AS id 

FROM pgr_alphAShape('SELECT id, x, y FROM vertex_table')

) as a) as foo;

\echo '-----------------------------'

SELECT round(st_area(ST_MakePolygon(ST_AddPoint(foo.openline, ST_StartPoint(foo.openline))))::numeric, 2) as st_area
from (select st_makeline(points order by id)  as openline from
(SELECT st_makepoint(x,y) as points ,row_number() over() AS id 

FROM pgr_alphAShape('SELECT id::integer, st_x(the_geom)::float as x, st_y(the_geom)::float as y  FROM edge_table_vertices_pgr')

) as a) as foo;
