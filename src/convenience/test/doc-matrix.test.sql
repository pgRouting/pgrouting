BEGIN;

SET client_min_messages TO NOTICE;

\echo --q1
SELECT * FROM pgr_pointstodmatrix(pgr_texttopoints('2,0;2,1;3,1;2,2', 0));

\echo --q1.1
SELECT * from pgr_tsp(
    (SELECT dMatrix FROM pgr_pointstodmatrix(pgr_texttopoints('2,0;2,1;3,1;2,2', 0))
    ),
    1
);

\echo --q2

SELECT * FROM pgr_vidstodmatrix(
    ARRAY[1,2,3,5],
    ARRAY(select the_geom FROM edge_table_vertices_pgr WHERE id in (1,2,3,5)),
    'edge_table'
);

\echo --q2.1
SELECT * FROM pgr_vidstodmatrix(
    pgr_pointstovids(pgr_texttopoints('2,0;2,1;3,1;2,2', 0), 'edge_table'),
    pgr_texttopoints('2,0;2,1;3,1;2,2', 0),
    'edge_table');

\echo --q2.2
SELECT * FROM pgr_tsp(
    (SELECT dMatrix FROM pgr_vidstodmatrix(
        pgr_pointstovids(pgr_texttopoints('2,0;2,1;3,1;2,2', 0), 'edge_table'),
        pgr_texttopoints('2,0;2,1;3,1;2,2', 0),
        'edge_table')
    ),
    1
);
\echo --q3
SELECT * FROM pgr_vidsToDMatrix(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    array[1,2,3,5],
    true, true, false);

SELECT * FROM pgr_vidsToDMatrix(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    array[1,2,3,5],
    true, true, true);
\echo --q4
SELECT * FROM pgr_tsp(
    (SELECT pgr_vidsToDMatrix(
        'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
        array[1,2,3,5],
        true, true, true)
    ),
    1
);
\echo --q5

ROLLBACK;
