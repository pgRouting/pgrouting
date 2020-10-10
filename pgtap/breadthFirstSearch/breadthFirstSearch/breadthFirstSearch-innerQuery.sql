\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(432);

SELECT style_dijkstra('pgr_breadthFirstSearch', ', 5)');
SELECT style_dijkstra('pgr_breadthFirstSearch', ', 5, 2)');
SELECT style_dijkstra('pgr_breadthFirstSearch', ', ARRAY[3,5])');
SELECT style_dijkstra('pgr_breadthFirstSearch', ', ARRAY[3,5], 2)');
SELECT style_dijkstra('pgr_breadthFirstSearch', ', 5, directed := true)');
SELECT style_dijkstra('pgr_breadthFirstSearch', ', 5, 2, directed := true)');
SELECT style_dijkstra('pgr_breadthFirstSearch', ', ARRAY[3,5], directed := true)');
SELECT style_dijkstra('pgr_breadthFirstSearch', ', ARRAY[3,5],2, directed := true)');

SELECT finish();
ROLLBACK;
