
\echo -- q1
SELECT * FROM pgr_areaContraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1,4,7,13]
);

\echo -- q2
SELECT * FROM pgr_areaContraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1,4,7,13],
    FALSE
);
\echo -- q3
