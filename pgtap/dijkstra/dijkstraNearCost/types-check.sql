\i setup.sql

SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(11) END;

CREATE OR REPLACE FUNCTION types_check()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;

UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;

RETURN QUERY
SELECT has_function('pgr_dijkstranearcost');

RETURN QUERY
SELECT has_function('pgr_dijkstranearcost', ARRAY['text','bigint','anyarray','boolean','bigint']);

RETURN QUERY
SELECT has_function('pgr_dijkstranearcost', ARRAY['text','anyarray','bigint','boolean','bigint']);

RETURN QUERY
SELECT has_function('pgr_dijkstranearcost', ARRAY['text','anyarray','anyarray','boolean','bigint','boolean']);

RETURN QUERY
SELECT has_function('pgr_dijkstranearcost', ARRAY['text','text','boolean','bigint','boolean']);

RETURN QUERY
SELECT function_returns('pgr_dijkstranearcost', ARRAY['text','bigint','anyarray','boolean','bigint'],  'setof record');

RETURN QUERY
SELECT function_returns('pgr_dijkstranearcost', ARRAY['text','anyarray','bigint','boolean','bigint'],  'setof record');

RETURN QUERY
SELECT function_returns('pgr_dijkstranearcost', ARRAY['text','anyarray','anyarray','boolean','bigint','boolean'],  'setof record');

RETURN QUERY
SELECT function_returns('pgr_dijkstranearcost', ARRAY['text','text','boolean','bigint','boolean'],  'setof record');

-- parameter's names
RETURN QUERY
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dijkstranearcost'$$,
    $$VALUES
        ('{"","","","directed","cap","start_vid","end_vid","agg_cost"}'::TEXT[]),
        ('{"","","","directed","cap","start_vid","end_vid","agg_cost"}'::TEXT[]),
        ('{"","","","directed","cap","global","start_vid","end_vid","agg_cost"}'::TEXT[]),
        ('{"","","directed","cap","global","start_vid","end_vid","agg_cost"}'::TEXT[]);
    $$
);

-- parameter types
RETURN QUERY
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_dijkstranearcost'$$,
    $$VALUES
    ('{25,2277,20,16,20,20,20,701}'::OID[]),
    ('{25,25,16,20,16,20,20,701}'::OID[]),
    ('{25,20,2277,16,20,20,20,701}'::OID[]),
    ('{25,2277,2277,16,20,16,20,20,701}'::OID[]);
$$
);
END;
$BODY$
LANGUAGE plpgsql;

SELECT * FROM types_check();


SELECT * FROM finish();
ROLLBACK;
