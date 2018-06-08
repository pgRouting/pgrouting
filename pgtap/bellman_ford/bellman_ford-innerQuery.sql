\i setup.sql

SELECT plan(143);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_bellman_ford', ARRAY['text','bigint','bigint','boolean']);
SELECT has_function('pgr_bellman_ford', ARRAY['text','bigint','anyarray','boolean']);
SELECT has_function('pgr_bellman_ford', ARRAY['text','anyarray','bigint','boolean']);
SELECT has_function('pgr_bellman_ford', ARRAY['text','anyarray','anyarray','boolean']);

SELECT function_returns('pgr_bellman_ford', ARRAY['text','bigint','bigint','boolean'],'setof record');
SELECT function_returns('pgr_bellman_ford', ARRAY['text','bigint','anyarray','boolean'],'setof record');
SELECT function_returns('pgr_bellman_ford', ARRAY['text','anyarray','bigint','boolean'],'setof record');
SELECT function_returns('pgr_bellman_ford', ARRAY['text','anyarray','anyarray','boolean'],'setof record');

-- ONE TO ONE
SELECT style_bellman_ford('pgr_bellman_ford', ', 2, 3, true)');
-- ONE TO MANY
SELECT style_bellman_ford('pgr_bellman_ford', ', 2, ARRAY[3], true)');
-- MANY TO ONE
SELECT style_bellman_ford('pgr_bellman_ford', ', ARRAY[2], 3, true)');
-- MANY TO MANY
SELECT style_bellman_ford('pgr_bellman_ford', ', ARRAY[2], ARRAY[3], true)');


SELECT finish();
ROLLBACK;
