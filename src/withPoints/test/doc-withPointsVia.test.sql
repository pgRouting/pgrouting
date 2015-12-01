
\echo -- q0

SELECT id, source, target FROM edge_table WHERE id IN (4, 5, 14);

\echo -- q1

SELECT * FROM pgr_withPointsVia(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    array[4, 5, 14], array[0.1, 0.5, 0.85]
);

SELECT * FROM pgr_withPointsVia(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    array[4, 5, 14], array[0.1, 0.5, 0.85],
    true
);

\echo -- q2

SELECT * FROM pgr_withPointsVia(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    array[4, 5, 14], array[0.1, 0.5, 0.85],
    false
);

\echo -- q3

SELECT * FROM pgr_withPointsVia(
    'SELECT id, source, target, cost FROM edge_table',
    array[4, 5, 14], array[0.1, 0.5, 0.85],
    true
);

\echo -- q4

SELECT * FROM pgr_withPointsVia(
    'SELECT id, source, target, cost FROM edge_table',
    array[4, 5, 14], array[0.1, 0.5, 0.85],
    false
);

\echo -- q5

