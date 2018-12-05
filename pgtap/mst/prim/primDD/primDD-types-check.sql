\i setup.sql

SELECT plan(11);

----------------------------------
-- tests for all
-- prefix:  pgr_prim
----------------------------------

SELECT has_function('pgr_primdd');

SELECT has_function('pgr_primdd',  ARRAY['text','bigint','numeric']);
SELECT has_function('pgr_primdd',  ARRAY['text','anyarray','numeric']);
SELECT has_function('pgr_primdd',  ARRAY['text','bigint','double precision']);
SELECT has_function('pgr_primdd',  ARRAY['text','anyarray','double precision']);
SELECT function_returns('pgr_primdd',  ARRAY['text','bigint','numeric'],  'setof record');
SELECT function_returns('pgr_primdd',  ARRAY['text','anyarray','numeric'],  'setof record');
SELECT function_returns('pgr_primdd',  ARRAY['text','bigint','double precision'],  'setof record');
SELECT function_returns('pgr_primdd',  ARRAY['text','anyarray','double precision'],  'setof record');


-- pgr_primdd
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_primdd'$$,
    $$VALUES
        ('{"","","","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_primdd'$$,
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
