\i setup.sql

SELECT plan(270);

SELECT style_dijkstra('pgr_kruskal', ')');
SELECT style_dijkstra('pgr_kruskal', ', $$bfs$$)');
SELECT style_dijkstra('pgr_kruskal', ', $$dfs$$)');

SELECT style_dijkstra('pgr_kruskal', ', 5)');
SELECT style_dijkstra('pgr_kruskal', ', 5, $$bfs$$)');
SELECT style_dijkstra('pgr_kruskal', ', 5, $$dfs$$)');

SELECT finish();
ROLLBACK;
