\echo --q1
SELECT * FROM pgr_kruskalDFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    0
);
\echo --q2
SELECT * FROM pgr_kruskalDFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5
);
\echo --q3
SELECT * FROM pgr_kruskalDFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5,
    max_depth := 3
);
\echo --q4
SELECT * FROM pgr_kruskalBFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[5, 3],
    max_depth := 3
);
\echo --q5
