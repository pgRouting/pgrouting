
\echo -- q1
SELECT * FROM pgr_bdDijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3
);

\echo -- q2
SELECT * FROM pgr_bdDijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3,
    false
);
\echo -- q3
SELECT * FROM pgr_bdDijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[3, 11]);
\echo -- q4
SELECT * FROM pgr_bdDijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2, 7], 3);
\echo -- q5
SELECT * FROM pgr_bdDijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2, 7], ARRAY[3, 11]);
\echo -- q6
SELECT * FROM pgr_bdDijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3,
    false, -- undirected
    false  -- no reverse_cost
);
\echo -- q7
