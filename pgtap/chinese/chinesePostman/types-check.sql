\i setup.sql

SELECT plan(5);

SELECT has_function('pgr_chinesepostman');
SELECT has_function('pgr_chinesepostman',    ARRAY['text']);
SELECT function_returns('pgr_chinesepostman', ARRAY['text'], 'setof record');

-- pgr_chinesepostman
-- parameter names
SELECT set_eq(
    $$SELECT proargnames from pg_proc where proname = 'pgr_chinesepostman'$$,
    $$SELECT '{"", "seq", "node", "edge", "cost", "agg_cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_chinesepostman'$$,
    $$SELECT  '{25,23,20,20,701,701}'::OID[] $$
);

SELECT * FROM finish();
ROLLBACK;
