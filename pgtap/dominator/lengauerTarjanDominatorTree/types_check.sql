\i setup.sql

SELECT plan(5);

SELECT has_function('pgr_lengauertarjandominatortree');

SELECT has_function('pgr_lengauertarjandominatortree', ARRAY['text','bigint']);
SELECT function_returns('pgr_lengauertarjandominatortree', ARRAY['text','bigint'],  'setof record');

-- pgr_bipartite
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_lengauertarjandominatortree'$$,
    $$SELECT  '{"","","seq","vertex_id","idom"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_lengauertarjandominatortree'$$,
    $$VALUES
        ('{25,20,23,20,20}'::OID[])
    $$
);

SELECT * FROM finish();
ROLLBACK;
