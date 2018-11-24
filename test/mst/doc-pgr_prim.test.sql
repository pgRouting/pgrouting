
\echo -- q1
SELECT * FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 14'
);

\echo -- q2
