\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(5) END;

CREATE OR REPLACE FUNCTION types_check()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;

RETURN QUERY
SELECT has_function('pgr_lengauertarjandominatortree');

RETURN QUERY
SELECT has_function('pgr_lengauertarjandominatortree', ARRAY['text','bigint']);

RETURN QUERY
SELECT function_returns('pgr_lengauertarjandominatortree', ARRAY['text','bigint'],  'setof record');

RETURN QUERY
SELECT bag_has(
  $$SELECT  proargnames from pg_proc where proname = 'pgr_lengauertarjandominatortree'$$,
  $$SELECT  '{"","","seq","vertex_id","idom"}'::TEXT[] $$
);

RETURN QUERY
SELECT set_eq(
  $$SELECT  proallargtypes from pg_proc where proname = 'pgr_lengauertarjandominatortree'$$,
  $$VALUES
  ('{25,20,23,20,20}'::OID[])
  $$
);
END;
$BODY$
LANGUAGE plpgsql;

SELECT types_check();

SELECT * FROM finish();
ROLLBACK;
