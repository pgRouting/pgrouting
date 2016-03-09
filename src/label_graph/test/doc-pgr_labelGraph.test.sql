BEGIN;
SET client_min_messages TO ERROR;
\echo --q1
SELECT pgr_labelGraph('edge_table', 'id', 'source', 'target', 'subgraph');
SELECT subgraph, count(*) FROM edge_table group by subgraph;
\echo --q2
SET client_min_messages TO DEBUG;
ROLLBACK;
