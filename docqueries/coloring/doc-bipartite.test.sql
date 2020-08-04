\echo --q1
SELECT * FROM pgr_bipartite(
    $$SELECT id,source,target,cost,reverse_cost FROM edge_table$$
);
\echo --q2
SELECT * FROM pgr_bipartite(
    $$SELECT id,source,target,cost,reverse_cost FROM edge_table WHERE id IN (8, 10, 11, 12)$$
);
\echo --q3