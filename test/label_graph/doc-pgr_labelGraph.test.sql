\echo --q1
SET client_min_messages TO WARNING;
SELECT pgr_labelGraph('edge_table', 'id', 'source', 'target', 'subgraph');
SELECT DISTINCT subgraph FROM edge_table ORDER BY subgraph;
\echo --q2
