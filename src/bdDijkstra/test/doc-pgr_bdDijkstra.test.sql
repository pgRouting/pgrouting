BEGIN;

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
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3,
    false, -- undirected
    false  -- no reverse_cost
);
\echo -- q4

ROLLBACK;
