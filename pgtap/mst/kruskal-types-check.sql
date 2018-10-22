\i setup.sql

SELECT plan(5);

SELECT has_function('pgr_kruskal');
SELECT has_function('pgr_kruskal', ARRAY['text']);

SELECT function_returns('pgr_kruskal', ARRAY['text'], 'setof record');

-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{"","seq","edge","cost"}'::TEXT[] $$
);

-- parameter types
SELECT bag_has(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{25,23,20,701}'::OID[] $$
);


SELECT * FROM finish();
ROLLBACK;
