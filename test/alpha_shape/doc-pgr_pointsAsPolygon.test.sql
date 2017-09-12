-------------------------------------------------------------------------------
--              PGR_pointsAsPolygon
-------------------------------------------------------------------------------

\echo --q1
SELECT ST_AsText(pgr_pointsAsPolygon('SELECT id::integer, ST_X(the_geom)::float AS x, ST_Y(the_geom)::float AS y
        FROM edge_table_vertices_pgr'));
\echo --q2
