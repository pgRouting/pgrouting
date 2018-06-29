
\echo -- q1
SELECT * FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);

\echo -- q2
SELECT * FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table', 4
);

\echo -- q3
SELECT * FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table where id > 10', 9
);

\echo -- q4
