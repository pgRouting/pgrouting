
SELECT plan(10);

SELECT has_function('pgrBellmanFord');

SELECT has_function('pgrBellmanFord', ARRAY['text','bigint','bigint','boolean']);
SELECT has_function('pgrBellmanFord', ARRAY['text','bigint','anyarray','boolean']);
SELECT has_function('pgrBellmanFord', ARRAY['text','anyarray','bigint','boolean']);
SELECT has_function('pgrBellmanFord', ARRAY['text','anyarray','anyarray','boolean']);

SELECT function_returns('pgrBellmanFord', ARRAY['text','bigint','bigint','boolean'],'setof record');
SELECT function_returns('pgrBellmanFord', ARRAY['text','bigint','anyarray','boolean'],'setof record');
SELECT function_returns('pgrBellmanFord', ARRAY['text','anyarray','bigint','boolean'],'setof record');
SELECT function_returns('pgrBellmanFord', ARRAY['text','anyarray','anyarray','boolean'],'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgrBellmanFord'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

