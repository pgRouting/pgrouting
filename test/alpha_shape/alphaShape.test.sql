--------------------------------------------------------------------------------
--              PGR_alphaShape
--------------------------------------------------------------------------------

\echo -- q1
SELECT ST_Area(geom), ST_isValid(geom), ST_NPoints(geom)
FROM pgr_alphaShape1((SELECT array_agg(the_geom) FROM edge_table), 1.581);
SELECT ST_Area(geom), ST_isValid(geom), ST_NPoints(geom)
FROM pgr_alphaShape1((SELECT array_agg(the_geom) FROM edge_table), 1.582);
\echo -- q2
