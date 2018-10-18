UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    order_by := 'dfs'
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'dfs'
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    order_by := 'dfs'
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'dfs'
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5,
    order_by := 'dfs'
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5,
    order_by := 'bfs'
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    5
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    order_by := ''
);

