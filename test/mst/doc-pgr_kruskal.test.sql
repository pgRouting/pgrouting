\echo -- q1
SELECT edge, cost FROM pgr_kruskal(
    'SELECT id, source, target, cost, reverse_cost
        FROM edge_table'
) ORDER BY edge;

\echo -- q2
