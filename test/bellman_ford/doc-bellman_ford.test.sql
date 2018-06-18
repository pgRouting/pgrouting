
\echo -- q1
SELECT * FROM pgrBellmanFord(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3
);

\echo -- q2
SELECT * FROM pgrBellmanFord(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3,
    FALSE
);
\echo -- q3

