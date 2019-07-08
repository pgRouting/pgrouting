\i setup.sql

SELECT plan(6);

SELECT has_function('pgr_chinesepostmancost');
SELECT has_function('pgr_chinesepostmancost',    ARRAY['text']);
SELECT function_returns('pgr_chinesepostmancost', ARRAY['text'], 'double precision');

-- pgr_kruskal
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_chinesepostmancost'$$,
    $$SELECT  NULL::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  prorettype from pg_proc where proname = 'pgr_chinesepostmancost'$$,
    $$SELECT  701 $$
);

SELECT set_eq(
    $$SELECT  proargtypes from pg_proc where proname = 'pgr_chinesepostmancost'$$,
    $$SELECT  '[0:0]={25}'::OID[] $$
);


SELECT * FROM finish();
ROLLBACK;
