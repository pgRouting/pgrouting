\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(378);
SET client_min_messages TO ERROR;


SELECT style_dijkstra('pgr_binaryBreadthFirstSearch', ', 2, 3)');
SELECT style_dijkstra('pgr_binaryBreadthFirstSearch', ', 2, 3, true)');
SELECT style_dijkstra('pgr_binaryBreadthFirstSearch', ', 2, 3, false)');

-- ONE TO ONE
SELECT style_dijkstra('pgr_binaryBreadthFirstSearch', ', 2, 3, true)');
-- ONE TO MANY
SELECT style_dijkstra('pgr_binaryBreadthFirstSearch', ', 2, ARRAY[3], true)');
-- MANY TO ONE
SELECT style_dijkstra('pgr_binaryBreadthFirstSearch', ', ARRAY[2], 3, true)');
-- MANY TO MANY
SELECT style_dijkstra('pgr_binaryBreadthFirstSearch', ', ARRAY[2], ARRAY[3], true)');


SELECT finish();
ROLLBACK;