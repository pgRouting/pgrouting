\i setup.sql

SELECT plan(548);


SELECT has_function('pgr_edgedisjointpaths',
    ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_edgedisjointpaths',
    ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_edgedisjointpaths',
    ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_edgedisjointpaths',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean']);

SELECT function_returns('pgr_edgedisjointpaths',
    ARRAY['text', 'bigint', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_edgedisjointpaths',
    ARRAY['text', 'bigint', 'anyarray', 'boolean'],
    'setof record');
SELECT function_returns('pgr_edgedisjointpaths',
    ARRAY['text', 'anyarray', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_edgedisjointpaths',
    ARRAY['text', 'anyarray', 'anyarray', 'boolean'],
    'setof record');


-- DIRECTED
SELECT style_dijkstra('pgr_edgedisjointpaths', ', 2, 3)');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', 2, ARRAY[3])');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', ARRAY[2], 3)');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', ARRAY[2], ARRAY[3])');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', 2, 3, true)');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', 2, ARRAY[3], true)');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', ARRAY[2], 3, true)');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', ARRAY[2], ARRAY[3], true)');
-- UNDIRECTED
SELECT style_dijkstra('pgr_edgedisjointpaths', ', 2, 3, false)');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', 2, ARRAY[3], false)');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', ARRAY[2], 3, false)');
SELECT style_dijkstra('pgr_edgedisjointpaths', ', ARRAY[2], ARRAY[3], false)');

SELECT finish();
ROLLBACK;
