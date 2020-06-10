\i setup.sql

SELECT plan(432);

SELECT todo_start('Complete the inner query tests');

-- SINGLE VERTEX
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5)');
-- Single vertex with depth
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, 2)');
-- Single vertex with directed parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, directed := true)');
-- Single vertex with depth and directed parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, 2, directed := true)');

-- MULTIPLE VERTICES
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5])');
-- Multiple vertices with depth
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], 2)');
-- Multiple vertices with directed parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], directed := true)');
-- Multiple vertices with depth and directed parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], 2, directed := true)');

SELECT todo_end();

SELECT finish();
ROLLBACK;
