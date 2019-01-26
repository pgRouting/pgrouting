--------------------------------------------------------------------------------
--              PGR_alphaShape
--------------------------------------------------------------------------------

\echo -- q1
SELECT * FROM pgr_alphaShape1((SELECT array_agg(the_geom) FROM edge_table));
\echo -- q2

SELECT (ST_Dump(ST_DelaunayTriangles(ST_union(the_geom), 0 , 0))).geom
INTO delauny FROM edge_table;

SELECT * FROM pgr_alphaShape1((SELECT array_agg(geom) FROM delauny), is_delauny := true);
\echo -- q3
