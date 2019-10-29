\echo --q1
SELECT * FROM pgr_kruskalBFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    2
);
\echo --q2
SELECT * FROM pgr_kruskalBFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[13,2], max_depth := 3
);
\echo --q3
