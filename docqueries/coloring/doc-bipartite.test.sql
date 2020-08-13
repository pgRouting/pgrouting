CREATE TABLE five_vertices_table (                                                                                    
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);
INSERT INTO five_vertices_table (source, target, cost, reverse_cost) VALUES (1,2,1,1),(2,3,1,1),(3,4,1,-1),(4,5,1,1),(5,1,1,-1);
\echo --q1
SELECT * FROM pgr_bipartite(
    $$SELECT id,source,target,cost,reverse_cost FROM edge_table$$
);
\echo --q2
SELECT * FROM pgr_bipartite(
    $$SELECT id,source,target,cost,reverse_cost FROM edge_table WHERE id IN (8, 10, 11, 12)$$
);
\echo --q3
SELECT * FROM pgr_bipartite(
    $$SELECT id,source,target,cost,reverse_cost FROM five_vertices_table$$
);
\echo --q4
