\echo -- q1
SELECT ST_Npoints(pgr_alphaShape((SELECT array_agg(the_geom) FROM edge_table_vertices_pgr)));
\echo -- q2
SELECT ST_Area(geom) FROM pgr_alphaShape((SELECT ST_Collect(the_geom) FROM edge_table_vertices_pgr), 1.5);
\echo -- q3
