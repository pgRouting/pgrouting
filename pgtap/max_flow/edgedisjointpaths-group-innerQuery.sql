\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(658);


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

-- new signature on 3.2
SELECT CASE
WHEN NOT min_version('3.2.0') THEN
  skip(2, 'Combinations functiontionality new on 3.2.0')
ELSE
  collect_tap(
    has_function('pgr_edgedisjointpaths', ARRAY['text', 'text', 'boolean']),
    function_returns('pgr_edgedisjointpaths', ARRAY['text', 'text', 'boolean'], 'setof record')
  )
END;

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
