\i setup.sql

SELECT plan(26);

----------------------------------
-- tests for all
-- prefix:  pgr_kruskal
----------------------------------

SELECT has_function('pgr_kruskal');
SELECT has_function('pgr_kruskaldfs');
SELECT has_function('pgr_kruskalbfs');
SELECT has_function('pgr_kruskaldd');

SELECT has_function('pgr_kruskal',    ARRAY['text']);
SELECT has_function('pgr_kruskaldfs', ARRAY['text','bigint','bigint']);
SELECT has_function('pgr_kruskalbfs', ARRAY['text','bigint','bigint']);
SELECT has_function('pgr_kruskaldd',  ARRAY['text','bigint','double precision']);
SELECT has_function('pgr_kruskaldfs', ARRAY['text','anyarray','bigint']);
SELECT has_function('pgr_kruskalbfs', ARRAY['text','anyarray','bigint']);
SELECT has_function('pgr_kruskaldd',  ARRAY['text','anyarray','double precision']);

SELECT function_returns('pgr_kruskal', ARRAY['text'], 'setof record');
SELECT function_returns('pgr_kruskaldfs', ARRAY['text','bigint','bigint'],  'setof record');
SELECT function_returns('pgr_kruskalbfs', ARRAY['text','bigint','bigint'],  'setof record');
SELECT function_returns('pgr_kruskaldd',  ARRAY['text','bigint','double precision'], 'setof record');
SELECT function_returns('pgr_kruskaldfs', ARRAY['text','anyarray','bigint'],  'setof record');
SELECT function_returns('pgr_kruskalbfs', ARRAY['text','anyarray','bigint'],  'setof record');
SELECT function_returns('pgr_kruskaldd',  ARRAY['text','anyarray','double precision'], 'setof record');

-- pgr_kruskal
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{"","seq","edge","cost"}'::TEXT[] $$
);

-- parameter types
SELECT bag_has(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{25,20,20,701}'::OID[] $$
);


-- pgr_kruskaldfs
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskaldfs'$$,
    $$VALUES
        ('{"","","max_depth","seq","start_vid","depth","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","max_depth","seq","start_vid","depth","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT bag_has(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskaldfs'$$,
    $$VALUES
        ('{25,20,20,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,20,20,20,20,20,701,701}'::OID[])
    $$
);


-- pgr_kruskalbfs
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskalbfs'$$,
    $$VALUES
        ('{"","","max_depth","seq","start_vid","depth","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","max_depth","seq","start_vid","depth","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT bag_has(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskalbfs'$$,
    $$VALUES
        ('{25,20,20,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,20,20,20,20,20,701,701}'::OID[])
    $$
);


-- pgr_kruskaldd
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskaldd'$$,
    $$VALUES
        ('{"","","","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT bag_has(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskaldd'$$,
    $$VALUES
        ('{25,20,701,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,701,20,20,20,20,20,701,701}'::OID[])
    $$
);


SELECT * FROM finish();
ROLLBACK;
