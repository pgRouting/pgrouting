\echo -- q1
SELECT * FROM pgr_TSPannealing(
    $$
    SELECT * FROM pgr_dijkstraCostMatrix(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE id < 14),
        directed := false)
    $$,
    randomize := false);
\echo -- q1.1
\echo -- q2
SELECT * FROM pgr_TSPannealing(
    $$
    SELECT id, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr
    $$,
    randomize := false);
\echo -- q2.1
\echo -- q3
SELECT * FROM pgr_TSPannealing(
    $$
    SELECT * FROM pgr_withPointsCostMatrix(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction from pointsOfInterest',
        array[-1, 3, 5, 6, -6], directed := false)
    $$,
    start_id := 5,
    randomize := false
);
\echo -- q3.1
