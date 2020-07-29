\i setup.sql

SELECT plan(7);


SELECT has_function('pgr_depthfirstsearch');

SELECT has_function('pgr_depthfirstsearch', ARRAY['text','bigint','boolean','bigint']);
SELECT has_function('pgr_depthfirstsearch', ARRAY['text','anyarray','boolean','bigint']);
SELECT function_returns('pgr_depthfirstsearch', ARRAY['text','bigint','boolean','bigint'],  'setof record');
SELECT function_returns('pgr_depthfirstsearch', ARRAY['text','anyarray','boolean','bigint'],  'setof record');

-- pgr_depthfirstsearch
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_depthfirstsearch'$$,
    $$SELECT  '{"","","directed","max_depth","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_depthfirstsearch'$$,
    $$VALUES
        ('{25,20,16,20,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,16,20,20,20,20,20,20,701,701}'::OID[])
    $$
);

SELECT * FROM finish();
ROLLBACK;
