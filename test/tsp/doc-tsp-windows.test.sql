
\echo -- q1
SELECT * FROM pgr_TSP(
    $$
    SELECT id::INTEGER, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr
    $$
    , 1);

\echo -- q2
SELECT * FROM pgr_eucledianTSP(
    $$
    SELECT id, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr
    $$,
    1,
    randomize := false
);

\echo -- q3

\echo -- q4
SELECT * FROM pgr_TSP(
    $$
    SELECT * FROM pgr_dijkstraCostMatrix(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        (SELECT array_agg(id) from edge_table_vertices_pgr WHERE id < 14), false)
    $$,
    1,
    randomize := false
);
\echo -- q5
