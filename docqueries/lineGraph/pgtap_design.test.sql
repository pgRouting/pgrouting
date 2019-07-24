
\echo -- one edge graph
SELECT * FROM pgr_lineGraph(
    'SELECT id, source, target, cost FROM edge_table WHERE id = 1'
);

\echo -- two edge graphs
SELECT * FROM pgr_lineGraph($$
    SELECT -id AS id, target, source, reverse_cost AS cost FROM edge_table WHERE id = 1
    UNION
    SELECT id, source, target, cost FROM edge_table WHERE id = 1
    $$
);
SELECT * FROM pgr_lineGraph($$
    SELECT id, target, source, reverse_cost AS cost FROM edge_table WHERE id = 1
    UNION
    SELECT -id AS id, source, target, cost FROM edge_table WHERE id = 1
    $$
);
SELECT * FROM pgr_lineGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1'
);
SELECT * FROM pgr_lineGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 17'
);

