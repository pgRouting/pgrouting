
\echo -- q1
SELECT edge, cost FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 14'
) ORDER BY edge;

\echo -- q2
