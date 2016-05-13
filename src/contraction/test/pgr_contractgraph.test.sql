\echo --q1
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost AS revcost FROM edge_table',
    0, false
);

\echo --q2
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost AS revcost FROM edge_table',
    1, false
);