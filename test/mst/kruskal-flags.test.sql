UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    max_depth := 3
);

SELECT * FROM pgr_kruskalDFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    0
);


SELECT * FROM pgr_kruskalDFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5
);

SELECT * FROM pgr_kruskalDFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5,
    max_depth := 3
);

SELECT * FROM pgr_kruskalBFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    0
);


SELECT * FROM pgr_kruskalBFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5
);

SELECT * FROM pgr_kruskalBFS(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5,
    max_depth := 3
);
