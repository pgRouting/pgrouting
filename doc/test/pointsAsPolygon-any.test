-------------------------------------------------------------------------------
--              PGR_pointsAsPolygon
-------------------------------------------------------------------------------


SELECT round(ST_Area(pgr_pointsAsPolygon('SELECT id, x, y FROM vertex_table'))::numeric, 2);
SELECT round(ST_Area(pgr_pointsASPolygon('SELECT id::integer, st_x(the_geom)::float as x, st_y(the_geom)::float as y  FROM edge_table_vertices_pgr'))::numeric, 2);









