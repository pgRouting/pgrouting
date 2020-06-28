\i setup.sql
SELECT plan(1);

SELECT todo_start('Complete this tests');

SELECT pass('Sample Test');

SELECT todo_end();

SELECT finish();
/*
SELECT plan(7);


SELECT has_function('pgr_dltdtree');

SELECT has_function('pgr_dltdtree', ARRAY['text','bigint']);
SELECT function_returns('pgr_dltdtree', ARRAY['text','bigint','bigint'],  'setof record');

-- pgr_dltdtree
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dltdtree'$$,
    $$SELECT  '{"","","max_depth","directed","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_dltdtree'$$,
    $$VALUES
        ('{25,20,20,16,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,16,20,20,20,20,20,701,701}'::OID[])
    $$
);

SELECT * FROM finish();

 */
ROLLBACK;