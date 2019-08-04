
SELECT plan(9);

SELECT has_function('pgr_bellmanford');

SELECT has_function('pgr_bellmanford', ARRAY['text','bigint','bigint','boolean']);
SELECT has_function('pgr_bellmanford', ARRAY['text','bigint','anyarray','boolean']);
SELECT has_function('pgr_bellmanford', ARRAY['text','anyarray','bigint','boolean']);
SELECT has_function('pgr_bellmanford', ARRAY['text','anyarray','anyarray','boolean']);

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


