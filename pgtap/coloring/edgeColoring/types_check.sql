\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.3.0') THEN plan(1) ELSE plan(5) END;

CREATE OR REPLACE FUNCTION types_check()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.3.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.3.0');
  RETURN;
END IF;

RETURN QUERY
SELECT has_function('pgr_edgecoloring');

RETURN QUERY
SELECT has_function('pgr_edgecoloring', ARRAY['text']);

RETURN QUERY
SELECT function_returns('pgr_edgecoloring', ARRAY['text'],  'setof record');

RETURN QUERY
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edgecoloring'$$,
    $$SELECT  '{"","edge_id","color_id"}'::TEXT[] $$
);

RETURN QUERY
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_edgecoloring'$$,
    $$VALUES
        ('{25,20,20}'::OID[])
    $$
);

END;
$BODY$
LANGUAGE plpgsql;

SELECT types_check();


SELECT * FROM finish();
ROLLBACK;
