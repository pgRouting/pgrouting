\i setup.sql

SELECT plan(9);

SELECT has_function('pgr_kruskal');
SELECT has_function('pgr_kruskal', ARRAY['text','text']);
SELECT has_function('pgr_kruskal', ARRAY['text','bigint','text']);

SELECT function_returns('pgr_kruskal', ARRAY['text','text'], 'setof record');
SELECT function_returns('pgr_kruskal', ARRAY['text','bigint','text'], 'setof record');

-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{"","order_by","seq","depth","node","edge","cost","agg_cost"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{"","order_by","seq","depth","node","edge","cost","agg_cost"}'::TEXT[] $$
);

-- parameter types
SELECT bag_has(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{25,25,23,20,20,20,701,701}'::OID[] $$
);

SELECT bag_has(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{25,20,25,23,20,20,20,701,701}'::OID[] $$
);


SELECT * FROM finish();
ROLLBACK;
