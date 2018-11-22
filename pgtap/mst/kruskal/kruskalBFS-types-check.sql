\i setup.sql

SELECT plan(30);

----------------------------------
-- tests for all
-- prefix:  pgr_kruskal
----------------------------------

SELECT has_function('pgr_kruskal');
SELECT has_function('pgr_kruskaldfs');
SELECT has_function('pgr_kruskalbfs');
SELECT has_function('pgr_kruskaldd');

SELECT has_function('pgr_kruskal',    ARRAY['text']);
SELECT function_returns('pgr_kruskal', ARRAY['text'], 'setof record');

SELECT has_function('pgr_kruskaldfs', ARRAY['text','bigint','bigint']);
SELECT has_function('pgr_kruskaldfs', ARRAY['text','anyarray','bigint']);
SELECT function_returns('pgr_kruskaldfs', ARRAY['text','bigint','bigint'],  'setof record');
SELECT function_returns('pgr_kruskaldfs', ARRAY['text','anyarray','bigint'],  'setof record');

SELECT has_function('pgr_kruskalbfs', ARRAY['text','bigint','bigint']);
SELECT has_function('pgr_kruskalbfs', ARRAY['text','anyarray','bigint']);
SELECT function_returns('pgr_kruskalbfs', ARRAY['text','bigint','bigint'],  'setof record');
SELECT function_returns('pgr_kruskalbfs', ARRAY['text','anyarray','bigint'],  'setof record');

SELECT has_function('pgr_kruskaldd',  ARRAY['text','bigint','numeric']);
SELECT has_function('pgr_kruskaldd',  ARRAY['text','anyarray','numeric']);
SELECT has_function('pgr_kruskaldd',  ARRAY['text','bigint','double precision']);
SELECT has_function('pgr_kruskaldd',  ARRAY['text','anyarray','double precision']);
SELECT function_returns('pgr_kruskaldd',  ARRAY['text','bigint','numeric'],  'setof record');
SELECT function_returns('pgr_kruskaldd',  ARRAY['text','anyarray','numeric'],  'setof record');
SELECT function_returns('pgr_kruskaldd',  ARRAY['text','bigint','double precision'],  'setof record');
SELECT function_returns('pgr_kruskaldd',  ARRAY['text','anyarray','double precision'],  'setof record');


-- pgr_kruskal
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{"","seq","edge","cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{25,20,20,701}'::OID[] $$
);


-- pgr_kruskaldfs
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskaldfs'$$,
    $$VALUES
        ('{"","","max_depth","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskaldfs'$$,
    $$VALUES
        ('{25,20,20,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,20,20,20,20,20,701,701}'::OID[])
    $$
);


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


-- pgr_kruskaldd
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskaldd'$$,
    $$VALUES
        ('{"","","","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskaldd'$$,
    $$VALUES
        ('{25,20,701,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,701,20,20,20,20,20,701,701}'::OID[]),
        ('{25,20,1700,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,1700,20,20,20,20,20,701,701}'::OID[]),
        ('{25,20,701,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,701,20,20,20,20,20,701,701}'::OID[])
    $$
);


SELECT * FROM finish();
ROLLBACK;
