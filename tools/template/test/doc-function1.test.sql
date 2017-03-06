
\echo -- q1
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3
);

\echo -- q2
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3,
    FALSE
);
\echo -- q3

