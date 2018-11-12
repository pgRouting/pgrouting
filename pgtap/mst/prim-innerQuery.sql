\i setup.sql

SELECT plan(180);

SELECT style_dijkstra('pgr_prim', ')');
SELECT todo_start();
SELECT style_dijkstra('pgr_primDFS', ', 5)');
SELECT style_dijkstra('pgr_primBFS', ', 5)');
SELECT style_dijkstra('pgr_primDD', ', 5, 3.5)');
SELECT todo_end();

SELECT finish();
ROLLBACK;
