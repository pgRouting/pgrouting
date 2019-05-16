\i setup.sql

SELECT plan(5);

----------------------------------
-- tests for all
-- prefix:  pgr_prim
----------------------------------

SELECT has_function('pgr_prim');
SELECT has_function('pgr_prim',    ARRAY['text']);
SELECT function_returns('pgr_prim', ARRAY['text'], 'setof record');

-- pgr_prim
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_prim'$$,
    $$SELECT  '{"","edge","cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_prim'$$,
    $$SELECT  '{25,20,701}'::OID[] $$
);


SELECT * FROM finish();
ROLLBACK;
