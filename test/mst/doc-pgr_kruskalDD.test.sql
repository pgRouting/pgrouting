\echo -- q1
SELECT * FROM pgr_kruskalDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5,
    3.5
);
\echo -- q2
