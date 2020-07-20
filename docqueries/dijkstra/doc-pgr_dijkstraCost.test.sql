\echo --q1
SELECT * FROM pgr_dijkstraCost(
    'select id, source, target, cost, reverse_cost from edge_table',
    2, 3);

\echo --q2
SELECT * FROM pgr_dijkstraCost(
    'select id, source, target, cost, reverse_cost from edge_table',
    2, 3, false);

\echo --q3
SELECT * FROM pgr_dijkstraCost(
    'select id, source, target, cost, reverse_cost from edge_table',
    ARRAY[2, 7], 3);

\echo --q4
SELECT * FROM pgr_dijkstraCost(
    'select id, source, target, cost, reverse_cost from edge_table',
    2, ARRAY[3, 11]);

\echo --q5
SELECT * FROM pgr_dijkstraCost(
    'select id, source, target, cost, reverse_cost from edge_table',
    ARRAY[2, 7], ARRAY[3, 11]);

\echo --q6
SELECT * FROM pgr_dijkstraCost(
        'select id, source, target, cost, reverse_cost from edge_table',
            ARRAY[5, 3, 4, 3, 3, 4], ARRAY[3, 5, 3, 4]);

\echo --q7
SELECT * FROM pgr_dijkstraCost(
        'select id, source, target, cost, reverse_cost from edge_table',
            ARRAY[5, 3, 4], ARRAY[5, 3, 4]);

-- pgr_dijkstra combinations SQL
-------------------------------------------------------------------------------

\echo --q8

SELECT * FROM pgr_dijkstraCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT source, target FROM combinations_table',
    FALSE
);

\echo --q9

SELECT * FROM pgr_dijkstraCost(
    'SELECT id, source, target, cost FROM edge_table',
    'SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3), (11, 5)) AS combinations (source, target)',
    FALSE
);

\echo --q10
