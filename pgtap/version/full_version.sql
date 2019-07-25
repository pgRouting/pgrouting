
\i setup.sql

SELECT plan(5);


SELECT has_function('pgr_full_version');
SELECT has_function('pgr_full_version', ARRAY[]::text[]);
SELECT function_returns('pgr_full_version', ARRAY[]::text[], 'record');

SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_full_version'$$,
    $$SELECT  '{"version","build_type","compile_date","library","system","postgresql","compiler","boost","hash"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_full_version'$$,
    $$SELECT  '{25,25,25,25,25,25,25,25,25}'::OID[] $$
);


SELECT finish();
ROLLBACK;
