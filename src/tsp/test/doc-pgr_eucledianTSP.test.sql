\echo -- q1
WITH
query AS (
    SELECT * FROM pgr_eucledianTSP(
        $$
        SELECT id, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr
        $$
    )
)
SELECT agg_cost < 20 AS under_20 FROM query WHERE seq = 18;

\echo -- q2
SELECT* from pgr_eucledianTSP(
    $$
    SELECT id, st_X(the_geom) AS x, st_Y(the_geom) AS y FROM edge_table_vertices_pgr
    $$,
    tries_per_temperature := 3,
    cooling_factor := 0.5,
    randomize := false
);
\echo -- q3
SET client_min_messages TO DEBUG1;

-- TODO see what this outputs

SELECT* from pgr_eucledianTSP(
    $$
    SELECT id, st_X(the_geom) AS x, st_Y(the_geom) AS y FROM edge_table_vertices_pgr
    $$,
    tries_per_temperature := 0,
    randomize := false
);
\echo -- q4
