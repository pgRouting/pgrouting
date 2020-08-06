\i setup.sql

SELECT plan(4);

SELECT has_function('pgr_isplanar');

SELECT function_returns('pgr_isplanar', ARRAY['text'], 'boolean');


-- pgr_isplanar
-- parameter names
SELECT todo_start('Fix types_check');

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_isplanar'$$,
    $$SELECT  '{}'::TEXT[] $$
);
-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_isplanar'$$,
    $$VALUES
        ('{}'::OID[])
    $$
);

SELECT todo_end();



SELECT * FROM finish();
ROLLBACK;
