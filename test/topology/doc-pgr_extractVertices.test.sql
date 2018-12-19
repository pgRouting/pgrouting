\echo --q1
SELECT  * FROM pgr_extractVertices('SELECT source, target, the_geom FROM edge_table');
\echo --q1.1

\echo --q2
SELECT  * FROM pgr_extractVertices('SELECT source, target FROM edge_table WHERE id < 5');
\echo --q2.1

\echo --q3
DROP TABLE IF EXISTS edge_table_vertices_pgr;
SELECT  * INTO edge_table_vertices_pgr
FROM pgr_extractVertices('SELECT source, target, the_geom FROM edge_table');
SELECT * FROM edge_table_vertices_pgr;
\echo --q3.1

