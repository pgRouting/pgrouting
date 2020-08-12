\i setup.sql

SELECT plan(4);

SELECT has_function('pgr_isplanar');

SELECT function_returns('pgr_isplanar', ARRAY['text'], 'boolean');

-- pgr_isplanar
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_isplanar'$$,
    $$SELECT  NULL::TEXT[] $$
);
-- parameter types
SELECT bag_has(
    $$SELECT  prorettype from pg_proc where proname = 'pgr_isplanar'$$,
    $$VALUES ( '16'::oid )$$
);

SELECT * FROM finish();
ROLLBACK;
