
\i setup.sql

SELECT plan(9);

SELECT has_function('pgr_bellmanford');

SELECT todo_start('not implemented yet');
SELECT function_returns('pgr_bellmanford', ARRAY['text','text','bigint','bigint','boolean'],'setof record');
SELECT function_returns('pgr_bellmanford', ARRAY['text','text','bigint','anyarray','boolean'],'setof record');
SELECT function_returns('pgr_bellmanford', ARRAY['text','text','anyarray','bigint','boolean'],'setof record');
SELECT function_returns('pgr_bellmanford', ARRAY['text','text','anyarray','anyarray','boolean'],'setof record');

-- testing column names on the negative edges function
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","","directed","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","","directed","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","","directed","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","","directed","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
