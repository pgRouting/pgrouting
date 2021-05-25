\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(432);

-- SINGLE VERTEX
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5)');
-- Single vertex with directed parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, true)');
-- Single vertex with max_depth parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, max_depth => 2)');
-- Single vertex with directed and max_depth parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', 5, true, max_depth => 2)');

-- MULTIPLE VERTICES
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5])');
-- Multiple vertices with directed parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], true)');
-- Multiple vertices with max_depth parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], max_depth => 2)');
-- Single vertex with directed and max_depth parameter
SELECT style_dijkstra('pgr_depthFirstSearch', ', ARRAY[3,5], true, max_depth => 2)');

SELECT finish();
ROLLBACK;
