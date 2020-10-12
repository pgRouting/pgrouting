\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(7);

----------------------------------
-- tests for all
-- prefix:  pgr_kruskal
----------------------------------

SELECT has_function('pgr_kruskalbfs');

SELECT has_function('pgr_kruskalbfs', ARRAY['text','bigint','bigint']);
SELECT has_function('pgr_kruskalbfs', ARRAY['text','anyarray','bigint']);
SELECT function_returns('pgr_kruskalbfs', ARRAY['text','bigint','bigint'],  'setof record');
SELECT function_returns('pgr_kruskalbfs', ARRAY['text','anyarray','bigint'],  'setof record');

-- pgr_kruskalbfs
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskalbfs'$$,
    $$VALUES
        ('{"","","max_depth","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskalbfs'$$,
    $$VALUES
        ('{25,20,20,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,20,20,20,20,20,701,701}'::OID[])
    $$
);

SELECT * FROM finish();
ROLLBACK;
