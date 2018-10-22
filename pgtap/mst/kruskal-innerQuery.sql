\i setup.sql

SELECT plan(180);

SELECT style_dijkstra('pgr_kruskal', ')');
SELECT style_dijkstra('pgr_kruskalDFS', ', 5)');
SELECT style_dijkstra('pgr_kruskalBFS', ', 5)');
SELECT style_dijkstra('pgr_kruskalDD', ', 5, 3.5)');

SELECT finish();
ROLLBACK;
