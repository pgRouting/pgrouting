\echo -- q1
SELECT * FROM pgr_TSPeuclidean(
    $$
    SELECT id, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr
    $$,
    randomize := false);
\echo -- q2
SELECT* from pgr_TSPeuclidean(
    $$
    SELECT id, st_X(the_geom) AS x, st_Y(the_geom) AS y FROM edge_table_vertices_pgr
    $$,
    tries_per_temperature := 3,
    cooling_factor := 0.5,
    randomize := false);
\echo -- q3
SET client_min_messages TO DEBUG1;

SELECT* from pgr_TSPeuclidean(
    $$
    SELECT id, st_X(the_geom) AS x, st_Y(the_geom) AS y FROM edge_table_vertices_pgr
    $$,
    tries_per_temperature := 0,
    randomize := false);
\echo -- q4
