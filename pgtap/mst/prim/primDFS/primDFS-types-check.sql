\i setup.sql

SELECT plan(7);

----------------------------------
-- tests for all
-- prefix:  pgr_prim
----------------------------------

SELECT has_function('pgr_primdfs');

SELECT has_function('pgr_primdfs', ARRAY['text','bigint','bigint']);
SELECT has_function('pgr_primdfs', ARRAY['text','anyarray','bigint']);
SELECT function_returns('pgr_primdfs', ARRAY['text','bigint','bigint'],  'setof record');
SELECT function_returns('pgr_primdfs', ARRAY['text','anyarray','bigint'],  'setof record');


-- pgr_primdfs
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_primdfs'$$,
    $$VALUES
        ('{"","","max_depth","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_primdfs'$$,
    $$VALUES
        ('{25,20,20,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,20,20,20,20,20,701,701}'::OID[])
    $$
);

SELECT * FROM finish();
ROLLBACK;
