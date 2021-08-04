SET log_min_duration_statement=-1;
SET extra_float_digits=-3;
SET client_min_messages TO WARNING;
\echo -- q1
SELECT * FROM pgr_TSPeuclidean(
    $$
    SELECT id, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr
    $$);
\echo -- q2
SELECT *
FROM pgr_TSPeuclidean($$SELECT * FROM wi29$$)
WHERE seq = 30;
\echo -- q3
WITH
tsp_results AS (SELECT seq, geom FROM pgr_TSPeuclidean($$SELECT * FROM wi29$$) JOIN wi29 ON (node = id))
SELECT ST_MakeLine(ARRAY(SELECT geom FROM tsp_results ORDER BY seq));
\echo -- q4
-- This query is used to create the optimal solution image visually deduced the ordering
SELECT 1 AS gid, ST_MakeLine(ARRAY(
    SELECT geom FROM unnest(array[1,2,6,10,11,12,15,19,18,17,21,22,23,29,28,26,20,25,27,24,16,14,13,9,7,3,4,8,5,1])
    JOIN wi29 ON (unnest = id)));
