\i setup.sql

SELECT plan(143);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_bellmanFord', ARRAY['text','bigint','bigint','boolean']);
SELECT has_function('pgr_bellmanFord', ARRAY['text','bigint','anyarray','boolean']);
SELECT has_function('pgr_bellmanFord', ARRAY['text','anyarray','bigint','boolean']);
SELECT has_function('pgr_bellmanFord', ARRAY['text','anyarray','anyarray','boolean']);

SELECT function_returns('pgr_bellmanFord', ARRAY['text','bigint','bigint','boolean'],'setof record');
SELECT function_returns('pgr_bellmanFord', ARRAY['text','bigint','anyarray','boolean'],'setof record');
SELECT function_returns('pgr_bellmanFord', ARRAY['text','anyarray','bigint','boolean'],'setof record');
SELECT function_returns('pgr_bellmanFord', ARRAY['text','anyarray','anyarray','boolean'],'setof record');

SELECT style_dijkstra('pgr_bellmanFord', ', 2, 3)');
SELECT style_dijkstra('pgr_bellmanFord', ', 2, 3, true)');
SELECT style_dijkstra('pgr_bellmanFord', ', 2, 3, false)');



SELECT finish();
ROLLBACK;
