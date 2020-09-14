\echo --q1
SELECT * FROM pgr_bipartite(
    $$SELECT id,source,target,cost,reverse_cost FROM edge_table$$
);
\echo --q2
INSERT INTO edge_table (source, target, cost, reverse_cost) VALUES
(1, 7, 1, 1);
\echo --q3
SELECT * FROM pgr_bipartite(
    $$SELECT id,source,target,cost,reverse_cost FROM edge_table$$
);
\echo --q4
