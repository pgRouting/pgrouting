\i setup.sql

SELECT plan(14);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_binarybreadthfirstsearch');


SELECT has_function('pgr_binarybreadthfirstsearch', ARRAY['text','bigint','bigint','boolean']);
SELECT has_function('pgr_binarybreadthfirstsearch', ARRAY['text','bigint','anyarray','boolean']);
SELECT has_function('pgr_binarybreadthfirstsearch', ARRAY['text','anyarray','bigint','boolean']);
SELECT has_function('pgr_binarybreadthfirstsearch', ARRAY['text','anyarray','anyarray','boolean']);
SELECT function_returns('pgr_binarybreadthfirstsearch', ARRAY['text','bigint','bigint','boolean'],  'setof record');
SELECT function_returns('pgr_binarybreadthfirstsearch', ARRAY['text','bigint','anyarray','boolean'],  'setof record');
SELECT function_returns('pgr_binarybreadthfirstsearch', ARRAY['text','anyarray','bigint','boolean'],  'setof record');
SELECT function_returns('pgr_binarybreadthfirstsearch', ARRAY['text','anyarray','anyarray','boolean'],  'setof record');

-- pgr_binarybreadthfirstsearch
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_binarybreadthfirstsearch'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_binarybreadthfirstsearch'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_binarybreadthfirstsearch'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_binarybreadthfirstsearch'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_binarybreadthfirstsearch'$$,
    $$VALUES
        ('{25,20,20,16,23,23,20,20,701,701}'::OID[]),
        ('{25,20,2277,16,23,23,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,16,23,23,20,20,20,701,701}'::OID[]),
        ('{25,2277,2277,16,23,23,20,20,20,20,701,701}'::OID[])
    $$
);


SELECT * FROM finish();
ROLLBACK;
