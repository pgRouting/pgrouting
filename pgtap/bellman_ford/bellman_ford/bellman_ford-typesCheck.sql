
UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(14);

SELECT has_function('pgr_bellmanford');

SELECT has_function('pgr_bellmanford', ARRAY['text','bigint','bigint','boolean']);
SELECT has_function('pgr_bellmanford', ARRAY['text','bigint','anyarray','boolean']);
SELECT has_function('pgr_bellmanford', ARRAY['text','anyarray','bigint','boolean']);
SELECT has_function('pgr_bellmanford', ARRAY['text','anyarray','anyarray','boolean']);
SELECT function_returns('pgr_bellmanford', ARRAY['text','bigint','bigint','boolean'], 'setof record');
SELECT function_returns('pgr_bellmanford', ARRAY['text','bigint','anyarray','boolean'], 'setof record');
SELECT function_returns('pgr_bellmanford', ARRAY['text','anyarray','bigint','boolean'], 'setof record');
SELECT function_returns('pgr_bellmanford', ARRAY['text','anyarray','anyarray','boolean'], 'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_bellmanford'$$,
    $$VALUES
        ('{25,20,20,16,23,23,20,20,701,701}'::OID[]),
        ('{25,20,2277,16,23,23,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,16,23,23,20,20,20,701,701}'::OID[]),
        ('{25,2277,2277,16,23,23,20,20,20,20,701,701}'::OID[])
    $$
);
