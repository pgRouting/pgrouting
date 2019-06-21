\i setup.sql

SELECT plan(360);

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
