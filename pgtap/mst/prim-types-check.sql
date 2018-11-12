\i setup.sql

SELECT plan(30);

----------------------------------
-- tests for all
-- prefix:  pgr_prim
----------------------------------

SELECT has_function('pgr_prim');
SELECT has_function('pgr_prim',    ARRAY['text']);
SELECT function_returns('pgr_prim', ARRAY['text'], 'setof record');

SELECT todo_start();
SELECT has_function('pgr_primdfs');
SELECT has_function('pgr_primdfs', ARRAY['text','bigint','bigint']);
SELECT has_function('pgr_primdfs', ARRAY['text','anyarray','bigint']);
SELECT function_returns('pgr_primdfs', ARRAY['text','bigint','bigint'],  'setof record');
SELECT function_returns('pgr_primdfs', ARRAY['text','anyarray','bigint'],  'setof record');

SELECT has_function('pgr_primbfs');
SELECT has_function('pgr_primbfs', ARRAY['text','bigint','bigint']);
SELECT has_function('pgr_primbfs', ARRAY['text','anyarray','bigint']);
SELECT function_returns('pgr_primbfs', ARRAY['text','bigint','bigint'],  'setof record');
SELECT function_returns('pgr_primbfs', ARRAY['text','anyarray','bigint'],  'setof record');

SELECT has_function('pgr_primdd');
SELECT has_function('pgr_primdd',  ARRAY['text','bigint','numeric']);
SELECT has_function('pgr_primdd',  ARRAY['text','anyarray','numeric']);
SELECT has_function('pgr_primdd',  ARRAY['text','bigint','double precision']);
SELECT has_function('pgr_primdd',  ARRAY['text','anyarray','double precision']);
SELECT function_returns('pgr_primdd',  ARRAY['text','bigint','numeric'],  'setof record');
SELECT function_returns('pgr_primdd',  ARRAY['text','anyarray','numeric'],  'setof record');
SELECT function_returns('pgr_primdd',  ARRAY['text','bigint','double precision'],  'setof record');
SELECT function_returns('pgr_primdd',  ARRAY['text','anyarray','double precision'],  'setof record');


-- pgr_prim
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_prim'$$,
    $$SELECT  '{"","seq","edge","cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_prim'$$,
    $$SELECT  '{25,20,20,701}'::OID[] $$
);


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


-- pgr_primbfs
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_primbfs'$$,
    $$VALUES
        ('{"","","max_depth","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_primbfs'$$,
    $$VALUES
        ('{25,20,20,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,20,20,20,20,20,701,701}'::OID[])
    $$
);


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
SELECT todo_end();


SELECT * FROM finish();
ROLLBACK;
