
\echo -- q1
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 3
);

\echo -- q2
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 3,
    FALSE
);
\echo -- q3
