\i setup.sql

SELECT plan(143);
SET client_min_messages TO ERROR;


SELECT has_function('pgrBellmanFord', ARRAY['text','bigint','bigint','boolean']);
SELECT has_function('pgrBellmanFord', ARRAY['text','bigint','anyarray','boolean']);
SELECT has_function('pgrBellmanFord', ARRAY['text','anyarray','bigint','boolean']);
SELECT has_function('pgrBellmanFord', ARRAY['text','anyarray','anyarray','boolean']);

SELECT function_returns('pgrBellmanFord', ARRAY['text','bigint','bigint','boolean'],'setof record');
SELECT function_returns('pgrBellmanFord', ARRAY['text','bigint','anyarray','boolean'],'setof record');
SELECT function_returns('pgrBellmanFord', ARRAY['text','anyarray','bigint','boolean'],'setof record');
SELECT function_returns('pgrBellmanFord', ARRAY['text','anyarray','anyarray','boolean'],'setof record');

SELECT style_dijkstra('pgrBellmanFord', ', 2, 3)');
SELECT style_dijkstra('pgrBellmanFord', ', 2, 3, true)');
SELECT style_dijkstra('pgrBellmanFord', ', 2, 3, false)');



SELECT finish();
ROLLBACK;
