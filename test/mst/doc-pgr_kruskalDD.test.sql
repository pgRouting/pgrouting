\echo -- q1
SELECT * FROM pgr_kruskalDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    2,
    3
);
\echo -- q2
SELECT * FROM pgr_kruskalDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[2,13],
    3
);
\echo -- q3
