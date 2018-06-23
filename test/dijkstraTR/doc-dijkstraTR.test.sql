
\echo -- q1
SELECT * FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 3
);

\echo -- q2
SELECT * FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 3,
    FALSE
);
\echo -- q3
