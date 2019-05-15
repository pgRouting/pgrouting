\i setup.sql

SELECT plan(5);

----------------------------------
-- tests for all
-- prefix:  pgr_kruskal
----------------------------------

SELECT has_function('pgr_kruskal');
SELECT has_function('pgr_kruskal',    ARRAY['text']);
SELECT function_returns('pgr_kruskal', ARRAY['text'], 'setof record');

-- pgr_kruskal
-- parameter names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{"","edge","cost"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_kruskal'$$,
    $$SELECT  '{25,20,701}'::OID[] $$
);


SELECT * FROM finish();
ROLLBACK;
