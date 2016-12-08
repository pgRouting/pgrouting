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
SELECT * FROM pgr_TSP(
    (SELECT * FROM pgr_vidsToDMatrix(
            'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
            ARRAY[8,11,12,13]::INTEGER[], false , true, true)
    ),
    1
);

\echo -- q4
SELECT * FROM pgr_TSP(
    $$
    SELECT * FROM pgr_dijkstraCostMatrix(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        ARRAY[8,11,12,13]::INTEGER[], false)
    $$,
    8,
    randomize := false
);
\echo -- q5
