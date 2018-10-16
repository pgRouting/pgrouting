UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    get_component := false
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    get_component := false
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    get_component := true
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    order_by := 1
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    order_by := 1,
    get_component := true
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    order_by := 2,
    get_component := true
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    order_by := 2,
    get_component := true
);

SELECT * FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    order_by := 2,
    get_component := true
);
