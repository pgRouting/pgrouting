\echo --q1
select * from pgr_bipartite($$select id,source,target,cost,reverse_cost from edge_table$$);
\echo --q2
