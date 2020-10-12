\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(656);

SELECT has_function('pgr_bddijkstra', ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_bddijkstra', ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_bddijkstra', ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_bddijkstra', ARRAY['text', 'anyarray', 'anyarray', 'boolean']);

SELECT function_returns('pgr_bddijkstra', ARRAY['text', 'bigint', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_bddijkstra', ARRAY['text', 'bigint', 'anyarray', 'boolean'],
    'setof record');
SELECT function_returns('pgr_bddijkstra', ARRAY['text', 'anyarray', 'bigint', 'boolean'],
    'setof record');
SELECT function_returns('pgr_bddijkstra', ARRAY['text', 'anyarray', 'anyarray', 'boolean'],
    'setof record');



SELECT style_dijkstra('pgr_bddijkstra', ', 2, 3, true)');
SELECT style_dijkstra('pgr_bddijkstra', ', 2, ARRAY[3], true)');
SELECT style_dijkstra('pgr_bddijkstra', ',  2, ARRAY[3], true)');
SELECT style_dijkstra('pgr_bddijkstra', ',  ARRAY[2], ARRAY[3], true)');

SELECT style_dijkstra('pgr_bddijkstra', ', 2, 3)');
SELECT style_dijkstra('pgr_bddijkstra', ', 2, ARRAY[3])');
SELECT style_dijkstra('pgr_bddijkstra', ',  2, ARRAY[3])');
SELECT style_dijkstra('pgr_bddijkstra', ',  ARRAY[2], ARRAY[3])');

SELECT style_dijkstra('pgr_bddijkstra', ', 2, 3, false)');
SELECT style_dijkstra('pgr_bddijkstra', ', 2, ARRAY[3], false)');
SELECT style_dijkstra('pgr_bddijkstra', ',  2, ARRAY[3], false)');
SELECT style_dijkstra('pgr_bddijkstra', ',  ARRAY[2], ARRAY[3], false)');

SELECT finish();
ROLLBACK;
