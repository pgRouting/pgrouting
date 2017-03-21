\echo --q1
SET client_min_messages TO WARNING;
SELECT pgr_labelGraph('edge_table', 'id', 'source', 'target', 'subgraph');
SELECT subgraph, count(*) FROM edge_table group by subgraph;
\echo --q2
