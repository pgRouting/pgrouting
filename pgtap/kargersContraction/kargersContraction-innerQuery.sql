\i setup.sql

SELECT plan(432);

SELECT style_dijkstra('pgr_kargersContraction', ', 5)');
SELECT style_dijkstra('pgr_kargersContraction', ', 5, 2)');
SELECT style_dijkstra('pgr_kargersContraction', ', ARRAY[3,5])');
SELECT style_dijkstra('pgr_kargersContraction', ', ARRAY[3,5], 2)');
SELECT style_dijkstra('pgr_kargersContraction', ', 5, directed := true)');
SELECT style_dijkstra('pgr_kargersContraction', ', 5, 2, directed := true)');
SELECT style_dijkstra('pgr_kargersContraction', ', ARRAY[3,5], directed := true)');
SELECT style_dijkstra('pgr_kargersContraction', ', ARRAY[3,5],2, directed := true)');

SELECT finish();
ROLLBACK;
