
\i setup.sql

SELECT plan(5);


SELECT has_function('pgr_version');
SELECT has_function('pgr_version', ARRAY[]::text[]);
SELECT function_returns('pgr_version', ARRAY[]::text[], 'text');

SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_version'$$,
    $$SELECT  NULL::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_version'$$,
    $$SELECT  NULL::OID[] $$
);


SELECT finish();
ROLLBACK;
