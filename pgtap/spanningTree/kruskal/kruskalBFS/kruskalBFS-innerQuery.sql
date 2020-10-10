\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(54);

SELECT style_dijkstra('pgr_kruskalBFS', ', 5)');

SELECT finish();
ROLLBACK;
