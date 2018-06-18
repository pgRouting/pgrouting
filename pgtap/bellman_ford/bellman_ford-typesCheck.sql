
SELECT plan(10);

SELECT has_function('pgr_bellmanFord');

SELECT has_function('pgr_bellmanFord', ARRAY['text','bigint','bigint','boolean']);
SELECT has_function('pgr_bellmanFord', ARRAY['text','bigint','anyarray','boolean']);
SELECT has_function('pgr_bellmanFord', ARRAY['text','anyarray','bigint','boolean']);
SELECT has_function('pgr_bellmanFord', ARRAY['text','anyarray','anyarray','boolean']);

SELECT function_returns('pgr_bellmanFord', ARRAY['text','bigint','bigint','boolean'],'setof record');
SELECT function_returns('pgr_bellmanFord', ARRAY['text','bigint','anyarray','boolean'],'setof record');
SELECT function_returns('pgr_bellmanFord', ARRAY['text','anyarray','bigint','boolean'],'setof record');
SELECT function_returns('pgr_bellmanFord', ARRAY['text','anyarray','anyarray','boolean'],'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanFord'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

